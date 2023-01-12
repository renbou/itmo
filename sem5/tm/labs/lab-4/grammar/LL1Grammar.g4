grammar LL1Grammar;

@parser::header {
  import "strings"
}

ll1Grammar returns [Grammar grammar]
  : t=ll1Tokens r=ll1Rules {$grammar = Grammar{LexTokens: $t.tokens, StartNonTerminal: $r.startNonTerm, ParseRules: $r.rules}; }
  ;

ll1Tokens returns [LexTokens tokens]
  : 'tokens' '{'
      r=optionalTokensNamed
    '}' { $tokens = LexTokens{named: $r.named}; }
  ;

optionalTokensNamed returns [[]namedToken named]
  : n=TokenIdent '=' l=Literal ';' r=optionalTokensNamed { $named = append($r.named, namedToken{name: $n.text, regexp: literalValue($l)}); }
  | { $named = nil; }
  ;

ll1Rules returns [ParseRules rules, string startNonTerm]
  : 'rules' '{'
      r=allRules
    '}' {
      $rules = $r.rules;
      $startNonTerm = $r.startNonTerm;
    }
  ;

allRules returns [ParseRules rules, string startNonTerm]
  : rest=allRules r=singleRule ';' {
      $startNonTerm = $rest.startNonTerm;
      $rules = $rest.rules;
      $rules[$r.name] = append($rules[$r.name], $r.rule);
    }
  | r=singleRule ';' {
      $startNonTerm = $r.name;
      $rules = ParseRules{$r.name: []ParseRule{$r.rule}};
    }
  ;

singleRule returns [string name, ParseRule rule]
  : n=RuleIdent attributes=Code? '=' c=ruleComponentList code=Code? {
      $name = $n.text;
      $rule = ParseRule{
        Components: $c.components,
        Attributes: strings.TrimPrefix(strings.TrimSuffix($attributes.text, "]"), "["),
        Code: strings.TrimPrefix(strings.TrimSuffix($code.text, "]"), "["),
      };
    }
  ;

ruleComponentList returns [[]ParseRuleComponent components]
  : c=ruleComponent rest=ruleComponentList { $components = append([]ParseRuleComponent{$c.component}, $rest.components...); }
  | { $components = nil; }
  ;

ruleComponent returns [ParseRuleComponent component]
  : l=Literal { $component = ParseRuleComponent{Type: ParseRuleComponentLiteral, Value: literalValue($l)}; }
  | t=TokenIdent { $component = ParseRuleComponent{Type: ParseRuleComponentToken, Value: $t.text}; }
  | r=RuleIdent { $component = ParseRuleComponent{Type: ParseRuleComponentRule, Value: $r.text}; }
  ;

Code : '[' CodeBlock* ']' ;
fragment CodeBlock : ~[[\]]+ | '[' CodeBlock* ']' ;

Literal : ["] (~["]|'\\"'|'\\\\')+ ["] ;
TokenIdent : 'a'..'z' (Letter | Digit | '_')* ;
RuleIdent : 'A'..'Z' (Letter | Digit | '_')* ;
fragment Digit : [0-9] ;
fragment Letter : 'a'..'z' | 'A'..'Z' ;

Whitespace : [\p{White_Space}]+ -> skip;