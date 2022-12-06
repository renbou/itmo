package lexer

type Token int

//go:generate stringer -type=Token -linecomment
const (
	TokenEnd    Token = iota // eps
	TokenLambda              // lambda
	TokenComma               // ,
	TokenColon               // :
	TokenIdent               // ident
	TokenOr                  // or
	TokenAnd                 // and
	TokenNot                 // not
	TokenFalse               // false
	TokenTrue                // true
	TokenLParen              // (
	TokenRParen              // )
)
