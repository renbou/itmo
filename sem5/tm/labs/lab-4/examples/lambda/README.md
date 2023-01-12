# Python lambdas

## Generation
```bash
ll1gen -i ./lambda.ll1 -o ./parser.go -p main
```

## Usage
[draw.py](./draw.py) can be used to render a tree of the parsed expression. Here are some examples:

1. `lambda a, b1, c_: False and True or c_` - shows that different precedence operators are parsed correctly

2. `lambda: not(True) or ((False) and (True or False))` - shows parsing of nested expressions