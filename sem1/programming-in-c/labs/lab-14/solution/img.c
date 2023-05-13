// ITMO 09.03.02 2020
// Artem Mikheev M3101
// Apache License

#include <stdlib.h>
#include <stdio.h>
#include "img.h"
#include "utils.h"
#include "errors.h"

/**
 * Helper function which gets a pixel value from a row of 1-bit pixels. This converts the value like specified in
 * read_bmp and MonochromeImage. The palette argument isn't used here.
 * @return 0 if pixel is white, 1 if black
 */
unsigned char get_pixel_1bit(unsigned char* palette, unsigned char *pixel_row, uint32_t i) {
	unsigned char byte = pixel_row[i / 8];
	unsigned char bit = (byte & (1 << (7 - (i & 7))));
	return !bit;
}

/**
 * Helper function which gets a pixel value from a row of 8-bit pixels. This converts the value like specified in
 * read_bmp and MonochromeImage. The palette argument isn't used here.
 * @return 0 if pixel is white, 1 if black
 */
unsigned char get_pixel_8bit(unsigned char* palette, unsigned char *pixel_row, uint32_t i) {
	unsigned char byte = pixel_row[i];
	byte /= 128;
	return !byte;
}

/**
 * Helper function which gets a pixel value from a row of palette indices, this is needed if the BMP image contains
 * a palette. The palette must be previously parsed from the file, but then this can be used just like the other
 * two functions without adding extra cases.
 * @return 0 if pixel is white, 1 if black
 */
unsigned char get_pixel_from_palette(unsigned char* palette, unsigned char *pixel_row, uint32_t i) {
	return palette[pixel_row[i]];
}

MonochromeImage *read_bmp(const char *filename) {
	if (filename == NULL) {
		PUT_ERROR("Unable to read image with invalid filename (NULL).");
		return NULL;
	}

	FILE *file = fopen(filename, "rb");
	if (file == NULL || fseek(file, 0, SEEK_END) != 0) {
		ERROR();
		return NULL;
	}
	int32_t file_length = ftell(file);
	if (file_length == -1 || fseek(file, 0, SEEK_SET) != 0) {
		ERROR();
		fclose(file);
		return NULL;
	}

	// the minimal size of the bmp file header and info header
	if (file_length < sizeof(BMP_FILE_HEADER) + sizeof(BITMAPINFOHEADER)) {
		PUT_ERROR("Unknown BMP file format - too small for any supported format.");
		fclose(file);
		return NULL;
	}

	BMP_FILE_HEADER file_header;
	fread(&file_header, sizeof(BMP_FILE_HEADER), 1, file);
	if (BAD_FILE(file, filename, "unable to read BMP file header")) {
		fclose(file);
		return NULL;
	}

	if (strncmp(file_header.identifier, "BM", 2) != 0) {
		PUT_ERROR(
			"Unsupported BMP file - magic header isn't 'BM'");
		fclose(file);
		return NULL;
	}

	BITMAPINFOHEADER info_header;
	fread(&info_header, sizeof(BITMAPINFOHEADER), 1, file);
	if (BAD_FILE(file, filename, "unable to read BMP info header")) {
		fclose(file);
		return NULL;
	}

	if (info_header.header_size != 40) {
		PUT_ERROR(
			"Unsupported BMP info header. We only support BITMAPINFOHEADER, 40 bytes in size.");
		fclose(file);
		return NULL;
	}

	if (info_header.width < 0) {
		PUT_ERROR(
			"The BMP width cannot be negative, only height can be negative.");
		fclose(file);
		return NULL;
	}

	if (info_header.num_color_planes != 1) {
		PUT_ERROR("Unsupported number of color planes encountered - only 1 color plane bmp images are supported.");
		fclose(file);
		return NULL;
	}

	if (info_header.bits_per_pixel != 1 && info_header.bits_per_pixel != 8) {
		PUT_ERROR(
			"Unsupported bits-per-pixel value encountered - we currently only support 1 and 8-bit monochrome bmp images.");
		fclose(file);
		return NULL;
	}

	if (info_header.compression_type != 0) {
		PUT_ERROR(
			"BMP info header specifies non-compressed bmp file but we don't support compression.");
		fclose(file);
		return NULL;
	}

	// if we have data in between the headers and the actual pixels
	int bmp_has_palette = (file_header.data_offset != sizeof(BMP_FILE_HEADER) + info_header.header_size);

	// the actual length of a row of pixels, counting the round-up for 1-bit images (to a byte)
	// and the round-up to the 4-byte padding
	uint32_t actual_row_width = ((((info_header.width *
		(bmp_has_palette ? 8 : info_header.bits_per_pixel) + 7) / 8) + 3) / 4) * 4;

	// if we don't have enough space to fit all pixels
	if (actual_row_width * abs(info_header.height) < file_length - file_header.data_offset)  {
		PUT_ERROR(
			"Invalid file size specified in the BMP file header - all pixels wouldn't fit in such a file.");
		fclose(file);
		return NULL;
	}

	unsigned char *palette = NULL;
	// if the image has a palette, lets parse the palette
	if (bmp_has_palette) {
		// the palette created is always of size 256, since that is the maximum we can have
		palette = xalloc(sizeof(unsigned char) * 256);

		uint32_t palette_size = file_header.data_offset - (sizeof(BMP_FILE_HEADER) + info_header.header_size);

		if (palette_size % 4 != 0) {
			PUT_ERROR(
				"BMP info header is 40 bytes in size, but the palette isn't made up of 32-bit colors.");
			fclose(file);
			return NULL;
		}

		uint32_t num_palette_colors = palette_size / 4;
		if (num_palette_colors > 256) {
			PUT_ERROR(
				"Palette has more than 256 colors, but a 1 or 8-bit BMP can have 256 at max.");
			fclose(file);
			return NULL;
		}

		// create a 4 byte array since that's the maximum
		uint8_t pixel[4];
		for (uint32_t i = 0; i < num_palette_colors; i++) {
			fread(pixel, sizeof(unsigned char), 4, file);
			if (BAD_FILE(file, filename, "unable to read the pixel data")) {
				xfree(palette);
				fclose(file);
				return NULL;
			}
			// we don't care about transparency since who knows how to interpret that, so we simply check if the first
			// three bytes are in the range [0, 127] meaning that the color is black and is stored as 1 for us
			// otherwise the color is white and stored as 0 for us
			palette[i] = ((!(pixel[0] / 128) + !(pixel[1] / 128) + !(pixel[2] / 128)) / 3);
		}
	}

	// we have read the headers completely so now we can read the pixels
	MonochromeImage *image = xalloc(sizeof(MonochromeImage) + info_header.width * abs(info_header.height));

	image->width = info_header.width;
	image->height = abs(info_header.height);

	unsigned char (*pixel_reader)(unsigned char*, unsigned char *, uint32_t);
	// if the bmp file is 1-bit depth
	if (bmp_has_palette) {
		pixel_reader = get_pixel_from_palette;
	} else if (info_header.bits_per_pixel == 1) {
		pixel_reader = get_pixel_1bit;
	} else {
		pixel_reader = get_pixel_8bit;
	}


	// a buffer for reading each row, this contains the padding and everything else already counted
	unsigned char *buffer = xalloc(actual_row_width);

	// we know that there is enough data in the file for the whole image, so we can simply read the pixel bytes
	// if the height was negative we can read from the top row, otherwise from the bottom row
	for (int32_t row = info_header.height < 0 ? 0 : image->height - 1;
		 	info_header.height < 0 ? row < image->height : row >= 0;
		 	info_header.height < 0 ? row++ : row--) {
		fread(buffer, sizeof(unsigned char), actual_row_width, file);
		if (BAD_FILE(file, filename, "unable to read the pixel data")) {
			xfree(image);
			xfree(buffer);
			xfree(palette);
			fclose(file);
			return NULL;
		}
		uint32_t row_offset = image->width * row;
		for (uint32_t col = 0; col < image->width; col++) {
			image->pixels[row_offset + col] = pixel_reader(palette, buffer, col);
		}
	}


	xfree(buffer);
	xfree(palette);
	fclose(file);
	return image;
}

