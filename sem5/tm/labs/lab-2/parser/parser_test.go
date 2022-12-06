package parser

import (
	"testing"

	"pylambda/lexer"

	"github.com/stretchr/testify/assert"
)

func Test_Parser(t *testing.T) {
	tests := []struct {
		name  string
		input string
		tree  *Tree
	}{
		{
			name:  "no parameters",
			input: "lambda: True",
			tree: T(
				"LambdaDecl",
				T("lambda"),
				T("Args"),
				T(":"),
				T("Expr",
					T("Term",
						T("Factor", T("true")),
						T("TermM"),
					),
					T("ExprM"),
				),
				T("eps"),
			),
		},

		{
			name:  "unused parameters",
			input: "lambda a, b,\tc  ,d:False or True",
			tree: T(
				"LambdaDecl",
				T("lambda"),
				T("Args", T("ident"), T("OptArgs", T(","), T("ident"), T("OptArgs", T(","), T("ident"), T("OptArgs", T(","), T("ident"), T("OptArgs"))))),
				T(":"),
				T("Expr",
					T("Term",
						T("Factor", T("false")),
						T("TermM"),
					),
					T(
						"ExprM",
						T("or"),
						T("Term",
							T("Factor", T("true")),
							T("TermM"),
						),
						T("ExprM"),
					),
				),
				T("eps"),
			),
		},

		{
			name:  "operator precedence",
			input: "lambda a, b: False or True and not not b or a",
			tree: T(
				"LambdaDecl",
				T("lambda"),
				T("Args", T("ident"), T("OptArgs", T(","), T("ident"), T("OptArgs"))),
				T(":"),
				T("Expr",
					T("Term",
						T("Factor", T("false")),
						T("TermM"),
					),
					T(
						"ExprM",
						T("or"),
						T("Term",
							T("Factor", T("true")),
							T(
								"TermM",
								T("and"),
								T("Factor", T("not"), T("Factor", T("not"), T("Factor", T("ident")))),
								T("TermM"),
							),
						),
						T(
							"ExprM",
							T("or"),
							T("Term",
								T("Factor", T("ident")),
								T("TermM"),
							),
							T("ExprM"),
						),
					),
				),
				T("eps"),
			),
		},

		{
			name:  "brackets",
			input: "lambda a: ((a or True) and not False)",
			tree: T(
				"LambdaDecl",
				T("lambda"),
				T("Args", T("ident"), T("OptArgs")),
				T(":"),
				T("Expr",
					T("Term",
						T(
							"Factor",
							T("("),
							T(
								"Expr",
								T(
									"Term",
									T(
										"Factor",
										T("("),
										T(
											"Expr",
											T(
												"Term",
												T("Factor", T("ident")),
												T("TermM"),
											),
											T(
												"ExprM",
												T("or"),
												T(
													"Term",
													T("Factor", T("true")),
													T("TermM"),
												),
												T("ExprM"),
											),
										),
										T(")"),
									),
									T(
										"TermM",
										T("and"),
										T("Factor", T("not"), T("Factor", T("false"))),
										T("TermM"),
									),
								),
								T("ExprM"),
							),
							T(")"),
						),
						T("TermM"),
					),
					T("ExprM"),
				),
				T("eps"),
			),
		},
	}

	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			tokens, err := lexer.Lex(tt.input)
			if !assert.NoError(t, err) {
				return
			}

			tree, err := Parse(tokens)
			if !assert.NoError(t, err) {
				return
			}

			assert.Equal(t, tt.tree, tree)

			// want, _ := tt.tree.MarshalJSON()
			// got, _ := tree.MarshalJSON()

			// assert.Equal(t, string(want), string(got))
		})
	}
}
