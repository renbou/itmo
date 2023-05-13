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
	inputFileName = "binsearch.in"
	outputFileName = "binsearch.out"
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
	defer bufferedOutputWriter.Flush()
	var n, m int
	scanf("%d\n", &n)
	var array = make([]int, n)
	for i := 0; i < n; i++ {
		scanf("%d", &array[i])
	}
	scanf("\n")
	scanf("%d\n", &m)

	for i := 0; i < m; i++ {
		var cur int
		scanf("%d", &cur)
		var (
			l = -1
			r = n - 1
		)
		for r - l > 1 {
			mid := (l + r) / 2
			if array[mid] >= cur {
				r = mid
			} else {
				l = mid
			}
		}

		if array[r] != cur {
			printf("-1 -1\n")
			continue
		}
		printf("%d ", r + 1)

		l, r = 0, n
		for r - l > 1 {
			mid := (l + r) / 2
			if array[mid] > cur {
				r = mid
			} else {
				l = mid
			}
		}
		printf("%d\n", l + 1)
	}
}