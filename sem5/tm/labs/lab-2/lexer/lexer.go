package lexer

import (
	"fmt"
	"unicode"
)

type lexer struct {
	input []rune
	pos   int
}

// Lex splits the input into parsed tokens and throws an error if an unknown token is found.
func Lex(input string) ([]Token, error) {
	l := lexer{
		input: []rune(input),
		pos:   0,
	}

	var tokens []Token
	for l.skipWS(); l.pos < len(l.input); l.skipWS() {
		token, err := l.nextToken()
		if err != nil {
			return nil, fmt.Errorf("failed to parse: %w on position %d", err, l.pos)
		}

		tokens = append(tokens, token)
	}

	return append(tokens, TokenEnd), nil
}

func (l *lexer) nextToken() (Token, error) {
	ch := l.input[l.pos]

	// First check if the lexeme is a simple character token
	singleChTokens := map[rune]Token{
		'(': TokenLParen,
		')': TokenRParen,
		',': TokenComma,
		':': TokenColon,
	}
	if token, ok := singleChTokens[ch]; ok {
		l.pos++
		return token, nil
	}

	// All other lexemes are non-whitespace character sequences
	start := l.pos
	l.skipIdent()
	end := l.pos
	lexeme := l.input[start:end]

	stringTokens := map[string]Token{
		"lambda": TokenLambda,
		"or":     TokenOr,
		"and":    TokenAnd,
		"not":    TokenNot,
		"True":   TokenTrue,
		"False":  TokenFalse,
	}
	if token, ok := stringTokens[string(lexeme)]; ok {
		return token, nil
	}

	if !isPythonIdentifier(lexeme) {
		return -1, ErrInvalidIdentifier
	}
	return TokenIdent, nil
}

func (l *lexer) skipIdent() {
	l.skipFunc(validPythonIdentifierChar)
}

func (l *lexer) skipWS() {
	l.skipFunc(unicode.IsSpace)
}

func (l *lexer) skipFunc(f func(rune) bool) {
	for l.pos < len(l.input) && f(l.input[l.pos]) {
		l.pos++
	}
}
