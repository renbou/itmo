// ITMO 09.03.02 2020
// Artem Mikheev M3101
// Apache License

#ifndef GOL_IMG_H
#define GOL_IMG_H

#include <stdint.h>

#pragma pack(push, 2)
/**
 * Struct defining the bitmap's file header. This is packed using a pragma so that we can parse it directly from a file
 * simply by reading sizeof(BMP_FILE_HEADER) bytes.
 */
typedef struct {
	// The identifier of this bitmap, we only support BM
	char identifier[2];
	uint32_t file_size;
	uint16_t reserved[2];
	uint32_t data_offset;
} BMP_FILE_HEADER;
#pragma pack(pop)

#pragma pack(push, 2)
/**
 * Struct defining the bitmap's information header. Currently this is the only one defined, since other ones are used rarely.
 * This is packed using a pragma so that we can parse it directly from a file simply by reading sizeof(BITMAPINFOHEADER) bytes.
 */
typedef struct {
	// The size of this header, should be 40
	uint32_t header_size;
	// The width of the image, should be positive
	int32_t width;
	// The height of the image, can be negative to represent different ordering of rows
	int32_t height;
	// Number of color planes, should be 1
	uint16_t num_color_planes;
	// Number of color planes, should be 1
	uint16_t bits_per_pixel;
	// The type of compression, we don't support it so should be 0
	uint32_t compression_type;
	// The size of the decompressed, decrypted data, for us this can be anything since we don't look at it
	uint32_t raw_image_size;
	uint32_t horizontal_resolution;
	uint32_t vertical_resolution;
	uint32_t color_palette_size;
	uint32_t num_important_colors;
} BITMAPINFOHEADER;
#pragma pack(pop)

/**
 * Struct representing a monochrome image. We will actually save this with 2 possible values only as a single-bit BMP/other
 * image file, but use a byte array for simpler access within the program.
 * !If the value in the pixel array isn't 0 - it will be written as black (pixel is turned on)
 * !and if the value *is* 0 then it will be written as white (pixel is turned off)
 */
typedef struct {
	uint32_t height;
	uint32_t width;
	unsigned char pixels[];
} MonochromeImage;

/**
 * Function which opens a BMP image and reads it. If it isn't actually monochrome (i.e. not a 1-bit or 8-bit bmp) or has a
 * palette, which we don't currently support, this will fail.
 * When reading a 1-bit image, this will convert 0's (which represent black in a bmp) to 1's (representing black in our
 * monochrome image), and convert 1's to 0's.
 * When reading an 8-bit image (no palette), this will convert all values in [0, 127] to 1's and [128, 255] to 0's.
 * @return a monochrome image read from the bmp file
 */
MonochromeImage *read_bmp(const char *filename);

/**
 * Function which writes a monochrome image to a file in the 1-bit bmp format. This will write all values in the
 * pixel array of the monochrome image which aren't 0 as a black, whilst writing 0 as a white.
 * @return 0 if succeeded, -1 if a file error occurred, 1 if the image isn't valid
 */
int write_bmp(const char *filename, MonochromeImage *image);

#endif //GOL_IMG_H
