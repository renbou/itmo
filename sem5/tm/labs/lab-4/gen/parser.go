package gen

import (
	"bytes"
	"fmt"
	"sort"
	"strconv"
	"strings"
	"text/template"

	"ll1gen/grammar"

	"github.com/samber/lo"
)

var parserTmpl = template.Must(template.New("parser").Parse(`
{{ $ffsetKeys := .ffsetKeys }}
{{ $ffsetContainsE := .ffsetContainsE }}
{{ $joinIDs := .joinIDs }}
{{ $rules := .rules }}
{{ $tokens := .tokens }}
{{ $follow := .follow }}
{{ $attributes := .attributes }}
{{ $code := .code }}

var ErrNotExhausted = errors.New("not all tokens have been exhausted after parsing")

type parser struct {
	tokens []token
	pos    int
}

func (p *parser) token() token {
	return p.tokens[p.pos]
}

func (p *parser) contin() {
	p.pos++
}

func (p *parser) errUnexpected(node string) error {
	return fmt.Errorf("unexpected %s in %s node", p.token().value, node)
}

{{range $name, $fs := .first}}
type ParserNode{{$name}} struct {
	children []any
	{{ index $attributes $name }}
}

func (node *ParserNode{{$name}}) Name() string {
	return "{{$name}}"
}

func (node *ParserNode{{$name}}) Children() []any {
	return node.children
}

func (p *parser) parse{{$name}}() (*ParserNode{{$name}}, error) {
	var children []any

	switch p.token().id {
	{{- range $ruleIndex, $ruleFirst := $fs }}
		{{- $rule := index $rules $name $ruleIndex }}
		{{- $containsE := call $ffsetContainsE $ruleFirst }}
		{{- if not $containsE }}
			{{- $transitions := call $ffsetKeys $ruleFirst }}
			{{- $joinIDs := call $joinIDs $transitions }}
			case {{ $joinIDs }}:
				{{- range $component := $rule.Components }}
					{{- if eq $component.Type 0 }}
						if p.token().id != {{ index $tokens.Literals $component.Value }} {
							return nil, p.errUnexpected("{{$name}}")
						} else {
							children = append(children, p.token().value)
						}
						p.contin()
					{{- else if eq $component.Type 1 }}
						if p.token().id != {{ index $tokens.Named $component.Value }} {
							return nil, p.errUnexpected("{{$name}}")
						} else {
							children = append(children, p.token().value)
						}
						p.contin()
					{{- else }}
						if child, err := p.parse{{$component.Value}}(); err != nil {
							return nil, err
						} else {
							children = append(children, child)
						}
					{{- end }}
				{{- end }}

				node := &ParserNode{{$name}}{children: children}

				{{ call $code $rule }}

				return node, nil
		{{- else }}
			{{- $nontermFollow := index $follow $name }}
			{{- $transitions := call $ffsetKeys $nontermFollow }}
			{{- $joinIDs := call $joinIDs $transitions }}
			case {{ $joinIDs }}:
				node := &ParserNode{{$name}}{}

				{{ call $code $rule }}

				return node, nil
		{{- end }}
	{{- end }}
	default:
		return nil, p.errUnexpected("{{$name}}")
	}
}

func Parse{{$name}}(text string) (*ParserNode{{$name}}, error) {
	tokens, err := lex(text)
	if err != nil {
		return nil, err
	}

	p := parser{tokens, 0}

	result, err := p.parse{{$name}}()
	if err != nil {
		return nil, err
	}

	if p.pos != len(tokens) - 1 {
		return nil, ErrNotExhausted
	}
	return result, nil
}
{{end}}
`))

func generateParser(rules grammar.ParseRules, first map[string][]ffset, follow map[string]ffset, tokens *tokenSet) ([]byte, error) {
	var b bytes.Buffer

	err := parserTmpl.Execute(&b, map[string]any{
		"ffsetKeys": lo.Keys[int, struct{}],
		"joinIDs": func(ids []int) string {
			return strings.Join(lo.Map(ids, func(id int, _ int) string {
				return strconv.Itoa(id)
			}), ", ")
		},
		"ffsetContainsE": func(f ffset) bool {
			return f.containsE()
		},
		"rules":      rules,
		"first":      first,
		"follow":     follow,
		"tokens":     tokens,
		"attributes": attributes(rules),
		"code":       code,
	})
	if err != nil {
		return nil, fmt.Errorf("executing parser template: %w", err)
	}

	return b.Bytes(), nil
}

// attributes extracts and groups the nonterminal attributes from the grammar
func attributes(r grammar.ParseRules) map[string]string {
	return lo.MapValues(r, func(rules []grammar.ParseRule, _ string) string {
		return strings.Join(lo.Map(rules, func(rule grammar.ParseRule, _ int) string {
			return rule.Attributes
		}), "\n")
	})
}

// code extracts and builds the code for this parse rule by replacing $0 with node
// and $n with the n'th child
func code(r grammar.ParseRule) string {
	c := strings.ReplaceAll(r.Code, "$0", "node")

	for i, component := range r.Components {
		result := fmt.Sprintf("node.children[%d]", i)

		if component.Type == grammar.ParseRuleComponentRule {
			result += fmt.Sprintf(".(*ParserNode%s)", component.Value)
		}
		c = strings.ReplaceAll(c,
			fmt.Sprintf("$%d", i+1),
			result,
		)
	}

	return strings.TrimSpace(c)
}

