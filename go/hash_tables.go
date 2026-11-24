package main

import (
	"errors"
	"fmt"
	"math"
)

// FoldHash is the same three-digit folding used by the C++ implementation.
func FoldHash(key uint32) uint32 {
	var sum uint32
	for {
		sum += key % 1000
		key /= 1000
		if key == 0 {
			break
		}
	}
	return sum
}

const maxTableCapacity = 10000000

type ChainHash struct {
	buckets []*FNode
	count   int
}

func NewChainHash(capacity int) (*ChainHash, error) {
	if capacity <= 0 || capacity > maxTableCapacity {
		return nil, errors.New("invalid capacity")
	}
	return &ChainHash{buckets: make([]*FNode, capacity)}, nil
}

func (h *ChainHash) index(key int) int {
	return int(FoldHash(uint32(int32(key))) % uint32(len(h.buckets)))
}

func (h *ChainHash) Contains(key int) bool {
	value := fmt.Sprint(key)
	for p := h.buckets[h.index(key)]; p != nil; p = p.next {
		if p.value == value {
			return true
		}
	}
	return false
}

func (h *ChainHash) grow() error {
	if len(h.buckets) > (maxTableCapacity-1)/2 {
		return errors.New("table too large")
	}
	fresh := make([]*FNode, len(h.buckets)*2+1)
	for _, head := range h.buckets {
		for p := head; p != nil; {
			next := p.next
			var key int
			if _, err := fmt.Sscan(p.value, &key); err != nil {
				return err
			}
			index := int(FoldHash(uint32(int32(key))) % uint32(len(fresh)))
			p.next = fresh[index]
			fresh[index] = p
			p = next
		}
	}
	h.buckets = fresh
	return nil
}

func (h *ChainHash) Add(key int) (bool, error) {
	if h.Contains(key) {
		return false, nil
	}
	if h.count >= len(h.buckets)*3/4 {
		if err := h.grow(); err != nil {
			return false, err
		}
	}
	index := h.index(key)
	h.buckets[index] = &FNode{value: fmt.Sprint(key), next: h.buckets[index]}
	h.count++
	return true, nil
}

func (h *ChainHash) Remove(key int) bool {
	index := h.index(key)
	value := fmt.Sprint(key)
	link := &h.buckets[index]
	for *link != nil {
		if (*link).value == value {
			*link = (*link).next
			h.count--
			return true
		}
		link = &(*link).next
	}
	return false
}

func (h *ChainHash) Clear() {
	for i := range h.buckets {
		h.buckets[i] = nil
	}
	h.count = 0
}

func (h *ChainHash) Keys() []int {
	keys := make([]int, 0, h.count)
	for _, head := range h.buckets {
		for p := head; p != nil; p = p.next {
			var key int
			fmt.Sscan(p.value, &key)
			keys = append(keys, key)
		}
	}
	return keys
}

func (h *ChainHash) Print() {
	for _, key := range h.Keys() {
		fmt.Print(" ", key)
	}
	fmt.Println()
}

func (h *ChainHash) PrintTable() {
	fmt.Println("CHAIN (method of chains)")
	fmt.Println("Index  | Keys")
	fmt.Println("-------+--------------------------")
	hasRows := false
	for i, head := range h.buckets {
		if head == nil {
			continue
		}
		hasRows = true
		fmt.Printf("%6d | ", i)
		for p := head; p != nil; p = p.next {
			if p != head {
				fmt.Print(" -> ")
			}
			fmt.Print(p.value)
		}
		fmt.Println()
	}
	if !hasRows {
		fmt.Println("(table is empty)")
	}
	fmt.Printf("Keys: %d, capacity: %d\n", h.count, len(h.buckets))
}

type Slot struct {
	key  int
	used bool
}

type CuckooHash struct {
	first, second []Slot
	count         int
	seed          uint32
}

func NewCuckooHash(capacity int) (*CuckooHash, error) {
	if capacity <= 0 || capacity > maxTableCapacity {
		return nil, errors.New("invalid capacity")
	}
	return &CuckooHash{first: make([]Slot, capacity), second: make([]Slot, capacity)}, nil
}

func (h *CuckooHash) h1(key, cap int, salt uint32) int {
	return int((FoldHash(uint32(int32(key))) + salt) % uint32(cap))
}

