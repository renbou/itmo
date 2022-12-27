package grammar

import (
	"fmt"
	"strconv"

	"github.com/antlr/antlr4/runtime/Go/antlr/v4"
)

func literalValue(literal antlr.Token) string {
	s, err := strconv.Unquote(literal.GetText())
	if err != nil {
		panic(fmt.Errorf("badly quoted literal %s: %w", literal.GetText(), err))
	}

	return s
}
