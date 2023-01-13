package main

import (
	"errors"
	"fmt"
	"regexp"
	"strconv"
)

func evaluate(op string, a, b float64) float64 {
	switch op {
	case "+":
		return a + b
	case "-":
		return a - b
	case "*":
		return a * b
	case "/":
		return a / b
	}
	panic(fmt.Sprintf("unknown operator: %s", op))
}

type token struct {
	id    int
	value string
}

var (
	tokenRegExps = []*regexp.Regexp{
		regexp.MustCompile(`^\(`),
		regexp.MustCompile(`^\)`),
		regexp.MustCompile(`^[*/]`),
		regexp.MustCompile(`^[+-]`),
		regexp.MustCompile(`^\d+(\.\d+)?`),
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

type ParserNodeExpr struct {
	children []any
	ret      float64
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
	case 4, 0, 3:
		if child, err := p.parseTerm(&ParserNodeTerm{}); err != nil {
			return nil, err
		} else {
			children = append(children, child)
		}
		if child, err := p.parseExprM(&ParserNodeExprM{acc: children[0].(*ParserNodeTerm).ret}); err != nil {
			return nil, err
		} else {
			children = append(children, child)
		}

		node.children = children

		node.ret = children[1].(*ParserNodeExprM).ret

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
	acc      float64
	ret      float64
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
	case 3:
		if p.token().id != 3 {
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
		if child, err := p.parseExprM(&ParserNodeExprM{acc: evaluate(children[0].(string), node.acc, children[1].(*ParserNodeTerm).ret)}); err != nil {
			return nil, err
		} else {
			children = append(children, child)
		}

		node.children = children

		node.ret = children[2].(*ParserNodeExprM).ret

		return node, nil
	case 1, -1:
		node.ret = node.acc

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
	ret      float64
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
	case 4:
		if p.token().id != 4 {
			return nil, p.errUnexpected("Factor")
		} else {
			children = append(children, p.token().value)
		}
		p.contin()

		node.children = children

		v, err := strconv.ParseFloat(children[0].(string), 64)
		if err != nil {
			panic(fmt.Sprintf("failed to parse number %s: %s", children[0].(string), err))
		}

		node.ret = v

		return node, nil
	case 0:
		if p.token().id != 0 {
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
		if p.token().id != 1 {
			return nil, p.errUnexpected("Factor")
		} else {
			children = append(children, p.token().value)
		}
		p.contin()

		node.children = children

		node.ret = children[1].(*ParserNodeExpr).ret

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

type ParserNodeTerm struct {
	children []any
	ret      float64
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
	case 4, 0, 3:
		if child, err := p.parseUFactor(&ParserNodeUFactor{}); err != nil {
			return nil, err
		} else {
			children = append(children, child)
		}
		if child, err := p.parseTermM(&ParserNodeTermM{acc: children[0].(*ParserNodeUFactor).ret}); err != nil {
			return nil, err
		} else {
			children = append(children, child)
		}

		node.children = children

		node.ret = children[1].(*ParserNodeTermM).ret

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
	acc      float64
	ret      float64
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
	case 2:
		if p.token().id != 2 {
			return nil, p.errUnexpected("TermM")
		} else {
			children = append(children, p.token().value)
		}
		p.contin()
		if child, err := p.parseUFactor(&ParserNodeUFactor{}); err != nil {
			return nil, err
		} else {
			children = append(children, child)
		}
		if child, err := p.parseTermM(&ParserNodeTermM{acc: evaluate(children[0].(string), node.acc, children[1].(*ParserNodeUFactor).ret)}); err != nil {
			return nil, err
		} else {
			children = append(children, child)
		}

		node.children = children

		node.ret = children[2].(*ParserNodeTermM).ret

		return node, nil
	case 3, -1, 1:
		node.ret = node.acc

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

type ParserNodeUFactor struct {
	children []any
	ret      float64
}

func (node *ParserNodeUFactor) Name() string {
	return "UFactor"
}

func (node *ParserNodeUFactor) Children() []any {
	return node.children
}

func (p *parser) parseUFactor(node *ParserNodeUFactor) (*ParserNodeUFactor, error) {
	var children []any

	switch p.token().id {
	case 3:
		if p.token().id != 3 {
			return nil, p.errUnexpected("UFactor")
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

		node.ret = evaluate(children[0].(string), 0, children[1].(*ParserNodeFactor).ret)

		return node, nil
	case 4, 0:
		if child, err := p.parseFactor(&ParserNodeFactor{}); err != nil {
			return nil, err
		} else {
			children = append(children, child)
		}

		node.children = children

		node.ret = children[0].(*ParserNodeFactor).ret

		return node, nil
	default:
		return nil, p.errUnexpected("UFactor")
	}
}

func ParseUFactor(text string) (*ParserNodeUFactor, error) {
	tokens, err := lex(text)
	if err != nil {
		return nil, err
	}

	p := parser{tokens, 0}

	result, err := p.parseUFactor(&ParserNodeUFactor{})
	if err != nil {
		return nil, err
	}

	if p.pos != len(tokens)-1 {
		return nil, ErrNotExhausted
	}
	return result, nil
}
