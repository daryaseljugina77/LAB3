package main

import (
	"bufio"
	"encoding/binary"
	"errors"
	"fmt"
	"io"
	"os"
	"strconv"
	"strings"
)

// The integer and string binary layout follows the C++ project on x86-64:
// signed 32-bit little-endian length, followed by the raw UTF-8 bytes.
func writeInt(w io.Writer, value int) error {
	if value < -2147483648 || value > 2147483647 {
		return errors.New("integer out of range")
	}
	return binary.Write(w, binary.LittleEndian, int32(value))
}
func readInt(r io.Reader) (int, error) {
	var value int32
	err := binary.Read(r, binary.LittleEndian, &value)
	return int(value), err
}
func writeString(w io.Writer, value string) error {
	if err := writeInt(w, len(value)); err != nil {
		return err
	}
	_, err := io.WriteString(w, value)
	return err
}
func readString(r io.Reader) (string, error) {
	size, err := readInt(r)
	if err != nil {
		return "", err
	}
	if size < 0 || size > 1<<24 {
		return "", errors.New("invalid string length")
	}
	data := make([]byte, size)
	_, err = io.ReadFull(r, data)
	return string(data), err
}

func valuesOf(data any) []string {
	switch v := data.(type) {
	case *Array:
		return v.Values()
	case *FList:
		return v.Values()
	case *DList:
		return v.Values()
	case *Stack:
		return v.Values()
	case *Queue:
		return v.Values()
	case *CompleteTree:
		return v.LevelValues()
	}
	return nil
}

func loadValues(data any, values []string) {
	switch v := data.(type) {
	case *Array:
		v.Clear()
		for _, value := range values {
			v.Push(value)
		}
	case *FList:
		v.Clear()
		for _, value := range values {
			v.PushBack(value)
		}
	case *DList:
		v.Clear()
		for _, value := range values {
			v.PushBack(value)
		}
	case *Stack:
		v.Clear()
		for i := len(values) - 1; i >= 0; i-- {
			v.Push(values[i])
		}
	case *Queue:
		v.Clear()
		for _, value := range values {
			v.Push(value)
		}
	case *CompleteTree:
		v.Clear()
		for _, value := range values {
			v.Insert(value)
		}
	}
}

func saveText(w io.Writer, data any) error {
	switch v := data.(type) {
	case *ChainHash:
		if _, err := fmt.Fprintf(w, "%d %d\n", len(v.buckets), v.count); err != nil {
			return err
		}
		for _, head := range v.buckets {
			for p := head; p != nil; p = p.next {
				if _, err := fmt.Fprintf(w, "%q %q\n", p.key, p.value); err != nil {
					return err
				}
			}
		}
		return nil
	case *CuckooHash:
		if _, err := fmt.Fprintf(w, "%d %d %d\n", len(v.first), v.count, v.seed); err != nil {
			return err
		}
		for i := range v.first {
			if v.first[i].used {
				e := v.first[i]
				if _, err := fmt.Fprintf(w, "1 %d %q %q\n", i, e.key, e.value); err != nil {
					return err
				}
			}
			if v.second[i].used {
				e := v.second[i]
				if _, err := fmt.Fprintf(w, "2 %d %q %q\n", i, e.key, e.value); err != nil {
					return err
				}
			}
		}
		return nil
	default:
		values := valuesOf(data)
		if _, err := fmt.Fprintln(w, len(values)); err != nil {
			return err
		}
		for _, value := range values {
			if _, err := fmt.Fprintln(w, strconv.Quote(value)); err != nil {
				return err
			}
		}
		return nil
	}
}

