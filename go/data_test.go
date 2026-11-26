package main

import (
	"bufio"
	"bytes"
	"encoding/binary"
	"fmt"
	"io"
	"os"
	"path/filepath"
	"reflect"
	"strings"
	"testing"
)

func equalValues(t *testing.T, got, want []string) {
	t.Helper()
	if !reflect.DeepEqual(got, want) {
		t.Fatalf("got %#v, want %#v", got, want)
	}
}

func TestArray(t *testing.T) {
	var a Array
	if _, ok := a.Get(0); ok || a.Set(-1, "x") || a.Erase(0) || a.Insert(1, "x") {
		t.Fatal("invalid index accepted")
	}
	for _, value := range []string{"A", "B", "C"} {
		a.Push(value)
	}
	if !a.Insert(1, "D") || !a.Set(0, "Z") || a.Size() != 4 {
		t.Fatal("array operations")
	}
	equalValues(t, a.Values(), []string{"Z", "D", "B", "C"})
	if value, ok := a.Get(1); !ok || value != "D" {
		t.Fatal("get")
	}
	if !a.Erase(2) || !a.Erase(0) || !a.Erase(1) || !a.Erase(0) {
		t.Fatal("erase")
	}
	if a.Erase(-1) || a.Erase(3) || a.Insert(-1, "x") || a.Set(3, "x") {
		t.Fatal("invalid index")
	}
	a.Push("again")
	a.Clear()
	if a.Size() != 0 {
		t.Fatal("clear")
	}
	a.Print()
}

func TestFList(t *testing.T) {
	var l FList
	if l.EraseHead() || l.EraseTail() || l.EraseBefore("A") || l.EraseAfter("A") || l.EraseValue("A") || l.Before("A", "X") || l.After("A", "X") || l.Find("A") {
		t.Fatal("empty list")
	}
	l.PushBack("B")
	l.PushFront("A")
	l.PushBack("D")
	if !l.Before("D", "C") || !l.After("D", "E") || !l.Find("C") {
		t.Fatal("insert")
	}
	if l.Before("missing", "X") || l.After("missing", "X") {
		t.Fatal("insert missing")
	}
	equalValues(t, l.Values(), []string{"A", "B", "C", "D", "E"})
	if !l.EraseBefore("C") || !l.EraseAfter("C") || !l.EraseValue("A") {
		t.Fatal("erase around")
	}
	equalValues(t, l.Values(), []string{"C", "E"})
	if l.EraseBefore("C") || l.EraseAfter("E") || l.EraseValue("missing") {
		t.Fatal("erase missing")
	}
	if !l.EraseTail() || !l.EraseTail() || l.EraseTail() {
		t.Fatal("erase tail")
	}
	l.PushFront("single")
	if !l.EraseHead() || l.EraseHead() {
		t.Fatal("erase head")
	}
	l.PushFront("A")
	l.PushBack("B")
	l.Before("A", "Z")
	l.After("A", "Q")
	l.Print()
	l.PrintReverse()
	l.Clear()
	if l.Find("A") {
		t.Fatal("clear")
	}
}

func TestDList(t *testing.T) {
	var l DList
	if l.EraseHead() || l.EraseTail() || l.EraseBefore("A") || l.EraseAfter("A") || l.EraseValue("A") {
		t.Fatal("empty list")
	}
	l.PushBack("B")
	l.PushFront("A")
	l.PushBack("D")
	if !l.Before("D", "C") || !l.After("D", "E") || !l.Find("C") {
		t.Fatal("insert")
	}
	if l.Before("missing", "X") || l.After("missing", "X") {
		t.Fatal("insert missing")
	}
	equalValues(t, l.Values(), []string{"A", "B", "C", "D", "E"})
	equalValues(t, l.ReverseValues(), []string{"E", "D", "C", "B", "A"})
	if !l.EraseBefore("C") || !l.EraseAfter("C") || !l.EraseValue("A") {
		t.Fatal("erase around")
	}
	equalValues(t, l.Values(), []string{"C", "E"})
	if l.EraseBefore("C") || l.EraseAfter("E") || l.EraseValue("missing") {
		t.Fatal("erase missing")
	}
	if !l.EraseTail() || !l.EraseHead() || l.EraseTail() {
		t.Fatal("erase tail")
	}
	l.PushFront("A")
	l.PushFront("B")
	l.Before("B", "Z")
	l.After("A", "Q")
	l.Print()
	l.PrintReverse()
	l.Clear()
	if l.Find("A") {
		t.Fatal("clear")
	}
}

