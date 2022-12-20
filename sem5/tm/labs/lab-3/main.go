package main

import (
	"bufio"
	"fmt"
	"os"

	"prefixexpr/grammar"

	"github.com/antlr/antlr4/runtime/Go/antlr/v4"
)

func translate(text string) string {
	is := antlr.NewInputStream(text)
	lexer := grammar.NewPrExprLexer(is)
	stream := antlr.NewCommonTokenStream(lexer, antlr.TokenDefaultChannel)
	parser := grammar.NewPrExprParser(stream)

	return parser.PrefixExpr().GetRepr()
}

func main() {
	scanner := bufio.NewScanner(os.Stdin)
	_ = scanner.Scan()
	text := scanner.Text()

	fmt.Println(translate(text))
}
