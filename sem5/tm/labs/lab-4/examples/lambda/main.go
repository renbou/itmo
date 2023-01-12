package main

import (
	"bufio"
	"encoding/json"
	"fmt"
	"log"
	"os"
)

func main() {
	scanner := bufio.NewScanner(os.Stdin)
	_ = scanner.Scan()
	text := scanner.Text()

	lambda, err := ParseLambdaDecl(text)
	if err != nil {
		log.Fatal(err)
	}

	j, err := json.Marshal(lambda.tree)
	if err != nil {
		log.Fatal(err)
	}

	fmt.Println(string(j))
}
