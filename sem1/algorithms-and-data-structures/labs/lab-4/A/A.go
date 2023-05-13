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
	inputFileName = "stack.in"
	outputFileName = "stack.out"
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

const MAXN = 1e6

func main() {
	defer bufferedOutputWriter.Flush()

	var array [MAXN]int
	var ptr = 0

	var n int
	scanf("%d\n", &n)
	for i := 0; i < n; i++ {
		var cmd byte
		scanf("%c", &cmd)
		if cmd == '+' {
			var num int
			scanf("%d\n", &num)
			array[ptr] = num
			ptr++
		} else {
			scanf("\n")
			printf("%d\n", array[ptr - 1])
			ptr--
		}
	}
}
