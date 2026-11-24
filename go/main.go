package main

import (
	"bufio"
	"fmt"
	"os"
	"regexp"
	"strconv"
	"strings"
)

var argumentPattern = regexp.MustCompile(`"(?:\\.|[^"\\])*"|\S+`)

func parseArgs(line string) ([]string, error) {
	parts := argumentPattern.FindAllString(line, -1)
	args := make([]string, 0, len(parts))
	for _, part := range parts {
		if strings.HasPrefix(part, `"`) {
			value, err := strconv.Unquote(part)
			if err != nil {
				return nil, err
			}
			args = append(args, value)
		} else {
			args = append(args, part)
		}
	}
	return args, nil
}

func printValues(values []string) {
	for _, value := range values {
		fmt.Print(value, " ")
	}
	fmt.Println()
}

func readIndex(args []string, at int) (int, bool) {
	if len(args) <= at {
		return 0, false
	}
	value, err := strconv.Atoi(args[at])
	return value, err == nil
}

func handleArray(a *Array, args []string) bool {
	cmd := args[0]
	switch cmd {
	case "push":
		if len(args) == 2 {
			a.Push(args[1])
			return true
		}
	case "insert", "set":
		index, ok := readIndex(args, 1)
		if ok && len(args) == 3 {
			if cmd == "insert" {
				return a.Insert(index, args[2])
			}
			return a.Set(index, args[2])
		}
	case "erase":
		index, ok := readIndex(args, 1)
		if ok && len(args) == 2 {
			return a.Erase(index)
		}
	case "get":
		index, ok := readIndex(args, 1)
		if ok && len(args) == 2 {
			value, found := a.Get(index)
			if found {
				fmt.Println(value)
			} else {
				fmt.Println("Invalid index")
			}
			return false
		}
	case "size":
		if len(args) == 1 {
			fmt.Println(a.Size())
			return false
		}
	case "print":
		if len(args) == 1 {
			a.Print()
			return false
		}
	case "clear":
		if len(args) == 1 {
			a.Clear()
			return true
		}
	}
	fmt.Println("Unknown command or invalid arguments")
	return false
}

type listOps interface {
	Clear()
	PushBack(string)
	PushFront(string)
	Before(string, string) bool
	After(string, string) bool
	EraseHead() bool
	EraseTail() bool
	EraseBefore(string) bool
	EraseAfter(string) bool
	EraseValue(string) bool
	Find(string) bool
	Print()
	PrintReverse()
}

func handleList(l listOps, args []string) bool {
	cmd := args[0]
	switch cmd {
	case "pushback":
		if len(args) == 2 {
			l.PushBack(args[1])
			return true
		}
	case "pushfront":
		if len(args) == 2 {
			l.PushFront(args[1])
			return true
		}
	case "before":
		if len(args) == 3 {
			return l.Before(args[1], args[2])
		}
	case "after":
		if len(args) == 3 {
			return l.After(args[1], args[2])
		}
	case "erasehead":
		if len(args) == 1 {
			return l.EraseHead()
		}
	case "erasetail":
		if len(args) == 1 {
			return l.EraseTail()
		}
	case "erasebefore":
		if len(args) == 2 {
			return l.EraseBefore(args[1])
		}
	case "eraseafter":
		if len(args) == 2 {
			return l.EraseAfter(args[1])
		}
	case "erasevalue":
		if len(args) == 2 {
			return l.EraseValue(args[1])
		}
	case "find":
		if len(args) == 2 {
			fmt.Println(l.Find(args[1]))
			return false
		}
	case "print":
		if len(args) == 1 {
			l.Print()
			return false
		}
	case "reverse":
		if len(args) == 1 {
			l.PrintReverse()
			return false
		}
	case "clear":
		if len(args) == 1 {
			l.Clear()
			return true
		}
	}
	fmt.Println("Unknown command or invalid arguments")
	return false
}

type pushPopOps interface {
	Clear()
	Push(string)
	Pop() (string, bool)
	Print()
}

func handlePushPop(p pushPopOps, args []string) bool {
	switch args[0] {
	case "push":
		if len(args) == 2 {
			p.Push(args[1])
			return true
		}
	case "pop":
		if len(args) == 1 {
			value, ok := p.Pop()
			if ok {
				fmt.Println("Removed:", value)
				return true
			}
			fmt.Println("Container is empty")
			return false
		}
	case "print":
		if len(args) == 1 {
			p.Print()
			return false
		}
	case "clear":
		if len(args) == 1 {
			p.Clear()
			return true
		}
	}
	fmt.Println("Unknown command or invalid arguments")
	return false
}

type hashOps interface {
	Add(int) (bool, error)
	Remove(int) bool
	Contains(int) bool
	Print()
	PrintTable()
	Clear()
}

