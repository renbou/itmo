package main

import (
	"bufio"
	"encoding/json"
	"fmt"
	"log"
	"os"
)

type TreeNode interface {
	Name() string
	Children() []any
}

func buildTree(node TreeNode) map[string][]any {
	var children []any
	for _, child := range node.Children() {
		switch v := child.(type) {
		case string:
			children = append(children, v)
		case TreeNode:
			children = append(children, buildTree(v))
		default:
			panic("unknown child")
		}
	}
	return map[string][]any{node.Name(): children}
}

func main() {
	scanner := bufio.NewScanner(os.Stdin)
	_ = scanner.Scan()
	text := scanner.Text()

	lambda, err := ParseLambdaDecl(text)
	if err != nil {
		log.Fatal(err)
	}

	tree := buildTree(lambda)
	j, err := json.Marshal(tree)
	if err != nil {
		log.Fatal(err)
	}

	fmt.Println(string(j))
}
