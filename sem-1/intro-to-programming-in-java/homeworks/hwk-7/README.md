# Домашнее задание 7. Разметка.

## Описание

1. Разработайте набор классов для текстовой разметки.
2. Класс `Paragraph` может содержать произвольное число других элементов разметки и текстовых элементов.
3. Класс `Text` – текстовый элемент.
4. Классы разметки `Emphasis`, `Strong`, `Strikeout` – выделение, сильное выделение и зачеркивание. Элементы разметки могут содержать произвольное число других элементов разметки и текстовых элементов.
5. Все классы должны реализовывать метод `toMarkdown(`[StringBuilder](https://docs.oracle.com/en/java/javase/11/docs/api/java.base/java/lang/StringBuilder.html)`)`, который должен генерировать [Markdown](https://ru.wikipedia.org/wiki/Markdown)-разметку по следующим правилам:
   1. текстовые элементы выводятся как есть;
   2. выделенный текст окружается символами '\*';
   3. сильно выделенный текст окружается символами '\_\_';
   4. зачеркнутый текст окружается символами '~'.
6. Следующий код должен успешно компилироваться:
   ```java
   Paragraph paragraph = new Paragraph(List.of(
       new Strong(List.of(
           new Text("1"),
           new Strikeout(List.of(
               new Text("2"),
               new Emphasis(List.of(
                   new Text("3"),
                   new Text("4")
               )),
               new Text("5")
           )),
           new Text("6")
       ))
   ));
   ```
   Вызов `paragraph.toMarkdown(new StringBuilder())` должен заполнять переданный `StringBuilder` следующим содержимым:
   `__1~2*34*5~6__`
7. Разработанные классы должны находиться в пакете markup.

## Модификация

_BBCode_:

- Дополнительно реализуйте метод `toBBCode`, генерирующий [BBCode](https://en.wikipedia.org/wiki/BBCode)-разметку:
  - выделеный текст окружается тегом `i`;
  - сильно выделеный текст окружается тегом `b`;
  - зачеркнутый текст окружается тегом `s`.

_BBCodeList_:

- Сделайте модификацию _BBCode_
- Добавьте поддержку:
  - Нумерованных списков (класс `OrderedList`, тег `list=1`): последовательность элементов
  - Ненумерованных списков (класс `UnorderedList`, тег `list`): последовательность элементов
  - Элементов списка (класс `ListItem`, открывающий тег `*`): последовательность абзацев и списков
- Для новых классов поддержка Markdown не требуется