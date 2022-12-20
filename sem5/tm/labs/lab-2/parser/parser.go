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

	p.contin()
	args, err := p.parseArgs()
	if err != nil {
		return nil, err
	}

	if p.token() != lexer.TokenColon {
		return nil, p.errUnexpected("LambdaDecl")
	}

	p.contin()
	expr, err := p.parseImplyExpr()
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
	switch p.token() {
	// 1 or more arguments
	case lexer.TokenIdent:
		p.contin()
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
	switch p.token() {
	// One more argument
	case lexer.TokenComma:
		p.contin()
		if p.token() != lexer.TokenIdent {
			return nil, p.errUnexpected("OptArgs")
		}

		// Continuation
		p.contin()
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

func (p *parser) parseImplyExpr() (*Tree, error) {
	switch p.token() {
	case lexer.TokenIdent, lexer.TokenFalse, lexer.TokenTrue, lexer.TokenNot, lexer.TokenLParen:
		expr, err := p.parseExpr()
		if err != nil {
			return nil, err
		}

		cont, err := p.parseImplyExprM()
		if err != nil {
			return nil, err
		}

		return T("ImplyExpr", expr, cont), nil
	default:
		return nil, p.errUnexpected("ImplyExpr")
	}
}

func (p *parser) parseImplyExprM() (*Tree, error) {
	switch p.token() {
	// Lowest-priority imply operation
	case lexer.TokenImply:
		p.contin()
		expr, err := p.parseExpr()
		if err != nil {
			return nil, err
		}

		cont, err := p.parseImplyExprM()
		if err != nil {
			return nil, err
		}

		return T("ImplyExprM",
			T(lexer.TokenImply.String()),
			expr,
			cont,
		), nil
	// end of expression
	case lexer.TokenRParen, lexer.TokenEnd:
		return T("ImplyExprM"), nil
	default:
		return nil, p.errUnexpected("ImplyExprM")
	}
}

func (p *parser) parseExpr() (*Tree, error) {
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
	case lexer.TokenImply, lexer.TokenRParen, lexer.TokenEnd:
		return T("ExprM"), nil
	default:
		return nil, p.errUnexpected("ExprM")
	}
}

func (p *parser) parseTerm() (*Tree, error) {
	switch p.token() {
	case lexer.TokenIdent, lexer.TokenFalse, lexer.TokenTrue, lexer.TokenNot, lexer.TokenLParen:
		xorTerm, err := p.parseXorTerm()
		if err != nil {
			return nil, err
		}

		cont, err := p.parseTermM()
		if err != nil {
			return nil, err
		}

		return T("Term", xorTerm, cont), nil
	default:
		return nil, p.errUnexpected("Term")
	}
}

func (p *parser) parseTermM() (*Tree, error) {
	switch p.token() {
	// High-priority and operation
	case lexer.TokenAnd:
		p.contin()
		xorTerm, err := p.parseXorTerm()
		if err != nil {
			return nil, err
		}

		cont, err := p.parseTermM()
		if err != nil {
			return nil, err
		}

		return T("TermM",
			T(lexer.TokenAnd.String()),
			xorTerm,
			cont,
		), nil
	// Lower priority op or end of expr
	case lexer.TokenImply, lexer.TokenOr, lexer.TokenRParen, lexer.TokenEnd:
		return T("TermM"), nil
	default:
		return nil, p.errUnexpected("TermM")
	}
}

func (p *parser) parseXorTerm() (*Tree, error) {
	switch p.token() {
	case lexer.TokenIdent, lexer.TokenFalse, lexer.TokenTrue, lexer.TokenNot, lexer.TokenLParen:
		factor, err := p.parseFactor()
		if err != nil {
			return nil, err
		}

		cont, err := p.parseXorTermM()
		if err != nil {
			return nil, err
		}

		return T("XorTerm", factor, cont), nil
	default:
		return nil, p.errUnexpected("XorTerm")
	}
}

func (p *parser) parseXorTermM() (*Tree, error) {
	switch p.token() {
	// Higher-priority xor operation
	case lexer.TokenXor:
		p.contin()
		factor, err := p.parseFactor()
		if err != nil {
			return nil, err
		}

		cont, err := p.parseXorTermM()
		if err != nil {
			return nil, err
		}

		return T("XorTermM",
			T(lexer.TokenXor.String()),
			factor,
			cont,
		), nil
	// Lower priority op or end of expr
	case lexer.TokenImply, lexer.TokenOr, lexer.TokenAnd, lexer.TokenRParen, lexer.TokenEnd:
		return T("XorTermM"), nil
	default:
		return nil, p.errUnexpected("XorTermM")
	}
}

func (p *parser) parseFactor() (*Tree, error) {
	switch p.token() {
	// simple factor
	case lexer.TokenIdent, lexer.TokenFalse, lexer.TokenTrue:
		token := p.token()
		p.contin()
		return T("Factor", T(token.String())), nil
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
		p.contin()
		expr, err := p.parseImplyExpr()
		if err != nil {
			return nil, err
		}

		if p.token() != lexer.TokenRParen {
			return nil, p.errUnexpected("Factor")
		}

		p.contin()
		return T("Factor",
			T(lexer.TokenLParen.String()),
			expr,
			T(lexer.TokenRParen.String()),
		), nil
	default:
		return nil, p.errUnexpected("Factor")
	}
}
