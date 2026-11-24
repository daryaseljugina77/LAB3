package main

type LNode struct {
	value string
	next  *LNode
	prev  *LNode
}

type DList struct {
	head *LNode
	tail *LNode
}

func (l *DList) Clear() { l.head, l.tail = nil, nil }

func (l *DList) PushFront(value string) {
	n := &LNode{value: value, next: l.head}
	if l.head != nil {
		l.head.prev = n
	} else {
		l.tail = n
	}
	l.head = n
}

func (l *DList) PushBack(value string) {
	n := &LNode{value: value, prev: l.tail}
	if l.tail != nil {
		l.tail.next = n
	} else {
		l.head = n
	}
	l.tail = n
}

func (l *DList) Before(target, value string) bool {
	for p := l.head; p != nil; p = p.next {
		if p.value == target {
			n := &LNode{value: value, next: p, prev: p.prev}
			if p.prev != nil {
				p.prev.next = n
			} else {
				l.head = n
			}
			p.prev = n
			return true
		}
	}
	return false
}

func (l *DList) After(target, value string) bool {
	for p := l.head; p != nil; p = p.next {
		if p.value == target {
			n := &LNode{value: value, prev: p, next: p.next}
			if p.next != nil {
				p.next.prev = n
			} else {
				l.tail = n
			}
			p.next = n
			return true
		}
	}
	return false
}

func (l *DList) removeNode(p *LNode) {
	if p.prev == nil {
		l.head = p.next
	} else {
		p.prev.next = p.next
	}
	if p.next == nil {
		l.tail = p.prev
	} else {
		p.next.prev = p.prev
	}
}

func (l *DList) EraseHead() bool {
	if l.head == nil {
		return false
	}
	l.removeNode(l.head)
	return true
}

func (l *DList) EraseTail() bool {
	if l.tail == nil {
		return false
	}
	l.removeNode(l.tail)
	return true
}

func (l *DList) EraseBefore(target string) bool {
	for p := l.head; p != nil; p = p.next {
		if p.value == target {
			if p.prev == nil {
				return false
			}
			l.removeNode(p.prev)
			return true
		}
	}
	return false
}

func (l *DList) EraseAfter(target string) bool {
	for p := l.head; p != nil; p = p.next {
		if p.value == target {
			if p.next == nil {
				return false
			}
			l.removeNode(p.next)
			return true
		}
	}
	return false
}

func (l *DList) EraseValue(value string) bool {
	for p := l.head; p != nil; p = p.next {
		if p.value == value {
			l.removeNode(p)
			return true
		}
	}
	return false
}

func (l *DList) Find(value string) bool {
	for p := l.head; p != nil; p = p.next {
		if p.value == value {
			return true
		}
	}
	return false
}

func (l *DList) Values() []string {
	var values []string
	for p := l.head; p != nil; p = p.next {
		values = append(values, p.value)
	}
	return values
}

func (l *DList) ReverseValues() []string {
	var values []string
	for p := l.tail; p != nil; p = p.prev {
		values = append(values, p.value)
	}
	return values
}

func (l *DList) Print()        { printValues(l.Values()) }
func (l *DList) PrintReverse() { printValues(l.ReverseValues()) }
