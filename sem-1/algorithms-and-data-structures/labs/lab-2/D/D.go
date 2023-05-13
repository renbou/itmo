package main

import "fmt"

func main() {
	var n int32
	fmt.Scanf("%d\n", &n)
	var array = make([]int32, n)
	for i := range array {
		array[i] = int32(i) + 1
	}

	for i := range array {
		array[i], array[i / 2] = array[i / 2], array[i]
	}

	for _, e := range array {
		fmt.Print(e, " ")
	}
	fmt.Println()
}