func TestStackQueue(t *testing.T) {
	var s Stack
	if _, ok := s.Pop(); ok {
		t.Fatal("empty stack")
	}
	s.Push("A")
	s.Push("B")
	if v, ok := s.Pop(); !ok || v != "B" {
		t.Fatal("LIFO")
	}
	equalValues(t, s.Values(), []string{"A"})
	s.Print()
	s.Clear()
	if _, ok := s.Pop(); ok {
		t.Fatal("clear")
	}
	var q Queue
	if _, ok := q.Pop(); ok {
		t.Fatal("empty queue")
	}
	q.Push("A")
	q.Push("B")
	if v, ok := q.Pop(); !ok || v != "A" {
		t.Fatal("FIFO")
	}
	equalValues(t, q.Values(), []string{"B"})
	if v, ok := q.Pop(); !ok || v != "B" {
		t.Fatal("last")
	}
	q.Push("C")
	q.Print()
	q.Clear()
	if _, ok := q.Pop(); ok {
		t.Fatal("clear")
	}
}

func TestCompleteTree(t *testing.T) {
	var tree CompleteTree
	if !tree.IsComplete() || tree.Find("A") || tree.Size() != 0 {
		t.Fatal("empty tree")
	}
	tree.PrintLevel()
	tree.PrintPre()
	tree.PrintIn()
	tree.PrintPost()
	for _, v := range []string{"A", "B", "C", "D", "E", "F"} {
		tree.Insert(v)
	}
	if !tree.IsComplete() || tree.Size() != 6 || !tree.Find("E") || tree.Find("X") {
		t.Fatal("tree")
	}
	equalValues(t, tree.LevelValues(), []string{"A", "B", "C", "D", "E", "F"})
	equalValues(t, tree.PreValues(), []string{"A", "B", "D", "E", "C", "F"})
	equalValues(t, tree.InValues(), []string{"D", "B", "E", "A", "F", "C"})
	equalValues(t, tree.PostValues(), []string{"D", "E", "B", "F", "C", "A"})
	tree.PrintPre()
	tree.PrintIn()
	tree.PrintPost()
	tree.PrintLevel()
	tree.Clear()
	tree.Insert("A")
	if tree.Size() != 1 {
		t.Fatal("reuse")
	}
	tree.root.right = &TNode{value: "B"}
	tree.count++
	if tree.IsComplete() {
		t.Fatal("right child without left")
	}
	tree.Clear()
	tree.Insert("A")
	tree.Insert("B")
	tree.root.left.right = &TNode{value: "C"}
	tree.count++
	if tree.IsComplete() {
		t.Fatal("level gap")
	}
}

func TestChainHash(t *testing.T) {
	if _, err := NewChainHash(0); err == nil {
		t.Fatal("invalid capacity")
	}
	h, _ := NewChainHash(3)
	if _, ok := h.Find("missing"); ok || h.Remove("missing") {
		t.Fatal("empty")
	}
	pairs := map[string]string{"10": "десять", "111": "сто одиннадцать", "212": "двести двенадцать", "-5": "минус пять", "": "", "Hello world": "Иванова Мария"}
	for key, value := range pairs {
		added, err := h.Add(key, value)
		if err != nil || !added {
			t.Fatal("add", key, err)
		}
	}
	for key, expected := range pairs {
		got, found := h.Find(key)
		if !found || got != expected {
			t.Fatalf("find %q: %q, %t", key, got, found)
		}
	}
	if added, err := h.Add("111", "обновлено"); err != nil || added {
		t.Fatal("update", err)
	}
	if got, ok := h.Find("111"); !ok || got != "обновлено" {
		t.Fatal("updated value")
	}
	if !h.Remove("212") || h.Remove("212") {
		t.Fatal("remove")
	}
	h.Print()
	h.PrintTable()
	h.Clear()
	h.PrintTable()
	if h.Contains("10") || h.count != 0 {
		t.Fatal("clear")
	}
}