int write_bmp(const char *filename, MonochromeImage *image) {
	if (filename == NULL) {
		PUT_ERROR(
			"Unable to write a BMP image without a filename (NULL).");
		return -1;
	}
	if (image == NULL) {
		PUT_ERROR(
			"Unable to write a BMP image from a NULL image argument.");
		return 1;
	}
	if (image->height == 0 || image->width == 0) {
		PUT_ERROR(
			"Unable to write BMP image which is 0x0 pixels.");
		return 1;
	}

	FILE *file = fopen(filename, "wb");
	if (file == NULL) {
		ERROR();
		return -1;
	}

	uint32_t padding = (4 - image->width % 4) % 4;
	uint32_t actual_width = image->width + padding;

	BMP_FILE_HEADER file_header = {
		.identifier = "BM",
		.file_size = sizeof(BMP_FILE_HEADER) + sizeof(BITMAPINFOHEADER) + 4*2 + image->height * actual_width,
		.reserved = {0, 0},
		.data_offset = sizeof(BMP_FILE_HEADER) + sizeof(BITMAPINFOHEADER) + 4*2
	};
	BITMAPINFOHEADER info_header = {
		.header_size = sizeof(BITMAPINFOHEADER),
		.width = image->width,
		.height = -image->height,
		.num_color_planes = 1,
		.bits_per_pixel = 8,
		.compression_type = 0,
		.raw_image_size = 0,
		.horizontal_resolution = 0,
		.vertical_resolution = 0,
		.color_palette_size = 2,
		.num_important_colors = 2
	};

	fwrite(&file_header, sizeof(BMP_FILE_HEADER), 1, file);
	if (BAD_FILE(file, filename, "unable to write BMP file header")) {
		fclose(file);
		return 1;
	}

	fwrite(&info_header, sizeof(BITMAPINFOHEADER), 1, file);
	if (BAD_FILE(file, filename, "unable to write BMP info header")) {
		fclose(file);
		return 1;
	}

	// the palette, first is white (since we store white as 0), second is black (we store black as 1)
	unsigned char palette[2][4] = {"\xff\xff\xff\xff", "\x00\x00\x00\xff"};
	fwrite(palette, sizeof(unsigned char), 2 * 4, file);
	if (BAD_FILE(file, filename, "unable to write the palette")) {
		fclose(file);
		return 1;
	}

	// extra padding bytes, 4 is the maximum
	unsigned char padding_bytes[4] = {0, 0, 0, 0};
	for (int32_t row = 0; row < image->height; row++) {
		uint32_t row_offset = image->width * row;
		fwrite(image->pixels + row_offset, sizeof(unsigned char), image->width, file);
		if (BAD_FILE(file, filename, "unable to write the pixel data")) {
			fclose(file);
			return 1;
		}
		fwrite(padding_bytes, sizeof(unsigned char), padding, file);
		if (BAD_FILE(file, filename, "unable to write the pixel data")) {
			fclose(file);
			return 1;
		}
	}

	fclose(file);
	return 0;
}
