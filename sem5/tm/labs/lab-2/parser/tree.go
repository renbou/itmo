package parser

import (
	"encoding/json"
	"unicode"
)

// If we should marshal into something prettier. Should be false for testing and true for the visualisation.
var pretty = true

type Tree struct {
	node     string
	children []*Tree
}

func T(node string, children ...*Tree) *Tree {
	return &Tree{node, children}
}

// Custom marshaler for prettier display
func (t *Tree) MarshalJSON() ([]byte, error) {
	var v any
	if pretty {
		v = t.prettify()
	} else {
		v = map[string][]*Tree{
			t.node: t.children,
		}
	}
	return json.MarshalIndent(v, "", "  ")
}

// prettify returns a prettified version of the tree for display
// by removed redundant nesting where possible
func (t *Tree) prettify() any {
	// All non-terminal nodes are marked as uppercase
	if !unicode.IsUpper(rune(t.node[0])) {
		return t.node
	}

	switch t.node {
	// simplify Args into list of "ident"
	case "Args":
		var args []string
		if t.children != nil {
			args = append(args, t.children[0].prettify().(string))
			args = append(args, t.children[1].prettify().([]string)...)
		}
		return map[string][]string{
			"Args": args,
		}
	case "OptArgs":
		var args []string
		if t.children != nil {
			args = append(args, t.children[1].prettify().(string))
			args = append(args, t.children[2].prettify().([]string)...)
		}
		return args
	// if only single element is present, avoid writing deeply nested structures
	case "Expr", "Term":
		if t.children[1].children == nil {
			return t.children[0]
		}
	case "ExprM", "TermM":
		if t.children != nil && t.children[2].children == nil {
			t.children = t.children[:2]
		}
	default:
	}

	return map[string][]*Tree{
		t.node: t.children,
	}
}
