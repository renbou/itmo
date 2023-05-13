package main

import (
	"bufio"
	"fmt"
	"os"
	"strconv"
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
	inputFileName = "quack.in"
	outputFileName = "quack.out"
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

/* * * * * * * * * * * * * * * *  *
 * Resizable queue implementation *
 * * * * * * * * * * * * * * * *  */

type Queue struct {
	Lstack, Rstack []uint16
}

func (q *Queue) Pop() uint16 {
	if len(q.Lstack) == 0 {
		// Check for invalid operation - popping an empty queue
		if len(q.Rstack) == 0 {
			panic("Popping an empty queue")
		}

		for len(q.Rstack) > 0 {
			q.Lstack = append(q.Lstack, q.Rstack[len(q.Rstack) - 1])
			q.Rstack = q.Rstack[:len(q.Rstack) - 1]
		}
	}

	// remember to shrink the left stack after popping
	defer func() {
		q.Lstack = q.Lstack[:len(q.Lstack) - 1]
	}()
	return q.Lstack[len(q.Lstack) - 1]
}

func (q *Queue) Push(value uint16) {
	q.Rstack = append(q.Rstack, value)
}

/* * * * * * * * *
 * Main VM stuff *
 * * * * * * * * */

type QuackVMStatus int
const (
	RUNNING QuackVMStatus = iota
	HALT
)

type QuackVM struct {
	// smth like harvard?
	InstructionMemory []string
	DataMemory *Queue
	// program counter / instruction pointer
	PC int
	// the registers a..z
	Registers [26]uint16
	// the saved labels of the program, we store the relevant PC here
	Labels map[string]int
}

func CreateQuackVM() *QuackVM {
	return &QuackVM{
		InstructionMemory: nil,
		DataMemory:        nil,
		PC:                0,
		Registers:         [26]uint16{},
	}
}

// Load the program into the qvm
func (qvm *QuackVM) LoadInstructions(instructionMemory []string) {
	qvm.InstructionMemory = instructionMemory
}

// Non-exportable helper to prepare the qvm's data memory / queue
func (qvm *QuackVM) prepareDataMemory() {
	qvm.DataMemory = &Queue{
		Lstack: make([]uint16, 0, 1000),
		Rstack: make([]uint16, 0, 1000),
	}
}

// Non-exportable helper to prepare all the labels
func (qvm *QuackVM) loadAllLabels() {
	qvm.Labels = make(map[string]int, 1000)
	for pc, instruction := range qvm.InstructionMemory {
		if instruction[0] == ':' {
			if _, ok := qvm.Labels[instruction[1:]]; ok {
				panic("Same label repeated twice in program")
			}
			qvm.Labels[instruction[1:]] = pc
		}
	}
}

// Non-exportable function which sets the specified register on a running qvm
func (qvm *QuackVM) setRegister(register byte, value uint16) {
	if register < 'a' || register > 'z' {
		panic("Trying to set invalid register")
	}
	qvm.Registers[register - 'a'] = value
}

// Non-exportable function which gets the specified register on a running qvm
func (qvm *QuackVM) getRegister(register byte) uint16 {
	if register < 'a' || register > 'z' {
		panic("Trying to get invalid register")
	}
	return qvm.Registers[register - 'a']
}

// Non-exportable function which executes one specified command on a started-up qvm
func (qvm *QuackVM) executeInstruction(inst string) QuackVMStatus {
	switch inst[0]  {
	case '+':
		qvm.DataMemory.Push(qvm.DataMemory.Pop() + qvm.DataMemory.Pop())
	case '-':
		qvm.DataMemory.Push(qvm.DataMemory.Pop() - qvm.DataMemory.Pop())
	case '*':
		qvm.DataMemory.Push(qvm.DataMemory.Pop() * qvm.DataMemory.Pop())
	case '/':
		var (
			a = qvm.DataMemory.Pop()
			b = qvm.DataMemory.Pop()
		)
		if b != 0 {
			qvm.DataMemory.Push(a / b)
		} else {
			qvm.DataMemory.Push(0)
		}
	case '%':
		var (
			a = qvm.DataMemory.Pop()
			b = qvm.DataMemory.Pop()
		)
		if b != 0 {
			qvm.DataMemory.Push(a % b)
		} else {
			qvm.DataMemory.Push(0)
		}
	case '>':
		qvm.setRegister(inst[1], qvm.DataMemory.Pop())
	case '<':
		qvm.DataMemory.Push(qvm.getRegister(inst[1]))
	case 'P':
		if len(inst) < 2 {
			printf("%d\n", qvm.DataMemory.Pop())
		} else {
			printf("%d\n", qvm.getRegister(inst[1]))
		}
	case 'C':
		if len(inst) < 2 {
			printf("%c", qvm.DataMemory.Pop() & 0xff)
		} else {
			printf("%c", qvm.getRegister(inst[1]) & 0xff)
		}
	case ':':
		// we parsed labels before starting
		break
	case 'J':
		if LabelPC, ok := qvm.Labels[inst[1:]]; ok {
			qvm.PC = LabelPC
		} else {
			panic("Tried jumping to non-existent label")
		}
	case 'Z':
		if qvm.getRegister(inst[1]) == 0 {
			if LabelPC, ok := qvm.Labels[inst[2:]]; ok {
				qvm.PC = LabelPC
			} else {
				panic("Tried jumping to non-existent label during jz")
			}
		}
	case 'E':
		if qvm.getRegister(inst[1]) == qvm.getRegister(inst[2]) {
			if LabelPC, ok := qvm.Labels[inst[3:]]; ok {
				qvm.PC = LabelPC
			} else {
				panic("Tried jumping to non-existent label during je")
			}
		}
	case 'G':
		if qvm.getRegister(inst[1]) > qvm.getRegister(inst[2]) {
			if LabelPC, ok := qvm.Labels[inst[3:]]; ok {
				qvm.PC = LabelPC
			} else {
				panic("Tried jumping to non-existent label during jg")
			}
		}
	case 'Q':
		return HALT
	case '0', '1', '2', '3', '4', '5', '6', '7', '8', '9':
		if res, err := strconv.Atoi(inst); err == nil {
			qvm.DataMemory.Push(uint16(res))
		} else {
			panic("Invalid instruction masked as a number in the beginning")
		}
	default:
		panic("Invalid instruction")
	}
	return RUNNING
}

// Start up the qvm, executing from PC=0
func (qvm *QuackVM) RunQVM() {
	if qvm.InstructionMemory == nil {
		panic("Instructions haven't been loaded into QVM")
	}
	// Reset and initialize the qvm
	qvm.prepareDataMemory()
	qvm.PC = 0
	qvm.Registers = [26]uint16{}

	qvm.loadAllLabels()

	// Execute all the instructions and quit if we halted
	for ; qvm.PC < len(qvm.InstructionMemory) ; qvm.PC++ {
		if qvm.executeInstruction(qvm.InstructionMemory[qvm.PC]) == HALT {
			return
		}
	}
	return
}

func main() {
	defer bufferedOutputWriter.Flush()

	var (
		qvm = CreateQuackVM()
		commands = make([]string, 0, 1000)
		command string
	)
	for _, err := scanf("%s\n", &command); err == nil; _, err = scanf("%s\n", &command) {
		commands = append(commands, command)
	}
	qvm.LoadInstructions(commands)
	qvm.RunQVM()
}