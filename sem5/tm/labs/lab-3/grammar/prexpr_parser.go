// Code generated from java-escape by ANTLR 4.11.1. DO NOT EDIT.

package grammar // PrExpr
import (
	"fmt"
	"strconv"
	"sync"

	"github.com/antlr/antlr4/runtime/Go/antlr/v4"
)

import "strings"

// Suppress unused import errors
var _ = fmt.Printf
var _ = strconv.Itoa
var _ = sync.Once{}

type PrExprParser struct {
	*antlr.BaseParser
}

var prexprParserStaticData struct {
	once                   sync.Once
	serializedATN          []int32
	literalNames           []string
	symbolicNames          []string
	ruleNames              []string
	predictionContextCache *antlr.PredictionContextCache
	atn                    *antlr.ATN
	decisionToDFA          []*antlr.DFA
}

func prexprParserInit() {
	staticData := &prexprParserStaticData
	staticData.literalNames = []string{
		"", "'print'", "'if'", "'&&'", "'||'", "'=='", "'<'", "'>'", "'*'",
		"'/'", "'+'", "'-'", "'!'", "'='",
	}
	staticData.symbolicNames = []string{
		"", "", "", "", "", "", "", "", "", "", "", "", "", "", "Bool", "Integer",
		"Ident", "Whitespace",
	}
	staticData.ruleNames = []string{
		"prefixExpr", "statement", "ifStmt", "ifContinuation", "expr",
	}
	staticData.predictionContextCache = antlr.NewPredictionContextCache()
	staticData.serializedATN = []int32{
		4, 1, 17, 60, 2, 0, 7, 0, 2, 1, 7, 1, 2, 2, 7, 2, 2, 3, 7, 3, 2, 4, 7,
		4, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 3, 1, 25, 8, 1, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 3,
		1, 3, 1, 3, 1, 3, 1, 3, 1, 3, 1, 3, 3, 3, 40, 8, 3, 1, 4, 1, 4, 1, 4, 1,
		4, 1, 4, 1, 4, 1, 4, 1, 4, 1, 4, 1, 4, 1, 4, 1, 4, 1, 4, 1, 4, 1, 4, 1,
		4, 3, 4, 58, 8, 4, 1, 4, 0, 0, 5, 0, 2, 4, 6, 8, 0, 2, 1, 0, 3, 11, 1,
		0, 14, 16, 61, 0, 10, 1, 0, 0, 0, 2, 24, 1, 0, 0, 0, 4, 26, 1, 0, 0, 0,
		6, 39, 1, 0, 0, 0, 8, 57, 1, 0, 0, 0, 10, 11, 3, 2, 1, 0, 11, 12, 5, 0,
		0, 1, 12, 13, 6, 0, -1, 0, 13, 1, 1, 0, 0, 0, 14, 15, 3, 8, 4, 0, 15, 16,
		6, 1, -1, 0, 16, 25, 1, 0, 0, 0, 17, 18, 3, 4, 2, 0, 18, 19, 6, 1, -1,
		0, 19, 25, 1, 0, 0, 0, 20, 21, 5, 1, 0, 0, 21, 22, 3, 8, 4, 0, 22, 23,
		6, 1, -1, 0, 23, 25, 1, 0, 0, 0, 24, 14, 1, 0, 0, 0, 24, 17, 1, 0, 0, 0,
		24, 20, 1, 0, 0, 0, 25, 3, 1, 0, 0, 0, 26, 27, 5, 2, 0, 0, 27, 28, 3, 8,
		4, 0, 28, 29, 3, 2, 1, 0, 29, 30, 3, 6, 3, 0, 30, 31, 6, 2, -1, 0, 31,
		5, 1, 0, 0, 0, 32, 40, 6, 3, -1, 0, 33, 34, 3, 4, 2, 0, 34, 35, 6, 3, -1,
		0, 35, 40, 1, 0, 0, 0, 36, 37, 3, 2, 1, 0, 37, 38, 6, 3, -1, 0, 38, 40,
		1, 0, 0, 0, 39, 32, 1, 0, 0, 0, 39, 33, 1, 0, 0, 0, 39, 36, 1, 0, 0, 0,
		40, 7, 1, 0, 0, 0, 41, 42, 7, 0, 0, 0, 42, 43, 3, 8, 4, 0, 43, 44, 3, 8,
		4, 0, 44, 45, 6, 4, -1, 0, 45, 58, 1, 0, 0, 0, 46, 47, 5, 12, 0, 0, 47,
		48, 3, 8, 4, 0, 48, 49, 6, 4, -1, 0, 49, 58, 1, 0, 0, 0, 50, 51, 5, 13,
		0, 0, 51, 52, 5, 16, 0, 0, 52, 53, 3, 8, 4, 0, 53, 54, 6, 4, -1, 0, 54,
		58, 1, 0, 0, 0, 55, 56, 7, 1, 0, 0, 56, 58, 6, 4, -1, 0, 57, 41, 1, 0,
		0, 0, 57, 46, 1, 0, 0, 0, 57, 50, 1, 0, 0, 0, 57, 55, 1, 0, 0, 0, 58, 9,
		1, 0, 0, 0, 3, 24, 39, 57,
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

// PrExprParserInit initializes any static state used to implement PrExprParser. By default the
// static state used to implement the parser is lazily initialized during the first call to
// NewPrExprParser(). You can call this function if you wish to initialize the static state ahead
// of time.
func PrExprParserInit() {
	staticData := &prexprParserStaticData
	staticData.once.Do(prexprParserInit)
}

// NewPrExprParser produces a new parser instance for the optional input antlr.TokenStream.
func NewPrExprParser(input antlr.TokenStream) *PrExprParser {
	PrExprParserInit()
	this := new(PrExprParser)
	this.BaseParser = antlr.NewBaseParser(input)
	staticData := &prexprParserStaticData
	this.Interpreter = antlr.NewParserATNSimulator(this, staticData.atn, staticData.decisionToDFA, staticData.predictionContextCache)
	this.RuleNames = staticData.ruleNames
	this.LiteralNames = staticData.literalNames
	this.SymbolicNames = staticData.symbolicNames
	this.GrammarFileName = "java-escape"

	return this
}

const newline = "\n"
const tab = "  "

func fmtExpr(repr string, composite bool) string {
	if composite {
		return "(" + repr + ")"
	}
	return repr
}

func fmtOp(op, a, b string) string {
	return a + " " + op + " " + b
}

func tabLines(text string) string {
	lines := strings.Split(text, newline)
	for i, line := range lines {
		lines[i] = tab + line
	}
	return strings.Join(lines, newline)
}

// PrExprParser tokens.
const (
	PrExprParserEOF        = antlr.TokenEOF
	PrExprParserT__0       = 1
	PrExprParserT__1       = 2
	PrExprParserT__2       = 3
	PrExprParserT__3       = 4
	PrExprParserT__4       = 5
	PrExprParserT__5       = 6
	PrExprParserT__6       = 7
	PrExprParserT__7       = 8
	PrExprParserT__8       = 9
	PrExprParserT__9       = 10
	PrExprParserT__10      = 11
	PrExprParserT__11      = 12
	PrExprParserT__12      = 13
	PrExprParserBool       = 14
	PrExprParserInteger    = 15
	PrExprParserIdent      = 16
	PrExprParserWhitespace = 17
)

// PrExprParser rules.
const (
	PrExprParserRULE_prefixExpr     = 0
	PrExprParserRULE_statement      = 1
	PrExprParserRULE_ifStmt         = 2
	PrExprParserRULE_ifContinuation = 3
	PrExprParserRULE_expr           = 4
)

// IPrefixExprContext is an interface to support dynamic dispatch.
type IPrefixExprContext interface {
	antlr.ParserRuleContext

	// GetParser returns the parser.
	GetParser() antlr.Parser

	// Get_statement returns the _statement rule contexts.
	Get_statement() IStatementContext

	// Set_statement sets the _statement rule contexts.
	Set_statement(IStatementContext)

	// GetRepr returns the repr attribute.
	GetRepr() string

	// SetRepr sets the repr attribute.
	SetRepr(string)

	// IsPrefixExprContext differentiates from other interfaces.
	IsPrefixExprContext()
}

type PrefixExprContext struct {
	*antlr.BaseParserRuleContext
	parser     antlr.Parser
	repr       string
	_statement IStatementContext
}

func NewEmptyPrefixExprContext() *PrefixExprContext {
	var p = new(PrefixExprContext)
	p.BaseParserRuleContext = antlr.NewBaseParserRuleContext(nil, -1)
	p.RuleIndex = PrExprParserRULE_prefixExpr
	return p
}

func (*PrefixExprContext) IsPrefixExprContext() {}

func NewPrefixExprContext(parser antlr.Parser, parent antlr.ParserRuleContext, invokingState int) *PrefixExprContext {
	var p = new(PrefixExprContext)

	p.BaseParserRuleContext = antlr.NewBaseParserRuleContext(parent, invokingState)

	p.parser = parser
	p.RuleIndex = PrExprParserRULE_prefixExpr

	return p
}

func (s *PrefixExprContext) GetParser() antlr.Parser { return s.parser }

func (s *PrefixExprContext) Get_statement() IStatementContext { return s._statement }

func (s *PrefixExprContext) Set_statement(v IStatementContext) { s._statement = v }

func (s *PrefixExprContext) GetRepr() string { return s.repr }

func (s *PrefixExprContext) SetRepr(v string) { s.repr = v }

func (s *PrefixExprContext) Statement() IStatementContext {
	var t antlr.RuleContext
	for _, ctx := range s.GetChildren() {
		if _, ok := ctx.(IStatementContext); ok {
			t = ctx.(antlr.RuleContext)
			break
		}
	}

	if t == nil {
		return nil
	}

	return t.(IStatementContext)
}

func (s *PrefixExprContext) EOF() antlr.TerminalNode {
	return s.GetToken(PrExprParserEOF, 0)
}

func (s *PrefixExprContext) GetRuleContext() antlr.RuleContext {
	return s
}

func (s *PrefixExprContext) ToStringTree(ruleNames []string, recog antlr.Recognizer) string {
	return antlr.TreesStringTree(s, ruleNames, recog)
}

func (p *PrExprParser) PrefixExpr() (localctx IPrefixExprContext) {
	this := p
	_ = this

	localctx = NewPrefixExprContext(p, p.GetParserRuleContext(), p.GetState())
	p.EnterRule(localctx, 0, PrExprParserRULE_prefixExpr)

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
		p.SetState(10)

		var _x = p.Statement()

		localctx.(*PrefixExprContext)._statement = _x
	}
	{
		p.SetState(11)
		p.Match(PrExprParserEOF)
	}
	localctx.(*PrefixExprContext).SetRepr(localctx.(*PrefixExprContext).Get_statement().GetRepr())

	return localctx
}

// IStatementContext is an interface to support dynamic dispatch.
type IStatementContext interface {
	antlr.ParserRuleContext

	// GetParser returns the parser.
	GetParser() antlr.Parser

	// Get_expr returns the _expr rule contexts.
	Get_expr() IExprContext

	// Get_ifStmt returns the _ifStmt rule contexts.
	Get_ifStmt() IIfStmtContext

	// Set_expr sets the _expr rule contexts.
	Set_expr(IExprContext)

	// Set_ifStmt sets the _ifStmt rule contexts.
	Set_ifStmt(IIfStmtContext)

	// GetRepr returns the repr attribute.
	GetRepr() string

	// SetRepr sets the repr attribute.
	SetRepr(string)

	// IsStatementContext differentiates from other interfaces.
	IsStatementContext()
}

type StatementContext struct {
	*antlr.BaseParserRuleContext
	parser  antlr.Parser
	repr    string
	_expr   IExprContext
	_ifStmt IIfStmtContext
}

func NewEmptyStatementContext() *StatementContext {
	var p = new(StatementContext)
	p.BaseParserRuleContext = antlr.NewBaseParserRuleContext(nil, -1)
	p.RuleIndex = PrExprParserRULE_statement
	return p
}

func (*StatementContext) IsStatementContext() {}

func NewStatementContext(parser antlr.Parser, parent antlr.ParserRuleContext, invokingState int) *StatementContext {
	var p = new(StatementContext)

	p.BaseParserRuleContext = antlr.NewBaseParserRuleContext(parent, invokingState)

	p.parser = parser
	p.RuleIndex = PrExprParserRULE_statement

	return p
}

func (s *StatementContext) GetParser() antlr.Parser { return s.parser }

func (s *StatementContext) Get_expr() IExprContext { return s._expr }

func (s *StatementContext) Get_ifStmt() IIfStmtContext { return s._ifStmt }

func (s *StatementContext) Set_expr(v IExprContext) { s._expr = v }

func (s *StatementContext) Set_ifStmt(v IIfStmtContext) { s._ifStmt = v }

func (s *StatementContext) GetRepr() string { return s.repr }

func (s *StatementContext) SetRepr(v string) { s.repr = v }

func (s *StatementContext) Expr() IExprContext {
	var t antlr.RuleContext
	for _, ctx := range s.GetChildren() {
		if _, ok := ctx.(IExprContext); ok {
			t = ctx.(antlr.RuleContext)
			break
		}
	}

	if t == nil {
		return nil
	}

	return t.(IExprContext)
}

func (s *StatementContext) IfStmt() IIfStmtContext {
	var t antlr.RuleContext
	for _, ctx := range s.GetChildren() {
		if _, ok := ctx.(IIfStmtContext); ok {
			t = ctx.(antlr.RuleContext)
			break
		}
	}

	if t == nil {
		return nil
	}

	return t.(IIfStmtContext)
}

func (s *StatementContext) GetRuleContext() antlr.RuleContext {
	return s
}

func (s *StatementContext) ToStringTree(ruleNames []string, recog antlr.Recognizer) string {
	return antlr.TreesStringTree(s, ruleNames, recog)
}

func (p *PrExprParser) Statement() (localctx IStatementContext) {
	this := p
	_ = this

	localctx = NewStatementContext(p, p.GetParserRuleContext(), p.GetState())
	p.EnterRule(localctx, 2, PrExprParserRULE_statement)

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

	p.SetState(24)
	p.GetErrorHandler().Sync(p)

	switch p.GetTokenStream().LA(1) {
	case PrExprParserT__2, PrExprParserT__3, PrExprParserT__4, PrExprParserT__5, PrExprParserT__6, PrExprParserT__7, PrExprParserT__8, PrExprParserT__9, PrExprParserT__10, PrExprParserT__11, PrExprParserT__12, PrExprParserBool, PrExprParserInteger, PrExprParserIdent:
		p.EnterOuterAlt(localctx, 1)
		{
			p.SetState(14)

			var _x = p.Expr()

			localctx.(*StatementContext)._expr = _x
		}
		localctx.(*StatementContext).SetRepr(localctx.(*StatementContext).Get_expr().GetRepr())

	case PrExprParserT__1:
		p.EnterOuterAlt(localctx, 2)
		{
			p.SetState(17)

			var _x = p.IfStmt()

			localctx.(*StatementContext)._ifStmt = _x
		}
		localctx.(*StatementContext).SetRepr(localctx.(*StatementContext).Get_ifStmt().GetRepr())

	case PrExprParserT__0:
		p.EnterOuterAlt(localctx, 3)
		{
			p.SetState(20)
			p.Match(PrExprParserT__0)
		}
		{
			p.SetState(21)

			var _x = p.Expr()

			localctx.(*StatementContext)._expr = _x
		}
		localctx.(*StatementContext).SetRepr("println(" + localctx.(*StatementContext).Get_expr().GetRepr() + ")")

	default:
		panic(antlr.NewNoViableAltException(p, nil, nil, nil, nil, nil))
	}

	return localctx
}

// IIfStmtContext is an interface to support dynamic dispatch.
type IIfStmtContext interface {
	antlr.ParserRuleContext

	// GetParser returns the parser.
	GetParser() antlr.Parser

	// Get_expr returns the _expr rule contexts.
	Get_expr() IExprContext

	// Get_statement returns the _statement rule contexts.
	Get_statement() IStatementContext

	// Get_ifContinuation returns the _ifContinuation rule contexts.
	Get_ifContinuation() IIfContinuationContext

	// Set_expr sets the _expr rule contexts.
	Set_expr(IExprContext)

	// Set_statement sets the _statement rule contexts.
	Set_statement(IStatementContext)

	// Set_ifContinuation sets the _ifContinuation rule contexts.
	Set_ifContinuation(IIfContinuationContext)

	// GetRepr returns the repr attribute.
	GetRepr() string

	// SetRepr sets the repr attribute.
	SetRepr(string)

	// IsIfStmtContext differentiates from other interfaces.
	IsIfStmtContext()
}

type IfStmtContext struct {
	*antlr.BaseParserRuleContext
	parser          antlr.Parser
	repr            string
	_expr           IExprContext
	_statement      IStatementContext
	_ifContinuation IIfContinuationContext
}

func NewEmptyIfStmtContext() *IfStmtContext {
	var p = new(IfStmtContext)
	p.BaseParserRuleContext = antlr.NewBaseParserRuleContext(nil, -1)
	p.RuleIndex = PrExprParserRULE_ifStmt
	return p
}

func (*IfStmtContext) IsIfStmtContext() {}

func NewIfStmtContext(parser antlr.Parser, parent antlr.ParserRuleContext, invokingState int) *IfStmtContext {
	var p = new(IfStmtContext)

	p.BaseParserRuleContext = antlr.NewBaseParserRuleContext(parent, invokingState)

	p.parser = parser
	p.RuleIndex = PrExprParserRULE_ifStmt

	return p
}

func (s *IfStmtContext) GetParser() antlr.Parser { return s.parser }

func (s *IfStmtContext) Get_expr() IExprContext { return s._expr }

func (s *IfStmtContext) Get_statement() IStatementContext { return s._statement }

func (s *IfStmtContext) Get_ifContinuation() IIfContinuationContext { return s._ifContinuation }

func (s *IfStmtContext) Set_expr(v IExprContext) { s._expr = v }

func (s *IfStmtContext) Set_statement(v IStatementContext) { s._statement = v }

func (s *IfStmtContext) Set_ifContinuation(v IIfContinuationContext) { s._ifContinuation = v }

func (s *IfStmtContext) GetRepr() string { return s.repr }

func (s *IfStmtContext) SetRepr(v string) { s.repr = v }

func (s *IfStmtContext) Expr() IExprContext {
	var t antlr.RuleContext
	for _, ctx := range s.GetChildren() {
		if _, ok := ctx.(IExprContext); ok {
			t = ctx.(antlr.RuleContext)
			break
		}
	}

	if t == nil {
		return nil
	}

	return t.(IExprContext)
}

func (s *IfStmtContext) Statement() IStatementContext {
	var t antlr.RuleContext
	for _, ctx := range s.GetChildren() {
		if _, ok := ctx.(IStatementContext); ok {
			t = ctx.(antlr.RuleContext)
			break
		}
	}

	if t == nil {
		return nil
	}

	return t.(IStatementContext)
}

func (s *IfStmtContext) IfContinuation() IIfContinuationContext {
	var t antlr.RuleContext
	for _, ctx := range s.GetChildren() {
		if _, ok := ctx.(IIfContinuationContext); ok {
			t = ctx.(antlr.RuleContext)
			break
		}
	}

	if t == nil {
		return nil
	}

	return t.(IIfContinuationContext)
}

func (s *IfStmtContext) GetRuleContext() antlr.RuleContext {
	return s
}

func (s *IfStmtContext) ToStringTree(ruleNames []string, recog antlr.Recognizer) string {
	return antlr.TreesStringTree(s, ruleNames, recog)
}

func (p *PrExprParser) IfStmt() (localctx IIfStmtContext) {
	this := p
	_ = this

	localctx = NewIfStmtContext(p, p.GetParserRuleContext(), p.GetState())
	p.EnterRule(localctx, 4, PrExprParserRULE_ifStmt)

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
		p.SetState(26)
		p.Match(PrExprParserT__1)
	}
	{
		p.SetState(27)

		var _x = p.Expr()

		localctx.(*IfStmtContext)._expr = _x
	}
	{
		p.SetState(28)

		var _x = p.Statement()

		localctx.(*IfStmtContext)._statement = _x
	}
	{
		p.SetState(29)

		var _x = p.IfContinuation()

		localctx.(*IfStmtContext)._ifContinuation = _x
	}

	localctx.(*IfStmtContext).SetRepr("if " + localctx.(*IfStmtContext).Get_expr().GetRepr() + " {\n" + tabLines(localctx.(*IfStmtContext).Get_statement().GetRepr()) + "\n}" + localctx.(*IfStmtContext).Get_ifContinuation().GetRepr())

	return localctx
}

