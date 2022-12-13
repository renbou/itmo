package main

import (
	"fmt"

	"prefixexpr/grammar"

	"github.com/antlr/antlr4/runtime/Go/antlr/v4"
)

func main() {
	is := antlr.NewInputStream("if = kek || < 1 * 4 7 == true && abobus ! false print + 3 kek if < 1 2 if true print 1337")
	lexer := grammar.NewPrExprLexer(is)
	stream := antlr.NewCommonTokenStream(lexer, antlr.TokenDefaultChannel)
	parser := grammar.NewPrExprParser(stream)

	fmt.Println(parser.PrefixExpr().GetRepr())
}
