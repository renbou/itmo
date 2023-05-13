#!/usr/bin/env python3
import numpy
import struct
import bitarray
from binascii import hexlify

# функция, которая строит коды шеннона-фано
# по сути бинарное дерево, т.к. рекурсивно строит для левого и правого
# принимает список пар символ-вероятность и префикс для всех
# символов в списке
def shannon_fano_tree(alph_probs, code = bitarray.bitarray()):
  if len(alph_probs) == 1:
    return dict([(alph_probs[0][1], code.to01())])
  left_part, right_part = 0, 0
  # когда у нас больше двух элементов, то построим массив
  # префиксных сумм вероятностей, и найдем, где стоит поделить,
  # чтобы сумма вероятностей с левой части была максимально близка
  # к сумме вероятностей с правой части
  if len(alph_probs) > 2:
    probabilities = numpy.array([i[0] for i in alph_probs])
    total_prob = numpy.sum(probabilities)
    prefix_sums = numpy.cumsum(probabilities)
    # бинарный поиск по отсортированному
    # массиву префиксных сумм
    split_point = numpy.searchsorted(prefix_sums, total_prob / 2)
    # если у нас элемент справа от середины на самом деле
    # ближе к ней, то сместим точку разделения
    if abs(prefix_sums[split_point] - total_prob / 2) < abs(prefix_sums[split_point - 1] - total_prob / 2):
      split_point += 1
    # разделим список на 2 части
    left_part, right_part = alph_probs[:split_point], alph_probs[split_point:]
  # иначе у нас только 2 элемента, присвоим левому
  # код заканчивающийся на 0, правому - на 1
  # и вернем как мн-во пар символ-код, которое
  # потом соединим с другими результатами
  else:
    return dict([(alph_probs[0][1], code + '0'), (alph_probs[1][1], code + '1')])
  return {**shannon_fano_tree(left_part, code + '0'),
        	**shannon_fano_tree(right_part, code + '1')}

# как в прошлый раз, прочитаем само сообщение
# и сделаем базовую статистику для него, посчитаем
# вероятности
data = numpy.array(list(open('message.bin', 'rb').read()), dtype=numpy.uint8)

alphabet, counts = numpy.unique(data, return_counts=True)
alphabet_size = alphabet.size
print(f'Number of unique elements in alphabet (the size of the alphabet): {alphabet_size}')
print(f'The alphabet itself: {list(alphabet)}\n')

ordered_probabilities = counts / data.size
probabilities = list(zip(ordered_probabilities, alphabet))
print(f'The probabilities of each letter: {probabilities}\n')

# теперь на созданном алфавите и вероятностях каждого символа
# построим коды фано
tree = shannon_fano_tree(sorted(probabilities))
print(f'Shannon-fano codes for this message: {tree}')

# закодируем наше сообщение полученными кодами
encoded = bitarray.bitarray()
for value in data:
  encoded.extend(tree[value])

# и выведем информацию о полученных результатах - длина 
# закодированного сообщения, средняя длина кодов, кол-во инф.
print(f'Length of message encoded with Shannon-Fano code = {len(encoded)}')
# считаем среднюю длину как мат. ожидание длин всех кодов
code_lengths = [len(tree[c]) for c in alphabet]
average_length = numpy.array(code_lengths)*ordered_probabilities
print(f'Average length of every code: {numpy.sum(average_length)}')
print(f'Message encoded with Shannon-Fano code: {encoded.to01()}')
print(f'Message encoded with Shannon-Fano code as hex: {hexlify(encoded.tobytes())}')
print(f'Information transmitted with message = {len(encoded)} bits')
