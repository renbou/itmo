from .text_preprocessing import preprocess_text
# Расчёты по тексту, используемые до и после устранения ошибок в тексте
# - подсчёт общего кол-ва словоформ в тексте
# - подсчёт кол-ва уникальных словоформ
# - подсчёт кол-ва уникальных словоформ, присутствующих в переданном словаре
#   (на самом деле в мн-ве слов, определенных словарем)

# Функция, умеющая сама по себе подсчитать кол-во всех словоформ (слов) в тексте
# Т.е. если переданный ей текст еще не был обработан, то сначала передадим его на предобработку, и потом подсчитаем слова
def count_words(text):
    if isinstance(text, list):
        return len(text)
    if isinstance(text, str):
        return count_words(preprocess_text(text))
    raise ValueError(
        "Невозможно подсчитать кол-во словоформ в тексте, \
            переменная отображающая который не является строкой или списком слов")

# Функция, умеющая сама по себе подсчитать кол-во всех уникальных словоформ (слов) в тексте
# Т.е. если переданный ей текст еще не был обработан, то сначала передадим его на предобработку, и потом подсчитаем уникальные
def count_unique_words(text):
    if isinstance(text, list) or isinstance(text, set):
        return len(set(text))
    if isinstance(text, str):
        return count_unique_words(preprocess_text(text))
    raise ValueError(
        "Невозможно подсчитать кол-во уникальных словоформ в тексте, \
            переменная отображающая который не является строкой или списком слов")

# Функция, умеющая сама по себе подсчитать кол-во всех словоформ в тексте, имеющихся в словаре (мн-ве слов из словаря)
# Т.е. если переданный ей текст еще не был обработан, то сначала передадим его на предобработку, и потом снова вызовем эту функцию
def count_words_present_in_dictionary(text, word_dict):
    if isinstance(word_dict, dict):
        word_dict = set(word_dict.keys())
    if isinstance(text, list) or isinstance(text, set) and \
            isinstance(word_dict, list) or isinstance(word_dict, set):
        return len(
            set(text).intersection(set(word_dict)) # пересечение мн-ва слов в нашем тексте со словами в словаре
        )
    if isinstance(text, str):
        return count_words_present_in_dictionary(preprocess_text(text), word_dict)
    raise ValueError(
        "Невозможно подсчитать кол-во словоформ в тексте, \
            переменная отображающая который не является строкой или списком слов, присутствующих в словаре")

# Функция, умеющая сама по себе возвращать все словоформы в тексте, НЕ имеющиеся в словаре (мн-ве слов из словаря)
# Т.е. если переданный ей текст еще не был обработан, то сначала передадим его на предобработку, и потом снова вызовем эту функцию
# Можно было реализовать это используя результат прошлой функции, но логичнее определить это именно через разность мн-в
def words_not_present_in_dictionary(text, word_dict):
    if isinstance(word_dict, dict):
        word_dict = set(word_dict.keys())
    if isinstance(text, list) or isinstance(text, set) and \
            isinstance(word_dict, list) or isinstance(word_dict, set):
        return set(text).difference(set(word_dict)) # разность мн-ва слов в нашем тексте со словами в словаре
    if isinstance(text, str):
        return count_words_present_in_dictionary(preprocess_text(text), word_dict)
    raise ValueError(
        "Невозможно подсчитать кол-во словоформ в тексте, \
            переменная отображающая который не является строкой или списком слов, присутствующих в словаре")

# Функция, которая по определенным функциям для создания статистики по тексту и словарю,
# выполняет вычисление каждой из этих функций, и выводит всю эту статистику в понятном виде
def text_statistics(text, word_dict):
    print(f'Количество словоформ в тексте: {count_words(text)}')
    print(f'Количество уникальных словоформ в тексте: {count_unique_words(text)}')
    print(f'Количество уникальных словоформ в тексте, присутствующие в словаре: {count_words_present_in_dictionary(text, word_dict)}')
