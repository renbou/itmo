package main

import (
	"flag"
	"fmt"
	"os"
	"sort"
	"strconv"
	"strings"

	"ll1gen/gen"
	"ll1gen/grammar"

	"github.com/samber/lo"
)

func main() {
	inputFile := flag.String("i", "", "Grammar input file.")
	outputFile := flag.String("o", "", "Output file for generated lexer and parser.")
	outputPackage := flag.String("p", "parser", "Generated file package.")
	flag.Parse()

	grammar, err := parseGrammarFile(*inputFile)
	if err != nil {
		failf("Parsing grammar file failed: %s\n", err)
	}

	if err := grammar.Validate(); err != nil {
		failf("Invalid grammar: %s\n", err)
	}

	fmt.Println("Grammar file parsed.")
	outputGrammar(grammar)

	result, err := gen.GenerateAll(grammar, *outputPackage)
	if err != nil {
		failf("Generation failed: %s\n", err)
	}

	if err := os.WriteFile(*outputFile, []byte(result), 0o644); err != nil {
		failf("Unable to save generated file to %s: %s", *outputFile, err)
	}
}

func failf(s string, args ...any) {
	fmt.Printf(s, args...)
	os.Exit(1)
}

func parseGrammarFile(file string) (grammar.Grammar, error) {
	data, err := os.ReadFile(file)
	if err != nil {
		return grammar.Grammar{}, fmt.Errorf("reading grammar file: %w", err)
	}

	return grammar.Parse(string(data))
}

func outputGrammar(g grammar.Grammar) {
	fmt.Printf(
		"Tokens:\n Literals: %s\n RegExps:\n",
		strings.Join(lo.Map(g.LexTokens.Literals, func(s string, _ int) string {
			return strconv.Quote(s)
		}), ", "),
	)

	tokenNames := lo.Keys(g.LexTokens.RegExps)
	sort.Strings(tokenNames)
	for _, name := range tokenNames {
		fmt.Printf("  %s = %s\n", name, strconv.Quote(g.LexTokens.RegExps[name]))
	}

	fmt.Println("Rules:")
	ruleNames := lo.Keys(g.ParseRules)
	sort.Strings(ruleNames)
	for _, name := range ruleNames {
		rules := g.ParseRules[name]
		for _, rule := range rules {
			fmt.Printf("  %s = %s\n", name, strings.Join(lo.Map(rule, func(c grammar.ParseRuleComponent, _ int) string {
				if c.Type == grammar.ParseRuleComponentLiteral {
					return strconv.Quote(c.Value)
				}
				return c.Value
			}), " "))
		}
	}
}