func saveBinary(w io.Writer, data any) error {
	switch v := data.(type) {
	case *ChainHash:
		if err := writeInt(w, len(v.buckets)); err != nil {
			return err
		}
		if err := writeInt(w, v.count); err != nil {
			return err
		}
		for _, head := range v.buckets {
			for p := head; p != nil; p = p.next {
				if err := writeString(w, p.key); err != nil {
					return err
				}
				if err := writeString(w, p.value); err != nil {
					return err
				}
			}
		}
	case *CuckooHash:
		if err := writeInt(w, len(v.first)); err != nil {
			return err
		}
		if err := writeInt(w, v.count); err != nil {
			return err
		}
		if err := writeInt(w, int(int32(v.seed))); err != nil {
			return err
		}
		for i := range v.first {
			if v.first[i].used {
				if err := writeInt(w, 1); err != nil {
					return err
				}
				if err := writeInt(w, i); err != nil {
					return err
				}
				if err := writeString(w, v.first[i].key); err != nil {
					return err
				}
				if err := writeString(w, v.first[i].value); err != nil {
					return err
				}
			}
			if v.second[i].used {
				if err := writeInt(w, 2); err != nil {
					return err
				}
				if err := writeInt(w, i); err != nil {
					return err
				}
				if err := writeString(w, v.second[i].key); err != nil {
					return err
				}
				if err := writeString(w, v.second[i].value); err != nil {
					return err
				}
			}
		}
	default:
		values := valuesOf(data)
		if err := writeInt(w, len(values)); err != nil {
			return err
		}
		for _, value := range values {
			if err := writeString(w, value); err != nil {
				return err
			}
		}
	}
	return nil
}

func validCount(n int) bool { return n >= 0 && n <= 1000000 }

func readQuotedFields(input *bufio.Reader, count int) ([]string, error) {
	line, err := input.ReadString('\n')
	if err != nil && err != io.EOF {
		return nil, err
	}
	args, err := parseArgs(strings.TrimSpace(line))
	if err != nil || len(args) != count {
		return nil, errors.New("неверная строка сериализации")
	}
	return args, nil
}

func loadText(r io.Reader, data any) error {
	input := bufio.NewReader(r)
	switch v := data.(type) {
	case *ChainHash:
		var cap, count int
		if _, err := fmt.Fscan(input, &cap, &count); err != nil {
			return err
		}
		if cap <= 0 || cap > maxTableCapacity || count < 0 || count > cap {
			return errors.New("неверный размер хеш-таблицы")
		}
		_, _ = input.ReadString('\n')
		temp, _ := NewChainHash(cap)
		for i := 0; i < count; i++ {
			args, err := readQuotedFields(input, 2)
			if err != nil {
				return err
			}
			// Every pair is quoted. Non-quoted legacy numeric data is rejected.
			if _, err := temp.Add(args[0], args[1]); err != nil {
				return err
			}
			if temp.count != i+1 {
				return errors.New("повторяющийся ключ")
			}
		}
		*v = *temp
		return nil
	case *CuckooHash:
		var cap, count int
		var seed uint32
		if _, err := fmt.Fscan(input, &cap, &count, &seed); err != nil {
			return err
		}
		if cap <= 0 || cap > maxTableCapacity || count < 0 || count > cap*2 {
			return errors.New("неверный размер хеш-таблицы")
		}
		_, _ = input.ReadString('\n')
		temp, _ := NewCuckooHash(cap)
		temp.seed = seed
		for i := 0; i < count; i++ {
			args, err := readQuotedFields(input, 4)
			if err != nil {
				return err
			}
			table, e1 := strconv.Atoi(args[0])
			index, e2 := strconv.Atoi(args[1])
			if e1 != nil || e2 != nil || index < 0 || index >= cap {
				return errors.New("неверная позиция")
			}
			key, value := args[2], args[3]
			if temp.Contains(key) {
				return errors.New("повторяющийся ключ")
			}
			entry := Slot{key: key, value: value, used: true}
			if table == 1 && index == temp.h1(key, cap, seed) && !temp.first[index].used {
				temp.first[index] = entry
			} else if table == 2 && index == temp.h2(key, cap, seed) && !temp.second[index].used {
				temp.second[index] = entry
			} else {
				return errors.New("неверная позиция")
			}
			temp.count++
		}
		*v = *temp
		return nil
	default:
		var count int
		if _, err := fmt.Fscan(input, &count); err != nil {
			return err
		}
		if !validCount(count) {
			return errors.New("invalid element count")
		}
		_, _ = input.ReadString('\n')
		values := make([]string, 0, count)
		for i := 0; i < count; i++ {
			line, err := input.ReadString('\n')
			if err != nil && err != io.EOF {
				return err
			}
			line = strings.TrimSpace(line)
			if !strings.HasPrefix(line, "\"") {
				return errors.New("invalid quoted string")
			}
			value, unquoteErr := strconv.Unquote(line)
			if unquoteErr != nil {
				return unquoteErr
			}
			values = append(values, value)
		}
		loadValues(data, values)
		return nil
	}
}

