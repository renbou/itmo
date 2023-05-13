// custom archive format for ITMO programming labs
// Copyright (C) 2020, Artem Mikheev <c@renbou.ru>.
// Licensed under the Apache License, Version 2.0.

#include <stdlib.h>
#include "renarclib.h"
#include "errors.h"

renarc *create_renarc(int mode) {
	if (mode != 0 && mode != 1) {
		return NULL;
	}
	renarc *new_archive = (renarc *)malloc(sizeof(renarc));
	if (new_archive == NULL) {
		return NULL;
	}
	new_archive->first = new_archive->last = NULL;
	new_archive->num_files = 0;
	new_archive->compression_mode = mode;
	return new_archive;
}

int destroy_renarc(renarc *archive) {
	if (archive == NULL) {
		return -1;
	}
	const renarc_file *current = archive->first;
	while (current != NULL) {
		const renarc_file *next = current->next;
		free(current);
		current = next;
	}
	free(archive);
	return 0;
}

int renarc_add_file(renarc *archive, const char *name, uint32_t data_length, const unsigned char *data) {
	if (archive == NULL || name == NULL || data == NULL) {
		return -1;
	}
	renarc_file *new_file = (renarc_file *)malloc(sizeof(renarc_file));

	with(new_file != NULL, free(new_file), {
		new_file->name = (const char *)malloc(sizeof(char) * strlen(name));
		with(new_file->name != NULL, free(new_file->name), {
			strcpy(new_file->name, name);
			new_file->next = NULL;
			new_file->data_length = data_length;

			if (archive->compression_mode == 0) {
				new_file->extracted_length = data_length;
				new_file->data = (unsigned char *)malloc(sizeof(unsigned char) * data_length);
				if (new_file->data == NULL) {
					return -2;
				}
				memcpy(new_file->data, data, data_length);
			} else if (archive->compression_mode == 1) {
				// no huffman done yet
			} else {
				break;
			}

			if (archive->first == NULL) {
				archive->first = new_file;
			} else {
				archive->last->next = new_file;
			}
			archive->last = new_file;
			archive->num_files++;
			return 0;
		});
	});
	return -2;
}

int renarc_extract_file(renarc *archive, const renarc_file *file, unsigned char *data) {
	if (archive == NULL || file == NULL || data == NULL) {
		return -1;
	}

	if (archive->compression_mode == 0) {
		memcpy(data, file->data, file->data_length);
	} else if (archive->compression_mode == 1) {
		// no huffman done yet
	} else {
		return -2;
	}
	return 0;
}

int renarc_write(renarc *archive, FILE *file) {
	if (archive == NULL || file == NULL) {
		return -1;
	}

	do {
		renarc_header header = {
			.magic = "\xDE\xADRENARC",
			.major_version = 0,
			.minor_version = 1,
			.compression_mode = archive->compression_mode,
			.num_files = archive->num_files
		};

		if (fwrite(&header, sizeof(renarc_header), 1, file) != 1)
			break;

		// save the offset table and continue writing files
		int offset_table_start;
		if ((offset_table_start = ftell(file)) == -1)
			break;

		// make the offset table the needed size
		uint32_t buffer_offset = -1;
		for (uint32_t i = 0; i < archive->num_files; i++) {
			if (fwrite(&buffer_offset, sizeof(uint32_t), 1, file) != 1)
				goto outer_fail;
		}

		const renarc_file *current_file = archive->first;
		uint32_t i = 0;
		while (current_file != NULL) {
			uint32_t current_offset;
			if ((current_offset = ftell(file)) == -1)
				goto outer_fail;

			// write the file offset
			if (fseek(file, offset_table_start + i * sizeof(uint32_t), SEEK_SET) != 0)
				goto outer_fail;
			if (fwrite(&current_offset, sizeof(uint32_t), 1, file) != 1)
				goto outer_fail;
			if (fseek(file, current_offset, SEEK_SET) != 0)
				goto outer_fail;

			// write file info
			uint32_t name_length = strlen(current_file->name);
			if (fwrite(&name_length, sizeof(uint32_t), 1, file) != 1)
				goto outer_fail;
			if (fwrite(&current_file->data_length, sizeof(uint32_t), 1, file) != 1)
				goto outer_fail;
			if (fwrite(&current_file->extracted_length, sizeof(uint32_t), 1, file) != 1)
				goto outer_fail;

			if (fwrite(current_file->name, sizeof(char), name_length, file) != name_length)
				goto outer_fail;

			// now we simply write the data, since we already store the data in a compressed form
			if (fwrite(current_file->data, sizeof(unsigned char), current_file->data_length, file)
					!= current_file->data_length)
				goto outer_fail;

			i++;
			current_file = current_file->next;
		}

		outer_fail:
		break;
	} while (0);

	if (ferror(file))
		return -2;
	return 0;
}

