#!/usr/bin/env python3
import numpy
import struct
import bitarray
from binascii import hexlify

data = numpy.array(list(open('message.bin', 'rb').read()), dtype=numpy.uint8)

alphabet, counts = numpy.unique(data, return_counts=True)
alphabet_size = alphabet.size
print(f'Number of unique elements in alphabet (the size of the alphabet): {alphabet_size}')
print(f'The alphabet itself: {list(alphabet)}\n')

ordered_probabilities = counts / data.size
probabilities = dict(zip(alphabet, ordered_probabilities))
print(f'The probabilities of each letter: {probabilities}\n')

entropy = -numpy.sum(ordered_probabilities * numpy.log2(ordered_probabilities))
print(f'Entropy of the alphabet = {entropy}\n')

binary_code_length = int(numpy.ceil(numpy.log2(alphabet_size)))

ordered_codes = [i for i in range(alphabet_size)]
binary_codes = dict(zip(alphabet, ordered_codes))
print(f'Minimal length of binary code = {binary_code_length}')
print(f'Equal-length binary codes for the alphabet: {binary_codes}\n')

binary_code_file = open('binary_alphabet.alph', 'wb')
binary_code_file.write(struct.pack('I', binary_code_length))
for letter, code in binary_codes.items():
    binary_code_file.write(struct.pack('BB', letter, code))
binary_code_file.close()

print(f'Printed binary code alphabet to binary_alphabet.alph')
print(f'Supposed length of message encoded with binary code = {data.size * binary_code_length}\n')

encoded = bitarray.bitarray()
for value in data:
    encoded.extend(bin(binary_codes[value])[2:].rjust(binary_code_length, '0'))

print(f'Length of message encoded with equal-length binary code = {len(encoded)}')
print(f'Length of every letter: {len(encoded) // len(data)}')
print(f'Message encoded with equal-length binary code: {encoded.to01()}')
print(f'Message encoded with equal-length binary code as hex: {hexlify(encoded.tobytes())}')
print(f'Information transmitted with message (length * bits) = {len(data) * binary_code_length} bits')