package gen

import (
	"bytes"
	"fmt"
	"regexp"
	"text/template"

	"ll1gen/grammar"

	"github.com/samber/lo"
)

const (
	whitespaceRegExp = `^\s+`
	eofTokenID       = -1
)

var lexerTmpl = template.Must(template.New("lexer").Parse(`
type token struct {
	id int
	value string
}

var (
	tokenRegExps = []*regexp.Regexp{
	{{range $id, $t := .tokens}}{{printf "\tregexp.MustCompile(%s),\n\t" $t}}{{end}}}

	whitespaceRegExp = regexp.MustCompile({{.ws}})
)

func lex(text string) ([]token, error) {
	var (
		tokens   []token
		leftover = text
		matched  = true
	)

	for matched {
		ws := whitespaceRegExp.FindStringIndex(leftover)
		if ws != nil {
			leftover = leftover[ws[1]:]
			continue
		}

		var (
			maxLen   int
			maxToken *token
		)

		for id, re := range tokenRegExps {
			m := re.FindString(leftover)
			if len(m) > maxLen {
				maxLen = len(m)
				maxToken = &token{
					id:    id,
					value: m,
				}
			}
		}

		if maxToken != nil {
			tokens = append(tokens, *maxToken)
			leftover = leftover[maxLen:]
		}
		matched = maxToken != nil
	}

	if leftover != "" {
		return nil, fmt.Errorf("unmatched suffix %s left over", strconv.Quote(leftover))
	}
	return tokens, nil
}
`))

type tokenSet struct {
	list     []string
	literals map[string]int
	named    map[string]int
}

func generateLexer(tokens *tokenSet) ([]byte, error) {
	var b bytes.Buffer

	// Validate regular expressions for all of the possible tokens.
	for _, t := range tokens.list {
		_, err := regexp.Compile(t)
		if err != nil {
			return nil, fmt.Errorf("compiling token regexp (%s): %w", t, err)
		}
	}

	// Prepare regexp for the template.
	tokenRegExps := lo.Map(tokens.list, func(s string, _ int) string {
		return "`^" + s + "`"
	})

	if err := lexerTmpl.Execute(&b, map[string]any{"tokens": tokenRegExps, "ws": "`" + whitespaceRegExp + "`"}); err != nil {
		return nil, err
	}

	return b.Bytes(), nil
}

func newTokenSet(tokens grammar.LexTokens) *tokenSet {
	set := &tokenSet{
		literals: make(map[string]int),
		named:    make(map[string]int),
	}

	for _, l := range tokens.Literals {
		set.literals[l] = len(set.list)
		set.list = append(set.list, regexp.QuoteMeta(l))
	}

	for n, r := range tokens.RegExps {
		set.named[n] = len(set.list)
		set.list = append(set.list, r)
	}

	return set
}

func (s *tokenSet) componentToID(c grammar.ParseRuleComponent) int {
	if c.Type == grammar.ParseRuleComponentToken {
		return s.named[c.Value]
	}
	return s.literals[c.Value]
}
