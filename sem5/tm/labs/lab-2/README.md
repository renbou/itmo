# Лабораторная работа No2. Ручное построение нисходящих синтаксических анализаторов

## Вариант 8. Описание лямбда функции в Python
Описание лямбда функции в Python. Описание начинается ключевым словом “lambda”, далее идет множество аргументов через запятую, двоеточие, выражение. Используйте логические и/или арифметические операции.  

Используйте один терминал для всех имен переменных. Используйте один терминал для ключевых слов lambda и т.п. (не несколько ‘l’, ‘a’, ‘m’ и т. д.).  

Пример: `lambda n : n + 2`

## Грамматика

Зададим КС-грамматику для лямбда-функций с логическими операций внутри выражений.  

```
LambdaDecl -> lambda Args : Expr

// Либо 0 аргументов, либо 1 аргумент и дальше 0 или более аргументов
Args    -> eps
Args    -> ident OptArgs
OptArgs -> eps
OptArgs -> , ident OptArgs

// Булевы выражения с поддержкой or/and/not
Expr   -> Expr or Term
Expr   -> Term
Term   -> Term and Factor
Term   -> Factor
Factor -> false
Factor -> true
Factor -> ident
// not можно повторять один или более раз
Factor -> not Factor
Factor -> ( Expr )
```

### Нетерминалы

| Нетерминал | Значение |
| --- | --- |
| LambdaDecl | Полное объявление лямбда-функции |
| Args | 0 или более аргументов |
| OptArgs | Продолжение 1 или более аргументов |
| Expr | Выражение в лямбда-функции |
| Term | "Слагаемое" в выражении |
| Factor | "Множитель" в выражении |

### Преобразование

Устраним левую рекурсию и правое ветвление (есть неявно в Factor, т.к. not и Ident оба могут начинаться с n) в грамматике.

```
LambdaDecl -> lambda Args : Expr

Args    -> ident OptArgs
Args    -> eps
OptArgs -> , ident OptArgs
OptArgs -> eps

Expr   -> Term ExprM
ExprM  -> or Term ExprM
ExprM  -> eps
Term   -> Factor TermM
TermM  -> and Factor TermM
TermM  -> eps
Factor -> false
Factor -> true
Factor -> ident
Factor -> not Factor
Factor -> ( Expr )
```

### FIRST, FOLLOW

| Нетерминал | FIRST | FOLLOW |
| --- | --- | --- |
| LambdaDecl | lambda | $ |
| Args | ident, eps | : |
| OptArgs | **','**, eps | : |
| Expr | ident, false, true, not, ( | ), $ |
| ExprM | or, eps | ), $ |
| Term | ident, false, true, not, ( | or, ), $ |
| TermM | and, eps | or, ), $ |
| Factor | ident, false, true, not, ( | or, and, ), $ |