// rulesFirst calculates the FIRST sets for all of the production rules,
// returning them in the same format as the parser rules
func rulesFirst(r grammar.ParseRules, tokens *tokenSet) map[string][]ffset {
	f := make(map[string][]ffset)

	// Empty sets by default
	for name, rules := range r {
		f[name] = make([]ffset, len(rules))
		for i := range rules {
			f[name][i] = make(ffset)
		}
	}

	changed := true
	for changed {
		changed = false

		for name, rules := range r {
			for i, rule := range rules {
				// Empty rule == EPS
				if len(rule.Components) == 0 {
					changed = f[name][i].add(eofTokenID) || changed
					continue
				}

				for ci, component := range rule.Components {
					// A terminal instantly goes into the FIRST set
					if component.Type == grammar.ParseRuleComponentLiteral || component.Type == grammar.ParseRuleComponentToken {
						changed = f[name][i].add(tokens.componentToID(component)) || changed
						break
					}

					// Compute the current FIRST set for the encountered non-terminal
					nontermF := nontermFirst(f, component.Value)

					// Add everything from its set to ours, except EPS
					changed = f[name][i].uniteWith(nontermF.withoutE()) || changed

					// If it doesn't contain an EPS, then we definitely encountered a terminal
					if !nontermF.containsE() {
						break
					}

					// All of the components were non-terminals which contained EPS, meaning this rule can also contain EPS
					if ci == len(rule.Components)-1 {
						changed = f[name][i].add(eofTokenID) || changed
					}
				}
			}
		}
	}

	debugFirst(f, tokens)
	return f
}

// nontermFirst returns the current FIRST set for the specified non-terminal
func nontermFirst(f map[string][]ffset, name string) ffset {
	set := make(ffset)
	for _, ruleSet := range f[name] {
		set.uniteWith(ruleSet)
	}
	return set
}

func debugFirst(f map[string][]ffset, tokens *tokenSet) {
	fmt.Println("FIRST for rules:")
	ruleNames := lo.Keys(f)
	sort.Strings(ruleNames)
	for _, name := range ruleNames {
		for _, rulef := range f[name] {
			terminals := lo.MapToSlice(rulef, func(s int, _ struct{}) string {
				if s < 0 {
					return "EPS"
				}
				return tokens.list[s]
			})
			sort.Strings(terminals)

			fmt.Printf("  %s = %s\n", name, strings.Join(terminals, ", "))
		}
	}
}

// nontermFollow calculates the FOLLOW set of all non-terminals
func nontermFollow(r grammar.ParseRules, start string, tokens *tokenSet, first map[string][]ffset) map[string]ffset {
	// Empty sets by default
	f := make(map[string]ffset)
	for name := range r {
		f[name] = make(ffset)
	}

	// EOF is always reachable from the start
	f[start].add(eofTokenID)

	changed := true
	for changed {
		changed = false

		for name, rules := range r {
			for _, rule := range rules {
				for ci, component := range rule.Components {
					// Only inspect sequences like ...Bβ
					if component.Type != grammar.ParseRuleComponentRule {
						continue
					}

					beta := rule.Components[ci+1:]
					cf := f[component.Value]

					// If this non-terminal is at the end of some rule, we need to add this rule's FOLLOW set
					if len(beta) == 0 {
						changed = cf.uniteWith(f[name]) || changed
						continue
					}

					for betaCI, betaComponent := range beta {
						// A terminal's FIRST set is equal to itself, so it instantly goes into the FOLLOW set
						if betaComponent.Type == grammar.ParseRuleComponentLiteral || betaComponent.Type == grammar.ParseRuleComponentToken {
							changed = cf.add(tokens.componentToID(betaComponent)) || changed
							break
						}

						// Always add non-terminal's FIRST \ EPS
						bfirst := nontermFirst(first, betaComponent.Value)
						changed = cf.uniteWith(bfirst.withoutE()) || changed

						// If beta consists solely of non-terminals with EPS',
						// FIRST(beta) contains EPS, so we need to add this rule's FOLLOW
						if !bfirst.containsE() {
							break
						}
						if betaCI == len(beta)-1 {
							changed = cf.uniteWith(f[name]) || changed
						}
					}
				}
			}
		}
	}

	debugFollow(f, tokens)
	return f
}

func debugFollow(f map[string]ffset, tokens *tokenSet) {
	fmt.Println("FOLLOW for non-terminals:")
	ruleNames := lo.Keys(f)
	sort.Strings(ruleNames)
	for _, name := range ruleNames {
		terminals := lo.MapToSlice(f[name], func(s int, _ struct{}) string {
			if s < 0 {
				return "EOF"
			}
			return tokens.list[s]
		})
		sort.Strings(terminals)
		fmt.Printf("  %s = %s\n", name, strings.Join(terminals, ", "))
	}
}

func firstFirstConflict(first map[string][]ffset) error {
	for name, fs := range first {
		total := make(ffset)

		for _, f := range fs {
			intersect := lo.SomeBy(lo.Keys(f), total.contains)
			if intersect {
				return fmt.Errorf("FIRST/FIRST conflict between rules for %s", name)
			}

			total.uniteWith(f)
		}
	}

	return nil
}

func firstFollowConflict(first map[string][]ffset, follow map[string]ffset) error {
	for name, fs := range first {
		someRuleHasE := lo.SomeBy(first[name], func(f ffset) bool {
			return f.containsE()
		})

		if !someRuleHasE {
			continue
		}
		for _, f := range fs {
			intersect := lo.SomeBy(lo.Keys(f), func(id int) bool {
				// follow set can definitely not contains EPS, but we mark them the same
				if id == eofTokenID {
					return false
				}

				return follow[name].contains(id)
			})
			if intersect {
				return fmt.Errorf("FIRST/FOLLOW conflict between rules for %s", name)
			}
		}
	}

	return nil
}
