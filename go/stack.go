package main

type Stack struct{ top *FNode }

func (s *Stack) Clear()            { s.top = nil }
func (s *Stack) Push(value string) { s.top = &FNode{value: value, next: s.top} }
func (s *Stack) Pop() (string, bool) {
	if s.top == nil {
		return "", false
	}
	value := s.top.value
	s.top = s.top.next
	return value, true
}
func (s *Stack) Values() []string {
	var values []string
	for p := s.top; p != nil; p = p.next {
		values = append(values, p.value)
	}
	return values
}
func (s *Stack) Print() { printValues(s.Values()) }
