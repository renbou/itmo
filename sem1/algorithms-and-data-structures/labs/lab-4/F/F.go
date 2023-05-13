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
	inputFileName = "garland.in"
	outputFileName = "garland.out"
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

	var n int
	var a1 float64
	scanf("%d %f\n", &n, &a1)

	var (
		l float64 = 0
		r float64 = a1
		last float64
	)

	for i := 0; i < 100; i++ {
		second := (l + r) / float64(2.0)
		pp, p := a1, second
		var good = true
		for j := 2; j < n; j++ {
			cur := 2 * p - pp + float64(2.0)
			if cur < 0 {
				good = false
				break
			}
			if j == n - 1 {
				last = cur
			}
			pp, p = p, cur
		}

		if good {
			r = second
		} else {
			l = second
		}
	}

	printf("%.9f\n", last)
}