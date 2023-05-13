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
	inputFileName = "sort.in"
	outputFileName = "sort.out"
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

func siftUp(array []int, n int) {
	for n > 0 {
		parent := n / 2
		if parent > n {
			array[parent], array[n] = array[n], array[parent]
		}
	}
}

func siftDown(array []int, i, n int) {
	var (
		cur = -1
		min = i
	)
	for min < n / 2 && cur != min {
		cur = min
		left, right := 2 * cur + 1, 2 * cur + 2
		if left < n && array[left] < array[min] {
			min = left
		}
		if right < n && array[right] < array[min] {
			min = right
		}
		array[min], array[cur] = array[cur], array[min]
	}
}

func heapify(array []int, n int) {
	for i := n / 2 - 1; i >= 0; i-- {
		siftDown(array, i, n)
	}
}

func heapPop(array []int, n int) {
	n--
	array[n], array[0] = array[0], array[n]
	siftDown(array, 0, n)
}

func main() {
	defer bufferedOutputWriter.Flush();

	var n int
	scanf("%d\n", &n)

	var array = make([]int, n)
	for i, _ := range array {
		scanf("%d", &array[i])
	}

	heapify(array, n)
	for sortN := n; sortN > 1; sortN-- {
		heapPop(array, sortN)
	}

	for ; n > 0; n-- {
		printf("%d ", array[n - 1])
	}
	printf("\n")
}