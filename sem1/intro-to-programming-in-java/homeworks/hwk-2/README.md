# Домашнее задание 2. Сумма чисел.

## Описание

1. Разработайте класс `Sum`, который при запуске из командной строки будет складывать переданные в качестве аргументов целые числа и выводить их сумму на консоль.
2. Примеры запуска программы:
   ```bash
   java Sum 1 2 3
   Результат: 6
   ```
   ```bash
   java Sum 1 2 -3
   Результат: 0
   ```
   ```bash
   java Sum "1 2 3"
   Результат: 6
   ```
   ```bash
   java Sum "1 2" " 3"
   Результат: 6
   ```
   ```bash
   java Sum " "
   Результат: 0
   ```
   Аргументы могут содержать:
   - цифры;
   - знаки + и -;
   - произвольные [пробельные символы](<https://docs.oracle.com/en/java/javase/17/docs/api/java.base/java/lang/Character.html#isWhitespace(char)>).
3. При выполнении задания можно считать, что для представления входных данных и промежуточных результатов достаточен тип `int`.
4. Перед выполнением задания ознакомьтесь с документацией к классам [String](https://docs.oracle.com/en/java/javase/17/docs/api/java.base/java/lang/String.html) и [Integer](https://docs.oracle.com/en/java/javase/17/docs/api/java.base/java/lang/Integer.html).
5. Для отладочного вывода используйте [System.err](https://docs.oracle.com/en/java/javase/17/docs/api/java.base/java/lang/System.html#err), тогда он будет игнорироваться проверяющей программой.

## Модификация

_BigIntegerSpace_:

- Входные данные помещаются в тип [BigInteger](https://docs.oracle.com/en/java/javase/17/docs/api/java.base/java/math/BigInteger.html)
- Класс должен иметь имя `SumBigIntegerSpace`
- Числа разделяются [пробелами-разделителями](https://docs.oracle.com/en/java/javase/17/docs/api/java.base/java/lang/Character.html#SPACE_SEPARATOR)
