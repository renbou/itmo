package main

import (
	"errors"
	"fmt"
	"regexp"
	"strconv"
)

type token struct {
	id    int
	value string
}

var (
	tokenRegExps = []*regexp.Regexp{
		regexp.MustCompile(`^lambda`),
		regexp.MustCompile(`^:`),
		regexp.MustCompile(`^,`),
		regexp.MustCompile(`^or`),
		regexp.MustCompile(`^and`),
		regexp.MustCompile(`^false`),
		regexp.MustCompile(`^true`),
		regexp.MustCompile(`^not`),
		regexp.MustCompile(`^\(`),
		regexp.MustCompile(`^\)`),
		regexp.MustCompile(`^[a-zA-Z_][a-zA-Z0-9_]*`),
	}

	whitespaceRegExp = regexp.MustCompile(`^\s+`)

	eofTokenID = -1
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
	return append(tokens, token{eofTokenID, "EOF"}), nil
}

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

type ParserNodeArgs struct {
	children []any
	tree     map[string][]any
}

func (node *ParserNodeArgs) Name() string {
	return "Args"
}

func (node *ParserNodeArgs) Children() []any {
	return node.children
}

func (p *parser) parseArgs() (*ParserNodeArgs, error) {
	var children []any

	switch p.token().id {
	case 10:
		if p.token().id != 10 {
			return nil, p.errUnexpected("Args")
		} else {
			children = append(children, p.token().value)
		}
		p.contin()
		if child, err := p.parseOptArgs(); err != nil {
			return nil, err
		} else {
			children = append(children, child)
		}

		node := &ParserNodeArgs{children: children}

		node.tree = map[string][]any{"Args": {node.children[0], node.children[1].(*ParserNodeOptArgs).tree}}

		return node, nil
	case 1:
		node := &ParserNodeArgs{}

		node.tree = map[string][]any{"Args": nil}

		return node, nil
	default:
		return nil, p.errUnexpected("Args")
	}
}

func ParseArgs(text string) (*ParserNodeArgs, error) {
	tokens, err := lex(text)
	if err != nil {
		return nil, err
	}

	p := parser{tokens, 0}

	result, err := p.parseArgs()
	if err != nil {
		return nil, err
	}

	if p.pos != len(tokens)-1 {
		return nil, ErrNotExhausted
	}
	return result, nil
}

type ParserNodeExpr struct {
	children []any
	tree     map[string][]any
}

func (node *ParserNodeExpr) Name() string {
	return "Expr"
}

func (node *ParserNodeExpr) Children() []any {
	return node.children
}

func (p *parser) parseExpr() (*ParserNodeExpr, error) {
	var children []any

	switch p.token().id {
	case 5, 6, 10, 7, 8:
		if child, err := p.parseTerm(); err != nil {
			return nil, err
		} else {
			children = append(children, child)
		}
		if child, err := p.parseExprM(); err != nil {
			return nil, err
		} else {
			children = append(children, child)
		}

		node := &ParserNodeExpr{children: children}

		node.tree = map[string][]any{"TermM": {node.children[0].(*ParserNodeTerm).tree, node.children[1].(*ParserNodeExprM).tree}}

		return node, nil
	default:
		return nil, p.errUnexpected("Expr")
	}
}

func ParseExpr(text string) (*ParserNodeExpr, error) {
	tokens, err := lex(text)
	if err != nil {
		return nil, err
	}

	p := parser{tokens, 0}

	result, err := p.parseExpr()
	if err != nil {
		return nil, err
	}

	if p.pos != len(tokens)-1 {
		return nil, ErrNotExhausted
	}
	return result, nil
}

type ParserNodeExprM struct {
	children []any
	tree     map[string][]any
}

func (node *ParserNodeExprM) Name() string {
	return "ExprM"
}

func (node *ParserNodeExprM) Children() []any {
	return node.children
}

func (p *parser) parseExprM() (*ParserNodeExprM, error) {
	var children []any

	switch p.token().id {
	case 3:
		if p.token().id != 3 {
			return nil, p.errUnexpected("ExprM")
		} else {
			children = append(children, p.token().value)
		}
		p.contin()
		if child, err := p.parseTerm(); err != nil {
			return nil, err
		} else {
			children = append(children, child)
		}
		if child, err := p.parseExprM(); err != nil {
			return nil, err
		} else {
			children = append(children, child)
		}

		node := &ParserNodeExprM{children: children}

		node.tree = map[string][]any{"ExprM": {node.children[0], node.children[1].(*ParserNodeTerm).tree, node.children[2].(*ParserNodeExprM).tree}}

		return node, nil
	case 9, -1:
		node := &ParserNodeExprM{}

		node.tree = map[string][]any{"ExprM": nil}

		return node, nil
	default:
		return nil, p.errUnexpected("ExprM")
	}
}

