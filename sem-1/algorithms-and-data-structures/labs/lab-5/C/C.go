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

const (
	inputFileName = "bstsimple.in"
	outputFileName = "bstsimple.out"
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

type bstNode struct {
	Key int
	Left, Right, Parent *bstNode
}

type BST struct {
	root *bstNode
}

// helper function for finding the position of the new node, or returning that node if it already exists
func (bst *BST) findNewNodePosition(key int) (current, parent **bstNode) {
	current = &bst.root
	// iteratively search for the node in the tree
	for *current != nil {
		if key > (*current).Key {
			parent = current
			current = &(*current).Right
		} else if key < (*current).Key {
			parent = current
			current = &(*current).Left
		} else {
			// if we already have the required node, don't insert it again, returning a pointer to a valid node
			break
		}
	}
	return current, parent
}

func (bst *BST) Insert(key int) {
	var insertPosition, parentPosition = bst.findNewNodePosition(key)
	// we don't need to add an existing key
	if *insertPosition != nil {
		return
	}

	*insertPosition = &bstNode{
		Key: key,
	}
	if parentPosition != nil {
		(*insertPosition).Parent = *parentPosition
	}

}

func (bst *BST) Delete(key int) {
	var deletePosition, _ = bst.findNewNodePosition(key)
	// we don't need to delete a non-existing key
	if *deletePosition == nil {
		return
	}

	// if we can replace this node with just one child, then do it
	if (*deletePosition).Left == nil {
		if (*deletePosition).Right != nil {
			(*deletePosition).Right.Parent = (*deletePosition).Parent
		}
		*deletePosition = (*deletePosition).Right
	} else if (*deletePosition).Right == nil {
		if (*deletePosition).Left != nil {
			(*deletePosition).Left.Parent = (*deletePosition).Parent
		}
		*deletePosition = (*deletePosition).Left
	// otherwise insert it as the left child of the leftmost node in the right subtree
	} else {
		resultPositionParent := (*deletePosition).Right
		for resultPositionParent.Left != nil {
			resultPositionParent = resultPositionParent.Left
		}
		resultPositionParent.Left = (*deletePosition).Left
		(*deletePosition).Left.Parent = resultPositionParent

		(*deletePosition).Right.Parent = (*deletePosition).Parent
		*deletePosition = (*deletePosition).Right
	}
}

func (bst *BST) Exists(key int) bool {
	var nodePosition, _ = bst.findNewNodePosition(key)
	return (*nodePosition) != nil
}

func (bst *BST) Next(key int) (bool, int) {
	var nodePosition, parentPosition = bst.findNewNodePosition(key)

	// if the node is actually in the tree and the right child subtree isn't null,
	// then the successor is the minimum in the right child subtree
	if (*nodePosition) != nil && (*nodePosition).Right != nil {
		resultPositionParent := (*nodePosition).Right
		for resultPositionParent.Left != nil {
			resultPositionParent = resultPositionParent.Left
		}

		return true, resultPositionParent.Key
	// otherwise lets search in the parents, the successor will be the first parent,
	// which is a left child
	} else if parentPosition != nil && (*parentPosition) != nil {
		// check the current node and parent as well
		if key < (*parentPosition).Key {
			return true, (*parentPosition).Key
		}

		resultPosition := *parentPosition
		for resultPosition.Parent != nil && resultPosition.Parent.Right == resultPosition {
			resultPosition = resultPosition.Parent
		}

		// if we haven't found such a parent, then we are supposed to be the rightmost node
		if resultPosition.Parent == nil {
			return false, 0
		} else {
			return true, resultPosition.Parent.Key
		}
	}

	return false, 0
}

func (bst *BST) Previous(key int) (bool, int) {
	var nodePosition, parentPosition = bst.findNewNodePosition(key)

	// if the node is actually in the tree and the right child subtree isn't null,
	// then the successor is the minimum in the right child subtree
	if (*nodePosition) != nil && (*nodePosition).Left != nil {
		resultPositionParent := (*nodePosition).Left
		for resultPositionParent.Right != nil {
			resultPositionParent = resultPositionParent.Right
		}

		return true, resultPositionParent.Key
	// otherwise lets search in the parents, the successor will be the first parent,
	// which is a left child
	} else if parentPosition != nil && (*parentPosition) != nil {
		// check the current node and parent as well
		if key > (*parentPosition).Key {
			return true, (*parentPosition).Key
		}

		resultPosition := *parentPosition
		for resultPosition.Parent != nil && resultPosition.Parent.Left == resultPosition {
			resultPosition = resultPosition.Parent
		}

		// if we haven't found such a parent, then we are supposed to be the rightmost node
		if resultPosition.Parent == nil {
			return false, 0
		} else {
			return true, resultPosition.Parent.Key
		}
	}

	return false, 0
}

func main() {
	defer bufferedOutputWriter.Flush()

	var (
		request string
		key int
		bst BST
	)
	for _, err := scanf("%s %d\n", &request, &key); err == nil; _, err = scanf("%s %d\n", &request, &key) {
		switch request {
		case "insert":
			bst.Insert(key)
		case "delete":
			bst.Delete(key)
		case "exists":
			if bst.Exists(key) {
				printf("true\n")
			} else {
				printf("false\n")
			}
		case "next":
			exists, value := bst.Next(key)
			if exists {
				printf("%d\n", value)
			} else {
				printf("none\n")
			}
		case "prev":
			exists, value := bst.Previous(key)
			if exists {
				printf("%d\n", value)
			} else {
				printf("none\n")
			}
		}
	}

}