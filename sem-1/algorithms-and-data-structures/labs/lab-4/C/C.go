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
	inputFileName = "brackets.in"
	outputFileName = "brackets.out"
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

const MAXN int = 1e4

func main() {
	defer bufferedOutputWriter.Flush()

	var array [MAXN]byte
	var ptr = 0
	var line string
	for _, err := scanf("%s", &line); err == nil; _, err = scanf("%s", &line) {
		var res = true
		ptr = 0
		for i := 0; i < len(line) && res; i++ {
			switch line[i] {
			case '(', '[':
				array[ptr] = line[i]
				ptr++
			case ')', ']':
				var rev byte
				switch line[i] {
				case ')':
					rev = '('
				case ']':
					rev = '['
				}
				if ptr < 1 || array[ptr-1] != rev {
					res = false
					break
				}
				ptr--
			}
		}
		scanf("\n")
		if !res || ptr != 0 {
			printf("NO\n")
		} else {
			printf("YES\n")
		}
	}
}