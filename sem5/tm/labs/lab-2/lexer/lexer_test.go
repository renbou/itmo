package lexer

import (
	"testing"

	"github.com/stretchr/testify/assert"
)

func Test_lexer_skipWS(t *testing.T) {
	tests := []struct {
		s   string
		end int
	}{
		{"", 0},
		{"a", 0},
		{"  a  ", 2},
		{"\t \tbb", 3},
	}

	for _, tt := range tests {
		l := lexer{
			input: []rune(tt.s),
		}
		l.skipWS()

		assert.Equal(t, tt.end, l.pos)
	}
}

func Test_lexer_skipIdent(t *testing.T) {
	tests := []struct {
		s   string
		end int
	}{
		{"", 0},
		{"a  ", 1},
		{"  a  ", 0},
		{"\t \tbb", 0},
		{"aрусский\t \t", 8},
	}

	for _, tt := range tests {
		l := lexer{
			input: []rune(tt.s),
		}
		l.skipIdent()

		assert.Equal(t, tt.end, l.pos)
	}
}

func Test_Lex(t *testing.T) {
	tests := []struct {
		s      string
		tokens []Token
	}{
		{"", []Token{TokenEnd}},
		{"lambda:", []Token{TokenLambda, TokenColon, TokenEnd}},
		{"lambda \t:", []Token{TokenLambda, TokenColon, TokenEnd}},
		{"lambda a, b:", []Token{TokenLambda, TokenIdent, TokenComma, TokenIdent, TokenColon, TokenEnd}},
		{"lambda a :", []Token{TokenLambda, TokenIdent, TokenColon, TokenEnd}},
		{"lambda a,  \tb: a or not not b", []Token{
			TokenLambda, TokenIdent, TokenComma, TokenIdent, TokenColon,
			TokenIdent, TokenOr, TokenNot, TokenNot, TokenIdent,
			TokenEnd,
		}},
		{"lambda _ident: (True or (False and not _ident))", []Token{
			TokenLambda, TokenIdent, TokenColon,
			TokenLParen,
			TokenTrue, TokenOr,
			TokenLParen, TokenFalse, TokenAnd, TokenNot, TokenIdent, TokenRParen,
			TokenRParen,
			TokenEnd,
		}},
	}

	for _, tt := range tests {
		tokens, err := Lex(tt.s)
		if !assert.NoError(t, err) {
			continue
		}

		assert.Equal(t, tt.tokens, tokens)
	}
}
