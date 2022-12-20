package main

import (
	"bufio"
	"fmt"
	"os"

	"prefixexpr/grammar"

	"github.com/antlr/antlr4/runtime/Go/antlr/v4"
)

func main() {
	scanner := bufio.NewScanner(os.Stdin)
	_ = scanner.Scan()
	text := scanner.Text()

	is := antlr.NewInputStream(text)
	lexer := grammar.NewPrExprLexer(is)
	stream := antlr.NewCommonTokenStream(lexer, antlr.TokenDefaultChannel)
	parser := grammar.NewPrExprParser(stream)

	fmt.Println(parser.PrefixExpr().GetRepr())
}