// IIfContinuationContext is an interface to support dynamic dispatch.
type IIfContinuationContext interface {
	antlr.ParserRuleContext

	// GetParser returns the parser.
	GetParser() antlr.Parser

	// Get_ifStmt returns the _ifStmt rule contexts.
	Get_ifStmt() IIfStmtContext

	// Get_statement returns the _statement rule contexts.
	Get_statement() IStatementContext

	// Set_ifStmt sets the _ifStmt rule contexts.
	Set_ifStmt(IIfStmtContext)

	// Set_statement sets the _statement rule contexts.
	Set_statement(IStatementContext)

	// GetRepr returns the repr attribute.
	GetRepr() string

	// SetRepr sets the repr attribute.
	SetRepr(string)

	// IsIfContinuationContext differentiates from other interfaces.
	IsIfContinuationContext()
}

type IfContinuationContext struct {
	*antlr.BaseParserRuleContext
	parser     antlr.Parser
	repr       string
	_ifStmt    IIfStmtContext
	_statement IStatementContext
}

func NewEmptyIfContinuationContext() *IfContinuationContext {
	var p = new(IfContinuationContext)
	p.BaseParserRuleContext = antlr.NewBaseParserRuleContext(nil, -1)
	p.RuleIndex = PrExprParserRULE_ifContinuation
	return p
}

