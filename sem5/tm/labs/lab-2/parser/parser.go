package parser

import (
	"pylambda/lexer"
)

type parser struct {
	tokens []lexer.Token
	pos    int
}

func Parse(tokens []lexer.Token) (*Tree, error) {
	if len(tokens) < 1 || tokens[len(tokens)-1] != lexer.TokenEnd {
		return nil, ErrNoEnd
	}

	p := parser{
		tokens: tokens,
	}
	return p.parseLambdaDecl()
}

func (p *parser) token() lexer.Token {
	return p.tokens[p.pos]
}

func (p *parser) contin() {
	p.pos++
}

func (p *parser) parseLambdaDecl() (*Tree, error) {
	if p.token() != lexer.TokenLambda {
		return nil, p.errUnexpected("LambdaDecl")
	}

	args, err := p.parseArgs()
	if err != nil {
		return nil, err
	}

	if p.token() != lexer.TokenColon {
		return nil, p.errUnexpected("LambdaDecl")
	}

	expr, err := p.parseExpr()
	if err != nil {
		return nil, err
	}

	if p.pos != len(p.tokens)-1 {
		return nil, ErrNotExhausted
	}

	return T("LambdaDecl",
		T(lexer.TokenLambda.String()),
		args,
		T(lexer.TokenColon.String()),
		expr,
		T(lexer.TokenEnd.String()),
	), nil
}

func (p *parser) parseArgs() (*Tree, error) {
	p.contin()
	switch p.token() {
	// 1 or more arguments
	case lexer.TokenIdent:
		cont, err := p.parseOptArgs()
		if err != nil {
			return nil, err
		}

		return T("Args", T(lexer.TokenIdent.String()), cont), nil
	// No arguments
	case lexer.TokenColon:
		return T("Args"), nil
	default:
		return nil, p.errUnexpected("Args")
	}
}

func (p *parser) parseOptArgs() (*Tree, error) {
	p.contin()
	switch p.token() {
	// One more argument
	case lexer.TokenComma:
		p.contin()
		if p.token() != lexer.TokenIdent {
			return nil, p.errUnexpected("OptArgs")
		}

		// Continuation
		cont, err := p.parseOptArgs()
		if err != nil {
			return nil, err
		}

		return T("OptArgs",
			T(lexer.TokenComma.String()),
			T(lexer.TokenIdent.String()),
			cont,
		), nil
	// End of arguments
	case lexer.TokenColon:
		return T("OptArgs"), nil
	default:
		return nil, p.errUnexpected("OptArgs")
	}
}

func (p *parser) parseExpr() (*Tree, error) {
	p.contin()
	switch p.token() {
	case lexer.TokenIdent, lexer.TokenFalse, lexer.TokenTrue, lexer.TokenNot, lexer.TokenLParen:
		term, err := p.parseTerm()
		if err != nil {
			return nil, err
		}

		cont, err := p.parseExprM()
		if err != nil {
			return nil, err
		}

		return T("Expr", term, cont), nil
	default:
		return nil, p.errUnexpected("Expr")
	}
}

func (p *parser) parseExprM() (*Tree, error) {
	switch p.token() {
	// Low-priority or operation
	case lexer.TokenOr:
		p.contin()
		term, err := p.parseTerm()
		if err != nil {
			return nil, err
		}

		cont, err := p.parseExprM()
		if err != nil {
			return nil, err
		}

		return T("ExprM",
			T(lexer.TokenOr.String()),
			term,
			cont,
		), nil
	// end of expression
	case lexer.TokenRParen, lexer.TokenEnd:
		return T("ExprM"), nil
	default:
		return nil, p.errUnexpected("ExprM")
	}
}

func (p *parser) parseTerm() (*Tree, error) {
	// p.contin already called elsewhere
	switch p.token() {
	case lexer.TokenIdent, lexer.TokenFalse, lexer.TokenTrue, lexer.TokenNot, lexer.TokenLParen:
		factor, err := p.parseFactor()
		if err != nil {
			return nil, err
		}

		cont, err := p.parseTermM()
		if err != nil {
			return nil, err
		}

		return T("Term", factor, cont), nil
	default:
		return nil, p.errUnexpected("Term")
	}
}

func (p *parser) parseTermM() (*Tree, error) {
	p.contin()
	switch p.token() {
	// High-priority and operation
	case lexer.TokenAnd:
		p.contin()
		factor, err := p.parseFactor()
		if err != nil {
			return nil, err
		}

		cont, err := p.parseTermM()
		if err != nil {
			return nil, err
		}

		return T("TermM",
			T(lexer.TokenAnd.String()),
			factor,
			cont,
		), nil
	// Lower priority op or end of expr
	case lexer.TokenOr, lexer.TokenRParen, lexer.TokenEnd:
		return T("TermM"), nil
	default:
		return nil, p.errUnexpected("TermM")
	}
}

func (p *parser) parseFactor() (*Tree, error) {
	// p.contin already called elsewhere
	switch p.token() {
	// simple factor
	case lexer.TokenIdent, lexer.TokenFalse, lexer.TokenTrue:
		return T("Factor", T(p.token().String())), nil
	// one or more not's
	case lexer.TokenNot:
		p.contin()
		factor, err := p.parseFactor()
		if err != nil {
			return nil, err
		}

		return T("Factor", T(lexer.TokenNot.String()), factor), nil
	// wrapped expression
	case lexer.TokenLParen:
		expr, err := p.parseExpr()
		if err != nil {
			return nil, err
		}

		if p.token() != lexer.TokenRParen {
			return nil, p.errUnexpected("Factor")
		}

		return T("Factor",
			T(lexer.TokenLParen.String()),
			expr,
			T(lexer.TokenRParen.String()),
		), nil
	default:
		return nil, p.errUnexpected("Factor")
	}
}
