package gen

import (
	"bytes"
	"fmt"
	"go/format"

	"ll1gen/grammar"
)

// GenerateAll generates both the lexer and parser.
func GenerateAll(g grammar.Grammar, pkg string) ([]byte, error) {
	tokens := newTokenSet(g.LexTokens)

	first := rulesFirst(g.ParseRules, tokens)
	_ = nontermFollow(g.ParseRules, g.StartNonTerminal, tokens, first)

	lexer, err := generateLexer(tokens)
	if err != nil {
		return nil, fmt.Errorf("generating lexer: %w", err)
	}

	return constructFile(pkg, lexer, nil)
}

func constructFile(pkg string, lexer, parser []byte) ([]byte, error) {
	var b bytes.Buffer
	b.WriteString("package " + pkg + "\n")
	b.WriteString(`import (
		"fmt"
		"regexp"
		"strconv"
	)`)
	b.Write(lexer)
	b.Write(parser)

	formatted, err := format.Source(b.Bytes())
	if err != nil {
		return nil, fmt.Errorf("formatting file: %w", err)
	}
	return formatted, nil
}
