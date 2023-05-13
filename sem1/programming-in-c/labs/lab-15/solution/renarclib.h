// custom archive format for ITMO programming labs
// Copyright (C) 2020, Artem Mikheev <c@renbou.ru>.
// Licensed under the Apache License, Version 2.0.

#ifndef RENARCLIB_H
#define RENARCLIB_H

#include <stdint.h>
#include <stdio.h>

#pragma pack(push, 1)
typedef struct {
	// "\xDE\xADRENARC"
	const char magic[8];
	// currently major version is 0
	uint16_t major_version;
	// curently minor version is 1
	uint16_t minor_version;
	// currently only 0 - none and 1 - huffman are supported
	uint32_t compression_mode;
	// number of files in the archive
	uint32_t num_files;
} renarc_header;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct {
	uint32_t name_length;
	uint32_t data_length;
	uint32_t extracted_length;
	// First contains name_length characters for the name of the file, then a null byte, and then the
	// actual file data, depending on the compression mode. The actual data is data_length bytes long, and
	// extracted_length long after decompression.
	unsigned char data[];
} renarc_file_entry;
#pragma pack(pop)

/**
 * Structure for use in an actual program, represents one file in a renarc
 */
typedef struct renarc_file_s {
	// store the files as a linked list
	const struct renarc_file_s *next;
	const char *name;
	uint32_t data_length;
	uint32_t extracted_length;
	unsigned char *data;
} renarc_file;

/**
 * Structure for use in an actual program, represents an archive as a linked list of files
 */
typedef struct renarc_s {
	uint32_t num_files;
	uint32_t compression_mode;
	renarc_file *first, *last;
} renarc;

/**
 * Function which allocates memory for and initializes an archive. WARNING - uses malloc
 * @param mode the compression mode, only 0 (none) and 1 (huffman coding) are supported currently
 */
renarc *create_renarc(int mode);

/**
 * Functions which completely destroys the archive, i.e. freeing all of the memory used by the files and the archive itself.
 * @return	0 on success,
 * 			-1 if archive argument is a null
 */
int destroy_renarc(renarc *archive);

/**
 * Function to "create" a file from memory and add it to the archive with the given filename. This stores the file
 * in a way which depends on the mode of the archive, i.e. "none" will simply save the data, "huffman" will try to
 * compress the data using huffman coding and will write the table first and then the compressed data.
 * @param archive the initialized archive to which to add the file
 * @param name the name of the file which should be stored in the archive
 * @param data_length how many bytes should be stored starting from data
 * @param data the actual data of the file
 * @return 0 on success,
 * 		   -1 if a null pointer has been passed as one of the arguments,
 * 		   -2 if for some reason we couldn't create a new file structure to add to the archive
 * 		   -3 if failed for some other reason
 */
int renarc_add_file(renarc *archive, const char *name, uint32_t data_length, const unsigned char *data);

/**
 * Extracts a file (which should be from this archive) using the archive's settings and places its data into the
 * location pointed to by the data argument. The data argument should contain enough memory to hold all of the extracted
 * data, so it should hold file->extracted_length bytes. MAKE SURE this is correct, cause otherwise you will get overflows.
 * @param archive the archive describing the settings we should use to extract the file
 * @param file the file structure, should be from the archive or there could be a mismatch of settings
 * @param data where to extract the file data
 * @return 0 on success,
 * 		   -1 if a null pointer has been passed as one of the arguments
 * 		   -2 if failed for some other reason
 */
int renarc_extract_file(renarc *archive, const renarc_file *file, unsigned char *data);

/**
 * Writes the archive to a file.
 * @return 0 on success,
 * 		   -1 if a null pointer has been passed as one of the arguments
 * 		   -2 if a file error occured
 */
int renarc_write(renarc *archive, FILE *file);

/**
 * Read an archive from a file. This uses malloc to create structures for the files in the archive, so beware.
 * @param archive should be an initialized archive, with null file list
 * @return 0 on success,
 * 		   -1 if a null pointer has been passed as one of the arguments
 * 		   -2 if for some reason the archive files couldn't be created
 * 		   -3 if the file isn't a supported renarc file
 */
int renarc_read(renarc *archive, FILE *file);

#endif //RENARCLIB_H
