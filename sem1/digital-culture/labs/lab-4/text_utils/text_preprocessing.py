import re
# Предобработка текста по заданным правилам, а именно:
# 1. Разделение текста на слова, т.е. в результате предобработки выдается список слов
# 2. Удаление из текста знаков препинания, а именно !?,;.:()—«»
# 3. Привод всех строк к нижнему регистру 

# список определенных знаков препинания
PUNCTUATION = '!?,;.:()—«»'
# заранее подготовленное регулярное выражение, которое будет
# использоваться для быстрого нахождения всех знаков препинания в тексте
PUNCTUATION_MATCHER = re.compile('[' + PUNCTUATION + ']')

def preprocess_text(text):
    if not isinstance(text, str):
        raise ValueError("Невозможно провести предобработку на нестроковом типе (не текст)")

    return convert_to_lowercase(
        remove_punctuation(
            text
        )
    ).split() # разделим возвращаемый текст на слова, после того как сделаем все обработки над строковым представлением текста

# Функция, убирающая все определенные нами знаки препинания. Для того чтобы не вызывать string.replace для каждого знака
# препинания, мы будем использовать модуль для регулярных выражений re, функция sub которого заменит все найденные знаки преп.
def remove_punctuation(text):
    return PUNCTUATION_MATCHER.sub(repl='', string=text)

# Функция которая переведёт все символы текста в нижний регистр. Пока что просто использует встроенный метод .lower, но вероятно
# в будущем захочится использовать какую-нибудь ускоренную или просто модифицированную версию.
def convert_to_lowercase(text):
    return text.lower()