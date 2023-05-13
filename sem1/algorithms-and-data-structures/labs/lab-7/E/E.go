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
	inputFileName = "avlset.in"
	outputFileName = "avlset.out"
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

func getBalance(node *Node) int64 {
	if node == nil {
		return 0
	} else {
		return node.getBalance()
	}
}

func main() {
	defer bufferedOutputWriter.Flush()

	var (
		n int
		choice byte
		value int64
	)
	scanf("%d\n", &n)
	avl := &AVLTree{}

	for i := 0; i < n; i++ {
		scanf("%c %d\n", &choice, &value)
		switch choice {
		case 'A':
			avl.Insert(value)
			printf("%d\n", getBalance(avl.root))
		case 'D':
			avl.Delete(value)
			printf("%d\n", getBalance(avl.root))
		case 'C':
			if avl.Find(value) {
				printf("Y\n")
			} else {
				printf("N\n")
			}
		default:
			panic("what")
		}
	}
}