/**
 * Helper function to read one file from an archive, starting at the current position in the archive file
 * @return 0 on success,
 * 		   -1 if failed to allocate memory for file data
 * 		   -2 if failed to read
 */
int internal_read_file(renarc *archive, FILE *archive_file, renarc_file *file) {
	do {
		renarc_file_entry file_entry;
		if (fread(&file_entry, sizeof(file_entry), 1, archive_file) != 1)
			break;

		file->name = (const char *)malloc(sizeof(char) * file_entry.name_length);
		with(file->name, free(file->name), {
			if (fread(file->name, sizeof(char), file_entry.name_length, archive_file) != file_entry.name_length)
				break;
			file->data_length = file_entry.data_length;
			file->extracted_length = file_entry.extracted_length;
			file->data = (unsigned char *)malloc(file_entry.data_length);
			file->next = NULL;
			with(file->data, free(file->data), {
				if (fread(file->data, sizeof(unsigned char), file_entry.data_length, archive_file) != file_entry.data_length)
					break;

				return 0;
			});
		});
	} while (0);

	if (ferror(archive_file)) {
		return -2;
	}
	return -1;
}

int renarc_read(renarc *archive, FILE *file) {
	if (archive == NULL || file == NULL) {
		return -1;
	}

	do {
		renarc_header header;
		// read the header and make sure that we support it
		if (fread(&header, sizeof(renarc_header), 1, file) != 1)
			break;
		if (strcmp(header.magic, "\xDE\xADRENARC") != 0) {
			return -3;
		}
		if (header.major_version != 0 || header.minor_version != 1) {
			return -3;
		}
		if (header.compression_mode != 0 && header.compression_mode != 1) {
			return -3;
		}
		archive->compression_mode = header.compression_mode;
		archive->num_files = header.num_files;

		uint32_t file_table_offset;
		if ((file_table_offset = ftell(file)) == -1) {
			return -3;
		}

		for (uint32_t i = 0; i < archive->num_files; i++) {
			// get the current file offset and go there
			uint32_t current_file_offset;
			if (fseek(file, file_table_offset + i * sizeof(uint32_t), SEEK_SET) != 0) {
				return -3;
			}
			if (fread(&current_file_offset, sizeof(uint32_t), 1, file) != 1) {
				return -3;
			}
			if (fseek(file, current_file_offset, SEEK_SET) != 0) {
				return -3;
			}

			// then allocate memory for a new file structure and read it from the current position in the file
			renarc_file *new_file = (renarc_file *)malloc(sizeof(renarc_file));
			if (new_file == NULL) {
				return -2;
			}
			int result = internal_read_file(archive, file, new_file);
			if (result != 0) {
				if (result == -2) {
					return -3;
				}
				return -2;
			}

			// add the file to the archive linked list
			if (archive->first == NULL) {
				archive->first = new_file;
			} else {
				archive->last->next = new_file;
			}
			archive->last = new_file;
		}

	} while(0);

	return 0;
}
