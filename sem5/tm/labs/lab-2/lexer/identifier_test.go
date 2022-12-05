package lexer

import (
	"testing"

	"github.com/stretchr/testify/assert"
)

func Test_isPythonIdentifier(t *testing.T) {
	tests := []struct {
		s  string
		ok bool
	}{
		{"", false},
		{"a", true},
		{"0", false},
		{"123", false},
		{"12asdf", false},
		{"123_asd", false},
		{"a1", true},
		{"abcdef", true},
		{"abc1def2", true},
		{"_", false},
		{"_a", true},
		{"__", true},
		{"русский0", true},
		{"__eng_рус1ски2й_", true},
	}

	for _, tt := range tests {
		assert.Equal(t, tt.ok, isPythonIdentifier([]rune(tt.s)))
	}
}
