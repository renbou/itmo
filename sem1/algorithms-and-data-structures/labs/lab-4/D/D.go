package main

import (
	"bufio"
	"fmt"
	"os"
	"strconv"
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
	inputFileName = "postfix.in"
	outputFileName = "postfix.out"
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

type stack struct {
	end *token
}

type token struct {
	number int32
	prev_token *token
}

func (s *stack) pop() *token {
	if s.end == nil {
		return nil
	}
	result := s.end
	s.end = s.end.prev_token
	return result
}

func (s *stack) push(t *token) {
	if s.end == nil {
		s.end = t
		return
	}
	t.prev_token = s.end
	s.end = t
}

func main() {
	defer bufferedOutputWriter.Flush()
	var st = &stack{nil}
	var tok string
	for _, err := scanf("%s", &tok); err == nil; _, err = scanf("%s", &tok) {
		if tok[0] == '+' {
			b, a := st.pop(), st.pop()
			st.push(&token{
				number:     a.number + b.number,
				prev_token: nil,
			})
		} else if tok[0] == '-' {
			b, a := st.pop(), st.pop()
			st.push(&token{
				number:     a.number - b.number,
				prev_token: nil,
			})
		} else if tok[0] == '*' {
			b, a := st.pop(), st.pop()
			st.push(&token{
				number:     a.number * b.number,
				prev_token: nil,
			})
		} else {
			var value int
			value, _ = strconv.Atoi(tok)
			st.push(&token{
				number: int32(value) ,
				prev_token: nil,
			})
		}
	}
	result := st.pop()
	if result == nil {
		printf("0\n")
	} else {
		printf("%d\n", result.number)
	}
}