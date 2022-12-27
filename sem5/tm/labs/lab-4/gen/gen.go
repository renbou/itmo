package gen

import (
	"fmt"

	"ll1gen/grammar"
)

// GenerateAll generates both the lexer and parser.
func GenerateAll(g grammar.Grammar, pkg string) (string, error) {
	tokens := newTokenSet(g.LexTokens)

	lexer, err := generateLexer(tokens)
	if err != nil {
		return "", fmt.Errorf("generating lexer: %w", err)
	}

	header := "package " + pkg + "\n"
	return header + lexer, nil
}
