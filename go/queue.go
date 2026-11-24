package main

type Queue struct{ head, tail *FNode }

func (q *Queue) Clear() { q.head, q.tail = nil, nil }
func (q *Queue) Push(value string) {
	n := &FNode{value: value}
	if q.tail != nil {
		q.tail.next = n
	} else {
		q.head = n
	}
	q.tail = n
}
func (q *Queue) Pop() (string, bool) {
	if q.head == nil {
		return "", false
	}
	value := q.head.value
	q.head = q.head.next
	if q.head == nil {
		q.tail = nil
	}
	return value, true
}
func (q *Queue) Values() []string {
	var values []string
	for p := q.head; p != nil; p = p.next {
		values = append(values, p.value)
	}
	return values
}
func (q *Queue) Print() { printValues(q.Values()) }
