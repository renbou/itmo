# Домашнее задание 3. Реверс.

## Описание

1. Разработайте класс `Reverse`, читающий числа из [стандартного ввода](https://docs.oracle.com/en/java/javase/17/docs/api/java.base/java/lang/System.html#in), и выводящий их на [стандартный вывод](https://docs.oracle.com/en/java/javase/17/docs/api/java.base/java/lang/System.html#out) в обратном порядке.
2. В каждой строке входа содержится некоторое количество целых чисел (возможно ноль). Числа разделены пробелами. Каждое число помещается в тип `int`.
3. Порядок строк в выходе должен быть обратным по сравнению с порядком строк во входе. Порядок чисел в каждой строке также должен быть обратным к порядку чисел во входе.
4. Вход содержит не более 10⁶ чисел и строк.
5. Для чтения чисел используйте класс [Scanner](https://docs.oracle.com/en/java/javase/17/docs/api/java.base/java/util/Scanner.html).
6. Примеры работы программы:

   | Ввод                                                                 | Вывод         |
   | -------------------------------------------------------------------- | ------------- |
   | 1 2<br>3                                                             | 3<br>2 1      |
   | 3<br>2 1                                                             | 1 2<br>3      |
   | 1<br><br>2 -3                                                        | -3 2<br><br>1 |
   | 1&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;2<br>3&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;4 | 4 3<br>2 1    |

## Модификация

_Avg_:

- Рассмотрим входные данные как (не полностью определенную) матрицу, вместо каждого числа выведите среднее из чисел в его столбце и строке
- Класс должен иметь имя `ReverseAvg`