func (*IfContinuationContext) IsIfContinuationContext() {}

func NewIfContinuationContext(parser antlr.Parser, parent antlr.ParserRuleContext, invokingState int) *IfContinuationContext {
	var p = new(IfContinuationContext)

	p.BaseParserRuleContext = antlr.NewBaseParserRuleContext(parent, invokingState)

	p.parser = parser
	p.RuleIndex = PrExprParserRULE_ifContinuation

	return p
}

func (s *IfContinuationContext) GetParser() antlr.Parser { return s.parser }

func (s *IfContinuationContext) Get_ifStmt() IIfStmtContext { return s._ifStmt }

func (s *IfContinuationContext) Get_statement() IStatementContext { return s._statement }

func (s *IfContinuationContext) Set_ifStmt(v IIfStmtContext) { s._ifStmt = v }

func (s *IfContinuationContext) Set_statement(v IStatementContext) { s._statement = v }

func (s *IfContinuationContext) GetRepr() string { return s.repr }

func (s *IfContinuationContext) SetRepr(v string) { s.repr = v }

func (s *IfContinuationContext) IfStmt() IIfStmtContext {
	var t antlr.RuleContext
	for _, ctx := range s.GetChildren() {
		if _, ok := ctx.(IIfStmtContext); ok {
			t = ctx.(antlr.RuleContext)
			break
		}
	}

	if t == nil {
		return nil
	}

	return t.(IIfStmtContext)
}

