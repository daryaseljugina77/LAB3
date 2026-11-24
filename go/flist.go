package main

type FNode struct {
	value string
	next  *FNode
}

type FList struct{ head *FNode }

func (l *FList) Clear() { l.head = nil }

func (l *FList) PushFront(value string) {
	l.head = &FNode{value: value, next: l.head}
}

func (l *FList) PushBack(value string) {
	node := &FNode{value: value}
	if l.head == nil {
		l.head = node
		return
	}
	p := l.head
	for p.next != nil {
		p = p.next
	}
	p.next = node
}

func (l *FList) Before(target, value string) bool {
	var prev *FNode
	for p := l.head; p != nil; p = p.next {
		if p.value == target {
			n := &FNode{value: value, next: p}
			if prev == nil {
				l.head = n
			} else {
				prev.next = n
			}
			return true
		}
		prev = p
	}
	return false
}

func (l *FList) After(target, value string) bool {
	for p := l.head; p != nil; p = p.next {
		if p.value == target {
			p.next = &FNode{value: value, next: p.next}
			return true
		}
	}
	return false
}

func (l *FList) EraseHead() bool {
	if l.head == nil {
		return false
	}
	l.head = l.head.next
	return true
}

func (l *FList) EraseTail() bool {
	if l.head == nil {
		return false
	}
	if l.head.next == nil {
		l.head = nil
		return true
	}
	p := l.head
	for p.next.next != nil {
		p = p.next
	}
	p.next = nil
	return true
}

func (l *FList) EraseBefore(target string) bool {
	var previousPrevious, previous *FNode
	for p := l.head; p != nil; p = p.next {
		if p.value == target {
			if previous == nil {
				return false
			}
			if previousPrevious == nil {
				l.head = p
			} else {
				previousPrevious.next = p
			}
			return true
		}
		previousPrevious = previous
		previous = p
	}
	return false
}

func (l *FList) EraseAfter(target string) bool {
	for p := l.head; p != nil; p = p.next {
		if p.value == target {
			if p.next == nil {
				return false
			}
			p.next = p.next.next
			return true
		}
	}
	return false
}

func (l *FList) EraseValue(value string) bool {
	var prev *FNode
	for p := l.head; p != nil; p = p.next {
		if p.value == value {
			if prev == nil {
				l.head = p.next
			} else {
				prev.next = p.next
			}
			return true
		}
		prev = p
	}
	return false
}

func (l *FList) Find(value string) bool {
	for p := l.head; p != nil; p = p.next {
		if p.value == value {
			return true
		}
	}
	return false
}

func (l *FList) Values() []string {
	var result []string
	for p := l.head; p != nil; p = p.next {
		result = append(result, p.value)
	}
	return result
}

func (l *FList) Print() { printValues(l.Values()) }

func (l *FList) PrintReverse() {
	values := l.Values()
	for i, j := 0, len(values)-1; i < j; i, j = i+1, j-1 {
		values[i], values[j] = values[j], values[i]
	}
	printValues(values)
}
