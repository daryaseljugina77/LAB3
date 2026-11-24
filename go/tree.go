package main

type TNode struct {
	value       string
	left, right *TNode
}

type CompleteTree struct {
	root  *TNode
	count int
}

func (t *CompleteTree) Clear() { t.root, t.count = nil, 0 }

func (t *CompleteTree) Insert(value string) {
	n := &TNode{value: value}
	if t.root == nil {
		t.root = n
		t.count = 1
		return
	}
	nodes := []*TNode{t.root}
	for i := 0; i < len(nodes); i++ {
		p := nodes[i]
		if p.left == nil {
			p.left = n
			t.count++
			return
		}
		nodes = append(nodes, p.left)
		if p.right == nil {
			p.right = n
			t.count++
			return
		}
		nodes = append(nodes, p.right)
	}
}

func (t *CompleteTree) Find(value string) bool {
	for _, item := range t.LevelValues() {
		if item == value {
			return true
		}
	}
	return false
}

func (t *CompleteTree) IsComplete() bool {
	if t.root == nil {
		return true
	}
	nodes := []*TNode{t.root}
	gap := false
	for i := 0; i < len(nodes); i++ {
		p := nodes[i]
		if p.left != nil {
			if gap {
				return false
			}
			nodes = append(nodes, p.left)
		} else {
			gap = true
		}
		if p.right != nil {
			if gap {
				return false
			}
			nodes = append(nodes, p.right)
		} else {
			gap = true
		}
	}
	return true
}

func (t *CompleteTree) Size() int { return t.count }

func (t *CompleteTree) LevelValues() []string {
	if t.root == nil {
		return nil
	}
	nodes := []*TNode{t.root}
	var values []string
	for i := 0; i < len(nodes); i++ {
		p := nodes[i]
		values = append(values, p.value)
		if p.left != nil {
			nodes = append(nodes, p.left)
		}
		if p.right != nil {
			nodes = append(nodes, p.right)
		}
	}
	return values
}

func (t *CompleteTree) PreValues() []string {
	var values []string
	var walk func(*TNode)
	walk = func(p *TNode) {
		if p == nil {
			return
		}
		values = append(values, p.value)
		walk(p.left)
		walk(p.right)
	}
	walk(t.root)
	return values
}
func (t *CompleteTree) InValues() []string {
	var values []string
	var walk func(*TNode)
	walk = func(p *TNode) {
		if p == nil {
			return
		}
		walk(p.left)
		values = append(values, p.value)
		walk(p.right)
	}
	walk(t.root)
	return values
}
func (t *CompleteTree) PostValues() []string {
	var values []string
	var walk func(*TNode)
	walk = func(p *TNode) {
		if p == nil {
			return
		}
		walk(p.left)
		walk(p.right)
		values = append(values, p.value)
	}
	walk(t.root)
	return values
}
func (t *CompleteTree) PrintPre()   { printValues(t.PreValues()) }
func (t *CompleteTree) PrintIn()    { printValues(t.InValues()) }
func (t *CompleteTree) PrintPost()  { printValues(t.PostValues()) }
func (t *CompleteTree) PrintLevel() { printValues(t.LevelValues()) }