func (s *IfContinuationContext) Statement() IStatementContext {
	var t antlr.RuleContext
	for _, ctx := range s.GetChildren() {
		if _, ok := ctx.(IStatementContext); ok {
			t = ctx.(antlr.RuleContext)
			break
		}
	}

	if t == nil {
		return nil
	}

	return t.(IStatementContext)
}

func (s *IfContinuationContext) GetRuleContext() antlr.RuleContext {
	return s
}

func (s *IfContinuationContext) ToStringTree(ruleNames []string, recog antlr.Recognizer) string {
	return antlr.TreesStringTree(s, ruleNames, recog)
}

func (p *PrExprParser) IfContinuation() (localctx IIfContinuationContext) {
	this := p
	_ = this

	localctx = NewIfContinuationContext(p, p.GetParserRuleContext(), p.GetState())
	p.EnterRule(localctx, 6, PrExprParserRULE_ifContinuation)

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

	p.SetState(39)
	p.GetErrorHandler().Sync(p)
	switch p.GetInterpreter().AdaptivePredict(p.GetTokenStream(), 1, p.GetParserRuleContext()) {
	case 1:
		p.EnterOuterAlt(localctx, 1)
		localctx.(*IfContinuationContext).SetRepr("")

	case 2:
		p.EnterOuterAlt(localctx, 2)
		{
			p.SetState(33)

			var _x = p.IfStmt()

			localctx.(*IfContinuationContext)._ifStmt = _x
		}
		localctx.(*IfContinuationContext).SetRepr(" else " + localctx.(*IfContinuationContext).Get_ifStmt().GetRepr())

	case 3:
		p.EnterOuterAlt(localctx, 3)
		{
			p.SetState(36)

			var _x = p.Statement()

			localctx.(*IfContinuationContext)._statement = _x
		}
		localctx.(*IfContinuationContext).SetRepr(" else {\n" + tabLines(localctx.(*IfContinuationContext).Get_statement().GetRepr()) + "\n}")

	}

	return localctx
}

