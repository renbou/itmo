grammar PrExpr;

@parser::header {
  import "strings"
}

@parser::members {
const newline = "\n";
const tab = "  ";

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
  lines := strings.Split(text, newline);
  for i, line := range lines {
    lines[i] = tab + line
  }
  return strings.Join(lines, newline);
}
}

prefixExpr returns [string repr]
  : statement EOF { $repr = $statement.repr; }
  ;

statement returns [string repr]
  : expr { $repr = $expr.repr; }
  | ifStmt { $repr = $ifStmt.repr; }
  | 'print' expr { $repr = "println(" + $expr.repr + ")"; }
  ;

ifStmt returns [string repr]
  : 'if' expr statement ifContinuation {
      $repr = "if " + $expr.repr + " {\n" + tabLines($statement.repr) + "\n}" + $ifContinuation.repr;
    }
  ;

ifContinuation returns [string repr]
  : { $repr = ""; }
  | ifStmt { $repr = " else " + $ifStmt.repr; }
  | statement { $repr = " else {\n" + tabLines($statement.repr) + "\n}"; }
  ;

expr returns [string repr, bool composite]
  : op=('&&'|'||'|'=='|'<'|'>'|'*'|'/'|'+'|'-') a=expr b=expr {
      $repr = fmtOp($op.text, fmtExpr($a.repr, $a.composite), fmtExpr($b.repr, $b.composite));
      $composite = true;
    }
  | '!' a=expr {
      $repr = "!" + fmtExpr($a.repr, $a.composite);
      $composite = false;
    }
  | '=' id=Ident b=expr {
      $repr = fmtOp("=", $id.text, fmtExpr($b.repr, $b.composite));
      $composite = true;
    }
  | val=(Integer|Bool|Ident) {
      $repr = $val.text;
      $composite = false;
    }
  ;

Bool : 'false' | 'true';
Integer : Digit+ ;
Ident : Letter (Letter | Digit | '_')*;
fragment Digit : [0-9] ;
fragment Letter : 'a'..'z' | 'A'..'Z' ;

Whitespace : [\p{White_Space}] -> skip;