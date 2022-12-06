package parser

import (
	"errors"
	"fmt"
)

var (
	ErrNoEnd        = errors.New("tokens must end with TokenEnd")
	ErrNotExhausted = errors.New("not all tokens have been exhausted after parsing")
)

func (p *parser) errUnexpected(node string) error {
	return fmt.Errorf("unexpected %s in %s node", p.token(), node)
}
