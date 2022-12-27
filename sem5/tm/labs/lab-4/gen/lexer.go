package gen

import (
	"fmt"
	"regexp"
	"strconv"
	"strings"
	"text/template"

	"ll1gen/grammar"

	"github.com/samber/lo"
)

var lexerTmpl = template.Must(template.New("lexer").Parse(`
type token struct {
	id int
	value string
}

var (
	tokenRe = {{.tokenRe}}
	tokenReGroupToID = map[int]int{
	{{range $id, $group := .groupIndices}}{{printf "\t%d: %d,\n\t" $group $id}}{{end}}}
)
`))

type tokenSet struct {
	list     []string
	literals map[string]int
	named    map[string]int
}

func generateLexer(tokens *tokenSet) (string, error) {
	var b strings.Builder

	// Construct regular expression for all of the possible tokens.
	groups := lo.Map(tokens.list, func(s string, i int) string {
		return fmt.Sprintf("(?P<token%d>^%s)", i, s)
	})
	r := strings.Join(groups, "|")

	re, err := regexp.Compile(r)
	if err != nil {
		return "", fmt.Errorf("compiling lexer regexp (%s): %w", r, err)
	}

	// Get indices of all groups, since token patterns may contain nested groups
	groupIndices := make([]int, len(tokens.list))
	for i := range tokens.list {
		groupIndices[i] = re.SubexpIndex(fmt.Sprintf("token%d", i))
	}

	if err := lexerTmpl.Execute(&b, map[string]any{"tokenRe": strconv.Quote(r), "groupIndices": groupIndices}); err != nil {
		return "", err
	}

	return b.String(), nil
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
