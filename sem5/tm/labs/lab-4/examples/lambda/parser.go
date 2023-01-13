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
		regexp.MustCompile(`^or`),
		regexp.MustCompile(`^and`),
		regexp.MustCompile(`^false`),
		regexp.MustCompile(`^true`),
		regexp.MustCompile(`^not`),
		regexp.MustCompile(`^\(`),
		regexp.MustCompile(`^\)`),
		regexp.MustCompile(`^lambda`),
		regexp.MustCompile(`^:`),
		regexp.MustCompile(`^,`),
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

func (p *parser) parseArgs(node *ParserNodeArgs) (*ParserNodeArgs, error) {
	var children []any

	switch p.token().id {
	case 10:
		if p.token().id != 10 {
			return nil, p.errUnexpected("Args")
		} else {
			children = append(children, p.token().value)
		}
		p.contin()
		if child, err := p.parseOptArgs(&ParserNodeOptArgs{}); err != nil {
			return nil, err
		} else {
			children = append(children, child)
		}

		node.children = children

		node.tree = map[string][]any{"Args": {children[0].(string), children[1].(*ParserNodeOptArgs).tree}}

		return node, nil
	case 8:
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

	result, err := p.parseArgs(&ParserNodeArgs{})
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

func (p *parser) parseExpr(node *ParserNodeExpr) (*ParserNodeExpr, error) {
	var children []any

	switch p.token().id {
	case 5, 2, 3, 10, 4:
		if child, err := p.parseTerm(&ParserNodeTerm{}); err != nil {
			return nil, err
		} else {
			children = append(children, child)
		}
		if child, err := p.parseExprM(&ParserNodeExprM{}); err != nil {
			return nil, err
		} else {
			children = append(children, child)
		}

		node.children = children

		node.tree = map[string][]any{"TermM": {children[0].(*ParserNodeTerm).tree, children[1].(*ParserNodeExprM).tree}}

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

	result, err := p.parseExpr(&ParserNodeExpr{})
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

func (p *parser) parseExprM(node *ParserNodeExprM) (*ParserNodeExprM, error) {
	var children []any

	switch p.token().id {
	case 0:
		if p.token().id != 0 {
			return nil, p.errUnexpected("ExprM")
		} else {
			children = append(children, p.token().value)
		}
		p.contin()
		if child, err := p.parseTerm(&ParserNodeTerm{}); err != nil {
			return nil, err
		} else {
			children = append(children, child)
		}
		if child, err := p.parseExprM(&ParserNodeExprM{}); err != nil {
			return nil, err
		} else {
			children = append(children, child)
		}

		node.children = children

		node.tree = map[string][]any{"ExprM": {children[0].(string), children[1].(*ParserNodeTerm).tree, children[2].(*ParserNodeExprM).tree}}

		return node, nil
	case 6, -1:
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

	result, err := p.parseExprM(&ParserNodeExprM{})
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

func (p *parser) parseFactor(node *ParserNodeFactor) (*ParserNodeFactor, error) {
	var children []any

	switch p.token().id {
	case 2:
		if p.token().id != 2 {
			return nil, p.errUnexpected("Factor")
		} else {
			children = append(children, p.token().value)
		}
		p.contin()

		node.children = children

		node.tree = map[string][]any{"Factor": {children[0].(string)}}

		return node, nil
	case 3:
		if p.token().id != 3 {
			return nil, p.errUnexpected("Factor")
		} else {
			children = append(children, p.token().value)
		}
		p.contin()

		node.children = children

		node.tree = map[string][]any{"Factor": {children[0].(string)}}

		return node, nil
	case 10:
		if p.token().id != 10 {
			return nil, p.errUnexpected("Factor")
		} else {
			children = append(children, p.token().value)
		}
		p.contin()

		node.children = children

		node.tree = map[string][]any{"Factor": {children[0].(string)}}

		return node, nil
	case 4:
		if p.token().id != 4 {
			return nil, p.errUnexpected("Factor")
		} else {
			children = append(children, p.token().value)
		}
		p.contin()
		if child, err := p.parseFactor(&ParserNodeFactor{}); err != nil {
			return nil, err
		} else {
			children = append(children, child)
		}

		node.children = children

		node.tree = map[string][]any{"Factor": {children[0].(string), children[1].(*ParserNodeFactor).tree}}

		return node, nil
	case 5:
		if p.token().id != 5 {
			return nil, p.errUnexpected("Factor")
		} else {
			children = append(children, p.token().value)
		}
		p.contin()
		if child, err := p.parseExpr(&ParserNodeExpr{}); err != nil {
			return nil, err
		} else {
			children = append(children, child)
		}
		if p.token().id != 6 {
			return nil, p.errUnexpected("Factor")
		} else {
			children = append(children, p.token().value)
		}
		p.contin()

		node.children = children

		node.tree = map[string][]any{"Factor": {children[0].(string), children[1].(*ParserNodeExpr).tree, children[2].(string)}}

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

	result, err := p.parseFactor(&ParserNodeFactor{})
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

func (p *parser) parseLambdaDecl(node *ParserNodeLambdaDecl) (*ParserNodeLambdaDecl, error) {
	var children []any

	switch p.token().id {
	case 7:
		if p.token().id != 7 {
			return nil, p.errUnexpected("LambdaDecl")
		} else {
			children = append(children, p.token().value)
		}
		p.contin()
		if child, err := p.parseArgs(&ParserNodeArgs{}); err != nil {
			return nil, err
		} else {
			children = append(children, child)
		}
		if p.token().id != 8 {
			return nil, p.errUnexpected("LambdaDecl")
		} else {
			children = append(children, p.token().value)
		}
		p.contin()
		if child, err := p.parseExpr(&ParserNodeExpr{}); err != nil {
			return nil, err
		} else {
			children = append(children, child)
		}

		node.children = children

		node.tree = map[string][]any{"LambdaDecl": {children[0].(string), children[1].(*ParserNodeArgs).tree, children[2].(string), children[3].(*ParserNodeExpr).tree}}

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

	result, err := p.parseLambdaDecl(&ParserNodeLambdaDecl{})
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

func (p *parser) parseOptArgs(node *ParserNodeOptArgs) (*ParserNodeOptArgs, error) {
	var children []any

	switch p.token().id {
	case 9:
		if p.token().id != 9 {
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
		if child, err := p.parseOptArgs(&ParserNodeOptArgs{}); err != nil {
			return nil, err
		} else {
			children = append(children, child)
		}

		node.children = children

		node.tree = map[string][]any{"OptArgs": {children[0].(string), children[1].(string), children[2].(*ParserNodeOptArgs).tree}}

		return node, nil
	case 8:
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

	result, err := p.parseOptArgs(&ParserNodeOptArgs{})
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

func (p *parser) parseTerm(node *ParserNodeTerm) (*ParserNodeTerm, error) {
	var children []any

	switch p.token().id {
	case 2, 3, 10, 4, 5:
		if child, err := p.parseFactor(&ParserNodeFactor{}); err != nil {
			return nil, err
		} else {
			children = append(children, child)
		}
		if child, err := p.parseTermM(&ParserNodeTermM{}); err != nil {
			return nil, err
		} else {
			children = append(children, child)
		}

		node.children = children

		node.tree = map[string][]any{"TermM": {children[0].(*ParserNodeFactor).tree, children[1].(*ParserNodeTermM).tree}}

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

	result, err := p.parseTerm(&ParserNodeTerm{})
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

func (p *parser) parseTermM(node *ParserNodeTermM) (*ParserNodeTermM, error) {
	var children []any

	switch p.token().id {
	case 1:
		if p.token().id != 1 {
			return nil, p.errUnexpected("TermM")
		} else {
			children = append(children, p.token().value)
		}
		p.contin()
		if child, err := p.parseFactor(&ParserNodeFactor{}); err != nil {
			return nil, err
		} else {
			children = append(children, child)
		}
		if child, err := p.parseTermM(&ParserNodeTermM{}); err != nil {
			return nil, err
		} else {
			children = append(children, child)
		}

		node.children = children

		node.tree = map[string][]any{"TermM": {children[0].(string), children[1].(*ParserNodeFactor).tree, children[2].(*ParserNodeTermM).tree}}

		return node, nil
	case 0, 6, -1:
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

	result, err := p.parseTermM(&ParserNodeTermM{})
	if err != nil {
		return nil, err
	}

	if p.pos != len(tokens)-1 {
		return nil, ErrNotExhausted
	}
	return result, nil
}