func handleHash(h hashOps, args []string) (bool, error) {
	if len(args) == 1 {
		switch args[0] {
		case "print":
			h.Print()
			return false, nil
		case "table":
			h.PrintTable()
			return false, nil
		case "clear":
			h.Clear()
			return true, nil
		}
	}
	key, ok := readIndex(args, 1)
	if ok && len(args) == 2 {
		switch args[0] {
		case "add":
			return h.Add(key)
		case "remove":
			return h.Remove(key), nil
		case "contains":
			fmt.Println(h.Contains(key))
			return false, nil
		}
	}
	fmt.Println("Unknown command or invalid arguments")
	return false, nil
}

func handleTree(t *CompleteTree, args []string) bool {
	if len(args) == 2 {
		switch args[0] {
		case "insert":
			t.Insert(args[1])
			return true
		case "find":
			fmt.Println(t.Find(args[1]))
			return false
		}
	}
	if len(args) == 1 {
		switch args[0] {
		case "complete":
			fmt.Println(t.IsComplete())
			return false
		case "size":
			fmt.Println(t.Size())
			return false
		case "pre":
			t.PrintPre()
			return false
		case "in":
			t.PrintIn()
			return false
		case "post":
			t.PrintPost()
			return false
		case "level", "print":
			t.PrintLevel()
			return false
		case "clear":
			t.Clear()
			return true
		}
	}
	fmt.Println("Unknown command or invalid arguments")
	return false
}

func runSession(reader *bufio.Reader, data any, name, commands string) {
	if err := loadData(data, name); err != nil {
		fmt.Println("Load error:", err)
	}
	fmt.Println("Commands:", commands)
	fmt.Println(`Use quotes for strings with spaces, e.g. push "Hello world"`)
	fmt.Println("Type back to choose another structure")
	for {
		fmt.Print(name, "> ")
		line, err := reader.ReadString('\n')
		if err != nil && line == "" {
			return
		}
		args, parseErr := parseArgs(strings.TrimSpace(line))
		if parseErr != nil {
			fmt.Println("Invalid arguments:", parseErr)
			continue
		}
		if len(args) == 0 {
			continue
		}
		if args[0] == "back" {
			return
		}
		if args[0] == "help" {
			fmt.Println("Commands:", commands)
			continue
		}
		changed := false
		switch v := data.(type) {
		case *Array:
			changed = handleArray(v, args)
		case *FList:
			changed = handleList(v, args)
		case *DList:
			changed = handleList(v, args)
		case *Stack:
			changed = handlePushPop(v, args)
		case *Queue:
			changed = handlePushPop(v, args)
		case *ChainHash:
			var actionErr error
			changed, actionErr = handleHash(v, args)
			if actionErr != nil {
				fmt.Println("Error:", actionErr)
			}
		case *CuckooHash:
			var actionErr error
			changed, actionErr = handleHash(v, args)
			if actionErr != nil {
				fmt.Println("Error:", actionErr)
			}
		case *CompleteTree:
			changed = handleTree(v, args)
		}
		if changed {
			if saveErr := saveData(data, name); saveErr != nil {
				fmt.Println("Save error:", saveErr)
			} else {
				fmt.Println("Saved")
			}
		}
	}
}

func main() {
	reader := bufio.NewReader(os.Stdin)
	fmt.Println("Data structures - LAB3 (Go)")
	fmt.Println("array, flist, dlist, stack, queue, chain, cuckoo, tree")
	fmt.Println("Type exit to finish")
	for {
		fmt.Print("Select structure: ")
		line, err := reader.ReadString('\n')
		if err != nil && line == "" {
			return
		}
		name := strings.TrimSpace(line)
		switch name {
		case "exit":
			return
		case "array":
			runSession(reader, &Array{}, name, "push, insert, get, set, erase, size, print, clear, back")
		case "flist":
			runSession(reader, &FList{}, name, "pushback, pushfront, before, after, erasehead, erasetail, erasebefore, eraseafter, erasevalue, find, print, reverse, clear, back")
		case "dlist":
			runSession(reader, &DList{}, name, "pushback, pushfront, before, after, erasehead, erasetail, erasebefore, eraseafter, erasevalue, find, print, reverse, clear, back")
		case "stack":
			runSession(reader, &Stack{}, name, "push, pop, print, clear, back")
		case "queue":
			runSession(reader, &Queue{}, name, "push, pop, print, clear, back")
		case "chain":
			h, _ := NewChainHash(101)
			runSession(reader, h, name, "add, remove, contains, print, table, clear, back")
		case "cuckoo":
			h, _ := NewCuckooHash(101)
			runSession(reader, h, name, "add, remove, contains, print, table, clear, back")
		case "tree":
			runSession(reader, &CompleteTree{}, name, "insert, find, complete, size, pre, in, post, level, print, clear, back")
		case "":
			continue
		default:
			fmt.Println("Unknown structure")
		}
	}
}
