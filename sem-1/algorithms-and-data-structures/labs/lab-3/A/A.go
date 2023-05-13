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
	inputFileName = "isheap.in"
	outputFileName = "isheap.out"
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

func main() {
	defer bufferedOutputWriter.Flush();

	var n int
	scanf("%d\n", &n)

	var array = make([]int, n)
	for i, _ := range array {
		scanf("%d", &array[i])
	}

	var isHeap = true
	for i := 0; i < n && isHeap; i++ {
		left, right := 2 * i + 1, 2 * i + 2
		if left < n && !(array[left] >= array[i]) {
			isHeap = false
		}
		if right < n && !(array[right] >= array[i]) {
			isHeap = false
		}
	}

	if isHeap {
		printf("YES\n")
	} else {
		printf("NO\n")
	}
}