package main

import (
	"bufio"
	"fmt"
	"os"
	"strconv"
	"strings"
)

var (
	bufferedInputReader = bufio.NewReader(os.Stdin)
	bufferedOutputWriter = bufio.NewWriter(os.Stdout)
	printf = func (format string, valist ...interface{}) {
				fmt.Fprintf(bufferedOutputWriter, format, valist...) }
	scanf = func (format string, valist ...interface{}) {
				fmt.Fscanf(bufferedInputReader, format, valist...) }
)

const (
	inputFileName = "priorityqueue.in"
	outputFileName = "priorityqueue.out"
)

var fileInputMode = true
func init() {
	if (fileInputMode) {
		inputFile, err := os.Open(inputFileName)
		if err != nil {
			fmt.Printf(err.Error())
			os.Exit(-1)
		}
		outputFile, err := os.Create(outputFileName)
		if err != nil {
			fmt.Printf(err.Error())
			os.Exit(-1)
		}
		bufferedInputReader = bufio.NewReader(inputFile)
		bufferedOutputWriter = bufio.NewWriter(outputFile)
	}
}

type PriorityQueue struct {
	heap, order, reverseOrder []int
	opIdToInsertOrder map[int]int
	opCnt int
}

func MakePriorityQueue(defaultCap int) *PriorityQueue {
	return &PriorityQueue{
		heap:  make([]int, 0, defaultCap),
		order: make([]int, 0, defaultCap),
		reverseOrder: make([]int, 0, defaultCap),
		opIdToInsertOrder: make(map[int]int),
		opCnt: 0,
	}
}

func (pq *PriorityQueue) siftUp(i, n int) {
	for n > 0 {
		parent := (n - 1) / 2
		if pq.order[pq.heap[parent]] > pq.order[pq.heap[n]] {
			pq.reverseOrder[i], pq.reverseOrder[pq.heap[parent]] = parent, n
			pq.heap[parent], pq.heap[n] = pq.heap[n], pq.heap[parent]
		}
		n = parent
	}
}

func (pq *PriorityQueue) insert(value int) {
	pq.opCnt++

	order_n := len(pq.order)
	n := len(pq.heap)
	pq.order = append(pq.order, value)
	pq.heap = append(pq.heap, order_n)
	pq.reverseOrder = append(pq.reverseOrder, n)
	pq.opIdToInsertOrder[pq.opCnt] = order_n
	pq.siftUp(order_n, n)
}

func (pq *PriorityQueue) siftDown(i, n int) {
	var (
		cur = -1
		min = i
	)
	for min < n / 2 && cur != min {
		cur = min
		left, right := 2 * cur + 1, 2 * cur + 2
		if left < n && pq.order[pq.heap[left]] < pq.order[pq.heap[min]] {
			min = left
		}
		if right < n && pq.order[pq.heap[right]] < pq.order[pq.heap[min]] {
			min = right
		}
		pq.reverseOrder[pq.heap[min]], pq.reverseOrder[pq.heap[cur]] =
			cur, min
		pq.heap[min], pq.heap[cur] = pq.heap[cur], pq.heap[min]
	}
}

func (pq *PriorityQueue) extractMin() string {
	pq.opCnt++

	n := len(pq.heap) - 1
	if n < 0 {
		return "*"
	}
	result := pq.order[pq.heap[0]]
	pq.reverseOrder[pq.heap[n]] = 0
	pq.heap[0] = pq.heap[n]
	pq.heap = pq.heap[:n]
	pq.siftDown(0, n)
	return strconv.FormatInt(int64(result), 10)
}

func (pq *PriorityQueue) decreaseKey(order, value int) {
	pq.opCnt++
	order = pq.opIdToInsertOrder[order]
	pq.order[order] = value
	pq.siftUp(order, pq.reverseOrder[order])
}

func main() {
	defer bufferedOutputWriter.Flush()

	const MAXN = 1000000 + 10

	pq := MakePriorityQueue(MAXN)
	line, _ := bufferedInputReader.ReadString('\n')
	for ; len(line) != 0; line, _ = bufferedInputReader.ReadString('\n') {
		line = strings.TrimSpace(line)
		tokens := strings.Split(line, " ")
		switch tokens[0] {
		case "push":
			value, _ := strconv.ParseInt(tokens[1], 10, 32)
			pq.insert(int(value))
			break
		case "extract-min":
			printf("%s\n", pq.extractMin())
			break
		case "decrease-key":
			order, _ := strconv.Atoi(tokens[1])
			value, _ := strconv.Atoi(tokens[2])
			pq.decreaseKey(order, value)
		}
		if !fileInputMode {
			bufferedOutputWriter.Flush()
		}
	}
}