package main

import (
	"bufio"
	"fmt"
	"math"
	"os"
)

var (
	bufferedInputReader = bufio.NewReader(os.Stdin)
	bufferedOutputWriter = bufio.NewWriter(os.Stdout)
	printf = func (format string, valist ...interface{}) (int, error) {
		return fmt.Fprintf(bufferedOutputWriter, format, valist...) }
	scanf = func (format string, valist ...interface{}) (int, error) {
		return fmt.Fscanf(bufferedInputReader, format, valist...) }
)

const (
	inputFileName = "multimap.in"
	outputFileName = "multimap.out"
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

// this will never be inserted into the set so we can use this to check
const fictiveElement = math.MaxInt32
const cellDeleted = math.MaxInt32 - 1

type Node struct {
	previous, next *Node
	previouslyAdded, nextAdded *Node
	key interface {}
	value interface{}
}

type LinkedHashMap struct {
	size, capacity uint
	table []*Node
	firstAdded, lastAdded *Node
	hash func(interface{}) uint64
}

/* Finds the next prime for the size */
func getNewSetSize(size uint) uint {
	for actualSize := size; ; actualSize++ {
		var (
			end = uint(math.Sqrt(float64(actualSize)))
			isPrime = true
		)
		for i := uint(2); i <= end + 1 && isPrime; i++ {
			if actualSize % i == 0 {
				isPrime = false
			}
		}
		if isPrime {
			return actualSize
		}
	}
}

func MakeLinkedHashMap(size uint, hashFunction func(interface{}) uint64) *LinkedHashMap {
	var actualSize = getNewSetSize(size)
	return &LinkedHashMap{
		size:         0,
		capacity:     actualSize,
		table:        make([]*Node, actualSize),
		hash: hashFunction,
	}
}

func (lhm *LinkedHashMap) expand() {
	var tempHashMap = MakeLinkedHashMap(lhm.capacity * 2, lhm.hash)
	var node = lhm.firstAdded
	for node != nil {
		tempHashMap.Insert(node.key, node.value)
		node = node.nextAdded
	}
	lhm.table = tempHashMap.table
	lhm.firstAdded = tempHashMap.firstAdded
	lhm.lastAdded = tempHashMap.lastAdded
	lhm.capacity = tempHashMap.capacity
}

func (lhm *LinkedHashMap) Insert(key interface{}, value interface{}) bool {
	var (
		hash = lhm.hash(key)
		location = hash % uint64(lhm.capacity)
		node *Node
	)

	if lhm.table[location] == nil {
		lhm.table[location] = &Node{}
		node = lhm.table[location]
	} else {
		node = lhm.table[location]
		for node.next != nil && node.key != key {
			node = node.next
		}
		if node.key == key {
			node.value = value
			return false
		}
		node.next = &Node{previous: node}
		node = node.next
	}

	if lhm.firstAdded == nil {
		lhm.firstAdded = node
		lhm.lastAdded = node
	} else {
		lhm.lastAdded.nextAdded = node
		node.previouslyAdded = lhm.lastAdded
		lhm.lastAdded = node
	}

	node.key = key
	node.value = value
	lhm.size++
	if lhm.size == lhm.capacity {
		lhm.expand()
	}
	return true
}

/* find the node for this key if it exists, this is for Delete and Get */
func (lhm *LinkedHashMap) find(key interface{}) *Node {
	var (
		hash = lhm.hash(key)
		location = hash % uint64(lhm.capacity)
	)
	if lhm.table[location] == nil {
		return nil
	}

	var node = lhm.table[location]
	for node != nil && node.key != key {
		node = node.next
	}
	return node
}

func (lhm *LinkedHashMap) Get(key interface{}) interface{} {
	var node = lhm.find(key)
	if node != nil {
		return node.value
	}
	return nil
}

func (lhm *LinkedHashMap) Delete(key interface{}) bool {
	var node = lhm.find(key)
	if node != nil {
		var (
			hash = lhm.hash(key)
			location = hash % uint64(lhm.capacity)
		)

		/* delete the node from the insert-order linked list */
		if lhm.firstAdded == node {
			lhm.firstAdded = node.nextAdded
		}
		if lhm.lastAdded == node {
			lhm.lastAdded = node.previouslyAdded
		}
		if node.previouslyAdded != nil {
			node.previouslyAdded.nextAdded = node.nextAdded
		}
		if node.nextAdded != nil {
			node.nextAdded.previouslyAdded = node.previouslyAdded
		}
		node.previouslyAdded = nil
		node.nextAdded = nil

		/* fix the table if we're deleting the head node */
		if lhm.table[location] == node {
			lhm.table[location] = node.next
		}
		/* delete the node in the bucket linked list */
		if node.previous != nil {
			node.previous.next = node.next
		}
		if node.next != nil {
			node.next.previous = node.previous
		}
		/* clear the pointers just to be sure there's nothing dangling anywhere */
		node.previous = nil
		node.next = nil

		lhm.size--
		return true
	}
	return false
}

func (lhm *LinkedHashMap) Previous(key interface{}) (prevKey, prevValue interface{}) {
	var node = lhm.find(key)
	if node != nil && node.previouslyAdded != nil {
		prevKey, prevValue = node.previouslyAdded.key, node.previouslyAdded.value
	}
	return prevKey, prevValue
}

func (lhm *LinkedHashMap) Next(key interface{}) (nextKey, nextValue interface{}) {
	var node = lhm.find(key)
	if node != nil && node.previouslyAdded != nil {
		nextKey, nextValue = node.previouslyAdded.key, node.previouslyAdded.value
	}
	return nextKey, nextValue
}

func readNext(nextCommand, nextKey, nextValue *string) bool {
	_, err := scanf("%s %s", nextCommand, nextKey)
	if (*nextCommand)[0] == 'p' || ((*nextCommand)[0] == 'd' && len(*nextCommand) == 6){
		scanf("%s\n", nextValue)
	} else {
		scanf("\n")
	}
	return err == nil
}

func polyHash(i interface{}) uint64 {
	switch i.(type) {
	case string:
		var (
			result = uint64(0)
			str = i.(string)
		)
		for i := 0; i < len(str); i++ {
			result = (result * 37 + uint64(str[i])) % 1000000021
		}
		return result
	default:
		panic("can't hash non-int value")
	}
}

func main() {
	defer bufferedOutputWriter.Flush()

	var (
		currentCommand string
		key, value string
		set = MakeLinkedHashMap(25000, polyHash)
	)
	for nextLinePresent := readNext(&currentCommand, &key, &value); nextLinePresent; nextLinePresent = readNext(&currentCommand, &key, &value) {
		switch currentCommand {
		case "put":
			var iValues = set.Get(key)
			if iValues == nil {
				var values = MakeLinkedHashMap(5, polyHash)
				values.Insert(value, true)
				set.Insert(key, values)
			} else {
				var values = iValues.(*LinkedHashMap)
				values.Insert(value, true)
			}
		case "get":
			var iValues = set.Get(key)
			if iValues == nil {
				printf("0\n")
			} else {
				var values = iValues.(*LinkedHashMap)
				printf("%d ", values.size)
				for node := values.firstAdded; node != nil; node = node.nextAdded{
					printf("%s ", node.key.(string))
				}
				printf("\n")
			}
		case "delete":
			var iValues = set.Get(key)
			if iValues != nil {
				var values = iValues.(*LinkedHashMap)
				values.Delete(value)
			}
		case "deleteall":
			set.Delete(key)
		}
	}
}