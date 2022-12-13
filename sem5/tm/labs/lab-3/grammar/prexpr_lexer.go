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

type PrExprLexer struct {
	*antlr.BaseLexer
	channelNames []string
	modeNames    []string
	// TODO: EOF string
}

var prexprlexerLexerStaticData struct {
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

func prexprlexerLexerInit() {
	staticData := &prexprlexerLexerStaticData
	staticData.channelNames = []string{
		"DEFAULT_TOKEN_CHANNEL", "HIDDEN",
	}
	staticData.modeNames = []string{
		"DEFAULT_MODE",
	}
	staticData.literalNames = []string{
		"", "'print'", "'if'", "'&&'", "'||'", "'=='", "'<'", "'>'", "'*'",
		"'/'", "'+'", "'-'", "'!'", "'='",
	}
	staticData.symbolicNames = []string{
		"", "", "", "", "", "", "", "", "", "", "", "", "", "", "Bool", "Integer",
		"Ident", "Whitespace",
	}
	staticData.ruleNames = []string{
		"T__0", "T__1", "T__2", "T__3", "T__4", "T__5", "T__6", "T__7", "T__8",
		"T__9", "T__10", "T__11", "T__12", "Bool", "Integer", "Ident", "Digit",
		"Letter", "Whitespace",
	}
	staticData.predictionContextCache = antlr.NewPredictionContextCache()
	staticData.serializedATN = []int32{
		4, 0, 17, 106, 6, -1, 2, 0, 7, 0, 2, 1, 7, 1, 2, 2, 7, 2, 2, 3, 7, 3, 2,
		4, 7, 4, 2, 5, 7, 5, 2, 6, 7, 6, 2, 7, 7, 7, 2, 8, 7, 8, 2, 9, 7, 9, 2,
		10, 7, 10, 2, 11, 7, 11, 2, 12, 7, 12, 2, 13, 7, 13, 2, 14, 7, 14, 2, 15,
		7, 15, 2, 16, 7, 16, 2, 17, 7, 17, 2, 18, 7, 18, 1, 0, 1, 0, 1, 0, 1, 0,
		1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 2, 1, 2, 1, 2, 1, 3, 1, 3, 1, 3, 1, 4,
		1, 4, 1, 4, 1, 5, 1, 5, 1, 6, 1, 6, 1, 7, 1, 7, 1, 8, 1, 8, 1, 9, 1, 9,
		1, 10, 1, 10, 1, 11, 1, 11, 1, 12, 1, 12, 1, 13, 1, 13, 1, 13, 1, 13, 1,
		13, 1, 13, 1, 13, 1, 13, 1, 13, 3, 13, 83, 8, 13, 1, 14, 4, 14, 86, 8,
		14, 11, 14, 12, 14, 87, 1, 15, 1, 15, 1, 15, 1, 15, 5, 15, 94, 8, 15, 10,
		15, 12, 15, 97, 9, 15, 1, 16, 1, 16, 1, 17, 1, 17, 1, 18, 1, 18, 1, 18,
		1, 18, 0, 0, 19, 1, 1, 3, 2, 5, 3, 7, 4, 9, 5, 11, 6, 13, 7, 15, 8, 17,
		9, 19, 10, 21, 11, 23, 12, 25, 13, 27, 14, 29, 15, 31, 16, 33, 0, 35, 0,
		37, 17, 1, 0, 3, 1, 0, 48, 57, 2, 0, 65, 90, 97, 122, 10, 0, 9, 13, 32,
		32, 133, 133, 160, 160, 5760, 5760, 8192, 8202, 8232, 8233, 8239, 8239,
		8287, 8287, 12288, 12288, 108, 0, 1, 1, 0, 0, 0, 0, 3, 1, 0, 0, 0, 0, 5,
		1, 0, 0, 0, 0, 7, 1, 0, 0, 0, 0, 9, 1, 0, 0, 0, 0, 11, 1, 0, 0, 0, 0, 13,
		1, 0, 0, 0, 0, 15, 1, 0, 0, 0, 0, 17, 1, 0, 0, 0, 0, 19, 1, 0, 0, 0, 0,
		21, 1, 0, 0, 0, 0, 23, 1, 0, 0, 0, 0, 25, 1, 0, 0, 0, 0, 27, 1, 0, 0, 0,
		0, 29, 1, 0, 0, 0, 0, 31, 1, 0, 0, 0, 0, 37, 1, 0, 0, 0, 1, 39, 1, 0, 0,
		0, 3, 45, 1, 0, 0, 0, 5, 48, 1, 0, 0, 0, 7, 51, 1, 0, 0, 0, 9, 54, 1, 0,
		0, 0, 11, 57, 1, 0, 0, 0, 13, 59, 1, 0, 0, 0, 15, 61, 1, 0, 0, 0, 17, 63,
		1, 0, 0, 0, 19, 65, 1, 0, 0, 0, 21, 67, 1, 0, 0, 0, 23, 69, 1, 0, 0, 0,
		25, 71, 1, 0, 0, 0, 27, 82, 1, 0, 0, 0, 29, 85, 1, 0, 0, 0, 31, 89, 1,
		0, 0, 0, 33, 98, 1, 0, 0, 0, 35, 100, 1, 0, 0, 0, 37, 102, 1, 0, 0, 0,
		39, 40, 5, 112, 0, 0, 40, 41, 5, 114, 0, 0, 41, 42, 5, 105, 0, 0, 42, 43,
		5, 110, 0, 0, 43, 44, 5, 116, 0, 0, 44, 2, 1, 0, 0, 0, 45, 46, 5, 105,
		0, 0, 46, 47, 5, 102, 0, 0, 47, 4, 1, 0, 0, 0, 48, 49, 5, 38, 0, 0, 49,
		50, 5, 38, 0, 0, 50, 6, 1, 0, 0, 0, 51, 52, 5, 124, 0, 0, 52, 53, 5, 124,
		0, 0, 53, 8, 1, 0, 0, 0, 54, 55, 5, 61, 0, 0, 55, 56, 5, 61, 0, 0, 56,
		10, 1, 0, 0, 0, 57, 58, 5, 60, 0, 0, 58, 12, 1, 0, 0, 0, 59, 60, 5, 62,
		0, 0, 60, 14, 1, 0, 0, 0, 61, 62, 5, 42, 0, 0, 62, 16, 1, 0, 0, 0, 63,
		64, 5, 47, 0, 0, 64, 18, 1, 0, 0, 0, 65, 66, 5, 43, 0, 0, 66, 20, 1, 0,
		0, 0, 67, 68, 5, 45, 0, 0, 68, 22, 1, 0, 0, 0, 69, 70, 5, 33, 0, 0, 70,
		24, 1, 0, 0, 0, 71, 72, 5, 61, 0, 0, 72, 26, 1, 0, 0, 0, 73, 74, 5, 102,
		0, 0, 74, 75, 5, 97, 0, 0, 75, 76, 5, 108, 0, 0, 76, 77, 5, 115, 0, 0,
		77, 83, 5, 101, 0, 0, 78, 79, 5, 116, 0, 0, 79, 80, 5, 114, 0, 0, 80, 81,
		5, 117, 0, 0, 81, 83, 5, 101, 0, 0, 82, 73, 1, 0, 0, 0, 82, 78, 1, 0, 0,
		0, 83, 28, 1, 0, 0, 0, 84, 86, 3, 33, 16, 0, 85, 84, 1, 0, 0, 0, 86, 87,
		1, 0, 0, 0, 87, 85, 1, 0, 0, 0, 87, 88, 1, 0, 0, 0, 88, 30, 1, 0, 0, 0,
		89, 95, 3, 35, 17, 0, 90, 94, 3, 35, 17, 0, 91, 94, 3, 33, 16, 0, 92, 94,
		5, 95, 0, 0, 93, 90, 1, 0, 0, 0, 93, 91, 1, 0, 0, 0, 93, 92, 1, 0, 0, 0,
		94, 97, 1, 0, 0, 0, 95, 93, 1, 0, 0, 0, 95, 96, 1, 0, 0, 0, 96, 32, 1,
		0, 0, 0, 97, 95, 1, 0, 0, 0, 98, 99, 7, 0, 0, 0, 99, 34, 1, 0, 0, 0, 100,
		101, 7, 1, 0, 0, 101, 36, 1, 0, 0, 0, 102, 103, 7, 2, 0, 0, 103, 104, 1,
		0, 0, 0, 104, 105, 6, 18, 0, 0, 105, 38, 1, 0, 0, 0, 5, 0, 82, 87, 93,
		95, 1, 6, 0, 0,
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

// PrExprLexerInit initializes any static state used to implement PrExprLexer. By default the
// static state used to implement the lexer is lazily initialized during the first call to
// NewPrExprLexer(). You can call this function if you wish to initialize the static state ahead
// of time.
func PrExprLexerInit() {
	staticData := &prexprlexerLexerStaticData
	staticData.once.Do(prexprlexerLexerInit)
}

// NewPrExprLexer produces a new lexer instance for the optional input antlr.CharStream.
func NewPrExprLexer(input antlr.CharStream) *PrExprLexer {
	PrExprLexerInit()
	l := new(PrExprLexer)
	l.BaseLexer = antlr.NewBaseLexer(input)
	staticData := &prexprlexerLexerStaticData
	l.Interpreter = antlr.NewLexerATNSimulator(l, staticData.atn, staticData.decisionToDFA, staticData.predictionContextCache)
	l.channelNames = staticData.channelNames
	l.modeNames = staticData.modeNames
	l.RuleNames = staticData.ruleNames
	l.LiteralNames = staticData.literalNames
	l.SymbolicNames = staticData.symbolicNames
	l.GrammarFileName = "PrExpr.g4"
	// TODO: l.EOF = antlr.TokenEOF

	return l
}

// PrExprLexer tokens.
const (
	PrExprLexerT__0       = 1
	PrExprLexerT__1       = 2
	PrExprLexerT__2       = 3
	PrExprLexerT__3       = 4
	PrExprLexerT__4       = 5
	PrExprLexerT__5       = 6
	PrExprLexerT__6       = 7
	PrExprLexerT__7       = 8
	PrExprLexerT__8       = 9
	PrExprLexerT__9       = 10
	PrExprLexerT__10      = 11
	PrExprLexerT__11      = 12
	PrExprLexerT__12      = 13
	PrExprLexerBool       = 14
	PrExprLexerInteger    = 15
	PrExprLexerIdent      = 16
	PrExprLexerWhitespace = 17
)
