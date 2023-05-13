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
	inputFileName = "height.in"
	outputFileName = "height.out"
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

func DepthDFS_(nodes []*BstNode, v, accum int) int {
	var leftChildDepth, rightChildDepth, depth int

	if nodes[v].Left != -1 {
		leftChildDepth = DepthDFS_(nodes, nodes[v].Left, accum)
	}
	if nodes[v].Right != -1 {
		rightChildDepth = DepthDFS_(nodes, nodes[v].Right, accum)
	}

	if leftChildDepth > rightChildDepth {
		depth = leftChildDepth
	} else {
		depth = rightChildDepth
	}
	return depth + 1
}

func DepthDFS(nodes []*BstNode, v int) int {
	return DepthDFS_(nodes, v, 0)
}

func main() {
	defer bufferedOutputWriter.Flush()

	var (
		n, depth int
		root = -1
	)
	scanf("%d\n", &n)

	if n == 0 || n == 1 {
		depth = n
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

		depth = DepthDFS(nodes, root)
	}

	printf("%d\n", depth)
}