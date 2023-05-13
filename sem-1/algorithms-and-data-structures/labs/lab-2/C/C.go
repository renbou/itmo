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

func merge(a, b []int32) ([]int32, int) {
	var (
		result = make([]int32, len(a)+len(b))
		i1 = 0
		i2 = 0
		sum = 0
	)
	for i1 < len(a) && i2 < len(b) {
		if a[i1] <= b[i2] {
			sum += i2
			result[i1 + i2] = a[i1]
			i1++
		} else {
			result[i1 + i2] = b[i2]
			i2++
		}
	}

	for ; i1 < len(a); i1++ {
		result[i1 + i2] = a[i1]
		sum += i2
	}
	for ; i2 < len(b); i2++ {
		result[i1 + i2] = b[i2]
	}
	return result, sum
}

func countInv(array []int32) ([]int32, int) {
	var n = len(array)
	if n <= 2 {
		if n == 1 {
			return array, 0
		} else {
			var add = 0
			if array[0] > array[1] {
				array[0], array[1] = array[1], array[0]
				add++
			}
			return array, add
		}
	}
	var mid = (n / 2)
	var (
		a = make([]int32, mid)
		b = make([]int32, n - mid)
	)
	copy(a, array[:mid])
	copy(b, array[mid:])
	left, lcnt := countInv(a)
	right, rcnt := countInv(b)
	result, cnt := merge(left, right)
	return result, lcnt + cnt + rcnt
}

var debug = true
func init() {
	if (debug) {
		file, err := os.Open("inversions.in")
		if err != nil {
			fmt.Printf(err.Error())
			os.Exit(-1)
		}
		reader = bufio.NewReader(file)
		file1, err := os.Create("inversions.out")
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
	var array = make([]int32, n)
	for i := range array {
		scanf("%d", &array[i])
	}

	var _, inversions = countInv(array)
	printf("%d\n", inversions)
}
