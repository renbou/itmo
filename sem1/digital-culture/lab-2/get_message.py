#!/usr/bin/env python3
import numpy
from PIL import Image
from binascii import hexlify

image = Image.open('cat.png')
image_data = numpy.array(image)

assert image.size == (128, 128), "Image should be of size 128x128 with only one gray 8-bit channel"

center_row = image_data[63]
print(f'Central row values: {list(center_row)}')
print(f'Central row bytes: {hexlify(bytes(center_row))}\n')

quantized_row = numpy.asarray(numpy.round(center_row / 20) * 20, dtype=numpy.uint8)
print(f'Quantized row values: {list(quantized_row)}')
print(f'Quantized row bytes: {hexlify(bytes(quantized_row))}')

open('message.bin', 'wb').write(bytes(quantized_row))