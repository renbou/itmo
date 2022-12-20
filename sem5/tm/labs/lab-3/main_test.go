package main

import (
	"testing"

	"github.com/stretchr/testify/assert"
)

func TestTranslate(t *testing.T) {
	tests := []struct {
		name string
		in   string
		out  string
	}{
		{
			name: "arithmetic operations",
			in:   "- + 1 3 * / 2 4 5",
			out:  "(1 + 3) - ((2 / 4) * 5)",
		},
		{
			name: "boolean operations",
			in:   "|| true && ! == 1 3 false",
			out:  "true || (!(1 == 3) && false)",
		},
		{
			name: "comparison operations",
			in:   "> 3 == 1 < 5 4",
			out:  "3 > (1 == (5 < 4))",
		},
		{
			name: "variables",
			in:   "= identifier ! == false true",
			out:  "identifier = !(false == true)",
		},
		{
			name: "conditional if",
			in:   "if > 2 3 print 3 if > 4 - 7 2 print + 3 4",
			out:  "if 2 > 3 {\n  println(3)\n} else if 4 > (7 - 2) {\n  println(3 + 4)\n}",
		},
		{
			name: "if, else if, else",
			in:   "if true print true if false print false print 1",
			out:  "if true {\n  println(true)\n} else if false {\n  println(false)\n} else {\n  println(1)\n}",
		},
	}

	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			got := translate(tt.in)
			assert.Equal(t, tt.out, got)
		})
	}
}
