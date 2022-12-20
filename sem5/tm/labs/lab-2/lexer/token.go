package lexer

type Token int

//go:generate stringer -type=Token -linecomment
const (
	TokenEnd    Token = iota // eps
	TokenLambda              // lambda
	TokenComma               // ,
	TokenColon               // :
	TokenIdent               // ident
	TokenImply               // imply
	TokenOr                  // or
	TokenAnd                 // and
	TokenXor                 // xor
	TokenNot                 // not
	TokenFalse               // false
	TokenTrue                // true
	TokenLParen              // (
	TokenRParen              // )
)
