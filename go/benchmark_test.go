package main

import (
	"fmt"
	"testing"
)

func BenchmarkArrayPush(b *testing.B) {
	for _, n := range []int{100, 500} {
		b.Run(fmt.Sprint(n), func(b *testing.B) {
			for i := 0; i < b.N; i++ {
				var a Array
				for j := 0; j < n; j++ {
					a.Push("A")
				}
			}
		})
	}
}
func BenchmarkFListPushFront(b *testing.B) {
	for _, n := range []int{100, 500} {
		b.Run(fmt.Sprint(n), func(b *testing.B) {
			for i := 0; i < b.N; i++ {
				var l FList
				for j := 0; j < n; j++ {
					l.PushFront("A")
				}
			}
		})
	}
}
func BenchmarkDListPushBack(b *testing.B) {
	for _, n := range []int{100, 500} {
		b.Run(fmt.Sprint(n), func(b *testing.B) {
			for i := 0; i < b.N; i++ {
				var l DList
				for j := 0; j < n; j++ {
					l.PushBack("A")
				}
			}
		})
	}
}
func BenchmarkStackPush(b *testing.B) {
	for _, n := range []int{100, 500} {
		b.Run(fmt.Sprint(n), func(b *testing.B) {
			for i := 0; i < b.N; i++ {
				var s Stack
				for j := 0; j < n; j++ {
					s.Push("A")
				}
			}
		})
	}
}
func BenchmarkQueuePush(b *testing.B) {
	for _, n := range []int{100, 500} {
		b.Run(fmt.Sprint(n), func(b *testing.B) {
			for i := 0; i < b.N; i++ {
				var q Queue
				for j := 0; j < n; j++ {
					q.Push("A")
				}
			}
		})
	}
}
func BenchmarkChainHashAdd(b *testing.B) {
	for _, n := range []int{100, 500} {
		b.Run(fmt.Sprint(n), func(b *testing.B) {
			for i := 0; i < b.N; i++ {
				h, _ := NewChainHash(101)
				for j := 0; j < n; j++ {
					h.Add(fmt.Sprint(j), "value")
				}
			}
		})
	}
}
func BenchmarkChainHashContains(b *testing.B) {
	h, _ := NewChainHash(101)
	for i := 0; i < 500; i++ {
		h.Add(fmt.Sprint(i), "value")
	}
	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		value, found := h.Find("250")
		if !found || value != "value" {
			b.Fatal("lookup failed")
		}
	}
}
func BenchmarkCuckooHashAdd(b *testing.B) {
	for i := 0; i < b.N; i++ {
		h, _ := NewCuckooHash(101)
		if _, err := h.Add("student", "Иванова"); err != nil {
			b.Fatal(err)
		}
	}
}
func BenchmarkCuckooHashContains(b *testing.B) {
	h, _ := NewCuckooHash(101)
	h.Add("student", "Иванова")
	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		value, found := h.Find("student")
		if !found || value != "Иванова" {
			b.Fatal("lookup failed")
		}
	}
}
func BenchmarkCompleteTreeInsert(b *testing.B) {
	for _, n := range []int{100, 500} {
		b.Run(fmt.Sprint(n), func(b *testing.B) {
			for i := 0; i < b.N; i++ {
				var t CompleteTree
				for j := 0; j < n; j++ {
					t.Insert("A")
				}
			}
		})
	}
}
func BenchmarkCompleteTreeFind(b *testing.B) {
	var t CompleteTree
	for i := 0; i < 500; i++ {
		t.Insert(fmt.Sprint(i))
	}
	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		t.Find("499")
	}
}
