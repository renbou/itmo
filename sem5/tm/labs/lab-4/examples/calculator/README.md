# Calculator

## Generation
```bash
ll1gen -i ./calculator.ll1 -o ./parser.go -p main
```

## Usage
```bash
❯ go run calculator
> 1 + 2 * -(3 / 2) 
-2.000000

> 1 / 2 + 3 * 2 / 4 
2.000000

>
```