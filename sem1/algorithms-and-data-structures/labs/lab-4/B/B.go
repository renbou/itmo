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
	inputFileName = "queue.in"
	outputFileName = "queue.out"
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
	var front = 0
	var back = 0

	var n int
	scanf("%d\n", &n)
	for i := 0; i < n; i++ {
		var cmd byte
		scanf("%c", &cmd)
		if cmd == '+' {
			var num int
			scanf("%d\n", &num)
			array[back] = num
			back = (back + 1) % MAXN
		} else {
			scanf("\n")
			printf("%d\n", array[front])
			front = (front + 1) % MAXN
		}
	}
}