func ParseExprM(text string) (*ParserNodeExprM, error) {
	tokens, err := lex(text)
	if err != nil {
		return nil, err
	}

	p := parser{tokens, 0}

	result, err := p.parseExprM()
	if err != nil {
		return nil, err
	}

	if p.pos != len(tokens)-1 {
		return nil, ErrNotExhausted
	}
	return result, nil
}

type ParserNodeFactor struct {
	children []any
	tree     map[string][]any
}

func (node *ParserNodeFactor) Name() string {
	return "Factor"
}

func (node *ParserNodeFactor) Children() []any {
	return node.children
}

func (p *parser) parseFactor() (*ParserNodeFactor, error) {
	var children []any

	switch p.token().id {
	case 5:
		if p.token().id != 5 {
			return nil, p.errUnexpected("Factor")
		} else {
			children = append(children, p.token().value)
		}
		p.contin()

		node := &ParserNodeFactor{children: children}

		node.tree = map[string][]any{"Factor": {node.children[0]}}

		return node, nil
	case 6:
		if p.token().id != 6 {
			return nil, p.errUnexpected("Factor")
		} else {
			children = append(children, p.token().value)
		}
		p.contin()

		node := &ParserNodeFactor{children: children}

		node.tree = map[string][]any{"Factor": {node.children[0]}}

		return node, nil
	case 10:
		if p.token().id != 10 {
			return nil, p.errUnexpected("Factor")
		} else {
			children = append(children, p.token().value)
		}
		p.contin()

		node := &ParserNodeFactor{children: children}

		node.tree = map[string][]any{"Factor": {node.children[0]}}

		return node, nil
	case 7:
		if p.token().id != 7 {
			return nil, p.errUnexpected("Factor")
		} else {
			children = append(children, p.token().value)
		}
		p.contin()
		if child, err := p.parseFactor(); err != nil {
			return nil, err
		} else {
			children = append(children, child)
		}

		node := &ParserNodeFactor{children: children}

		node.tree = map[string][]any{"Factor": {node.children[0], node.children[1].(*ParserNodeFactor).tree}}

		return node, nil
	case 8:
		if p.token().id != 8 {
			return nil, p.errUnexpected("Factor")
		} else {
			children = append(children, p.token().value)
		}
		p.contin()
		if child, err := p.parseExpr(); err != nil {
			return nil, err
		} else {
			children = append(children, child)
		}
		if p.token().id != 9 {
			return nil, p.errUnexpected("Factor")
		} else {
			children = append(children, p.token().value)
		}
		p.contin()

		node := &ParserNodeFactor{children: children}

		node.tree = map[string][]any{"Factor": {node.children[0], node.children[1].(*ParserNodeExpr).tree, node.children[2]}}

		return node, nil
	default:
		return nil, p.errUnexpected("Factor")
	}
}

func ParseFactor(text string) (*ParserNodeFactor, error) {
	tokens, err := lex(text)
	if err != nil {
		return nil, err
	}

	p := parser{tokens, 0}

	result, err := p.parseFactor()
	if err != nil {
		return nil, err
	}

	if p.pos != len(tokens)-1 {
		return nil, ErrNotExhausted
	}
	return result, nil
}

type ParserNodeLambdaDecl struct {
	children []any
	tree     map[string][]any
}

func (node *ParserNodeLambdaDecl) Name() string {
	return "LambdaDecl"
}

func (node *ParserNodeLambdaDecl) Children() []any {
	return node.children
}

func (p *parser) parseLambdaDecl() (*ParserNodeLambdaDecl, error) {
	var children []any

	switch p.token().id {
	case 0:
		if p.token().id != 0 {
			return nil, p.errUnexpected("LambdaDecl")
		} else {
			children = append(children, p.token().value)
		}
		p.contin()
		if child, err := p.parseArgs(); err != nil {
			return nil, err
		} else {
			children = append(children, child)
		}
		if p.token().id != 1 {
			return nil, p.errUnexpected("LambdaDecl")
		} else {
			children = append(children, p.token().value)
		}
		p.contin()
		if child, err := p.parseExpr(); err != nil {
			return nil, err
		} else {
			children = append(children, child)
		}

		node := &ParserNodeLambdaDecl{children: children}

		node.tree = map[string][]any{"LambdaDecl": {node.children[0], node.children[1].(*ParserNodeArgs).tree, node.children[2], node.children[3].(*ParserNodeExpr).tree}}

		return node, nil
	default:
		return nil, p.errUnexpected("LambdaDecl")
	}
}