// IExprContext is an interface to support dynamic dispatch.
type IExprContext interface {
	antlr.ParserRuleContext

	// GetParser returns the parser.
	GetParser() antlr.Parser

	// GetOp returns the op token.
	GetOp() antlr.Token

	// GetId returns the id token.
	GetId() antlr.Token

	// GetVal returns the val token.
	GetVal() antlr.Token

	// SetOp sets the op token.
	SetOp(antlr.Token)

	// SetId sets the id token.
	SetId(antlr.Token)

	// SetVal sets the val token.
	SetVal(antlr.Token)

	// GetA returns the a rule contexts.
	GetA() IExprContext

	// GetB returns the b rule contexts.
	GetB() IExprContext

	// SetA sets the a rule contexts.
	SetA(IExprContext)

	// SetB sets the b rule contexts.
	SetB(IExprContext)

	// GetRepr returns the repr attribute.
	GetRepr() string

	// GetComposite returns the composite attribute.
	GetComposite() bool

	// SetRepr sets the repr attribute.
	SetRepr(string)

	// SetComposite sets the composite attribute.
	SetComposite(bool)

	// IsExprContext differentiates from other interfaces.
	IsExprContext()
}

type ExprContext struct {
	*antlr.BaseParserRuleContext
	parser    antlr.Parser
	repr      string
	composite bool
	op        antlr.Token
	a         IExprContext
	b         IExprContext
	id        antlr.Token
	val       antlr.Token
}

