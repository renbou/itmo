// Code generated from java-escape by ANTLR 4.11.1. DO NOT EDIT.

package grammar // LL1Grammar
import (
	"fmt"
	"strconv"
	"sync"

	"github.com/antlr/antlr4/runtime/Go/antlr/v4"
)

// Suppress unused import errors
var _ = fmt.Printf
var _ = strconv.Itoa
var _ = sync.Once{}

type LL1GrammarParser struct {
	*antlr.BaseParser
}

var ll1grammarParserStaticData struct {
	once                   sync.Once
	serializedATN          []int32
	literalNames           []string
	symbolicNames          []string
	ruleNames              []string
	predictionContextCache *antlr.PredictionContextCache
	atn                    *antlr.ATN
	decisionToDFA          []*antlr.DFA
}

func ll1grammarParserInit() {
	staticData := &ll1grammarParserStaticData
	staticData.literalNames = []string{
		"", "'tokens'", "'{'", "'}'", "'='", "';'", "'rules'",
	}
	staticData.symbolicNames = []string{
		"", "", "", "", "", "", "", "Literal", "TokenIdent", "RuleIdent", "Whitespace",
	}
	staticData.ruleNames = []string{
		"ll1Grammar", "ll1Tokens", "optionalTokensNamed", "ll1Rules", "allRules",
		"singleRule", "ruleComponentList", "ruleComponent",
	}
	staticData.predictionContextCache = antlr.NewPredictionContextCache()
	staticData.serializedATN = []int32{
		4, 1, 10, 78, 2, 0, 7, 0, 2, 1, 7, 1, 2, 2, 7, 2, 2, 3, 7, 3, 2, 4, 7,
		4, 2, 5, 7, 5, 2, 6, 7, 6, 2, 7, 7, 7, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1,
		2, 3, 2, 35, 8, 2, 1, 3, 1, 3, 1, 3, 1, 3, 1, 3, 1, 3, 1, 4, 1, 4, 1, 4,
		1, 4, 1, 4, 1, 4, 1, 4, 1, 4, 1, 4, 1, 4, 5, 4, 53, 8, 4, 10, 4, 12, 4,
		56, 9, 4, 1, 5, 1, 5, 1, 5, 1, 5, 1, 5, 1, 6, 1, 6, 1, 6, 1, 6, 1, 6, 3,
		6, 68, 8, 6, 1, 7, 1, 7, 1, 7, 1, 7, 1, 7, 1, 7, 3, 7, 76, 8, 7, 1, 7,
		0, 1, 8, 8, 0, 2, 4, 6, 8, 10, 12, 14, 0, 0, 74, 0, 16, 1, 0, 0, 0, 2,
		20, 1, 0, 0, 0, 4, 34, 1, 0, 0, 0, 6, 36, 1, 0, 0, 0, 8, 42, 1, 0, 0, 0,
		10, 57, 1, 0, 0, 0, 12, 67, 1, 0, 0, 0, 14, 75, 1, 0, 0, 0, 16, 17, 3,
		2, 1, 0, 17, 18, 3, 6, 3, 0, 18, 19, 6, 0, -1, 0, 19, 1, 1, 0, 0, 0, 20,
		21, 5, 1, 0, 0, 21, 22, 5, 2, 0, 0, 22, 23, 3, 4, 2, 0, 23, 24, 5, 3, 0,
		0, 24, 25, 6, 1, -1, 0, 25, 3, 1, 0, 0, 0, 26, 27, 5, 8, 0, 0, 27, 28,
		5, 4, 0, 0, 28, 29, 5, 7, 0, 0, 29, 30, 5, 5, 0, 0, 30, 31, 3, 4, 2, 0,
		31, 32, 6, 2, -1, 0, 32, 35, 1, 0, 0, 0, 33, 35, 6, 2, -1, 0, 34, 26, 1,
		0, 0, 0, 34, 33, 1, 0, 0, 0, 35, 5, 1, 0, 0, 0, 36, 37, 5, 6, 0, 0, 37,
		38, 5, 2, 0, 0, 38, 39, 3, 8, 4, 0, 39, 40, 5, 3, 0, 0, 40, 41, 6, 3, -1,
		0, 41, 7, 1, 0, 0, 0, 42, 43, 6, 4, -1, 0, 43, 44, 3, 10, 5, 0, 44, 45,
		5, 5, 0, 0, 45, 46, 6, 4, -1, 0, 46, 54, 1, 0, 0, 0, 47, 48, 10, 2, 0,
		0, 48, 49, 3, 10, 5, 0, 49, 50, 5, 5, 0, 0, 50, 51, 6, 4, -1, 0, 51, 53,
		1, 0, 0, 0, 52, 47, 1, 0, 0, 0, 53, 56, 1, 0, 0, 0, 54, 52, 1, 0, 0, 0,
		54, 55, 1, 0, 0, 0, 55, 9, 1, 0, 0, 0, 56, 54, 1, 0, 0, 0, 57, 58, 5, 9,
		0, 0, 58, 59, 5, 4, 0, 0, 59, 60, 3, 12, 6, 0, 60, 61, 6, 5, -1, 0, 61,
		11, 1, 0, 0, 0, 62, 63, 3, 14, 7, 0, 63, 64, 3, 12, 6, 0, 64, 65, 6, 6,
		-1, 0, 65, 68, 1, 0, 0, 0, 66, 68, 6, 6, -1, 0, 67, 62, 1, 0, 0, 0, 67,
		66, 1, 0, 0, 0, 68, 13, 1, 0, 0, 0, 69, 70, 5, 7, 0, 0, 70, 76, 6, 7, -1,
		0, 71, 72, 5, 8, 0, 0, 72, 76, 6, 7, -1, 0, 73, 74, 5, 9, 0, 0, 74, 76,
		6, 7, -1, 0, 75, 69, 1, 0, 0, 0, 75, 71, 1, 0, 0, 0, 75, 73, 1, 0, 0, 0,
		76, 15, 1, 0, 0, 0, 4, 34, 54, 67, 75,
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

// LL1GrammarParserInit initializes any static state used to implement LL1GrammarParser. By default the
// static state used to implement the parser is lazily initialized during the first call to
// NewLL1GrammarParser(). You can call this function if you wish to initialize the static state ahead
// of time.
func LL1GrammarParserInit() {
	staticData := &ll1grammarParserStaticData
	staticData.once.Do(ll1grammarParserInit)
}

// NewLL1GrammarParser produces a new parser instance for the optional input antlr.TokenStream.
func NewLL1GrammarParser(input antlr.TokenStream) *LL1GrammarParser {
	LL1GrammarParserInit()
	this := new(LL1GrammarParser)
	this.BaseParser = antlr.NewBaseParser(input)
	staticData := &ll1grammarParserStaticData
	this.Interpreter = antlr.NewParserATNSimulator(this, staticData.atn, staticData.decisionToDFA, staticData.predictionContextCache)
	this.RuleNames = staticData.ruleNames
	this.LiteralNames = staticData.literalNames
	this.SymbolicNames = staticData.symbolicNames
	this.GrammarFileName = "java-escape"

	return this
}

// LL1GrammarParser tokens.
const (
	LL1GrammarParserEOF        = antlr.TokenEOF
	LL1GrammarParserT__0       = 1
	LL1GrammarParserT__1       = 2
	LL1GrammarParserT__2       = 3
	LL1GrammarParserT__3       = 4
	LL1GrammarParserT__4       = 5
	LL1GrammarParserT__5       = 6
	LL1GrammarParserLiteral    = 7
	LL1GrammarParserTokenIdent = 8
	LL1GrammarParserRuleIdent  = 9
	LL1GrammarParserWhitespace = 10
)

// LL1GrammarParser rules.
const (
	LL1GrammarParserRULE_ll1Grammar          = 0
	LL1GrammarParserRULE_ll1Tokens           = 1
	LL1GrammarParserRULE_optionalTokensNamed = 2
	LL1GrammarParserRULE_ll1Rules            = 3
	LL1GrammarParserRULE_allRules            = 4
	LL1GrammarParserRULE_singleRule          = 5
	LL1GrammarParserRULE_ruleComponentList   = 6
	LL1GrammarParserRULE_ruleComponent       = 7
)

// ILl1GrammarContext is an interface to support dynamic dispatch.
type ILl1GrammarContext interface {
	antlr.ParserRuleContext

	// GetParser returns the parser.
	GetParser() antlr.Parser

	// GetT returns the t rule contexts.
	GetT() ILl1TokensContext

	// GetR returns the r rule contexts.
	GetR() ILl1RulesContext

	// SetT sets the t rule contexts.
	SetT(ILl1TokensContext)

	// SetR sets the r rule contexts.
	SetR(ILl1RulesContext)

	// GetGrammar returns the grammar attribute.
	GetGrammar() Grammar

	// SetGrammar sets the grammar attribute.
	SetGrammar(Grammar)

	// IsLl1GrammarContext differentiates from other interfaces.
	IsLl1GrammarContext()
}

type Ll1GrammarContext struct {
	*antlr.BaseParserRuleContext
	parser  antlr.Parser
	grammar Grammar
	t       ILl1TokensContext
	r       ILl1RulesContext
}

func NewEmptyLl1GrammarContext() *Ll1GrammarContext {
	var p = new(Ll1GrammarContext)
	p.BaseParserRuleContext = antlr.NewBaseParserRuleContext(nil, -1)
	p.RuleIndex = LL1GrammarParserRULE_ll1Grammar
	return p
}

func (*Ll1GrammarContext) IsLl1GrammarContext() {}

func NewLl1GrammarContext(parser antlr.Parser, parent antlr.ParserRuleContext, invokingState int) *Ll1GrammarContext {
	var p = new(Ll1GrammarContext)

	p.BaseParserRuleContext = antlr.NewBaseParserRuleContext(parent, invokingState)

	p.parser = parser
	p.RuleIndex = LL1GrammarParserRULE_ll1Grammar

	return p
}

func (s *Ll1GrammarContext) GetParser() antlr.Parser { return s.parser }

func (s *Ll1GrammarContext) GetT() ILl1TokensContext { return s.t }

func (s *Ll1GrammarContext) GetR() ILl1RulesContext { return s.r }

func (s *Ll1GrammarContext) SetT(v ILl1TokensContext) { s.t = v }

func (s *Ll1GrammarContext) SetR(v ILl1RulesContext) { s.r = v }

func (s *Ll1GrammarContext) GetGrammar() Grammar { return s.grammar }

func (s *Ll1GrammarContext) SetGrammar(v Grammar) { s.grammar = v }

func (s *Ll1GrammarContext) Ll1Tokens() ILl1TokensContext {
	var t antlr.RuleContext
	for _, ctx := range s.GetChildren() {
		if _, ok := ctx.(ILl1TokensContext); ok {
			t = ctx.(antlr.RuleContext)
			break
		}
	}

	if t == nil {
		return nil
	}

	return t.(ILl1TokensContext)
}

func (s *Ll1GrammarContext) Ll1Rules() ILl1RulesContext {
	var t antlr.RuleContext
	for _, ctx := range s.GetChildren() {
		if _, ok := ctx.(ILl1RulesContext); ok {
			t = ctx.(antlr.RuleContext)
			break
		}
	}

	if t == nil {
		return nil
	}

	return t.(ILl1RulesContext)
}

func (s *Ll1GrammarContext) GetRuleContext() antlr.RuleContext {
	return s
}

func (s *Ll1GrammarContext) ToStringTree(ruleNames []string, recog antlr.Recognizer) string {
	return antlr.TreesStringTree(s, ruleNames, recog)
}

func (p *LL1GrammarParser) Ll1Grammar() (localctx ILl1GrammarContext) {
	this := p
	_ = this

	localctx = NewLl1GrammarContext(p, p.GetParserRuleContext(), p.GetState())
	p.EnterRule(localctx, 0, LL1GrammarParserRULE_ll1Grammar)

	defer func() {
		p.ExitRule()
	}()

	defer func() {
		if err := recover(); err != nil {
			if v, ok := err.(antlr.RecognitionException); ok {
				localctx.SetException(v)
				p.GetErrorHandler().ReportError(p, v)
				p.GetErrorHandler().Recover(p, v)
			} else {
				panic(err)
			}
		}
	}()

	p.EnterOuterAlt(localctx, 1)
	{
		p.SetState(16)

		var _x = p.Ll1Tokens()

		localctx.(*Ll1GrammarContext).t = _x
	}
	{
		p.SetState(17)

		var _x = p.Ll1Rules()

		localctx.(*Ll1GrammarContext).r = _x
	}
	localctx.(*Ll1GrammarContext).SetGrammar(Grammar{LexTokens: localctx.(*Ll1GrammarContext).GetT().GetTokens(), StartNonTerminal: localctx.(*Ll1GrammarContext).GetR().GetStartNonTerm(), ParseRules: localctx.(*Ll1GrammarContext).GetR().GetRules()})

	return localctx
}

// ILl1TokensContext is an interface to support dynamic dispatch.
type ILl1TokensContext interface {
	antlr.ParserRuleContext

	// GetParser returns the parser.
	GetParser() antlr.Parser

	// GetR returns the r rule contexts.
	GetR() IOptionalTokensNamedContext

	// SetR sets the r rule contexts.
	SetR(IOptionalTokensNamedContext)

	// GetTokens returns the tokens attribute.
	GetTokens() LexTokens

	// SetTokens sets the tokens attribute.
	SetTokens(LexTokens)

	// IsLl1TokensContext differentiates from other interfaces.
	IsLl1TokensContext()
}

type Ll1TokensContext struct {
	*antlr.BaseParserRuleContext
	parser antlr.Parser
	tokens LexTokens
	r      IOptionalTokensNamedContext
}

func NewEmptyLl1TokensContext() *Ll1TokensContext {
	var p = new(Ll1TokensContext)
	p.BaseParserRuleContext = antlr.NewBaseParserRuleContext(nil, -1)
	p.RuleIndex = LL1GrammarParserRULE_ll1Tokens
	return p
}

func (*Ll1TokensContext) IsLl1TokensContext() {}

func NewLl1TokensContext(parser antlr.Parser, parent antlr.ParserRuleContext, invokingState int) *Ll1TokensContext {
	var p = new(Ll1TokensContext)

	p.BaseParserRuleContext = antlr.NewBaseParserRuleContext(parent, invokingState)

	p.parser = parser
	p.RuleIndex = LL1GrammarParserRULE_ll1Tokens

	return p
}

func (s *Ll1TokensContext) GetParser() antlr.Parser { return s.parser }

func (s *Ll1TokensContext) GetR() IOptionalTokensNamedContext { return s.r }

func (s *Ll1TokensContext) SetR(v IOptionalTokensNamedContext) { s.r = v }

func (s *Ll1TokensContext) GetTokens() LexTokens { return s.tokens }

func (s *Ll1TokensContext) SetTokens(v LexTokens) { s.tokens = v }

func (s *Ll1TokensContext) OptionalTokensNamed() IOptionalTokensNamedContext {
	var t antlr.RuleContext
	for _, ctx := range s.GetChildren() {
		if _, ok := ctx.(IOptionalTokensNamedContext); ok {
			t = ctx.(antlr.RuleContext)
			break
		}
	}

	if t == nil {
		return nil
	}

	return t.(IOptionalTokensNamedContext)
}

func (s *Ll1TokensContext) GetRuleContext() antlr.RuleContext {
	return s
}

func (s *Ll1TokensContext) ToStringTree(ruleNames []string, recog antlr.Recognizer) string {
	return antlr.TreesStringTree(s, ruleNames, recog)
}

func (p *LL1GrammarParser) Ll1Tokens() (localctx ILl1TokensContext) {
	this := p
	_ = this

	localctx = NewLl1TokensContext(p, p.GetParserRuleContext(), p.GetState())
	p.EnterRule(localctx, 2, LL1GrammarParserRULE_ll1Tokens)

	defer func() {
		p.ExitRule()
	}()

	defer func() {
		if err := recover(); err != nil {
			if v, ok := err.(antlr.RecognitionException); ok {
				localctx.SetException(v)
				p.GetErrorHandler().ReportError(p, v)
				p.GetErrorHandler().Recover(p, v)
			} else {
				panic(err)
			}
		}
	}()

	p.EnterOuterAlt(localctx, 1)
	{
		p.SetState(20)
		p.Match(LL1GrammarParserT__0)
	}
	{
		p.SetState(21)
		p.Match(LL1GrammarParserT__1)
	}
	{
		p.SetState(22)

		var _x = p.OptionalTokensNamed()

		localctx.(*Ll1TokensContext).r = _x
	}
	{
		p.SetState(23)
		p.Match(LL1GrammarParserT__2)
	}
	localctx.(*Ll1TokensContext).SetTokens(LexTokens{named: localctx.(*Ll1TokensContext).GetR().GetNamed()})

	return localctx
}

// IOptionalTokensNamedContext is an interface to support dynamic dispatch.
type IOptionalTokensNamedContext interface {
	antlr.ParserRuleContext

	// GetParser returns the parser.
	GetParser() antlr.Parser

	// GetN returns the n token.
	GetN() antlr.Token

	// GetL returns the l token.
	GetL() antlr.Token

	// SetN sets the n token.
	SetN(antlr.Token)

	// SetL sets the l token.
	SetL(antlr.Token)

	// GetR returns the r rule contexts.
	GetR() IOptionalTokensNamedContext

	// SetR sets the r rule contexts.
	SetR(IOptionalTokensNamedContext)

	// GetNamed returns the named attribute.
	GetNamed() []namedToken

	// SetNamed sets the named attribute.
	SetNamed([]namedToken)

	// IsOptionalTokensNamedContext differentiates from other interfaces.
	IsOptionalTokensNamedContext()
}

type OptionalTokensNamedContext struct {
	*antlr.BaseParserRuleContext
	parser antlr.Parser
	named  []namedToken
	n      antlr.Token
	l      antlr.Token
	r      IOptionalTokensNamedContext
}

func NewEmptyOptionalTokensNamedContext() *OptionalTokensNamedContext {
	var p = new(OptionalTokensNamedContext)
	p.BaseParserRuleContext = antlr.NewBaseParserRuleContext(nil, -1)
	p.RuleIndex = LL1GrammarParserRULE_optionalTokensNamed
	return p
}

func (*OptionalTokensNamedContext) IsOptionalTokensNamedContext() {}

func NewOptionalTokensNamedContext(parser antlr.Parser, parent antlr.ParserRuleContext, invokingState int) *OptionalTokensNamedContext {
	var p = new(OptionalTokensNamedContext)

	p.BaseParserRuleContext = antlr.NewBaseParserRuleContext(parent, invokingState)

	p.parser = parser
	p.RuleIndex = LL1GrammarParserRULE_optionalTokensNamed

	return p
}

func (s *OptionalTokensNamedContext) GetParser() antlr.Parser { return s.parser }

func (s *OptionalTokensNamedContext) GetN() antlr.Token { return s.n }

func (s *OptionalTokensNamedContext) GetL() antlr.Token { return s.l }

func (s *OptionalTokensNamedContext) SetN(v antlr.Token) { s.n = v }

func (s *OptionalTokensNamedContext) SetL(v antlr.Token) { s.l = v }

func (s *OptionalTokensNamedContext) GetR() IOptionalTokensNamedContext { return s.r }

func (s *OptionalTokensNamedContext) SetR(v IOptionalTokensNamedContext) { s.r = v }

func (s *OptionalTokensNamedContext) GetNamed() []namedToken { return s.named }

func (s *OptionalTokensNamedContext) SetNamed(v []namedToken) { s.named = v }

func (s *OptionalTokensNamedContext) TokenIdent() antlr.TerminalNode {
	return s.GetToken(LL1GrammarParserTokenIdent, 0)
}

func (s *OptionalTokensNamedContext) Literal() antlr.TerminalNode {
	return s.GetToken(LL1GrammarParserLiteral, 0)
}

func (s *OptionalTokensNamedContext) OptionalTokensNamed() IOptionalTokensNamedContext {
	var t antlr.RuleContext
	for _, ctx := range s.GetChildren() {
		if _, ok := ctx.(IOptionalTokensNamedContext); ok {
			t = ctx.(antlr.RuleContext)
			break
		}
	}

	if t == nil {
		return nil
	}

	return t.(IOptionalTokensNamedContext)
}

func (s *OptionalTokensNamedContext) GetRuleContext() antlr.RuleContext {
	return s
}

func (s *OptionalTokensNamedContext) ToStringTree(ruleNames []string, recog antlr.Recognizer) string {
	return antlr.TreesStringTree(s, ruleNames, recog)
}

func (p *LL1GrammarParser) OptionalTokensNamed() (localctx IOptionalTokensNamedContext) {
	this := p
	_ = this

	localctx = NewOptionalTokensNamedContext(p, p.GetParserRuleContext(), p.GetState())
	p.EnterRule(localctx, 4, LL1GrammarParserRULE_optionalTokensNamed)

	defer func() {
		p.ExitRule()
	}()

	defer func() {
		if err := recover(); err != nil {
			if v, ok := err.(antlr.RecognitionException); ok {
				localctx.SetException(v)
				p.GetErrorHandler().ReportError(p, v)
				p.GetErrorHandler().Recover(p, v)
			} else {
				panic(err)
			}
		}
	}()

	p.SetState(34)
	p.GetErrorHandler().Sync(p)

	switch p.GetTokenStream().LA(1) {
	case LL1GrammarParserTokenIdent:
		p.EnterOuterAlt(localctx, 1)
		{
			p.SetState(26)

			var _m = p.Match(LL1GrammarParserTokenIdent)

			localctx.(*OptionalTokensNamedContext).n = _m
		}
		{
			p.SetState(27)
			p.Match(LL1GrammarParserT__3)
		}
		{
			p.SetState(28)

			var _m = p.Match(LL1GrammarParserLiteral)

			localctx.(*OptionalTokensNamedContext).l = _m
		}
		{
			p.SetState(29)
			p.Match(LL1GrammarParserT__4)
		}
		{
			p.SetState(30)

			var _x = p.OptionalTokensNamed()

			localctx.(*OptionalTokensNamedContext).r = _x
		}
		localctx.(*OptionalTokensNamedContext).SetNamed(append(localctx.(*OptionalTokensNamedContext).GetR().GetNamed(), namedToken{name: (func() string {
			if localctx.(*OptionalTokensNamedContext).GetN() == nil {
				return ""
			} else {
				return localctx.(*OptionalTokensNamedContext).GetN().GetText()
			}
		}()), regexp: literalValue(localctx.(*OptionalTokensNamedContext).GetL())}))

	case LL1GrammarParserT__2:
		p.EnterOuterAlt(localctx, 2)
		localctx.(*OptionalTokensNamedContext).SetNamed(nil)

	default:
		panic(antlr.NewNoViableAltException(p, nil, nil, nil, nil, nil))
	}

	return localctx
}

// ILl1RulesContext is an interface to support dynamic dispatch.
type ILl1RulesContext interface {
	antlr.ParserRuleContext

	// GetParser returns the parser.
	GetParser() antlr.Parser

	// GetR returns the r rule contexts.
	GetR() IAllRulesContext

	// SetR sets the r rule contexts.
	SetR(IAllRulesContext)

	// GetRules returns the rules attribute.
	GetRules() ParseRules

	// GetStartNonTerm returns the startNonTerm attribute.
	GetStartNonTerm() string

	// SetRules sets the rules attribute.
	SetRules(ParseRules)

	// SetStartNonTerm sets the startNonTerm attribute.
	SetStartNonTerm(string)

	// IsLl1RulesContext differentiates from other interfaces.
	IsLl1RulesContext()
}

type Ll1RulesContext struct {
	*antlr.BaseParserRuleContext
	parser       antlr.Parser
	rules        ParseRules
	startNonTerm string
	r            IAllRulesContext
}

func NewEmptyLl1RulesContext() *Ll1RulesContext {
	var p = new(Ll1RulesContext)
	p.BaseParserRuleContext = antlr.NewBaseParserRuleContext(nil, -1)
	p.RuleIndex = LL1GrammarParserRULE_ll1Rules
	return p
}

func (*Ll1RulesContext) IsLl1RulesContext() {}

func NewLl1RulesContext(parser antlr.Parser, parent antlr.ParserRuleContext, invokingState int) *Ll1RulesContext {
	var p = new(Ll1RulesContext)

	p.BaseParserRuleContext = antlr.NewBaseParserRuleContext(parent, invokingState)

	p.parser = parser
	p.RuleIndex = LL1GrammarParserRULE_ll1Rules

	return p
}

func (s *Ll1RulesContext) GetParser() antlr.Parser { return s.parser }

func (s *Ll1RulesContext) GetR() IAllRulesContext { return s.r }

func (s *Ll1RulesContext) SetR(v IAllRulesContext) { s.r = v }

func (s *Ll1RulesContext) GetRules() ParseRules { return s.rules }

func (s *Ll1RulesContext) GetStartNonTerm() string { return s.startNonTerm }

func (s *Ll1RulesContext) SetRules(v ParseRules) { s.rules = v }

func (s *Ll1RulesContext) SetStartNonTerm(v string) { s.startNonTerm = v }

func (s *Ll1RulesContext) AllRules() IAllRulesContext {
	var t antlr.RuleContext
	for _, ctx := range s.GetChildren() {
		if _, ok := ctx.(IAllRulesContext); ok {
			t = ctx.(antlr.RuleContext)
			break
		}
	}

	if t == nil {
		return nil
	}

	return t.(IAllRulesContext)
}

func (s *Ll1RulesContext) GetRuleContext() antlr.RuleContext {
	return s
}

func (s *Ll1RulesContext) ToStringTree(ruleNames []string, recog antlr.Recognizer) string {
	return antlr.TreesStringTree(s, ruleNames, recog)
}

func (p *LL1GrammarParser) Ll1Rules() (localctx ILl1RulesContext) {
	this := p
	_ = this

	localctx = NewLl1RulesContext(p, p.GetParserRuleContext(), p.GetState())
	p.EnterRule(localctx, 6, LL1GrammarParserRULE_ll1Rules)

	defer func() {
		p.ExitRule()
	}()

	defer func() {
		if err := recover(); err != nil {
			if v, ok := err.(antlr.RecognitionException); ok {
				localctx.SetException(v)
				p.GetErrorHandler().ReportError(p, v)
				p.GetErrorHandler().Recover(p, v)
			} else {
				panic(err)
			}
		}
	}()

	p.EnterOuterAlt(localctx, 1)
	{
		p.SetState(36)
		p.Match(LL1GrammarParserT__5)
	}
	{
		p.SetState(37)
		p.Match(LL1GrammarParserT__1)
	}
	{
		p.SetState(38)

		var _x = p.allRules(0)

		localctx.(*Ll1RulesContext).r = _x
	}
	{
		p.SetState(39)
		p.Match(LL1GrammarParserT__2)
	}

	localctx.(*Ll1RulesContext).SetRules(localctx.(*Ll1RulesContext).GetR().GetRules())
	localctx.(*Ll1RulesContext).SetStartNonTerm(localctx.(*Ll1RulesContext).GetR().GetStartNonTerm())

	return localctx
}

// IAllRulesContext is an interface to support dynamic dispatch.
type IAllRulesContext interface {
	antlr.ParserRuleContext

	// GetParser returns the parser.
	GetParser() antlr.Parser

	// GetRest returns the rest rule contexts.
	GetRest() IAllRulesContext

	// GetR returns the r rule contexts.
	GetR() ISingleRuleContext

	// SetRest sets the rest rule contexts.
	SetRest(IAllRulesContext)

	// SetR sets the r rule contexts.
	SetR(ISingleRuleContext)

	// GetRules returns the rules attribute.
	GetRules() ParseRules

	// GetStartNonTerm returns the startNonTerm attribute.
	GetStartNonTerm() string

	// SetRules sets the rules attribute.
	SetRules(ParseRules)

	// SetStartNonTerm sets the startNonTerm attribute.
	SetStartNonTerm(string)

	// IsAllRulesContext differentiates from other interfaces.
	IsAllRulesContext()
}

type AllRulesContext struct {
	*antlr.BaseParserRuleContext
	parser       antlr.Parser
	rules        ParseRules
	startNonTerm string
	rest         IAllRulesContext
	r            ISingleRuleContext
}

func NewEmptyAllRulesContext() *AllRulesContext {
	var p = new(AllRulesContext)
	p.BaseParserRuleContext = antlr.NewBaseParserRuleContext(nil, -1)
	p.RuleIndex = LL1GrammarParserRULE_allRules
	return p
}

func (*AllRulesContext) IsAllRulesContext() {}

func NewAllRulesContext(parser antlr.Parser, parent antlr.ParserRuleContext, invokingState int) *AllRulesContext {
	var p = new(AllRulesContext)

	p.BaseParserRuleContext = antlr.NewBaseParserRuleContext(parent, invokingState)

	p.parser = parser
	p.RuleIndex = LL1GrammarParserRULE_allRules

	return p
}

func (s *AllRulesContext) GetParser() antlr.Parser { return s.parser }

func (s *AllRulesContext) GetRest() IAllRulesContext { return s.rest }

func (s *AllRulesContext) GetR() ISingleRuleContext { return s.r }

func (s *AllRulesContext) SetRest(v IAllRulesContext) { s.rest = v }

func (s *AllRulesContext) SetR(v ISingleRuleContext) { s.r = v }

func (s *AllRulesContext) GetRules() ParseRules { return s.rules }

func (s *AllRulesContext) GetStartNonTerm() string { return s.startNonTerm }

func (s *AllRulesContext) SetRules(v ParseRules) { s.rules = v }

func (s *AllRulesContext) SetStartNonTerm(v string) { s.startNonTerm = v }

func (s *AllRulesContext) SingleRule() ISingleRuleContext {
	var t antlr.RuleContext
	for _, ctx := range s.GetChildren() {
		if _, ok := ctx.(ISingleRuleContext); ok {
			t = ctx.(antlr.RuleContext)
			break
		}
	}

	if t == nil {
		return nil
	}

	return t.(ISingleRuleContext)
}

func (s *AllRulesContext) AllRules() IAllRulesContext {
	var t antlr.RuleContext
	for _, ctx := range s.GetChildren() {
		if _, ok := ctx.(IAllRulesContext); ok {
			t = ctx.(antlr.RuleContext)
			break
		}
	}

	if t == nil {
		return nil
	}

	return t.(IAllRulesContext)
}

func (s *AllRulesContext) GetRuleContext() antlr.RuleContext {
	return s
}

func (s *AllRulesContext) ToStringTree(ruleNames []string, recog antlr.Recognizer) string {
	return antlr.TreesStringTree(s, ruleNames, recog)
}

func (p *LL1GrammarParser) AllRules() (localctx IAllRulesContext) {
	return p.allRules(0)
}

func (p *LL1GrammarParser) allRules(_p int) (localctx IAllRulesContext) {
	this := p
	_ = this

	var _parentctx antlr.ParserRuleContext = p.GetParserRuleContext()
	_parentState := p.GetState()
	localctx = NewAllRulesContext(p, p.GetParserRuleContext(), _parentState)
	var _prevctx IAllRulesContext = localctx
	var _ antlr.ParserRuleContext = _prevctx // TODO: To prevent unused variable warning.
	_startState := 8
	p.EnterRecursionRule(localctx, 8, LL1GrammarParserRULE_allRules, _p)

	defer func() {
		p.UnrollRecursionContexts(_parentctx)
	}()

	defer func() {
		if err := recover(); err != nil {
			if v, ok := err.(antlr.RecognitionException); ok {
				localctx.SetException(v)
				p.GetErrorHandler().ReportError(p, v)
				p.GetErrorHandler().Recover(p, v)
			} else {
				panic(err)
			}
		}
	}()

	var _alt int

	p.EnterOuterAlt(localctx, 1)
	{
		p.SetState(43)

		var _x = p.SingleRule()

		localctx.(*AllRulesContext).r = _x
	}
	{
		p.SetState(44)
		p.Match(LL1GrammarParserT__4)
	}

	localctx.(*AllRulesContext).SetStartNonTerm(localctx.(*AllRulesContext).GetR().GetName())
	localctx.(*AllRulesContext).SetRules(ParseRules{localctx.(*AllRulesContext).GetR().GetName(): []ParseRule{localctx.(*AllRulesContext).GetR().GetComponents()}})

	p.GetParserRuleContext().SetStop(p.GetTokenStream().LT(-1))
	p.SetState(54)
	p.GetErrorHandler().Sync(p)
	_alt = p.GetInterpreter().AdaptivePredict(p.GetTokenStream(), 1, p.GetParserRuleContext())

	for _alt != 2 && _alt != antlr.ATNInvalidAltNumber {
		if _alt == 1 {
			if p.GetParseListeners() != nil {
				p.TriggerExitRuleEvent()
			}
			_prevctx = localctx
			localctx = NewAllRulesContext(p, _parentctx, _parentState)
			localctx.(*AllRulesContext).rest = _prevctx
			p.PushNewRecursionContext(localctx, _startState, LL1GrammarParserRULE_allRules)
			p.SetState(47)

			if !(p.Precpred(p.GetParserRuleContext(), 2)) {
				panic(antlr.NewFailedPredicateException(p, "p.Precpred(p.GetParserRuleContext(), 2)", ""))
			}
			{
				p.SetState(48)

				var _x = p.SingleRule()

				localctx.(*AllRulesContext).r = _x
			}
			{
				p.SetState(49)
				p.Match(LL1GrammarParserT__4)
			}

			localctx.(*AllRulesContext).SetStartNonTerm(localctx.(*AllRulesContext).GetRest().GetStartNonTerm())
			localctx.(*AllRulesContext).SetRules(localctx.(*AllRulesContext).GetRest().GetRules())
			localctx.(*AllRulesContext).rules[localctx.(*AllRulesContext).GetR().GetName()] = append(localctx.(*AllRulesContext).rules[localctx.(*AllRulesContext).GetR().GetName()], localctx.(*AllRulesContext).GetR().GetComponents())

		}
		p.SetState(56)
		p.GetErrorHandler().Sync(p)
		_alt = p.GetInterpreter().AdaptivePredict(p.GetTokenStream(), 1, p.GetParserRuleContext())
	}

	return localctx
}

// ISingleRuleContext is an interface to support dynamic dispatch.
type ISingleRuleContext interface {
	antlr.ParserRuleContext

	// GetParser returns the parser.
	GetParser() antlr.Parser

	// GetN returns the n token.
	GetN() antlr.Token

	// SetN sets the n token.
	SetN(antlr.Token)

	// GetC returns the c rule contexts.
	GetC() IRuleComponentListContext

	// SetC sets the c rule contexts.
	SetC(IRuleComponentListContext)

	// GetName returns the name attribute.
	GetName() string

	// GetComponents returns the components attribute.
	GetComponents() []ParseRuleComponent

	// SetName sets the name attribute.
	SetName(string)

	// SetComponents sets the components attribute.
	SetComponents([]ParseRuleComponent)

	// IsSingleRuleContext differentiates from other interfaces.
	IsSingleRuleContext()
}

type SingleRuleContext struct {
	*antlr.BaseParserRuleContext
	parser     antlr.Parser
	name       string
	components []ParseRuleComponent
	n          antlr.Token
	c          IRuleComponentListContext
}

func NewEmptySingleRuleContext() *SingleRuleContext {
	var p = new(SingleRuleContext)
	p.BaseParserRuleContext = antlr.NewBaseParserRuleContext(nil, -1)
	p.RuleIndex = LL1GrammarParserRULE_singleRule
	return p
}

func (*SingleRuleContext) IsSingleRuleContext() {}

func NewSingleRuleContext(parser antlr.Parser, parent antlr.ParserRuleContext, invokingState int) *SingleRuleContext {
	var p = new(SingleRuleContext)

	p.BaseParserRuleContext = antlr.NewBaseParserRuleContext(parent, invokingState)

	p.parser = parser
	p.RuleIndex = LL1GrammarParserRULE_singleRule

	return p
}

func (s *SingleRuleContext) GetParser() antlr.Parser { return s.parser }

func (s *SingleRuleContext) GetN() antlr.Token { return s.n }

func (s *SingleRuleContext) SetN(v antlr.Token) { s.n = v }

func (s *SingleRuleContext) GetC() IRuleComponentListContext { return s.c }

func (s *SingleRuleContext) SetC(v IRuleComponentListContext) { s.c = v }

func (s *SingleRuleContext) GetName() string { return s.name }

func (s *SingleRuleContext) GetComponents() []ParseRuleComponent { return s.components }

func (s *SingleRuleContext) SetName(v string) { s.name = v }

func (s *SingleRuleContext) SetComponents(v []ParseRuleComponent) { s.components = v }

func (s *SingleRuleContext) RuleIdent() antlr.TerminalNode {
	return s.GetToken(LL1GrammarParserRuleIdent, 0)
}

func (s *SingleRuleContext) RuleComponentList() IRuleComponentListContext {
	var t antlr.RuleContext
	for _, ctx := range s.GetChildren() {
		if _, ok := ctx.(IRuleComponentListContext); ok {
			t = ctx.(antlr.RuleContext)
			break
		}
	}

	if t == nil {
		return nil
	}

	return t.(IRuleComponentListContext)
}

func (s *SingleRuleContext) GetRuleContext() antlr.RuleContext {
	return s
}

func (s *SingleRuleContext) ToStringTree(ruleNames []string, recog antlr.Recognizer) string {
	return antlr.TreesStringTree(s, ruleNames, recog)
}

func (p *LL1GrammarParser) SingleRule() (localctx ISingleRuleContext) {
	this := p
	_ = this

	localctx = NewSingleRuleContext(p, p.GetParserRuleContext(), p.GetState())
	p.EnterRule(localctx, 10, LL1GrammarParserRULE_singleRule)

	defer func() {
		p.ExitRule()
	}()

	defer func() {
		if err := recover(); err != nil {
			if v, ok := err.(antlr.RecognitionException); ok {
				localctx.SetException(v)
				p.GetErrorHandler().ReportError(p, v)
				p.GetErrorHandler().Recover(p, v)
			} else {
				panic(err)
			}
		}
	}()

	p.EnterOuterAlt(localctx, 1)
	{
		p.SetState(57)

		var _m = p.Match(LL1GrammarParserRuleIdent)

		localctx.(*SingleRuleContext).n = _m
	}
	{
		p.SetState(58)
		p.Match(LL1GrammarParserT__3)
	}
	{
		p.SetState(59)

		var _x = p.RuleComponentList()

		localctx.(*SingleRuleContext).c = _x
	}

	localctx.(*SingleRuleContext).SetName((func() string {
		if localctx.(*SingleRuleContext).GetN() == nil {
			return ""
		} else {
			return localctx.(*SingleRuleContext).GetN().GetText()
		}
	}()))
	localctx.(*SingleRuleContext).SetComponents(localctx.(*SingleRuleContext).GetC().GetComponents())

	return localctx
}

// IRuleComponentListContext is an interface to support dynamic dispatch.
type IRuleComponentListContext interface {
	antlr.ParserRuleContext

	// GetParser returns the parser.
	GetParser() antlr.Parser

	// GetC returns the c rule contexts.
	GetC() IRuleComponentContext

	// GetRest returns the rest rule contexts.
	GetRest() IRuleComponentListContext

	// SetC sets the c rule contexts.
	SetC(IRuleComponentContext)

	// SetRest sets the rest rule contexts.
	SetRest(IRuleComponentListContext)

	// GetComponents returns the components attribute.
	GetComponents() []ParseRuleComponent

	// SetComponents sets the components attribute.
	SetComponents([]ParseRuleComponent)

	// IsRuleComponentListContext differentiates from other interfaces.
	IsRuleComponentListContext()
}

type RuleComponentListContext struct {
	*antlr.BaseParserRuleContext
	parser     antlr.Parser
	components []ParseRuleComponent
	c          IRuleComponentContext
	rest       IRuleComponentListContext
}

func NewEmptyRuleComponentListContext() *RuleComponentListContext {
	var p = new(RuleComponentListContext)
	p.BaseParserRuleContext = antlr.NewBaseParserRuleContext(nil, -1)
	p.RuleIndex = LL1GrammarParserRULE_ruleComponentList
	return p
}

func (*RuleComponentListContext) IsRuleComponentListContext() {}

func NewRuleComponentListContext(parser antlr.Parser, parent antlr.ParserRuleContext, invokingState int) *RuleComponentListContext {
	var p = new(RuleComponentListContext)

	p.BaseParserRuleContext = antlr.NewBaseParserRuleContext(parent, invokingState)

	p.parser = parser
	p.RuleIndex = LL1GrammarParserRULE_ruleComponentList

	return p
}

func (s *RuleComponentListContext) GetParser() antlr.Parser { return s.parser }

func (s *RuleComponentListContext) GetC() IRuleComponentContext { return s.c }

func (s *RuleComponentListContext) GetRest() IRuleComponentListContext { return s.rest }

func (s *RuleComponentListContext) SetC(v IRuleComponentContext) { s.c = v }

func (s *RuleComponentListContext) SetRest(v IRuleComponentListContext) { s.rest = v }

func (s *RuleComponentListContext) GetComponents() []ParseRuleComponent { return s.components }

func (s *RuleComponentListContext) SetComponents(v []ParseRuleComponent) { s.components = v }

func (s *RuleComponentListContext) RuleComponent() IRuleComponentContext {
	var t antlr.RuleContext
	for _, ctx := range s.GetChildren() {
		if _, ok := ctx.(IRuleComponentContext); ok {
			t = ctx.(antlr.RuleContext)
			break
		}
	}

	if t == nil {
		return nil
	}

	return t.(IRuleComponentContext)
}

func (s *RuleComponentListContext) RuleComponentList() IRuleComponentListContext {
	var t antlr.RuleContext
	for _, ctx := range s.GetChildren() {
		if _, ok := ctx.(IRuleComponentListContext); ok {
			t = ctx.(antlr.RuleContext)
			break
		}
	}

	if t == nil {
		return nil
	}

	return t.(IRuleComponentListContext)
}

func (s *RuleComponentListContext) GetRuleContext() antlr.RuleContext {
	return s
}

func (s *RuleComponentListContext) ToStringTree(ruleNames []string, recog antlr.Recognizer) string {
	return antlr.TreesStringTree(s, ruleNames, recog)
}

func (p *LL1GrammarParser) RuleComponentList() (localctx IRuleComponentListContext) {
	this := p
	_ = this

	localctx = NewRuleComponentListContext(p, p.GetParserRuleContext(), p.GetState())
	p.EnterRule(localctx, 12, LL1GrammarParserRULE_ruleComponentList)

	defer func() {
		p.ExitRule()
	}()

	defer func() {
		if err := recover(); err != nil {
			if v, ok := err.(antlr.RecognitionException); ok {
				localctx.SetException(v)
				p.GetErrorHandler().ReportError(p, v)
				p.GetErrorHandler().Recover(p, v)
			} else {
				panic(err)
			}
		}
	}()

	p.SetState(67)
	p.GetErrorHandler().Sync(p)

	switch p.GetTokenStream().LA(1) {
	case LL1GrammarParserLiteral, LL1GrammarParserTokenIdent, LL1GrammarParserRuleIdent:
		p.EnterOuterAlt(localctx, 1)
		{
			p.SetState(62)

			var _x = p.RuleComponent()

			localctx.(*RuleComponentListContext).c = _x
		}
		{
			p.SetState(63)

			var _x = p.RuleComponentList()

			localctx.(*RuleComponentListContext).rest = _x
		}
		localctx.(*RuleComponentListContext).SetComponents(append([]ParseRuleComponent{localctx.(*RuleComponentListContext).GetC().GetComponent()}, localctx.(*RuleComponentListContext).GetRest().GetComponents()...))

	case LL1GrammarParserT__4:
		p.EnterOuterAlt(localctx, 2)
		localctx.(*RuleComponentListContext).SetComponents(nil)

	default:
		panic(antlr.NewNoViableAltException(p, nil, nil, nil, nil, nil))
	}

	return localctx
}

// IRuleComponentContext is an interface to support dynamic dispatch.
type IRuleComponentContext interface {
	antlr.ParserRuleContext

	// GetParser returns the parser.
	GetParser() antlr.Parser

	// GetL returns the l token.
	GetL() antlr.Token

	// GetT returns the t token.
	GetT() antlr.Token

	// GetR returns the r token.
	GetR() antlr.Token

	// SetL sets the l token.
	SetL(antlr.Token)

	// SetT sets the t token.
	SetT(antlr.Token)

	// SetR sets the r token.
	SetR(antlr.Token)

	// GetComponent returns the component attribute.
	GetComponent() ParseRuleComponent

	// SetComponent sets the component attribute.
	SetComponent(ParseRuleComponent)

	// IsRuleComponentContext differentiates from other interfaces.
	IsRuleComponentContext()
}

type RuleComponentContext struct {
	*antlr.BaseParserRuleContext
	parser    antlr.Parser
	component ParseRuleComponent
	l         antlr.Token
	t         antlr.Token
	r         antlr.Token
}

func NewEmptyRuleComponentContext() *RuleComponentContext {
	var p = new(RuleComponentContext)
	p.BaseParserRuleContext = antlr.NewBaseParserRuleContext(nil, -1)
	p.RuleIndex = LL1GrammarParserRULE_ruleComponent
	return p
}

func (*RuleComponentContext) IsRuleComponentContext() {}

func NewRuleComponentContext(parser antlr.Parser, parent antlr.ParserRuleContext, invokingState int) *RuleComponentContext {
	var p = new(RuleComponentContext)

	p.BaseParserRuleContext = antlr.NewBaseParserRuleContext(parent, invokingState)

	p.parser = parser
	p.RuleIndex = LL1GrammarParserRULE_ruleComponent

	return p
}

func (s *RuleComponentContext) GetParser() antlr.Parser { return s.parser }

func (s *RuleComponentContext) GetL() antlr.Token { return s.l }

func (s *RuleComponentContext) GetT() antlr.Token { return s.t }

func (s *RuleComponentContext) GetR() antlr.Token { return s.r }

func (s *RuleComponentContext) SetL(v antlr.Token) { s.l = v }

func (s *RuleComponentContext) SetT(v antlr.Token) { s.t = v }

func (s *RuleComponentContext) SetR(v antlr.Token) { s.r = v }

func (s *RuleComponentContext) GetComponent() ParseRuleComponent { return s.component }

func (s *RuleComponentContext) SetComponent(v ParseRuleComponent) { s.component = v }

func (s *RuleComponentContext) Literal() antlr.TerminalNode {
	return s.GetToken(LL1GrammarParserLiteral, 0)
}

func (s *RuleComponentContext) TokenIdent() antlr.TerminalNode {
	return s.GetToken(LL1GrammarParserTokenIdent, 0)
}

func (s *RuleComponentContext) RuleIdent() antlr.TerminalNode {
	return s.GetToken(LL1GrammarParserRuleIdent, 0)
}

func (s *RuleComponentContext) GetRuleContext() antlr.RuleContext {
	return s
}

func (s *RuleComponentContext) ToStringTree(ruleNames []string, recog antlr.Recognizer) string {
	return antlr.TreesStringTree(s, ruleNames, recog)
}

func (p *LL1GrammarParser) RuleComponent() (localctx IRuleComponentContext) {
	this := p
	_ = this

	localctx = NewRuleComponentContext(p, p.GetParserRuleContext(), p.GetState())
	p.EnterRule(localctx, 14, LL1GrammarParserRULE_ruleComponent)

	defer func() {
		p.ExitRule()
	}()

	defer func() {
		if err := recover(); err != nil {
			if v, ok := err.(antlr.RecognitionException); ok {
				localctx.SetException(v)
				p.GetErrorHandler().ReportError(p, v)
				p.GetErrorHandler().Recover(p, v)
			} else {
				panic(err)
			}
		}
	}()

	p.SetState(75)
	p.GetErrorHandler().Sync(p)

	switch p.GetTokenStream().LA(1) {
	case LL1GrammarParserLiteral:
		p.EnterOuterAlt(localctx, 1)
		{
			p.SetState(69)

			var _m = p.Match(LL1GrammarParserLiteral)

			localctx.(*RuleComponentContext).l = _m
		}
		localctx.(*RuleComponentContext).SetComponent(ParseRuleComponent{Type: ParseRuleComponentLiteral, Value: literalValue(localctx.(*RuleComponentContext).GetL())})

	case LL1GrammarParserTokenIdent:
		p.EnterOuterAlt(localctx, 2)
		{
			p.SetState(71)

			var _m = p.Match(LL1GrammarParserTokenIdent)

			localctx.(*RuleComponentContext).t = _m
		}
		localctx.(*RuleComponentContext).SetComponent(ParseRuleComponent{Type: ParseRuleComponentToken, Value: (func() string {
			if localctx.(*RuleComponentContext).GetT() == nil {
				return ""
			} else {
				return localctx.(*RuleComponentContext).GetT().GetText()
			}
		}())})

	case LL1GrammarParserRuleIdent:
		p.EnterOuterAlt(localctx, 3)
		{
			p.SetState(73)

			var _m = p.Match(LL1GrammarParserRuleIdent)

			localctx.(*RuleComponentContext).r = _m
		}
		localctx.(*RuleComponentContext).SetComponent(ParseRuleComponent{Type: ParseRuleComponentRule, Value: (func() string {
			if localctx.(*RuleComponentContext).GetR() == nil {
				return ""
			} else {
				return localctx.(*RuleComponentContext).GetR().GetText()
			}
		}())})

	default:
		panic(antlr.NewNoViableAltException(p, nil, nil, nil, nil, nil))
	}

	return localctx
}

func (p *LL1GrammarParser) Sempred(localctx antlr.RuleContext, ruleIndex, predIndex int) bool {
	switch ruleIndex {
	case 4:
		var t *AllRulesContext = nil
		if localctx != nil {
			t = localctx.(*AllRulesContext)
		}
		return p.AllRules_Sempred(t, predIndex)

	default:
		panic("No predicate with index: " + fmt.Sprint(ruleIndex))
	}
}

func (p *LL1GrammarParser) AllRules_Sempred(localctx antlr.RuleContext, predIndex int) bool {
	this := p
	_ = this

	switch predIndex {
	case 0:
		return p.Precpred(p.GetParserRuleContext(), 2)

	default:
		panic("No predicate with index: " + fmt.Sprint(predIndex))
	}
}
