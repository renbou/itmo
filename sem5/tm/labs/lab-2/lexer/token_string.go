// Code generated by "stringer -type=Token"; DO NOT EDIT.

package lexer

import "strconv"

func _() {
	// An "invalid array index" compiler error signifies that the constant values have changed.
	// Re-run the stringer command to generate them again.
	var x [1]struct{}
	_ = x[TokenEnd-0]
	_ = x[TokenLambda-1]
	_ = x[TokenComma-2]
	_ = x[TokenColon-3]
	_ = x[TokenIdent-4]
	_ = x[TokenOr-5]
	_ = x[TokenAnd-6]
	_ = x[TokenNot-7]
	_ = x[TokenFalse-8]
	_ = x[TokenTrue-9]
	_ = x[TokenLParen-10]
	_ = x[TokenRParen-11]
}

const _Token_name = "TokenEndTokenLambdaTokenCommaTokenColonTokenIdentTokenOrTokenAndTokenNotTokenFalseTokenTrueTokenLParenTokenRParen"

var _Token_index = [...]uint8{0, 8, 19, 29, 39, 49, 56, 64, 72, 82, 91, 102, 113}

func (i Token) String() string {
	if i < 0 || i >= Token(len(_Token_index)-1) {
		return "Token(" + strconv.FormatInt(int64(i), 10) + ")"
	}
	return _Token_name[_Token_index[i]:_Token_index[i+1]]
}
