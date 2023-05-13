# from lab4.text_processing.text_utils.text_preprocessing import *
# from lab4.text_processing.text_utils.word_statistics import *
# from lab4.text_processing.text_utils.word_edit_distance import *
from os import error
from text_utils.text_preprocessing import *
from text_utils.word_edit_distance import *
from text_utils.word_statistics import *
import sys

def read_text_file(filename):
    try:
        with open(filename, 'r') as file:
            return file.read()
    except FileNotFoundError as error:
        raise FileNotFoundError(f"Невозможно продолжить исполнения без текстового файла: {error}")

def write_text_file(filename, data):
    try:
        with open(filename, 'w') as file:
            file.write(data)
    except PermissionError as error:
        raise PermissionError(f"Невозможно записать результат в вайл: {error}")

def read_dictionary_file(filename):
    try:
        with open(filename, 'r') as file:
            return dict([ (i[0], int(i[1])) for i in
                map(str.split,
                    file.read().splitlines()
                )
            ])
    except ValueError as error:
        raise ValueError(f"Файл со словарем составлен в неправильном формате: {error}")
    except FileNotFoundError as error:
        raise FileNotFoundError(f"Невозможно продолжить исполнения без файла со словарем: {error}")

def main():
    if len(sys.argv) < 3:
        raise RuntimeError("Невозможно продолжить без необходимых файлов (словарь и текст)")
    word_dict = read_dictionary_file(sys.argv[1])
    text = read_text_file(sys.argv[2])

    text_words = preprocess_text(text)

    print('[2] Первичные расчёты:')
    text_statistics(text_words, word_dict)


    print ('[3] Кол-во словоформ, не присутствующих в словаре:')
    bad_words = words_not_present_in_dictionary(text_words, word_dict)
    print(len(bad_words))
    bad_word_replacement_dict = dict()
    for word in bad_words:
        bad_word_replacement_dict[word] = minimal_edit_distance(word, word_dict)
    
    # чиним ошибки в тексте
    new_text_words = []
    word_fixes = {1:[], 2:[], 'lots':[]}

    get_word_count = lambda x: 0 if x not in word_dict else word_dict[x]

    for word in text_words:
        if word in bad_words:
            simple_edit, split_edit = bad_word_replacement_dict[word]['simple_edit'], bad_word_replacement_dict[word]['split']
            edit, words = 3, ()
            if simple_edit['edit_distance'] < split_edit['edit_distance']:
                edit = simple_edit['edit_distance']
                words = (simple_edit['best_fit'],)
            elif simple_edit['edit_distance'] == split_edit['edit_distance']:
                if word_dict[simple_edit['best_fit']] >= word_dict[split_edit['best_fit'][0]] + word_dict[split_edit['best_fit'][1]]:
                    edit = split_edit['edit_distance']
                    words = (simple_edit['best_fit'],)
                else:
                    edit = simple_edit['edit_distance']
                    words = split_edit['best_fit']
            else:
                edit = simple_edit['edit_distance']
                words = split_edit['best_fit']
            
            if edit < 3:
                word_fixes[edit].append((word, words, edit))
                new_text_words.append(' '.join(words))
            else:
                word_fixes['lots'].append(word)
        else:
            new_text_words.append(word)
    
    print('[4] Расчёты после исправления ошибок:')
    text_statistics(new_text_words, word_dict)

    print('[5] Потенциальные ошибки:')
    for word_stats in word_fixes[1]+word_fixes[2]:
        fixes = ' '.join(word_stats[1])
        print(f'{word_stats[0]} - {fixes} - {word_stats[2]}')
    
    for word in word_fixes['lots']:
        print(f'{word} - не найдено!')

    new_text = ' '.join(new_text_words)
    write_text_file(sys.argv[2] + '_fix.txt', new_text)

if __name__ == '__main__':
    main()