func TestCuckooHash(t *testing.T) {
	if _, err := NewCuckooHash(0); err == nil {
		t.Fatal("invalid capacity")
	}
	h, _ := NewCuckooHash(3)
	if _, ok := h.Find("10"); ok || h.Remove("10") {
		t.Fatal("empty")
	}
	pairs := map[string]string{}
	for i := 0; i < 140; i++ {
		key, value := fmt.Sprint(i), fmt.Sprintf("значение %d", i)
		pairs[key] = value
		added, err := h.Add(key, value)
		if err != nil || !added {
			t.Fatalf("add %s: %v", key, err)
		}
	}
	pairs[""] = "пустой ключ"
	if _, err := h.Add("", "пустой ключ"); err != nil {
		t.Fatal(err)
	}
	pairs["имя с пробелами"] = "Иванова Мария"
	if _, err := h.Add("имя с пробелами", "Иванова Мария"); err != nil {
		t.Fatal(err)
	}
	for key, expected := range pairs {
		got, found := h.Find(key)
		if !found || got != expected {
			t.Fatalf("missing %q: %q", key, got)
		}
	}
	count := h.count
	if added, err := h.Add("10", "новое значение"); err != nil || added || h.count != count {
		t.Fatal("update", err)
	}
	if got, ok := h.Find("10"); !ok || got != "новое значение" {
		t.Fatal("update value")
	}
	if !h.Remove("10") || h.Remove("10") || h.Contains("10") {
		t.Fatal("remove")
	}
	h.Print()
	h.PrintTable()
	h.Clear()
	h.PrintTable()
	if h.Contains("111") || h.count != 0 {
		t.Fatal("clear")
	}
}

func TestSerializationRoundtrip(t *testing.T) {
	objects := []struct {
		name  string
		data  any
		fresh func() any
		key   string
	}{
		{"array", &Array{data: []string{"A", "Hello world", ""}}, func() any { return &Array{} }, "Hello world"},
		{"flist", func() any { p := &FList{}; p.PushBack("A"); p.PushBack("Hello world"); return p }(), func() any { return &FList{} }, "Hello world"},
		{"dlist", func() any { p := &DList{}; p.PushBack("A"); p.PushBack("Hello world"); return p }(), func() any { return &DList{} }, "Hello world"},
		{"stack", func() any { p := &Stack{}; p.Push("A"); p.Push("Hello world"); return p }(), func() any { return &Stack{} }, "Hello world"},
		{"queue", func() any { p := &Queue{}; p.Push("A"); p.Push("Hello world"); return p }(), func() any { return &Queue{} }, "Hello world"},
		{"tree", func() any { p := &CompleteTree{}; p.Insert("A"); p.Insert("Hello world"); return p }(), func() any { return &CompleteTree{} }, "Hello world"},
		{"chain", func() any {
			p, _ := NewChainHash(101)
			p.Add("student", "Иванова")
			p.Add("group", "АБС525")
			return p
		}(), func() any { p, _ := NewChainHash(101); return p }, ""},
		{"cuckoo", func() any {
			p, _ := NewCuckooHash(101)
			p.Add("student", "Иванова")
			p.Add("group", "АБС525")
			return p
		}(), func() any { p, _ := NewCuckooHash(101); return p }, ""},
	}
	for _, tc := range objects {
		t.Run(tc.name, func(t *testing.T) {
			for _, format := range []string{"text", "binary"} {
				var buf bytes.Buffer
				var err error
				if format == "text" {
					err = saveText(&buf, tc.data)
				} else {
					err = saveBinary(&buf, tc.data)
				}
				if err != nil {
					t.Fatal(err)
				}
				target := tc.fresh()
				if format == "text" {
					err = loadText(&buf, target)
				} else {
					err = loadBinary(&buf, target)
				}
				if err != nil {
					t.Fatal(err)
				}
				if !reflect.DeepEqual(valuesOf(tc.data), valuesOf(target)) && tc.key != "" {
					t.Fatal("roundtrip values")
				}
				switch p := target.(type) {
				case *ChainHash:
					if v, ok := p.Find("student"); !ok || v != "Иванова" {
						t.Fatal("chain roundtrip")
					}
				case *CuckooHash:
					if v, ok := p.Find("student"); !ok || v != "Иванова" {
						t.Fatal("cuckoo roundtrip")
					}
				}
			}
		})
	}
}