func NewEmptyExprContext() *ExprContext {
	var p = new(ExprContext)
	p.BaseParserRuleContext = antlr.NewBaseParserRuleContext(nil, -1)
	p.RuleIndex = PrExprParserRULE_expr
	return p
}

func (*ExprContext) IsExprContext() {}

func NewExprContext(parser antlr.Parser, parent antlr.ParserRuleContext, invokingState int) *ExprContext {
	var p = new(ExprContext)

	p.BaseParserRuleContext = antlr.NewBaseParserRuleContext(parent, invokingState)

	p.parser = parser
	p.RuleIndex = PrExprParserRULE_expr

	return p
}

func (s *ExprContext) GetParser() antlr.Parser { return s.parser }

func (s *ExprContext) GetOp() antlr.Token { return s.op }

func (s *ExprContext) GetId() antlr.Token { return s.id }

func (s *ExprContext) GetVal() antlr.Token { return s.val }

func (s *ExprContext) SetOp(v antlr.Token) { s.op = v }

func (s *ExprContext) SetId(v antlr.Token) { s.id = v }

func (s *ExprContext) SetVal(v antlr.Token) { s.val = v }

func (s *ExprContext) GetA() IExprContext { return s.a }

func (s *ExprContext) GetB() IExprContext { return s.b }

