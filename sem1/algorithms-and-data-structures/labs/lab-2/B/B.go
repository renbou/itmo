package main

import (
	"bufio"
	"fmt"
	"os"
)

var reader = bufio.NewReader(os.Stdin)
var writer = bufio.NewWriter(os.Stdout)
var printf = func (format string, valist ...interface{}) { fmt.Fprintf(writer, format, valist...) }
var scanf = func (format string, valist ...interface{}) { fmt.Fscanf(reader, format, valist...) }

type person struct {
	country, person string
}

func merge(a, b []person) ([]person) {
	var (
		result = make([]person, len(a)+len(b))
		i1 = 0
		i2 = 0
	)
	for i1 < len(a) && i2 < len(b) {
		if a[i1].country <= b[i2].country {
			result[i1 + i2] = a[i1]
			i1++
		} else {
			result[i1 + i2] = b[i2]
			i2++
		}
	}

	for ; i1 < len(a); i1++ {
		result[i1 + i2] = a[i1]
	}
	for ; i2 < len(b); i2++ {
		result[i1 + i2] = b[i2]
	}
	return result
}

func stableMergeSort(array []person) ([]person) {
	var n = len(array)
	if n <= 2 {
		if n == 1 {
			return array
		} else {
			var add = 0
			if array[0].country > array[1].country {
				array[0], array[1] = array[1], array[0]
				add++
			}
			return array
		}
	}
	var mid = (n / 2)
	var (
		a = make([]person, mid)
		b = make([]person, n - mid)
	)
	copy(a, array[:mid])
	copy(b, array[mid:])
	left := stableMergeSort(a)
	right := stableMergeSort(b)
	return merge(left, right)
}

var debug = true
func init() {
	if (debug) {
		file, err := os.Open("race.in")
		if err != nil {
			fmt.Printf(err.Error())
			os.Exit(-1)
		}
		reader = bufio.NewReader(file)
		file1, err := os.Create("race.out")
		if err != nil {
			fmt.Printf(err.Error())
			os.Exit(-1)
		}
		writer = bufio.NewWriter(file1)
	}
}

func main() {
	defer writer.Flush()

	var n int32
	scanf("%d\n", &n)
	var array = make([]person, n)
	var country, name string
	for i := range array {
		scanf("%s %s\n", &country, &name)
		array[i] = person{country, name}
	}
	array = stableMergeSort(array)

	var current = array[0].country
	printf("=== %s ===\n", current)
	for _, e := range array {
		if e.country != current {
			current = e.country
			printf("=== %s ===\n", current)
		}
		printf("%s\n", e.person)
	}
}