func TestSerializationInvalid(t *testing.T) {
	for _, bad := range []string{"xyz", "-1\n", "2\n\"A\"\n", "1\nA\n", "1\n\"unterminated\n"} {
		if err := loadText(strings.NewReader(bad), &Array{}); err == nil {
			t.Fatalf("accepted %q", bad)
		}
	}
	for _, bad := range [][]byte{{}, {255, 255, 255, 255}, {2, 0, 0, 0, 1, 0, 0, 0}} {
		if err := loadBinary(bytes.NewReader(bad), &Array{}); err == nil {
			t.Fatalf("accepted bytes %v", bad)
		}
	}
	for _, kind := range []string{"chain", "cuckoo"} {
		var data any
		if kind == "chain" {
			data, _ = NewChainHash(101)
		} else {
			data, _ = NewCuckooHash(101)
		}
		for _, bad := range []string{"x", "0 1\n", "101 -1\n", "101 2\n1\n"} {
			if err := loadText(strings.NewReader(bad), data); err == nil {
				t.Fatalf("accepted %s %q", kind, bad)
			}
		}
	}
}

func TestPersistence(t *testing.T) {
	dir := t.TempDir()
	name := filepath.Join(dir, "myarray")
	original := &Array{}
	original.Push("A")
	original.Push("Hello world")
	if err := saveData(original, name); err != nil {
		t.Fatal(err)
	}
	loaded := &Array{}
	if err := loadData(loaded, name); err != nil {
		t.Fatal(err)
	}
	equalValues(t, loaded.Values(), original.Values())
	if err := os.Remove(name + ".txt"); err != nil {
		t.Fatal(err)
	}
	loaded.Clear()
	if err := loadData(loaded, name); err != nil {
		t.Fatal(err)
	}
	equalValues(t, loaded.Values(), original.Values())
	os.Remove(name + ".bin")
	loaded.Clear()
	if err := loadData(loaded, name); err != nil {
		t.Fatal(err)
	}
	equalValues(t, loaded.Values(), nil)
	os.WriteFile(name+".txt", []byte("wrong"), 0600)
	if err := loadData(loaded, name); err == nil {
		t.Fatal("invalid data")
	}
}

func TestPrimitiveSerialization(t *testing.T) {
	var buf bytes.Buffer
	if err := writeInt(&buf, 42); err != nil {
		t.Fatal(err)
	}
	if err := writeString(&buf, "Hello world"); err != nil {
		t.Fatal(err)
	}
	n, err := readInt(&buf)
	if err != nil || n != 42 {
		t.Fatal(err)
	}
	s, err := readString(&buf)
	if err != nil || s != "Hello world" {
		t.Fatal(err)
	}
	if _, err := readInt(&buf); err == nil {
		t.Fatal("truncation")
	}
	buf.Reset()
	binary.Write(&buf, binary.LittleEndian, int32(-1))
	if _, err := readString(&buf); err == nil {
		t.Fatal("negative string length")
	}
	buf.Reset()
	binary.Write(&buf, binary.LittleEndian, int32(1<<25))
	if _, err := readString(&buf); err == nil {
		t.Fatal("huge string length")
	}
	if err := writeInt(io.Discard, 1<<33); err == nil {
		t.Fatal("overflow")
	}
}

