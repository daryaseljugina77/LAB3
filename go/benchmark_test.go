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
					h.Add(j)
				}
			}
		})
	}
}
func BenchmarkChainHashContains(b *testing.B) {
	h, _ := NewChainHash(101)
	for i := 0; i < 500; i++ {
		h.Add(i)
	}
	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		h.Contains(250)
	}
}
func BenchmarkCuckooHashAdd(b *testing.B) {
	for i := 0; i < b.N; i++ {
		h, _ := NewCuckooHash(101)
		h.Add(10)
	}
}
func BenchmarkCuckooHashContains(b *testing.B) {
	h, _ := NewCuckooHash(101)
	h.Add(10)
	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		h.Contains(10)
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
