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
		"", "", "", "", "", "", "", "Literal", "TokenIdent", "RuleIdent", "Whitespace",
	}
	staticData.ruleNames = []string{
		"T__0", "T__1", "T__2", "T__3", "T__4", "T__5", "Literal", "TokenIdent",
		"RuleIdent", "Digit", "Letter", "Whitespace",
	}
	staticData.predictionContextCache = antlr.NewPredictionContextCache()
	staticData.serializedATN = []int32{
		4, 0, 10, 87, 6, -1, 2, 0, 7, 0, 2, 1, 7, 1, 2, 2, 7, 2, 2, 3, 7, 3, 2,
		4, 7, 4, 2, 5, 7, 5, 2, 6, 7, 6, 2, 7, 7, 7, 2, 8, 7, 8, 2, 9, 7, 9, 2,
		10, 7, 10, 2, 11, 7, 11, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1,
		1, 1, 1, 2, 1, 2, 1, 3, 1, 3, 1, 4, 1, 4, 1, 5, 1, 5, 1, 5, 1, 5, 1, 5,
		1, 5, 1, 6, 1, 6, 1, 6, 1, 6, 1, 6, 1, 6, 4, 6, 53, 8, 6, 11, 6, 12, 6,
		54, 1, 6, 1, 6, 1, 7, 1, 7, 1, 7, 1, 7, 5, 7, 63, 8, 7, 10, 7, 12, 7, 66,
		9, 7, 1, 8, 1, 8, 1, 8, 1, 8, 5, 8, 72, 8, 8, 10, 8, 12, 8, 75, 9, 8, 1,
		9, 1, 9, 1, 10, 1, 10, 1, 11, 4, 11, 82, 8, 11, 11, 11, 12, 11, 83, 1,
		11, 1, 11, 0, 0, 12, 1, 1, 3, 2, 5, 3, 7, 4, 9, 5, 11, 6, 13, 7, 15, 8,
		17, 9, 19, 0, 21, 0, 23, 10, 1, 0, 4, 1, 0, 34, 34, 1, 0, 48, 57, 2, 0,
		65, 90, 97, 122, 10, 0, 9, 13, 32, 32, 133, 133, 160, 160, 5760, 5760,
		8192, 8202, 8232, 8233, 8239, 8239, 8287, 8287, 12288, 12288, 94, 0, 1,
		1, 0, 0, 0, 0, 3, 1, 0, 0, 0, 0, 5, 1, 0, 0, 0, 0, 7, 1, 0, 0, 0, 0, 9,
		1, 0, 0, 0, 0, 11, 1, 0, 0, 0, 0, 13, 1, 0, 0, 0, 0, 15, 1, 0, 0, 0, 0,
		17, 1, 0, 0, 0, 0, 23, 1, 0, 0, 0, 1, 25, 1, 0, 0, 0, 3, 32, 1, 0, 0, 0,
		5, 34, 1, 0, 0, 0, 7, 36, 1, 0, 0, 0, 9, 38, 1, 0, 0, 0, 11, 40, 1, 0,
		0, 0, 13, 46, 1, 0, 0, 0, 15, 58, 1, 0, 0, 0, 17, 67, 1, 0, 0, 0, 19, 76,
		1, 0, 0, 0, 21, 78, 1, 0, 0, 0, 23, 81, 1, 0, 0, 0, 25, 26, 5, 116, 0,
		0, 26, 27, 5, 111, 0, 0, 27, 28, 5, 107, 0, 0, 28, 29, 5, 101, 0, 0, 29,
		30, 5, 110, 0, 0, 30, 31, 5, 115, 0, 0, 31, 2, 1, 0, 0, 0, 32, 33, 5, 123,
		0, 0, 33, 4, 1, 0, 0, 0, 34, 35, 5, 125, 0, 0, 35, 6, 1, 0, 0, 0, 36, 37,
		5, 61, 0, 0, 37, 8, 1, 0, 0, 0, 38, 39, 5, 59, 0, 0, 39, 10, 1, 0, 0, 0,
		40, 41, 5, 114, 0, 0, 41, 42, 5, 117, 0, 0, 42, 43, 5, 108, 0, 0, 43, 44,
		5, 101, 0, 0, 44, 45, 5, 115, 0, 0, 45, 12, 1, 0, 0, 0, 46, 52, 7, 0, 0,
		0, 47, 53, 8, 0, 0, 0, 48, 49, 5, 92, 0, 0, 49, 53, 5, 34, 0, 0, 50, 51,
		5, 92, 0, 0, 51, 53, 5, 92, 0, 0, 52, 47, 1, 0, 0, 0, 52, 48, 1, 0, 0,
		0, 52, 50, 1, 0, 0, 0, 53, 54, 1, 0, 0, 0, 54, 52, 1, 0, 0, 0, 54, 55,
		1, 0, 0, 0, 55, 56, 1, 0, 0, 0, 56, 57, 7, 0, 0, 0, 57, 14, 1, 0, 0, 0,
		58, 64, 2, 97, 122, 0, 59, 63, 3, 21, 10, 0, 60, 63, 3, 19, 9, 0, 61, 63,
		5, 95, 0, 0, 62, 59, 1, 0, 0, 0, 62, 60, 1, 0, 0, 0, 62, 61, 1, 0, 0, 0,
		63, 66, 1, 0, 0, 0, 64, 62, 1, 0, 0, 0, 64, 65, 1, 0, 0, 0, 65, 16, 1,
		0, 0, 0, 66, 64, 1, 0, 0, 0, 67, 73, 2, 65, 90, 0, 68, 72, 3, 21, 10, 0,
		69, 72, 3, 19, 9, 0, 70, 72, 5, 95, 0, 0, 71, 68, 1, 0, 0, 0, 71, 69, 1,
		0, 0, 0, 71, 70, 1, 0, 0, 0, 72, 75, 1, 0, 0, 0, 73, 71, 1, 0, 0, 0, 73,
		74, 1, 0, 0, 0, 74, 18, 1, 0, 0, 0, 75, 73, 1, 0, 0, 0, 76, 77, 7, 1, 0,
		0, 77, 20, 1, 0, 0, 0, 78, 79, 7, 2, 0, 0, 79, 22, 1, 0, 0, 0, 80, 82,
		7, 3, 0, 0, 81, 80, 1, 0, 0, 0, 82, 83, 1, 0, 0, 0, 83, 81, 1, 0, 0, 0,
		83, 84, 1, 0, 0, 0, 84, 85, 1, 0, 0, 0, 85, 86, 6, 11, 0, 0, 86, 24, 1,
		0, 0, 0, 8, 0, 52, 54, 62, 64, 71, 73, 83, 1, 6, 0, 0,
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
	LL1GrammarLexerLiteral    = 7
	LL1GrammarLexerTokenIdent = 8
	LL1GrammarLexerRuleIdent  = 9
	LL1GrammarLexerWhitespace = 10
)
