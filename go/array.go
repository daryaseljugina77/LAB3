package main

type Array struct {
	data []string
}

func (a *Array) Push(value string) {
	a.data = append(a.data, value)
}

func (a *Array) Insert(index int, value string) bool {
	if index < 0 || index > len(a.data) {
		return false
	}
	a.data = append(a.data, "")
	copy(a.data[index+1:], a.data[index:len(a.data)-1])
	a.data[index] = value
	return true
}

func (a *Array) Get(index int) (string, bool) {
	if index < 0 || index >= len(a.data) {
		return "", false
	}
	return a.data[index], true
}

func (a *Array) Set(index int, value string) bool {
	if index < 0 || index >= len(a.data) {
		return false
	}
	a.data[index] = value
	return true
}

func (a *Array) Erase(index int) bool {
	if index < 0 || index >= len(a.data) {
		return false
	}
	copy(a.data[index:], a.data[index+1:])
	a.data[len(a.data)-1] = ""
	a.data = a.data[:len(a.data)-1]
	return true
}

func (a *Array) Size() int        { return len(a.data) }
func (a *Array) Clear()           { a.data = nil }
func (a *Array) Values() []string { return append([]string(nil), a.data...) }
func (a *Array) Print()           { printValues(a.data) }
