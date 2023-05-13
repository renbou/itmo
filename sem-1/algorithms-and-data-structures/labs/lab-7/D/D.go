package main

import (
	"bufio"
	"fmt"
	"os"
)

var (
	bufferedInputReader = bufio.NewReader(os.Stdin)
	bufferedOutputWriter = bufio.NewWriter(os.Stdout)
	printf = func (format string, valist ...interface{}) (int, error) {
		return fmt.Fprintf(bufferedOutputWriter, format, valist...) }
	scanf = func (format string, valist ...interface{}) (int, error) {
		return fmt.Fscanf(bufferedInputReader, format, valist...) }
)

type Node struct {
	Key int64
	Index int
	Height int64
	Left, Right, Parent *Node
}

type AVLTree struct {
	root *Node
}

func getHeight (node *Node) int64 {
	if node != nil {
		return node.Height
	}
	return 0;
}

func (node *Node) updateHeight () {
	leftHeight := getHeight(node.Left)
	rightHeight := getHeight(node.Right)
	if leftHeight > rightHeight {
		node.Height = leftHeight + 1
	} else {
		node.Height = rightHeight + 1
	}
}

func (node *Node) getBalance() int64 {
	return getHeight(node.Right) - getHeight(node.Left)
}

func (avl *AVLTree) Find(key int64) bool {
	current := avl.root
	for current != nil {
		if key < current.Key {
			current = current.Left
		} else if key > current.Key {
			current = current.Right
		} else {
			return true
		}
	}
	return false
}

func (node *Node) leftRotate() *Node {
	rightChild := node.Right
	rightLeftChild := rightChild.Left

	rightChild.Left = node
	node.Right = rightLeftChild

	node.updateHeight()
	rightChild.updateHeight()

	return rightChild
}

func (node *Node) rightRotate() *Node {
	leftChild := node.Left
	leftRightChild := leftChild.Right

	leftChild.Right = node
	node.Left = leftRightChild

	node.updateHeight()
	leftChild.updateHeight()

	return leftChild
}

func (node *Node) Rebalance() *Node {
	balance := node.getBalance()
	// Right child height - left child height = 2
	if balance > 1 {
		rightBalance := node.Right.getBalance()
		// right-left case
		if rightBalance < 0 {
			node.Right = node.Right.rightRotate()
			return node.leftRotate()
		} else {
		// right-right case
			return node.leftRotate()
		}
	} else if balance < -1 {
		leftBalance := node.Left.getBalance()
		// left-right case
		if leftBalance > 0 {
			node.Left = node.Left.leftRotate()
			return node.rightRotate()
		} else {
		// left-left case
			return node.rightRotate()
		}
	}
	return node
}

func insert(node *Node, key int64) *Node {
	if node == nil {
		return &Node{
			Key:    key,
			Height: 1,
		}
	}

	if key < node.Key {
		node.Left = insert(node.Left, key)
	} else if key > node.Key {
		node.Right = insert(node.Right, key)
	} else {
		return node
	}

	node.updateHeight()
	return node.Rebalance()
}

func delete(node *Node, key int64) *Node {
	if node == nil {
		return nil
	}

	if key < node.Key {
		node.Left = delete(node.Left, key)
	} else if key > node.Key {
		node.Right = delete(node.Right, key)
	} else {
		if node.Left == nil && node.Right == nil {
			node = nil
		} else if node.Left == nil {
			node = node.Right
		} else {
			smallest := node.Left
			for smallest.Right != nil {
				smallest = smallest.Right
			}
			node.Key = smallest.Key
			node.Left = delete(node.Left, smallest.Key)
		}
	}

	if node == nil {
		return nil
	}

	node.updateHeight()
	return node.Rebalance()
}

func (avl *AVLTree) Insert(key int64) {
	avl.root = insert(avl.root, key)
}

func (avl *AVLTree) Delete(key int64) {
	avl.root = delete(avl.root, key)
}

const (
	inputFileName = "deletion.in"
	outputFileName = "deletion.out"
)

var fileInputMode = true
func init() {
	if (fileInputMode) {
		inputFile, err := os.Open(inputFileName)
		if err != nil {
			fmt.Printf(err.Error())
			os.Exit(-1)
		}
		outputFile, err := os.Create(outputFileName)
		if err != nil {
			fmt.Printf(err.Error())
			os.Exit(-1)
		}
		bufferedInputReader = bufio.NewReader(inputFile)
		bufferedOutputWriter = bufio.NewWriter(outputFile)
	}
}

type entry struct {
	key int64
	left, right int
}

func reindex(node *Node, index *int) {
	if node == nil {
		return
	}
	node.Index = *index
	*index++
	reindex(node.Left, index)
	reindex(node.Right, index)
}

func print(node *Node) {
	if node == nil {
		return
	}
	var (
		leftIndex = 0
		rightIndex = 0
	)
	if node.Left != nil {
		leftIndex = node.Left.Index
	}
	if node.Right != nil {
		rightIndex = node.Right.Index
	}
	printf("%d %d %d\n", node.Key, leftIndex, rightIndex)
	print(node.Left)
	print(node.Right)
}

func main() {
	defer bufferedOutputWriter.Flush()

	var n int
	scanf("%d\n", &n)

	entries := make([]entry, n)
	nodes := make([]*Node, n)
	for i := 0; i < n; i++ {
		scanf("%d %d %d\n", &entries[i].key, &entries[i].left, &entries[i].right)
		nodes[i] = &Node{Key: entries[i].key, Height: 1}
	}
	for i := n - 1; i >= 0; i-- {
		if entries[i].left != 0 {
			nodes[i].Left = nodes[entries[i].left - 1]
		}
		if entries[i].right != 0 {
			nodes[i].Right = nodes[entries[i].right - 1]
		}
		nodes[i].updateHeight()
	}
	avl := &AVLTree{}
	if n > 0 {
		avl.root = nodes[0]
	}

	var deleteKey int
	scanf("%d\n", &deleteKey)

	avl.Delete(int64(deleteKey))
	index := 1
	reindex(avl.root, &index)
	printf("%d\n", n - 1)
	print(avl.root)
}