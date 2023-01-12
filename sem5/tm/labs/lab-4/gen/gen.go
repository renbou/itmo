package gen

import (
	"bytes"
	"fmt"

	"mvdan.cc/gofumpt/format"

	"ll1gen/grammar"
)

// GenerateAll generates both the lexer and parser.
func GenerateAll(g grammar.Grammar, pkg string) ([]byte, error) {
	tokens := newTokenSet(g.LexTokens)

	first := rulesFirst(g.ParseRules, tokens)
	if err := firstFirstConflict(first); err != nil {
		return nil, err
	}

	follow := nontermFollow(g.ParseRules, g.StartNonTerminal, tokens, first)
	if err := firstFollowConflict(first, follow); err != nil {
		return nil, err
	}

	lexer, err := generateLexer(tokens)
	if err != nil {
		return nil, fmt.Errorf("generating lexer: %w", err)
	}

	parser, err := generateParser(g.ParseRules, first, follow, tokens)
	if err != nil {
		return nil, fmt.Errorf("generating parser: %w", err)
	}

	return constructFile(pkg, lexer, parser)
}

func constructFile(pkg string, lexer, parser []byte) ([]byte, error) {
	var b bytes.Buffer
	b.WriteString("package " + pkg + "\n")
	b.WriteString(`import (
		"errors"
		"fmt"
		"regexp"
		"strconv"
	)`)
	b.Write(lexer)
	b.Write(parser)

	formatted, err := format.Source(b.Bytes(), format.Options{})
	if err != nil {
		fmt.Printf("Warning: cannot format file: %s\n", err)
		formatted = b.Bytes()
	}
	return formatted, nil
}
