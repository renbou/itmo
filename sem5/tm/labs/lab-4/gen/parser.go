package gen

import (
	"fmt"
	"sort"
	"strings"

	"ll1gen/grammar"

	"github.com/samber/lo"
)

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
				if len(rule) == 0 {
					changed = f[name][i].add(eofTokenID) || changed
					continue
				}

				for ci, component := range rule {
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
					if ci == len(rule)-1 {
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
				for ci, component := range rule {
					// Only inspect sequences like ...Bβ
					if component.Type != grammar.ParseRuleComponentRule {
						continue
					}

					beta := rule[ci+1:]
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
							continue
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
