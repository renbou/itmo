# Домашнее задание 4. Подсчёт слов.

## Описание

1. Разработайте класс `WordStatInput`, подсчитывающий статистику встречаемости слов во входном файле.
2. Словом называется непрерывная последовательность букв, апострофов (') и дефисов (Unicode category [Punctuation, Dash](https://docs.oracle.com/en/java/javase/17/docs/api/java.base/java/lang/Character.html#DASH_PUNCTUATION)). Для подсчета статистики слова приводятся к нижнему регистру.
3. Выходной файл должен содержать все различные слова, встречающиеся во входном файле, в порядке их появления. Для каждого слова должна быть выведена одна строка, содержащая слово и число его вхождений во входном файле.
4. Имена входного и выходного файла задаются в качестве аргументов командной строки. Кодировка файлов: UTF-8.
5. Примеры работы программы:

   | Входной файл                                                          | Выходной файл                                                                                 |
   | --------------------------------------------------------------------- | --------------------------------------------------------------------------------------------- |
   | To be, or not to be, that is the question:                            | to 2<br>be 2<br>or 1<br>not 1<br>that 1<br>is 1<br>the 1<br>question 1                        |
   | Monday's child is fair of face.<br>Tuesday's child is full of grace.  | monday's 1<br>child 2<br>is 2<br>fair 1<br>of 2<br>face 1<br>tuesday's 1<br>full 1<br>grace 1 |
   | Шалтай-Болтай<br>Сидел на стене.<br>Шалтай-Болтай<br>Свалился во сне. | шалтай-болтай 2<br>сидел 1<br>на 1<br>стене 1<br>свалился 1<br>во 1<br>сне 1                  |

## Модификация

_CountShingles_:

- Выходной файл должен содержать все различные подстроки длины 3 слов встречающихся во входном файле, упорядоченые по возрастанию числа вхождений, а при равном числе вхождений – по порядку первого вхождения во входном файле.
- Класс должен иметь имя `WordStatCountShingles`