func ParseLambdaDecl(text string) (*ParserNodeLambdaDecl, error) {
	tokens, err := lex(text)
	if err != nil {
		return nil, err
	}

	p := parser{tokens, 0}

	result, err := p.parseLambdaDecl()
	if err != nil {
		return nil, err
	}

	if p.pos != len(tokens)-1 {
		return nil, ErrNotExhausted
	}
	return result, nil
}

type ParserNodeOptArgs struct {
	children []any
	tree     map[string][]any
}

func (node *ParserNodeOptArgs) Name() string {
	return "OptArgs"
}

func (node *ParserNodeOptArgs) Children() []any {
	return node.children
}

func (p *parser) parseOptArgs() (*ParserNodeOptArgs, error) {
	var children []any

	switch p.token().id {
	case 2:
		if p.token().id != 2 {
			return nil, p.errUnexpected("OptArgs")
		} else {
			children = append(children, p.token().value)
		}
		p.contin()
		if p.token().id != 10 {
			return nil, p.errUnexpected("OptArgs")
		} else {
			children = append(children, p.token().value)
		}
		p.contin()
		if child, err := p.parseOptArgs(); err != nil {
			return nil, err
		} else {
			children = append(children, child)
		}

		node := &ParserNodeOptArgs{children: children}

		node.tree = map[string][]any{"OptArgs": {node.children[0], node.children[1], node.children[2].(*ParserNodeOptArgs).tree}}

		return node, nil
	case 1:
		node := &ParserNodeOptArgs{}

		node.tree = map[string][]any{"OptArgs": nil}

		return node, nil
	default:
		return nil, p.errUnexpected("OptArgs")
	}
}

func ParseOptArgs(text string) (*ParserNodeOptArgs, error) {
	tokens, err := lex(text)
	if err != nil {
		return nil, err
	}

	p := parser{tokens, 0}

	result, err := p.parseOptArgs()
	if err != nil {
		return nil, err
	}

	if p.pos != len(tokens)-1 {
		return nil, ErrNotExhausted
	}
	return result, nil
}

type ParserNodeTerm struct {
	children []any
	tree     map[string][]any
}

func (node *ParserNodeTerm) Name() string {
	return "Term"
}

func (node *ParserNodeTerm) Children() []any {
	return node.children
}

func (p *parser) parseTerm() (*ParserNodeTerm, error) {
	var children []any

	switch p.token().id {
	case 5, 6, 10, 7, 8:
		if child, err := p.parseFactor(); err != nil {
			return nil, err
		} else {
			children = append(children, child)
		}
		if child, err := p.parseTermM(); err != nil {
			return nil, err
		} else {
			children = append(children, child)
		}

		node := &ParserNodeTerm{children: children}

		node.tree = map[string][]any{"TermM": {node.children[0].(*ParserNodeFactor).tree, node.children[1].(*ParserNodeTermM).tree}}

		return node, nil
	default:
		return nil, p.errUnexpected("Term")
	}
}

func ParseTerm(text string) (*ParserNodeTerm, error) {
	tokens, err := lex(text)
	if err != nil {
		return nil, err
	}

	p := parser{tokens, 0}

	result, err := p.parseTerm()
	if err != nil {
		return nil, err
	}

	if p.pos != len(tokens)-1 {
		return nil, ErrNotExhausted
	}
	return result, nil
}

type ParserNodeTermM struct {
	children []any
	tree     map[string][]any
}

func (node *ParserNodeTermM) Name() string {
	return "TermM"
}

func (node *ParserNodeTermM) Children() []any {
	return node.children
}

func (p *parser) parseTermM() (*ParserNodeTermM, error) {
	var children []any

	switch p.token().id {
	case 4:
		if p.token().id != 4 {
			return nil, p.errUnexpected("TermM")
		} else {
			children = append(children, p.token().value)
		}
		p.contin()
		if child, err := p.parseFactor(); err != nil {
			return nil, err
		} else {
			children = append(children, child)
		}
		if child, err := p.parseTermM(); err != nil {
			return nil, err
		} else {
			children = append(children, child)
		}

		node := &ParserNodeTermM{children: children}

		node.tree = map[string][]any{"TermM": {node.children[0], node.children[1].(*ParserNodeFactor).tree, node.children[2].(*ParserNodeTermM).tree}}

		return node, nil
	case 3, 9, -1:
		node := &ParserNodeTermM{}

		node.tree = map[string][]any{"TermM": nil}

		return node, nil
	default:
		return nil, p.errUnexpected("TermM")
	}
}

func ParseTermM(text string) (*ParserNodeTermM, error) {
	tokens, err := lex(text)
	if err != nil {
		return nil, err
	}

	p := parser{tokens, 0}

	result, err := p.parseTermM()
	if err != nil {
		return nil, err
	}

	if p.pos != len(tokens)-1 {
		return nil, ErrNotExhausted
	}
	return result, nil
}
