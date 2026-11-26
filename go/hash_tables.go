package main

import (
	"errors"
	"fmt"
)

const maxTableCapacity = 10000000

// stringHash computes a stable hash from all bytes of the UTF-8 key.
func stringHash(key string, salt uint32) uint32 {
	hash := uint32(2166136261) ^ salt
	for i := 0; i < len(key); i++ {
		hash ^= uint32(key[i])
		hash *= 16777619
	}
	return hash
}

type HashNode struct {
	key, value string
	next       *HashNode
}

type ChainHash struct {
	buckets []*HashNode
	count   int
}

func NewChainHash(capacity int) (*ChainHash, error) {
	if capacity <= 0 || capacity > maxTableCapacity {
		return nil, errors.New("неверная вместимость таблицы")
	}
	return &ChainHash{buckets: make([]*HashNode, capacity)}, nil
}

func (h *ChainHash) index(key string) int {
	return int(stringHash(key, 0) % uint32(len(h.buckets)))
}

func (h *ChainHash) Find(key string) (string, bool) {
	for p := h.buckets[h.index(key)]; p != nil; p = p.next {
		if p.key == key {
			return p.value, true
		}
	}
	return "", false
}

func (h *ChainHash) Contains(key string) bool {
	_, found := h.Find(key)
	return found
}

func (h *ChainHash) grow() error {
	if len(h.buckets) > (maxTableCapacity-1)/2 {
		return errors.New("таблица слишком большая")
	}
	fresh := make([]*HashNode, len(h.buckets)*2+1)
	for _, head := range h.buckets {
		for p := head; p != nil; {
			next := p.next
			index := int(stringHash(p.key, 0) % uint32(len(fresh)))
			p.next = fresh[index]
			fresh[index] = p
			p = next
		}
	}
	h.buckets = fresh
	return nil
}

// Add inserts a key-value pair or replaces the value for an existing key.
// The boolean is true only when a new key was inserted.
func (h *ChainHash) Add(key, value string) (bool, error) {
	for p := h.buckets[h.index(key)]; p != nil; p = p.next {
		if p.key == key {
			p.value = value
			return false, nil
		}
	}
	if h.count+1 > len(h.buckets)*3/4 {
		if err := h.grow(); err != nil {
			return false, err
		}
	}
	index := h.index(key)
	h.buckets[index] = &HashNode{key: key, value: value, next: h.buckets[index]}
	h.count++
	return true, nil
}

