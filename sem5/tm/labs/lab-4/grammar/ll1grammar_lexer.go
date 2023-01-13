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
		"", "", "", "", "", "", "", "Code", "Args", "Literal", "TokenIdent",
		"RuleIdent", "Whitespace",
	}
	staticData.ruleNames = []string{
		"T__0", "T__1", "T__2", "T__3", "T__4", "T__5", "Code", "CodeBlock",
		"Args", "ArgBlock", "Literal", "TokenIdent", "RuleIdent", "Digit", "Letter",
		"Whitespace",
	}
	staticData.predictionContextCache = antlr.NewPredictionContextCache()
	staticData.serializedATN = []int32{
		4, 0, 12, 143, 6, -1, 2, 0, 7, 0, 2, 1, 7, 1, 2, 2, 7, 2, 2, 3, 7, 3, 2,
		4, 7, 4, 2, 5, 7, 5, 2, 6, 7, 6, 2, 7, 7, 7, 2, 8, 7, 8, 2, 9, 7, 9, 2,
		10, 7, 10, 2, 11, 7, 11, 2, 12, 7, 12, 2, 13, 7, 13, 2, 14, 7, 14, 2, 15,
		7, 15, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 1, 2, 1, 2,
		1, 3, 1, 3, 1, 4, 1, 4, 1, 5, 1, 5, 1, 5, 1, 5, 1, 5, 1, 5, 1, 6, 1, 6,
		5, 6, 57, 8, 6, 10, 6, 12, 6, 60, 9, 6, 1, 6, 1, 6, 1, 7, 4, 7, 65, 8,
		7, 11, 7, 12, 7, 66, 1, 7, 1, 7, 5, 7, 71, 8, 7, 10, 7, 12, 7, 74, 9, 7,
		1, 7, 3, 7, 77, 8, 7, 1, 8, 1, 8, 5, 8, 81, 8, 8, 10, 8, 12, 8, 84, 9,
		8, 1, 8, 1, 8, 1, 9, 4, 9, 89, 8, 9, 11, 9, 12, 9, 90, 1, 9, 1, 9, 5, 9,
		95, 8, 9, 10, 9, 12, 9, 98, 9, 9, 1, 9, 3, 9, 101, 8, 9, 1, 10, 1, 10,
		1, 10, 1, 10, 1, 10, 1, 10, 4, 10, 109, 8, 10, 11, 10, 12, 10, 110, 1,
		10, 1, 10, 1, 11, 1, 11, 1, 11, 1, 11, 5, 11, 119, 8, 11, 10, 11, 12, 11,
		122, 9, 11, 1, 12, 1, 12, 1, 12, 1, 12, 5, 12, 128, 8, 12, 10, 12, 12,
		12, 131, 9, 12, 1, 13, 1, 13, 1, 14, 1, 14, 1, 15, 4, 15, 138, 8, 15, 11,
		15, 12, 15, 139, 1, 15, 1, 15, 0, 0, 16, 1, 1, 3, 2, 5, 3, 7, 4, 9, 5,
		11, 6, 13, 7, 15, 0, 17, 8, 19, 0, 21, 9, 23, 10, 25, 11, 27, 0, 29, 0,
		31, 12, 1, 0, 6, 2, 0, 91, 91, 93, 93, 2, 0, 60, 60, 62, 62, 1, 0, 34,
		34, 1, 0, 48, 57, 2, 0, 65, 90, 97, 122, 10, 0, 9, 13, 32, 32, 133, 133,
		160, 160, 5760, 5760, 8192, 8202, 8232, 8233, 8239, 8239, 8287, 8287, 12288,
		12288, 156, 0, 1, 1, 0, 0, 0, 0, 3, 1, 0, 0, 0, 0, 5, 1, 0, 0, 0, 0, 7,
		1, 0, 0, 0, 0, 9, 1, 0, 0, 0, 0, 11, 1, 0, 0, 0, 0, 13, 1, 0, 0, 0, 0,
		17, 1, 0, 0, 0, 0, 21, 1, 0, 0, 0, 0, 23, 1, 0, 0, 0, 0, 25, 1, 0, 0, 0,
		0, 31, 1, 0, 0, 0, 1, 33, 1, 0, 0, 0, 3, 40, 1, 0, 0, 0, 5, 42, 1, 0, 0,
		0, 7, 44, 1, 0, 0, 0, 9, 46, 1, 0, 0, 0, 11, 48, 1, 0, 0, 0, 13, 54, 1,
		0, 0, 0, 15, 76, 1, 0, 0, 0, 17, 78, 1, 0, 0, 0, 19, 100, 1, 0, 0, 0, 21,
		102, 1, 0, 0, 0, 23, 114, 1, 0, 0, 0, 25, 123, 1, 0, 0, 0, 27, 132, 1,
		0, 0, 0, 29, 134, 1, 0, 0, 0, 31, 137, 1, 0, 0, 0, 33, 34, 5, 116, 0, 0,
		34, 35, 5, 111, 0, 0, 35, 36, 5, 107, 0, 0, 36, 37, 5, 101, 0, 0, 37, 38,
		5, 110, 0, 0, 38, 39, 5, 115, 0, 0, 39, 2, 1, 0, 0, 0, 40, 41, 5, 123,
		0, 0, 41, 4, 1, 0, 0, 0, 42, 43, 5, 125, 0, 0, 43, 6, 1, 0, 0, 0, 44, 45,
		5, 61, 0, 0, 45, 8, 1, 0, 0, 0, 46, 47, 5, 59, 0, 0, 47, 10, 1, 0, 0, 0,
		48, 49, 5, 114, 0, 0, 49, 50, 5, 117, 0, 0, 50, 51, 5, 108, 0, 0, 51, 52,
		5, 101, 0, 0, 52, 53, 5, 115, 0, 0, 53, 12, 1, 0, 0, 0, 54, 58, 5, 91,
		0, 0, 55, 57, 3, 15, 7, 0, 56, 55, 1, 0, 0, 0, 57, 60, 1, 0, 0, 0, 58,
		56, 1, 0, 0, 0, 58, 59, 1, 0, 0, 0, 59, 61, 1, 0, 0, 0, 60, 58, 1, 0, 0,
		0, 61, 62, 5, 93, 0, 0, 62, 14, 1, 0, 0, 0, 63, 65, 8, 0, 0, 0, 64, 63,
		1, 0, 0, 0, 65, 66, 1, 0, 0, 0, 66, 64, 1, 0, 0, 0, 66, 67, 1, 0, 0, 0,
		67, 77, 1, 0, 0, 0, 68, 72, 5, 91, 0, 0, 69, 71, 3, 15, 7, 0, 70, 69, 1,
		0, 0, 0, 71, 74, 1, 0, 0, 0, 72, 70, 1, 0, 0, 0, 72, 73, 1, 0, 0, 0, 73,
		75, 1, 0, 0, 0, 74, 72, 1, 0, 0, 0, 75, 77, 5, 93, 0, 0, 76, 64, 1, 0,
		0, 0, 76, 68, 1, 0, 0, 0, 77, 16, 1, 0, 0, 0, 78, 82, 5, 60, 0, 0, 79,
		81, 3, 19, 9, 0, 80, 79, 1, 0, 0, 0, 81, 84, 1, 0, 0, 0, 82, 80, 1, 0,
		0, 0, 82, 83, 1, 0, 0, 0, 83, 85, 1, 0, 0, 0, 84, 82, 1, 0, 0, 0, 85, 86,
		5, 62, 0, 0, 86, 18, 1, 0, 0, 0, 87, 89, 8, 1, 0, 0, 88, 87, 1, 0, 0, 0,
		89, 90, 1, 0, 0, 0, 90, 88, 1, 0, 0, 0, 90, 91, 1, 0, 0, 0, 91, 101, 1,
		0, 0, 0, 92, 96, 5, 60, 0, 0, 93, 95, 3, 19, 9, 0, 94, 93, 1, 0, 0, 0,
		95, 98, 1, 0, 0, 0, 96, 94, 1, 0, 0, 0, 96, 97, 1, 0, 0, 0, 97, 99, 1,
		0, 0, 0, 98, 96, 1, 0, 0, 0, 99, 101, 5, 62, 0, 0, 100, 88, 1, 0, 0, 0,
		100, 92, 1, 0, 0, 0, 101, 20, 1, 0, 0, 0, 102, 108, 7, 2, 0, 0, 103, 109,
		8, 2, 0, 0, 104, 105, 5, 92, 0, 0, 105, 109, 5, 34, 0, 0, 106, 107, 5,
		92, 0, 0, 107, 109, 5, 92, 0, 0, 108, 103, 1, 0, 0, 0, 108, 104, 1, 0,
		0, 0, 108, 106, 1, 0, 0, 0, 109, 110, 1, 0, 0, 0, 110, 108, 1, 0, 0, 0,
		110, 111, 1, 0, 0, 0, 111, 112, 1, 0, 0, 0, 112, 113, 7, 2, 0, 0, 113,
		22, 1, 0, 0, 0, 114, 120, 2, 97, 122, 0, 115, 119, 3, 29, 14, 0, 116, 119,
		3, 27, 13, 0, 117, 119, 5, 95, 0, 0, 118, 115, 1, 0, 0, 0, 118, 116, 1,
		0, 0, 0, 118, 117, 1, 0, 0, 0, 119, 122, 1, 0, 0, 0, 120, 118, 1, 0, 0,
		0, 120, 121, 1, 0, 0, 0, 121, 24, 1, 0, 0, 0, 122, 120, 1, 0, 0, 0, 123,
		129, 2, 65, 90, 0, 124, 128, 3, 29, 14, 0, 125, 128, 3, 27, 13, 0, 126,
		128, 5, 95, 0, 0, 127, 124, 1, 0, 0, 0, 127, 125, 1, 0, 0, 0, 127, 126,
		1, 0, 0, 0, 128, 131, 1, 0, 0, 0, 129, 127, 1, 0, 0, 0, 129, 130, 1, 0,
		0, 0, 130, 26, 1, 0, 0, 0, 131, 129, 1, 0, 0, 0, 132, 133, 7, 3, 0, 0,
		133, 28, 1, 0, 0, 0, 134, 135, 7, 4, 0, 0, 135, 30, 1, 0, 0, 0, 136, 138,
		7, 5, 0, 0, 137, 136, 1, 0, 0, 0, 138, 139, 1, 0, 0, 0, 139, 137, 1, 0,
		0, 0, 139, 140, 1, 0, 0, 0, 140, 141, 1, 0, 0, 0, 141, 142, 6, 15, 0, 0,
		142, 32, 1, 0, 0, 0, 16, 0, 58, 66, 72, 76, 82, 90, 96, 100, 108, 110,
		118, 120, 127, 129, 139, 1, 6, 0, 0,
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
	LL1GrammarLexerArgs       = 8
	LL1GrammarLexerLiteral    = 9
	LL1GrammarLexerTokenIdent = 10
	LL1GrammarLexerRuleIdent  = 11
	LL1GrammarLexerWhitespace = 12
)