func (h *CuckooHash) h2(key, cap int, salt uint32) int {
	x := uint32(int32(key))
	n := uint32(cap)
	return int((x%n + x/n + salt) % n)
}

func (h *CuckooHash) Contains(key int) bool {
	cap := len(h.first)
	a, b := h.h1(key, cap, h.seed), h.h2(key, cap, h.seed)
	return (h.first[a].used && h.first[a].key == key) ||
		(h.second[b].used && h.second[b].key == key)
}

func (h *CuckooHash) place(key int, a, b []Slot, salt uint32) bool {
	first := true
	for step := 0; step < len(a)*2; step++ {
		var slot *Slot
		if first {
			slot = &a[h.h1(key, len(a), salt)]
		} else {
			slot = &b[h.h2(key, len(a), salt)]
		}
		if !slot.used {
			slot.used, slot.key = true, key
			return true
		}
		key, slot.key = slot.key, key
		first = !first
	}
	return false
}

func (h *CuckooHash) rebuild(extra int) error {
	keys := h.Keys()
	keys = append(keys, extra)
	cap := len(h.first)
	salt := h.seed
	for attempt := 0; ; attempt++ {
		if attempt != 0 && attempt%8 == 0 {
			if cap > (maxTableCapacity-1)/2 {
				return errors.New("table too large")
			}
			cap = cap*2 + 1
		}
		salt++
		a, b := make([]Slot, cap), make([]Slot, cap)
		ok := true
		for _, key := range keys {
			if !h.place(key, a, b, salt) {
				ok = false
				break
			}
		}
		if ok {
			h.first, h.second, h.seed, h.count = a, b, salt, len(keys)
			return nil
		}
		if attempt == math.MaxInt {
			return errors.New("table too large")
		}
	}
}

func (h *CuckooHash) Add(key int) (bool, error) {
	if h.Contains(key) {
		return false, nil
	}
	cap := len(h.first)
	a, b := h.h1(key, cap, h.seed), h.h2(key, cap, h.seed)
	if !h.first[a].used {
		h.first[a] = Slot{key, true}
		h.count++
		return true, nil
	}
	if !h.second[b].used {
		h.second[b] = Slot{key, true}
		h.count++
		return true, nil
	}
	first := append([]Slot(nil), h.first...)
	second := append([]Slot(nil), h.second...)
	if h.place(key, first, second, h.seed) {
		h.first, h.second = first, second
		h.count++
		return true, nil
	}
	if err := h.rebuild(key); err != nil {
		return false, err
	}
	return true, nil
}

func (h *CuckooHash) Remove(key int) bool {
	cap := len(h.first)
	a, b := h.h1(key, cap, h.seed), h.h2(key, cap, h.seed)
	if h.first[a].used && h.first[a].key == key {
		h.first[a].used = false
		h.count--
		return true
	}
	if h.second[b].used && h.second[b].key == key {
		h.second[b].used = false
		h.count--
		return true
	}
	return false
}

func (h *CuckooHash) Clear() {
	for i := range h.first {
		h.first[i].used = false
		h.second[i].used = false
	}
	h.count, h.seed = 0, 0
}

func (h *CuckooHash) Keys() []int {
	keys := make([]int, 0, h.count)
	for i := range h.first {
		if h.first[i].used {
			keys = append(keys, h.first[i].key)
		}
		if h.second[i].used {
			keys = append(keys, h.second[i].key)
		}
	}
	return keys
}

func (h *CuckooHash) Print() {
	for _, key := range h.Keys() {
		fmt.Print(" ", key)
	}
	fmt.Println()
}

func (h *CuckooHash) PrintTable() {
	fmt.Println("CUCKOO (cuckoo hashing)")
	fmt.Println("Index  | Table 1   | Table 2")
	fmt.Println("-------+-----------+-----------")
	hasRows := false
	for i := range h.first {
		if !h.first[i].used && !h.second[i].used {
			continue
		}
		hasRows = true
		a, b := "-", "-"
		if h.first[i].used {
			a = fmt.Sprint(h.first[i].key)
		}
		if h.second[i].used {
			b = fmt.Sprint(h.second[i].key)
		}
		fmt.Printf("%6d | %9s | %9s\n", i, a, b)
	}
	if !hasRows {
		fmt.Println("(table is empty)")
	}
	fmt.Printf("Keys: %d, capacity of each table: %d\n", h.count, len(h.first))
}
