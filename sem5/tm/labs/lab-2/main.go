package main

import (
	"bufio"
	"encoding/json"
	"fmt"
	"log"
	"os"

	"pylambda/lexer"
	"pylambda/parser"
)

func main() {
	scanner := bufio.NewScanner(os.Stdin)
	_ = scanner.Scan()
	text := scanner.Text()

	tokens, err := lexer.Lex(text)
	if err != nil {
		log.Fatalln("lexical analysis error", err)
	}

	tree, err := parser.Parse(tokens)
	if err != nil {
		log.Fatalln("syntactical analysis error", err)
	}

	data, _ := json.MarshalIndent(tree, "", "  ")
	fmt.Println(string(data))
}
