grammar LL1Grammar;

ll1Grammar returns [Grammar grammar]
  : t=ll1Tokens r=ll1Rules {$grammar = Grammar{LexTokens: $t.tokens, ParseRules: $r.rules}; }
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

ll1Rules returns [ParseRules rules]
  : 'rules' '{'
      r=allRules
    '}' { $rules = $r.rules; }
  ;

allRules returns [ParseRules rules]
  : rest=allRules r=singleRule ';' {
      $rules = $rest.rules;
      $rules[$r.name] = append([]ParseRule{$r.components}, $rules[$r.name]...);
    }
  | r=singleRule ';' { $rules = ParseRules{$r.name: []ParseRule{$r.components}}; }
  ;

singleRule returns [string name, []ParseRuleComponent components]
  : n=RuleIdent '=' c=ruleComponentList {
      $name = $n.text;
      $components = $c.components;
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

Literal : ["] (~["]|'\\"'|'\\\\')+ ["] ;
TokenIdent : 'a'..'z' (Letter | Digit | '_')* ;
RuleIdent : 'A'..'Z' (Letter | Digit | '_')* ;
fragment Digit : [0-9] ;
fragment Letter : 'a'..'z' | 'A'..'Z' ;

Whitespace : [\p{White_Space}]+ -> skip;