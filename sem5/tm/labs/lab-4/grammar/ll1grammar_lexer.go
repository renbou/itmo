// Code generated from java-escape by ANTLR 4.11.1. DO NOT EDIT.

package grammar

import (
	"fmt"
	"sync"
	"unicode"

	"github.com/antlr/antlr4/runtime/Go/antlr/v4"
)

// Suppress unused import error
var _ = fmt.Printf
var _ = sync.Once{}
var _ = unicode.IsLetter

type LL1GrammarLexer struct {
	*antlr.BaseLexer
	channelNames []string
	modeNames    []string
	// TODO: EOF string
}

var ll1grammarlexerLexerStaticData struct {
	once                   sync.Once
	serializedATN          []int32
	channelNames           []string
	modeNames              []string
	literalNames           []string
	symbolicNames          []string
	ruleNames              []string
	predictionContextCache *antlr.PredictionContextCache
	atn                    *antlr.ATN
	decisionToDFA          []*antlr.DFA
}

func ll1grammarlexerLexerInit() {
	staticData := &ll1grammarlexerLexerStaticData
	staticData.channelNames = []string{
		"DEFAULT_TOKEN_CHANNEL", "HIDDEN",
	}
	staticData.modeNames = []string{
		"DEFAULT_MODE",
	}
	staticData.literalNames = []string{
		"", "'tokens'", "'{'", "'}'", "'='", "';'", "'rules'",
	}
	staticData.symbolicNames = []string{
		"", "", "", "", "", "", "", "Code", "Literal", "TokenIdent", "RuleIdent",
		"Whitespace",
	}
	staticData.ruleNames = []string{
		"T__0", "T__1", "T__2", "T__3", "T__4", "T__5", "Code", "CodeBlock",
		"Literal", "TokenIdent", "RuleIdent", "Digit", "Letter", "Whitespace",
	}
	staticData.predictionContextCache = antlr.NewPredictionContextCache()
	staticData.serializedATN = []int32{
		4, 0, 11, 115, 6, -1, 2, 0, 7, 0, 2, 1, 7, 1, 2, 2, 7, 2, 2, 3, 7, 3, 2,
		4, 7, 4, 2, 5, 7, 5, 2, 6, 7, 6, 2, 7, 7, 7, 2, 8, 7, 8, 2, 9, 7, 9, 2,
		10, 7, 10, 2, 11, 7, 11, 2, 12, 7, 12, 2, 13, 7, 13, 1, 0, 1, 0, 1, 0,
		1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 1, 2, 1, 2, 1, 3, 1, 3, 1, 4, 1, 4,
		1, 5, 1, 5, 1, 5, 1, 5, 1, 5, 1, 5, 1, 6, 1, 6, 5, 6, 53, 8, 6, 10, 6,
		12, 6, 56, 9, 6, 1, 6, 1, 6, 1, 7, 4, 7, 61, 8, 7, 11, 7, 12, 7, 62, 1,
		7, 1, 7, 5, 7, 67, 8, 7, 10, 7, 12, 7, 70, 9, 7, 1, 7, 3, 7, 73, 8, 7,
		1, 8, 1, 8, 1, 8, 1, 8, 1, 8, 1, 8, 4, 8, 81, 8, 8, 11, 8, 12, 8, 82, 1,
		8, 1, 8, 1, 9, 1, 9, 1, 9, 1, 9, 5, 9, 91, 8, 9, 10, 9, 12, 9, 94, 9, 9,
		1, 10, 1, 10, 1, 10, 1, 10, 5, 10, 100, 8, 10, 10, 10, 12, 10, 103, 9,
		10, 1, 11, 1, 11, 1, 12, 1, 12, 1, 13, 4, 13, 110, 8, 13, 11, 13, 12, 13,
		111, 1, 13, 1, 13, 0, 0, 14, 1, 1, 3, 2, 5, 3, 7, 4, 9, 5, 11, 6, 13, 7,
		15, 0, 17, 8, 19, 9, 21, 10, 23, 0, 25, 0, 27, 11, 1, 0, 5, 2, 0, 91, 91,
		93, 93, 1, 0, 34, 34, 1, 0, 48, 57, 2, 0, 65, 90, 97, 122, 10, 0, 9, 13,
		32, 32, 133, 133, 160, 160, 5760, 5760, 8192, 8202, 8232, 8233, 8239, 8239,
		8287, 8287, 12288, 12288, 125, 0, 1, 1, 0, 0, 0, 0, 3, 1, 0, 0, 0, 0, 5,
		1, 0, 0, 0, 0, 7, 1, 0, 0, 0, 0, 9, 1, 0, 0, 0, 0, 11, 1, 0, 0, 0, 0, 13,
		1, 0, 0, 0, 0, 17, 1, 0, 0, 0, 0, 19, 1, 0, 0, 0, 0, 21, 1, 0, 0, 0, 0,
		27, 1, 0, 0, 0, 1, 29, 1, 0, 0, 0, 3, 36, 1, 0, 0, 0, 5, 38, 1, 0, 0, 0,
		7, 40, 1, 0, 0, 0, 9, 42, 1, 0, 0, 0, 11, 44, 1, 0, 0, 0, 13, 50, 1, 0,
		0, 0, 15, 72, 1, 0, 0, 0, 17, 74, 1, 0, 0, 0, 19, 86, 1, 0, 0, 0, 21, 95,
		1, 0, 0, 0, 23, 104, 1, 0, 0, 0, 25, 106, 1, 0, 0, 0, 27, 109, 1, 0, 0,
		0, 29, 30, 5, 116, 0, 0, 30, 31, 5, 111, 0, 0, 31, 32, 5, 107, 0, 0, 32,
		33, 5, 101, 0, 0, 33, 34, 5, 110, 0, 0, 34, 35, 5, 115, 0, 0, 35, 2, 1,
		0, 0, 0, 36, 37, 5, 123, 0, 0, 37, 4, 1, 0, 0, 0, 38, 39, 5, 125, 0, 0,
		39, 6, 1, 0, 0, 0, 40, 41, 5, 61, 0, 0, 41, 8, 1, 0, 0, 0, 42, 43, 5, 59,
		0, 0, 43, 10, 1, 0, 0, 0, 44, 45, 5, 114, 0, 0, 45, 46, 5, 117, 0, 0, 46,
		47, 5, 108, 0, 0, 47, 48, 5, 101, 0, 0, 48, 49, 5, 115, 0, 0, 49, 12, 1,
		0, 0, 0, 50, 54, 5, 91, 0, 0, 51, 53, 3, 15, 7, 0, 52, 51, 1, 0, 0, 0,
		53, 56, 1, 0, 0, 0, 54, 52, 1, 0, 0, 0, 54, 55, 1, 0, 0, 0, 55, 57, 1,
		0, 0, 0, 56, 54, 1, 0, 0, 0, 57, 58, 5, 93, 0, 0, 58, 14, 1, 0, 0, 0, 59,
		61, 8, 0, 0, 0, 60, 59, 1, 0, 0, 0, 61, 62, 1, 0, 0, 0, 62, 60, 1, 0, 0,
		0, 62, 63, 1, 0, 0, 0, 63, 73, 1, 0, 0, 0, 64, 68, 5, 91, 0, 0, 65, 67,
		3, 15, 7, 0, 66, 65, 1, 0, 0, 0, 67, 70, 1, 0, 0, 0, 68, 66, 1, 0, 0, 0,
		68, 69, 1, 0, 0, 0, 69, 71, 1, 0, 0, 0, 70, 68, 1, 0, 0, 0, 71, 73, 5,
		93, 0, 0, 72, 60, 1, 0, 0, 0, 72, 64, 1, 0, 0, 0, 73, 16, 1, 0, 0, 0, 74,
		80, 7, 1, 0, 0, 75, 81, 8, 1, 0, 0, 76, 77, 5, 92, 0, 0, 77, 81, 5, 34,
		0, 0, 78, 79, 5, 92, 0, 0, 79, 81, 5, 92, 0, 0, 80, 75, 1, 0, 0, 0, 80,
		76, 1, 0, 0, 0, 80, 78, 1, 0, 0, 0, 81, 82, 1, 0, 0, 0, 82, 80, 1, 0, 0,
		0, 82, 83, 1, 0, 0, 0, 83, 84, 1, 0, 0, 0, 84, 85, 7, 1, 0, 0, 85, 18,
		1, 0, 0, 0, 86, 92, 2, 97, 122, 0, 87, 91, 3, 25, 12, 0, 88, 91, 3, 23,
		11, 0, 89, 91, 5, 95, 0, 0, 90, 87, 1, 0, 0, 0, 90, 88, 1, 0, 0, 0, 90,
		89, 1, 0, 0, 0, 91, 94, 1, 0, 0, 0, 92, 90, 1, 0, 0, 0, 92, 93, 1, 0, 0,
		0, 93, 20, 1, 0, 0, 0, 94, 92, 1, 0, 0, 0, 95, 101, 2, 65, 90, 0, 96, 100,
		3, 25, 12, 0, 97, 100, 3, 23, 11, 0, 98, 100, 5, 95, 0, 0, 99, 96, 1, 0,
		0, 0, 99, 97, 1, 0, 0, 0, 99, 98, 1, 0, 0, 0, 100, 103, 1, 0, 0, 0, 101,
		99, 1, 0, 0, 0, 101, 102, 1, 0, 0, 0, 102, 22, 1, 0, 0, 0, 103, 101, 1,
		0, 0, 0, 104, 105, 7, 2, 0, 0, 105, 24, 1, 0, 0, 0, 106, 107, 7, 3, 0,
		0, 107, 26, 1, 0, 0, 0, 108, 110, 7, 4, 0, 0, 109, 108, 1, 0, 0, 0, 110,
		111, 1, 0, 0, 0, 111, 109, 1, 0, 0, 0, 111, 112, 1, 0, 0, 0, 112, 113,
		1, 0, 0, 0, 113, 114, 6, 13, 0, 0, 114, 28, 1, 0, 0, 0, 12, 0, 54, 62,
		68, 72, 80, 82, 90, 92, 99, 101, 111, 1, 6, 0, 0,
	}
	deserializer := antlr.NewATNDeserializer(nil)
	staticData.atn = deserializer.Deserialize(staticData.serializedATN)
	atn := staticData.atn
	staticData.decisionToDFA = make([]*antlr.DFA, len(atn.DecisionToState))
	decisionToDFA := staticData.decisionToDFA
	for index, state := range atn.DecisionToState {
		decisionToDFA[index] = antlr.NewDFA(state, index)
	}
}