func loadBinary(r io.Reader, data any) error {
	countOrCap, err := readInt(r)
	if err != nil {
		return err
	}
	switch v := data.(type) {
	case *ChainHash:
		cap := countOrCap
		count, err := readInt(r)
		if err != nil {
			return err
		}
		if cap <= 0 || cap > maxTableCapacity || count < 0 || count > cap {
			return errors.New("неверный размер хеш-таблицы")
		}
		temp, _ := NewChainHash(cap)
		for i := 0; i < count; i++ {
			key, err := readString(r)
			if err != nil {
				return err
			}
			value, err := readString(r)
			if err != nil {
				return err
			}
			if _, err = temp.Add(key, value); err != nil {
				return err
			}
			if temp.count != i+1 {
				return errors.New("повторяющийся ключ")
			}
		}
		*v = *temp
	case *CuckooHash:
		cap := countOrCap
		count, err := readInt(r)
		if err != nil {
			return err
		}
		seed, err := readInt(r)
		if err != nil {
			return err
		}
		if cap <= 0 || cap > maxTableCapacity || count < 0 || count > cap*2 {
			return errors.New("неверный размер хеш-таблицы")
		}
		temp, _ := NewCuckooHash(cap)
		temp.seed = uint32(seed)
		for i := 0; i < count; i++ {
			table, err := readInt(r)
			if err != nil {
				return err
			}
			index, err := readInt(r)
			if err != nil {
				return err
			}
			key, err := readString(r)
			if err != nil {
				return err
			}
			value, err := readString(r)
			if err != nil {
				return err
			}
			if index < 0 || index >= cap || temp.Contains(key) {
				return errors.New("неверная запись таблицы")
			}
			entry := Slot{key: key, value: value, used: true}
			if table == 1 && index == temp.h1(key, cap, temp.seed) && !temp.first[index].used {
				temp.first[index] = entry
			} else if table == 2 && index == temp.h2(key, cap, temp.seed) && !temp.second[index].used {
				temp.second[index] = entry
			} else {
				return errors.New("неверная запись таблицы")
			}
			temp.count++
		}
		*v = *temp
	default:
		if !validCount(countOrCap) {
			return errors.New("invalid element count")
		}
		values := make([]string, 0, countOrCap)
		for i := 0; i < countOrCap; i++ {
			value, err := readString(r)
			if err != nil {
				return err
			}
			values = append(values, value)
		}
		loadValues(data, values)
	}
	return nil
}

func saveData(data any, name string) error {
	textFile, err := os.Create(name + ".txt")
	if err != nil {
		return err
	}
	if err = saveText(textFile, data); err != nil {
		textFile.Close()
		return err
	}
	if err = textFile.Close(); err != nil {
		return err
	}
	binaryFile, err := os.Create(name + ".bin")
	if err != nil {
		return err
	}
	if err = saveBinary(binaryFile, data); err != nil {
		binaryFile.Close()
		return err
	}
	return binaryFile.Close()
}

func loadData(data any, name string) error {
	f, err := os.Open(name + ".txt")
	if err == nil {
		defer f.Close()
		return loadText(f, data)
	}
	if !errors.Is(err, os.ErrNotExist) {
		return err
	}
	f, err = os.Open(name + ".bin")
	if errors.Is(err, os.ErrNotExist) {
		return nil
	}
	if err != nil {
		return err
	}
	defer f.Close()
	return loadBinary(f, data)
}
