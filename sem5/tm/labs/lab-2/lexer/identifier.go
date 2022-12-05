package lexer

import (
	"errors"
	"unicode"
)

var ErrInvalidIdentifier = errors.New("invalid identifier")

// isPythonIdentifier checks if a string is a valid python identifier.
// Python identifiers can contain letters + 0-9 + _, but must start with letters + _
// and must not be _ (reserved identifier).
func isPythonIdentifier(s []rune) bool {
	if len(s) < 1 {
		return false
	}

	for _, r := range s {
		if !validPythonIdentifierChar(r) {
			return false
		}
	}

	// cannot start with a digit
	if r := s[0]; r >= '0' && r <= '9' {
		return false
	}

	// must not equal to "_"
	return len(s) > 1 || s[0] != '_'
}

func validPythonIdentifierChar(r rune) bool {
	return unicode.IsLetter(r) || (r >= '0' && r <= '9') || r == '_'
}
