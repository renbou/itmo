package grammar

import (
	"errors"
	"fmt"
	"regexp"
	"strconv"

	"github.com/antlr/antlr4/runtime/Go/antlr/v4"
	"github.com/samber/lo"
)

// Grammar describes a fully parsed grammar.
type Grammar struct {
	LexTokens  LexTokens
	ParseRules ParseRules
}

// LexTokens describes a grammar's lexer tokens.
type LexTokens struct {
	Literals []string
	RegExps  map[string]string
	named    []namedToken
}

// namedToken describes a single token linked to an identifier. Token identifier begin with a lowercase letter.
type namedToken struct {
	name   string
	regexp string
}

// ParseRuleComponentType is an enum for the possible types of components of a single parse rule.
type ParseRuleComponentType int

const (
	// ParseRuleComponentLiteral represents a literal (ex: "literal")
	ParseRuleComponentLiteral ParseRuleComponentType = iota
	// ParseRuleComponentRule represents a named token (ex: named)
	ParseRuleComponentToken
	// ParseRuleComponentRule represents another rule (ex: OtherRule)
	ParseRuleComponentRule
)

// ParseRuleComponent describes a parse rule component.
type ParseRuleComponent struct {
	Type  ParseRuleComponentType
	Value string
}

// ParseRule describes a grammar's parse rule, consisting of multiple components one after the other.
type ParseRule []ParseRuleComponent

// ParseRules describes all parse rules of a grammar, consisting of multiple rules linked to names.
type ParseRules map[string][]ParseRule

// Parse parses grammar from the specified text.
func Parse(text string) (g Grammar, err error) {
	defer func() {
		r := recover()
		if r == nil {
			return
		}

		if e, ok := r.(error); ok {
			err = e
		} else {
			err = fmt.Errorf("panic while parsing grammar: %v", r)
		}
	}()

	errListener := panicErrorListener{antlr.NewDefaultErrorListener()}

	is := antlr.NewInputStream(text)
	lexer := NewLL1GrammarLexer(is)
	lexer.RemoveErrorListeners()
	lexer.AddErrorListener(errListener)

	stream := antlr.NewCommonTokenStream(lexer, antlr.TokenDefaultChannel)
	parser := NewLL1GrammarParser(stream)
	parser.RemoveErrorListeners()
	parser.AddErrorListener(errListener)

	g = parser.Ll1Grammar().GetGrammar()
	return
}

func (grammar *Grammar) Validate() error {
	if err := grammar.LexTokens.validate(); err != nil {
		return err
	}

	grammar.LexTokens.RegExps = lo.SliceToMap(grammar.LexTokens.named, func(t namedToken) (string, string) {
		return t.name, t.regexp
	})

	if err := grammar.ParseRules.validate(grammar.LexTokens); err != nil {
		return err
	}

	literals := lo.Flatten(lo.MapToSlice(grammar.ParseRules, func(_ string, rules []ParseRule) []string {
		return lo.FlatMap(rules, func(r ParseRule, _ int) []string {
			return lo.FilterMap(r, func(c ParseRuleComponent, _ int) (string, bool) {
				return c.Value, c.Type == ParseRuleComponentLiteral
			})
		})
	}))
	literals = lo.Uniq(literals)
	grammar.LexTokens.Literals = literals

	return nil
}

func (t *LexTokens) validate() error {
	for _, t := range t.named {
		if _, err := regexp.CompilePOSIX(t.regexp); err != nil {
			return fmt.Errorf("invalid regexp %s for named token %s: %w", strconv.Quote(t.regexp), t.name, err)
		}
	}

	namedTokenCounts := lo.CountValuesBy(t.named, func(t namedToken) string {
		return t.name
	})

	for name, count := range namedTokenCounts {
		if count > 1 {
			return fmt.Errorf("named tokens should be unique, but %s appears %d times", name, count)
		}
	}

	return nil
}

func (r ParseRules) validate(tokens LexTokens) error {
	usedTokens := make(map[string]struct{})

	for name, rules := range r {
		for _, rule := range rules {
			if err := rule.validate(name, r, tokens, usedTokens); err != nil {
				return err
			}
		}
	}

	for name := range tokens.RegExps {
		if _, ok := usedTokens[name]; !ok {
			return fmt.Errorf("named token %s is unused in rules", name)
		}
	}
	return nil
}

func (r ParseRule) validate(name string, rules ParseRules, tokens LexTokens, usedTokens map[string]struct{}) error {
	for _, component := range r {
		switch component.Type {
		case ParseRuleComponentToken:
			if _, ok := tokens.RegExps[component.Value]; !ok {
				return fmt.Errorf("parse rule for %s references unknown named token %s", name, component.Value)
			}

			usedTokens[component.Value] = struct{}{}
		case ParseRuleComponentRule:
			if _, ok := rules[component.Value]; !ok {
				return fmt.Errorf("parse rule for %s references unknown rule %s", name, component.Value)
			}
		}
	}

	return nil
}

type panicErrorListener struct {
	*antlr.DefaultErrorListener
}

func (panicErrorListener) SyntaxError(recognizer antlr.Recognizer, offendingSymbol interface{}, line, column int, msg string, e antlr.RecognitionException) {
	panic(errors.New("line " + strconv.Itoa(line) + ":" + strconv.Itoa(column) + " " + msg))
}
