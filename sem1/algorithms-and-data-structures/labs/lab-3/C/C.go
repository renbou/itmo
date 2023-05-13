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
	inputFileName = "radixsort.in"
	outputFileName = "radixsort.out"
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

func countSort(array []string, n, exp int) []string {
	counts := make([]int, 256)
	result := make([]string, n)

	for _, cur := range array {
		counts[cur[exp]]++
	}

	for i := 1; i < 256; i++ {
		counts[i] += counts[i - 1]
	}

	for i := n - 1; i >= 0; i-- {
		elem := array[i]
		result[counts[elem[exp]] - 1] = elem
		counts[elem[exp]]--
	}

	return result
}

func main() {
	defer bufferedOutputWriter.Flush();

	var n, m, k int
	scanf("%d %d %d\n", &n, &m, &k)

	var array = make([]string, n)
	for i, _ := range array {
		scanf("%s\n", &array[i])
	}

	for i := 0; i < k; i++ {
		array = countSort(array, n, m - i - 1)
	}

	for _, elem := range array {
		printf("%s\n", elem)
	}
}