func (s *ExprContext) SetA(v IExprContext) { s.a = v }

func (s *ExprContext) SetB(v IExprContext) { s.b = v }

func (s *ExprContext) GetRepr() string { return s.repr }

func (s *ExprContext) GetComposite() bool { return s.composite }

func (s *ExprContext) SetRepr(v string) { s.repr = v }

func (s *ExprContext) SetComposite(v bool) { s.composite = v }

func (s *ExprContext) AllExpr() []IExprContext {
	children := s.GetChildren()
	len := 0
	for _, ctx := range children {
		if _, ok := ctx.(IExprContext); ok {
			len++
		}
	}

	tst := make([]IExprContext, len)
	i := 0
	for _, ctx := range children {
		if t, ok := ctx.(IExprContext); ok {
			tst[i] = t.(IExprContext)
			i++
		}
	}

	return tst
}

func (s *ExprContext) Expr(i int) IExprContext {
	var t antlr.RuleContext
	j := 0
	for _, ctx := range s.GetChildren() {
		if _, ok := ctx.(IExprContext); ok {
			if j == i {
				t = ctx.(antlr.RuleContext)
				break
			}
			j++
		}
	}

	if t == nil {
		return nil
	}

	return t.(IExprContext)
}

func (s *ExprContext) Ident() antlr.TerminalNode {
	return s.GetToken(PrExprParserIdent, 0)
}

func (s *ExprContext) Integer() antlr.TerminalNode {
	return s.GetToken(PrExprParserInteger, 0)
}

func (s *ExprContext) Bool() antlr.TerminalNode {
	return s.GetToken(PrExprParserBool, 0)
}

func (s *ExprContext) GetRuleContext() antlr.RuleContext {
	return s
}

func (s *ExprContext) ToStringTree(ruleNames []string, recog antlr.Recognizer) string {
	return antlr.TreesStringTree(s, ruleNames, recog)
}