// LL1GrammarLexerInit initializes any static state used to implement LL1GrammarLexer. By default the
// static state used to implement the lexer is lazily initialized during the first call to
// NewLL1GrammarLexer(). You can call this function if you wish to initialize the static state ahead
// of time.
func LL1GrammarLexerInit() {
	staticData := &ll1grammarlexerLexerStaticData
	staticData.once.Do(ll1grammarlexerLexerInit)
}

// NewLL1GrammarLexer produces a new lexer instance for the optional input antlr.CharStream.
func NewLL1GrammarLexer(input antlr.CharStream) *LL1GrammarLexer {
	LL1GrammarLexerInit()
	l := new(LL1GrammarLexer)
	l.BaseLexer = antlr.NewBaseLexer(input)
	staticData := &ll1grammarlexerLexerStaticData
	l.Interpreter = antlr.NewLexerATNSimulator(l, staticData.atn, staticData.decisionToDFA, staticData.predictionContextCache)
	l.channelNames = staticData.channelNames
	l.modeNames = staticData.modeNames
	l.RuleNames = staticData.ruleNames
	l.LiteralNames = staticData.literalNames
	l.SymbolicNames = staticData.symbolicNames
	l.GrammarFileName = "LL1Grammar.g4"
	// TODO: l.EOF = antlr.TokenEOF

	return l
}

// LL1GrammarLexer tokens.
const (
	LL1GrammarLexerT__0       = 1
	LL1GrammarLexerT__1       = 2
	LL1GrammarLexerT__2       = 3
	LL1GrammarLexerT__3       = 4
	LL1GrammarLexerT__4       = 5
	LL1GrammarLexerT__5       = 6
	LL1GrammarLexerCode       = 7
	LL1GrammarLexerLiteral    = 8
	LL1GrammarLexerTokenIdent = 9
	LL1GrammarLexerRuleIdent  = 10
	LL1GrammarLexerWhitespace = 11
)
