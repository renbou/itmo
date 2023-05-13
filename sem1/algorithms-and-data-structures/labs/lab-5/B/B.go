package main

import (
	"bufio"
	"fmt"
	"os"
)

var (
	bufferedInputReader = bufio.NewReader(os.Stdin)
	bufferedOutputWriter = bufio.NewWriter(os.Stdout)
	printf = func (format string, valist ...interface{}) {
		fmt.Fprintf(bufferedOutputWriter, format, valist...) }
	scanf = func (format string, valist ...interface{}) {
		fmt.Fscanf(bufferedInputReader, format, valist...) }
)

const (
	inputFileName = "check.in"
	outputFileName = "check.out"
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

type BstNode struct {
	Key int
	Left, Right int
}

func CheckBST(nodes []*BstNode, v, left, right int) bool {
	var (
		leftResult, rightResult, result = true, true, true
	)

	result = left < nodes[v].Key && nodes[v].Key < right

	if result && nodes[v].Left != -1 {
		leftResult = CheckBST(nodes, nodes[v].Left, left, nodes[v].Key)
	}
	if result && leftResult && nodes[v].Right != -1 {
		rightResult = CheckBST(nodes, nodes[v].Right, nodes[v].Key, right)
	}

	return result && leftResult && rightResult
}

func main() {
	defer bufferedOutputWriter.Flush()

	var (
		answer = "NO"
		n int
		root = -1
	)
	scanf("%d\n", &n)

	if n == 0 || n == 1 {
		answer = "YES"
	} else {
		var nodes = make([]*BstNode, n)

		// nodes that are children of some other nodes, the root node is the only one which will have false here
		var children = make([]bool, n)

		for i := 0; i < n; i++ {
			var k, l, r int
			scanf("%d %d %d\n", &k, &l, &r)
			l--; r--
			nodes[i] = &BstNode{
				Key:   k,
				Left:  l,
				Right: r,
			}

			if l != -1 {
				children[l] = true
			}
			if r != -1 {
				children[r] = true
			}
		}

		for i := 0; i < n && root == -1; i++ {
			if !children[i] {
				root = i
			}
		}

		if CheckBST(nodes, root, -2000000000, 2000000000) {
			answer = "YES"
		}
	}

	printf("%s\n", answer)
}