func (p *PrExprParser) Expr() (localctx IExprContext) {
	this := p
	_ = this

	localctx = NewExprContext(p, p.GetParserRuleContext(), p.GetState())
	p.EnterRule(localctx, 8, PrExprParserRULE_expr)
	var _la int

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

	p.SetState(57)
	p.GetErrorHandler().Sync(p)

	switch p.GetTokenStream().LA(1) {
	case PrExprParserT__2, PrExprParserT__3, PrExprParserT__4, PrExprParserT__5, PrExprParserT__6, PrExprParserT__7, PrExprParserT__8, PrExprParserT__9, PrExprParserT__10:
		p.EnterOuterAlt(localctx, 1)
		{
			p.SetState(41)

			var _lt = p.GetTokenStream().LT(1)

			localctx.(*ExprContext).op = _lt

			_la = p.GetTokenStream().LA(1)

			if !((int64(_la) & ^0x3f) == 0 && ((int64(1)<<_la)&4088) != 0) {
				var _ri = p.GetErrorHandler().RecoverInline(p)

				localctx.(*ExprContext).op = _ri
			} else {
				p.GetErrorHandler().ReportMatch(p)
				p.Consume()
			}
		}
		{
			p.SetState(42)

			var _x = p.Expr()

			localctx.(*ExprContext).a = _x
		}
		{
			p.SetState(43)

			var _x = p.Expr()

			localctx.(*ExprContext).b = _x
		}

		localctx.(*ExprContext).SetRepr(fmtOp((func() string {
			if localctx.(*ExprContext).GetOp() == nil {
				return ""
			} else {
				return localctx.(*ExprContext).GetOp().GetText()
			}
		}()), fmtExpr(localctx.(*ExprContext).GetA().GetRepr(), localctx.(*ExprContext).GetA().GetComposite()), fmtExpr(localctx.(*ExprContext).GetB().GetRepr(), localctx.(*ExprContext).GetB().GetComposite())))
		localctx.(*ExprContext).SetComposite(true)

	case PrExprParserT__11:
		p.EnterOuterAlt(localctx, 2)
		{
			p.SetState(46)
			p.Match(PrExprParserT__11)
		}
		{
			p.SetState(47)

			var _x = p.Expr()

			localctx.(*ExprContext).a = _x
		}

		localctx.(*ExprContext).SetRepr("!" + fmtExpr(localctx.(*ExprContext).GetA().GetRepr(), localctx.(*ExprContext).GetA().GetComposite()))
		localctx.(*ExprContext).SetComposite(false)

	case PrExprParserT__12:
		p.EnterOuterAlt(localctx, 3)
		{
			p.SetState(50)
			p.Match(PrExprParserT__12)
		}
		{
			p.SetState(51)

			var _m = p.Match(PrExprParserIdent)

			localctx.(*ExprContext).id = _m
		}
		{
			p.SetState(52)

			var _x = p.Expr()

			localctx.(*ExprContext).b = _x
		}

		localctx.(*ExprContext).SetRepr(fmtOp("=", (func() string {
			if localctx.(*ExprContext).GetId() == nil {
				return ""
			} else {
				return localctx.(*ExprContext).GetId().GetText()
			}
		}()), fmtExpr(localctx.(*ExprContext).GetB().GetRepr(), localctx.(*ExprContext).GetB().GetComposite())))
		localctx.(*ExprContext).SetComposite(true)

	case PrExprParserBool, PrExprParserInteger, PrExprParserIdent:
		p.EnterOuterAlt(localctx, 4)
		{
			p.SetState(55)

			var _lt = p.GetTokenStream().LT(1)

			localctx.(*ExprContext).val = _lt

			_la = p.GetTokenStream().LA(1)

			if !((int64(_la) & ^0x3f) == 0 && ((int64(1)<<_la)&114688) != 0) {
				var _ri = p.GetErrorHandler().RecoverInline(p)

				localctx.(*ExprContext).val = _ri
			} else {
				p.GetErrorHandler().ReportMatch(p)
				p.Consume()
			}
		}

		localctx.(*ExprContext).SetRepr((func() string {
			if localctx.(*ExprContext).GetVal() == nil {
				return ""
			} else {
				return localctx.(*ExprContext).GetVal().GetText()
			}
		}()))
		localctx.(*ExprContext).SetComposite(false)

	default:
		panic(antlr.NewNoViableAltException(p, nil, nil, nil, nil, nil))
	}

	return localctx
}
