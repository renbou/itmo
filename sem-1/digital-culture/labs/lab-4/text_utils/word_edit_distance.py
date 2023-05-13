# Определение а также имплементация нахождения редакторского расстояния в заданном тексте
# Мы считаем следующие операции допустимыми и имеющими единичный вес (прибавляют 1 к расстоянию) при поиске редакторского расст.:
# - вставка одного символа в строку
# - удаление одного символа из строки
# - замена символа в строке на другой символ
# - разделение строки на 2 строчки, то есть добавление пробела посередине строки
# Так как первые 3 из этих операции определены обычным расстоянием Левенштейна, то в качестве основы для алгоритма будем
# использовать алгоритм для вычисления расстояния Левенштейна, а к нему доделаем нашу модификацию.

# Итеративная (с использованием техники динамического программирования) реализация алгоритма нахождения
# Редакторского расстояния с определенными выше операциями
# 
# Оригинальная реализация для нахождения расстояния Левенштейна
# Описана в Английской версии статьи на Wikipedia (Iterative with two matrix rows):
#   https://en.wikipedia.org/wiki/Levenshtein_distance#Computing_Levenshtein_distance
# Отличие этой версии итеративного алгоритма от обычного в том, что хранится информация только о последних двух строках
# матрицы с расстояниями Левенштейна между префиксами строк, уже высчитанными на данный момент.
# 
# Для того, чтобы также учитывать возможность деления строки на две части, мы будем делать всё то же самое,
# но дополнительно после прохода по всей строке, смотреть, какое редакторское расстояние для данного префикса первой строки
# до конца второй строки, то есть current_row[n]
def edit_distance(first, second):
    if not isinstance(first, str) or not isinstance(second, str):
        raise ValueError("Невозможно найти редакторское расстояние между двумя нестроковыми типами")

    m, n = len(first), len(second)

    # для первой строки матрицы, когда префикс первой строки пустой,
    # это просто кол-во символов которое нужно удалить из начала второй, то есть i
    v0 = [i for i in range(0, n+1)]
    # вторую не инициализируем
    v1 = [0 for i in range(0, n+1)]

    # минимальное расстояние, если поделим строку на две
    min_split_cost, min_split_prefix_length = n, n

    for i in range(0, m):
        # Кол-во операций чтобы префикс i+1 первой строки привести к пустому префиксу второй строки
        v1[0] = i + 1

        for j in range(0, n):
            # С помощью ДП считаем стоимости всех операций при данных индексах
            deletion_cost = v0[j + 1] + 1
            insertion_cost = v1[j] + 1
            if first[i] == second[j]:
                substitution_cost = v0[j]
            else:
                substitution_cost = v0[j] + 1

            v1[j + 1] = min(deletion_cost, insertion_cost, substitution_cost)
        
        # так как индекс i соотв. i-ому префиксу первой строки, то v1[n] это необходимое кол-во
        # изменений чтобы перейти от такого префикса ко второй строке
        if v1[n] + 1 < min_split_cost:
            min_split_cost = v1[n] + 1
            min_split_prefix_length = i + 1

        # Старая строка теперь меняется, т.к. мы прошли очередную строку
        v0, v1 = v1, v0
    
    # Ответ в итоге будет лежать тут
    return v0[n], {'split_edit_distance':min_split_cost, 'split_prefix_length': min_split_prefix_length}

# Функция, которая для конкретного слова считает минимальное редакторское расстояние по всему словарю,
# а также минимальное редакторское расстояние, если это слово нам разделить на 2 слова
def minimal_edit_distance(word, word_dict):
    if not isinstance(word, str):
        raise ValueError("Cannot find minimal edit distance for non-string word")
    if not isinstance(word_dict, dict):
        raise ValueError("Cannot find minimal edit distance without a valid dictionary of words")

    get_word_count = lambda x: 0 if x not in word_dict else word_dict[x]
    
    # минимальное редакторское расстояние и соотв. слово из словаря 
    min_edit_distance, min_edit_distance_word = len(word), ''
    # минимальное редакторское расстояние, если делить слово на 2 части, а также 2 слова которые получатся если поделить
    min_split_edit_distance, min_split_edit_distance_words = len(word) + 1, ('','')

    for fix_word in word_dict:
        cur_edit_distance, split_edit_distance_results = edit_distance(word, fix_word)
        # если расстояние улучшилось или улучшилась повторяемость слова
        if cur_edit_distance < min_edit_distance or \
                (cur_edit_distance == min_edit_distance and get_word_count(fix_word) > get_word_count(min_edit_distance_word)):
            min_edit_distance = cur_edit_distance
            min_edit_distance_word = fix_word

        # проверим деление, второе слово должно быть адекватным
        first_word = word[:split_edit_distance_results['split_prefix_length']+1]
        second_word = word[split_edit_distance_results['split_prefix_length']+1:]
        if (split_edit_distance_results['split_edit_distance'] < min_split_edit_distance or \
            (split_edit_distance_results['split_edit_distance'] == min_split_edit_distance and \
                        get_word_count(first_word) + get_word_count(second_word) > \
                            get_word_count(min_split_edit_distance_words[0]) + get_word_count(min_split_edit_distance_words[1])))\
                                 and second_word in word_dict:
            min_split_edit_distance = split_edit_distance_results['split_edit_distance']
            min_split_edit_distance_words = fix_word, second_word

    return {
            'simple_edit':
                {
                    'edit_distance': min_edit_distance,
                    'best_fit': min_edit_distance_word
                },
            'split': {
                    'edit_distance': min_split_edit_distance,
                    'best_fit': min_split_edit_distance_words
                }
           }