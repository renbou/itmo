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

var insertSortLimit int32 = 12

func insertionSort(array []int32, left, right int32) {
	for j := left + 1; j <= right; j++ {
		key := array[j]
		i := j - 1
		for ; i >= left && array[i] > key; i-- {
			array[i + 1] = array[i]
		}
		array[i + 1] = key
	}
}

func partition(array []int32, left, right int32) int32 {
	pivot := array[(left + right) / 2]
	i, j := left, right
	for ; i <= j ; i, j = i+1, j-1 {
		for ; array[i] < pivot ; {
			i++
		}
		for ; array[j] > pivot ; {
			j--
		}
		if i >= j {
			break
		}
		array[i], array[j] = array[j], array[i]
	}
	return j
}

func quickSort(array []int32, left, right int32) {
	var medians = make([]struct {a, b int32}, 5)
	var i, current, median, mid int32
	for ; left < right ; {
		if (right - left) < insertSortLimit {
			insertionSort(array, left, right)
			return
		}
		var partSize int32 = (right - left + 1) / 5
		// find the median element out of 5
		current = 0
		for i = 0; i < 5; i++ {
			medians[i].b = left + current
			medians[i].a = array[medians[i].b]
			current += partSize
		}
		if medians[1].a < medians[0].a {
			medians[0], medians[1] = medians[1], medians[0]
		}
		if medians[3].a < medians[2].a {
			medians[2], medians[3] = medians[3], medians[2]
		}
		if medians[3].a < medians[1].a {
			medians[0], medians[1], medians[2], medians[3] = medians[2], medians[3], medians[0], medians[1]
		}
		if medians[2].a < medians[0].a {
			medians[0], medians[1], medians[2] = medians[2], medians[0], medians[1]
		} else if medians[2].a < medians[1].a {
			medians[1], medians[2] = medians[2], medians[1]
		}
		// now if we sort using insertion sort we need only 5 swaps maximum
		for i = 3; i >= 0 && medians[i].a > medians[4].a; i-- {
			medians[i + 1] = medians[i]
		}
		medians[i + 1] = medians[4]
		median = medians[2].b
		mid = (left + right) / 2
		array[mid], array[median] = array[median], array[mid]

		pivot := partition(array, left, right)

		// select smallest part and sort it recursively
		if pivot - left < right - pivot {
			quickSort(array, left, pivot)
			left = pivot + 1
		} else {
			quickSort(array, pivot + 1, right)
			right = pivot
		}
	}
}

var debug = true
func init() {
	if (debug) {
		file, err := os.Open("./sort.in")
		if err != nil {
			fmt.Printf(err.Error())
			os.Exit(-1)
		}
		reader = bufio.NewReader(file)
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
	quickSort(array, 0, n - 1)

	for _, e := range array {
		printf("%d ", e)
	}
	printf("\n")
}