func TestHandleCommands(t *testing.T) {
	a := &Array{}
	handleArray(a, []string{"push", "A"})
	handleArray(a, []string{"insert", "0", "B"})
	handleArray(a, []string{"set", "1", "C"})
	handleArray(a, []string{"size"})
	handleArray(a, []string{"get", "1"})
	handleArray(a, []string{"print"})
	handleArray(a, []string{"get", "-1"})
	handleArray(a, []string{"invalid"})
	handleArray(a, []string{"erase", "0"})
	handleArray(a, []string{"clear"})
	for _, l := range []listOps{&FList{}, &DList{}} {
		for _, args := range [][]string{{"pushback", "A"}, {"pushfront", "B"}, {"before", "A", "C"}, {"after", "A", "D"}, {"find", "C"}, {"print"}, {"reverse"}, {"erasebefore", "A"}, {"eraseafter", "A"}, {"erasevalue", "B"}, {"erasehead"}, {"pushback", "A"}, {"erasetail"}, {"clear"}, {"unknown"}} {
			handleList(l, args)
		}
	}
	for _, p := range []pushPopOps{&Stack{}, &Queue{}} {
		for _, args := range [][]string{{"pop"}, {"push", "A"}, {"push", "B"}, {"print"}, {"pop"}, {"clear"}, {"invalid"}} {
			handlePushPop(p, args)
		}
	}
	for _, h := range []hashOps{func() hashOps { p, _ := NewChainHash(101); return p }(), func() hashOps { p, _ := NewCuckooHash(101); return p }()} {
		for _, args := range [][]string{{"add", "student", "Иванова"}, {"add", "student", "Петрова"}, {"find", "student"}, {"print"}, {"table"}, {"remove", "student"}, {"clear"}, {"unknown"}} {
			handleHash(h, args)
		}
	}
	tree := &CompleteTree{}
	for _, args := range [][]string{{"insert", "A"}, {"find", "A"}, {"complete"}, {"size"}, {"pre"}, {"in"}, {"post"}, {"level"}, {"print"}, {"clear"}, {"unknown"}} {
		handleTree(tree, args)
	}
	for _, value := range []string{`push "Hello world"`, `push A`, `insert 1 C`} {
		if parts, err := parseArgs(value); err != nil || len(parts) == 0 {
			t.Fatal(err)
		}
	}
	if _, err := parseArgs(`push "oops`); err == nil {
		t.Fatal("bad quotation")
	}
	if i, ok := readIndex([]string{"get", "5"}, 1); !ok || i != 5 {
		t.Fatal("index")
	}
	if _, ok := readIndex([]string{"get"}, 1); ok {
		t.Fatal("missing index")
	}
	if _, ok := readIndex([]string{"get", "NaN"}, 1); ok {
		t.Fatal("bad index")
	}
}

func TestRunSession(t *testing.T) {
	old, err := os.Getwd()
	if err != nil {
		t.Fatal(err)
	}
	dir := t.TempDir()
	if err := os.Chdir(dir); err != nil {
		t.Fatal(err)
	}
	defer os.Chdir(old)
	reader := strings.NewReader("push A\npush B\nprint\nhelp\nback\n")
	runSession(bufioReader(reader), &Array{}, "array", "push, back")
	loaded := &Array{}
	if err := loadData(loaded, "array"); err != nil || loaded.Size() != 2 {
		t.Fatal(err)
	}
}

// Tiny wrapper avoids coupling tests to the actual terminal.
func bufioReader(r io.Reader) *bufio.Reader { return bufio.NewReader(r) }
