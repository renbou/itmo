package lexer

type Token int

//go:generate stringer -type=Token
const (
	TokenEnd Token = iota
	TokenLambda
	TokenComma
	TokenColon
	TokenIdent
	TokenOr
	TokenAnd
	TokenNot
	TokenFalse
	TokenTrue
	TokenLParen
	TokenRParen
)