func (h *ChainHash) Remove(key string) bool {
	link := &h.buckets[h.index(key)]
	for *link != nil {
		if (*link).key == key {
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

func (h *ChainHash) Print() {
	for _, head := range h.buckets {
		for p := head; p != nil; p = p.next {
			fmt.Printf(" [%s: %s]", p.key, p.value)
		}
	}
	fmt.Println()
}

func (h *ChainHash) PrintTable() {
	fmt.Println("CHAIN (метод цепочек)")
	fmt.Println("Индекс | Пары ключ-значение")
	fmt.Println("-------+--------------------------------")
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
			fmt.Printf("[%s: %s]", p.key, p.value)
		}
		fmt.Println()
	}
	if !hasRows {
		fmt.Println("(таблица пуста)")
	}
	fmt.Printf("Записей: %d, вместимость: %d\n", h.count, len(h.buckets))
}

type Slot struct {
	key, value string
	used       bool
}

type CuckooHash struct {
	first, second []Slot
	count         int
	seed          uint32
}

func NewCuckooHash(capacity int) (*CuckooHash, error) {
	if capacity <= 0 || capacity > maxTableCapacity {
		return nil, errors.New("неверная вместимость таблицы")
	}
	return &CuckooHash{first: make([]Slot, capacity), second: make([]Slot, capacity)}, nil
}

func (h *CuckooHash) h1(key string, cap int, salt uint32) int {
	return int(stringHash(key, salt) % uint32(cap))
}
func (h *CuckooHash) h2(key string, cap int, salt uint32) int {
	// Distinct hash function for the second table.
	return int(stringHash(key, salt^0x9e3779b9) % uint32(cap))
}

func (h *CuckooHash) Find(key string) (string, bool) {
	cap := len(h.first)
	a, b := h.h1(key, cap, h.seed), h.h2(key, cap, h.seed)
	if h.first[a].used && h.first[a].key == key {
		return h.first[a].value, true
	}
	if h.second[b].used && h.second[b].key == key {
		return h.second[b].value, true
	}
	return "", false
}
func (h *CuckooHash) Contains(key string) bool {
	_, found := h.Find(key)
	return found
}

func (h *CuckooHash) place(entry Slot, a, b []Slot, salt uint32) bool {
	first := true
	for step := 0; step < len(a)*2; step++ {
		var slot *Slot
		if first {
			slot = &a[h.h1(entry.key, len(a), salt)]
		} else {
			slot = &b[h.h2(entry.key, len(a), salt)]
		}
		if !slot.used {
			*slot = entry
			return true
		}
		entry, *slot = *slot, entry
		first = !first
	}
	return false
}

func (h *CuckooHash) entries() []Slot {
	result := make([]Slot, 0, h.count)
	for i := range h.first {
		if h.first[i].used {
			result = append(result, h.first[i])
		}
		if h.second[i].used {
			result = append(result, h.second[i])
		}
	}
	return result
}

func (h *CuckooHash) rebuild(extra Slot) error {
	entries := h.entries()
	entries = append(entries, extra)
	cap := len(h.first)
	salt := h.seed
	for attempt := 0; attempt < 96; attempt++ {
		if attempt != 0 && attempt%8 == 0 {
			if cap > (maxTableCapacity-1)/2 {
				return errors.New("таблица слишком большая")
			}
			cap = cap*2 + 1
		}
		salt++
		a, b := make([]Slot, cap), make([]Slot, cap)
		ok := true
		for _, entry := range entries {
			if !h.place(entry, a, b, salt) {
				ok = false
				break
			}
		}
		if ok {
			h.first, h.second, h.seed, h.count = a, b, salt, len(entries)
			return nil
		}
	}
	return errors.New("не удалось перестроить хеш-таблицу")
}

func (h *CuckooHash) Add(key, value string) (bool, error) {
	cap := len(h.first)
	a, b := h.h1(key, cap, h.seed), h.h2(key, cap, h.seed)
	if h.first[a].used && h.first[a].key == key {
		h.first[a].value = value
		return false, nil
	}
	if h.second[b].used && h.second[b].key == key {
		h.second[b].value = value
		return false, nil
	}
	entry := Slot{key: key, value: value, used: true}
	if !h.first[a].used {
		h.first[a] = entry
		h.count++
		return true, nil
	}
	if !h.second[b].used {
		h.second[b] = entry
		h.count++
		return true, nil
	}
	first := append([]Slot(nil), h.first...)
	second := append([]Slot(nil), h.second...)
	if h.place(entry, first, second, h.seed) {
		h.first, h.second = first, second
		h.count++
		return true, nil
	}
	if err := h.rebuild(entry); err != nil {
		return false, err
	}
	return true, nil
}

func (h *CuckooHash) Remove(key string) bool {
	cap := len(h.first)
	a, b := h.h1(key, cap, h.seed), h.h2(key, cap, h.seed)
	if h.first[a].used && h.first[a].key == key {
		h.first[a] = Slot{}
		h.count--
		return true
	}
	if h.second[b].used && h.second[b].key == key {
		h.second[b] = Slot{}
		h.count--
		return true
	}
	return false
}
func (h *CuckooHash) Clear() {
	for i := range h.first {
		h.first[i] = Slot{}
		h.second[i] = Slot{}
	}
	h.count, h.seed = 0, 0
}
func (h *CuckooHash) Print() {
	for _, entry := range h.entries() {
		fmt.Printf(" [%s: %s]", entry.key, entry.value)
	}
	fmt.Println()
}
func (h *CuckooHash) PrintTable() {
	fmt.Println("CUCKOO (кукушкино хеширование)")
	fmt.Println("Индекс | Таблица 1             | Таблица 2")
	fmt.Println("-------+-----------------------+-----------------------")
	hasRows := false
	for i := range h.first {
		if !h.first[i].used && !h.second[i].used {
			continue
		}
		hasRows = true
		a, b := "-", "-"
		if h.first[i].used {
			a = fmt.Sprintf("[%s: %s]", h.first[i].key, h.first[i].value)
		}
		if h.second[i].used {
			b = fmt.Sprintf("[%s: %s]", h.second[i].key, h.second[i].value)
		}
		fmt.Printf("%6d | %-21s | %s\n", i, a, b)
	}
	if !hasRows {
		fmt.Println("(таблица пуста)")
	}
	fmt.Printf("Записей: %d, вместимость каждой таблицы: %d\n", h.count, len(h.first))
}
