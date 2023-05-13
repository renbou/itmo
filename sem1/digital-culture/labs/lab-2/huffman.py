#!/usr/bin/env python3
import numpy
import struct
import bitarray
import heapq
from binascii import hexlify

# функция, которая на вход принимает мн-во пар символ-вероятность
# и строит по нему дерево Хаффмана, а точнее коды для символов
def huffman_tree(probabilities):
  # создаем из символов и вероятностей мин. кучу, сортированную
  # по сумме вероятности всех символов, находящихся в поддереве
  # данной вершины
  heap = [[prob, [char, bitarray.bitarray()]] for char, prob in probabilities.items()]
  heapq.heapify(heap)
  # строим дерево пока у нас не останется одна единственная вершина
  # которая будет корнем всего дерева
  while len(heap) > 1:
    # берем две вершины с самыми минимальными суммами
    # вероятностей символов в поддереве
    low = heapq.heappop(heap)
    # для каждого символа в левом поддереве добавляем к его
    # коду бит 0
    for code in low[1:]:
      code[1] = code[1] + '0'
    # а для правого бит 1
    high = heapq.heappop(heap)
    for code in high[1:]:
      code[1] = code[1] + '1'
    # и вставляем в кучу новую вершину, у которой вероятность
    # равна сумме этих двух, а символы это все символы из
    # левого и из правого поддерева 
    heapq.heappush(heap, [low[0]+high[0]] + low[1:] + high[1:])
  result = dict()
  # просто переводим кучу в вид мн-ва символ – код
  for pair in heap[0][1:]:
    pair[1].reverse()
    result[pair[0]] = pair[1]
  return result

data = numpy.array(list(open('message.bin', 'rb').read()), dtype=numpy.uint8)

alphabet, counts = numpy.unique(data, return_counts=True)
alphabet_size = alphabet.size
print(f'Number of unique elements in alphabet (the size of the alphabet): {alphabet_size}')
print(f'The alphabet itself: {list(alphabet)}\n')

ordered_probabilities = counts / data.size
probabilities = dict(zip(alphabet, ordered_probabilities))
print(f'The probabilities of each letter: {probabilities}\n')

# здесь строим дерево кодов Хаффмана, а потом с помощью него кодируем
# как и с помощью кодирования Шеннона-Фано
tree = huffman_tree(probabilities)
print(f'Huffman codes for this message: {tree}')

encoded = bitarray.bitarray()
for value in data:
  encoded.extend(tree[value])

print(f'Length of message encoded with Huffman code = {len(encoded)}')
code_lengths = [len(tree[c]) for c in alphabet]
average_length = numpy.array(code_lengths)*ordered_probabilities
print(f'Average length of every code: {numpy.sum(average_length)}')
print(f'Message encoded with Huffman code: {encoded.to01()}')
print(f'Message encoded with Huffman code as hex: {hexlify(encoded.tobytes())}')
print(f'Information transmitted with message = {len(encoded)} bits')
