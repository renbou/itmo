// ITMO 09.03.02 2020
// Artem Mikheev M3101
// Apache License

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "argument_parser.h"
#include "errors.h"
#include "renarclib.h"

const void *get_entry_value(const map_entry_t *entry) {
	if (entry == NULL) {
		return 0;
	}
	return entry->value;
}

/**
 * Function for parsing JSON-like arrays of strings. The format is like ["string1","string2"] (so without spaces).
 * Escapes for " (\") are supported. This doesn't do any null pointer checking since we expect to use it with argv.
 * The strange function signature is due to the usage - we use this as a type parser for our argument parser.
 * @return 0 on success, -1 on failure (invalid format)
 */
int parse_array(const char *str, void **result) {
	if (str[0] != '[')
		return -1;
	unsigned int length = strlen(str);
	if (str[length - 1] != ']')
		return -1;

	const char **array = (const char **)malloc(sizeof(const char *) * 4);
	uint32_t size = 0, capacity = 4;

	with(array != NULL, free(array), {
		const char *position = str + 1;
		while (position != str + length) {
			if (*position != '"')
				goto fail;
			const char *end = strchr(position + 1, '"');
			if (end == NULL) {
				goto fail;
			}
			while (*(end - 1) == '\\') {
				end = strchr(end + 1, '"');
				if (end == NULL) {
					goto fail;
				}
			}
			unsigned int current_length = (end - 1) - (position + 1) + 1;

			char *copy = (char *)malloc(sizeof(char) * current_length + 1);
			if (copy == NULL)
				goto fail;
			strncpy(copy, position + 1, current_length);

			if (size + 1 == capacity) {
				const char **new_array = (const char **)malloc(sizeof(const char *) * capacity * 2);
				if (new_array == NULL)
					goto fail;
				for (uint32_t i = 0; i < capacity; i++) {
					new_array[i] = array[i];
					new_array[i + capacity] = NULL;
				}
				capacity *= 2;
				free(array);
				array = new_array;
			}
			array[size++] = copy;

			if (*(end + 1) != ',' && (end + 1) != str + length - 1)
				goto fail;
			position = end + 2;
		}
		array[size] = NULL;
		*result = array;
		return 0;

		fail: break;
	});
	return -1;
}
/**
 * Parses the supported modes. This could be done better, and return an enum, but currently it should do anyways.
 * @return 0 on success, -1 on failure
 */
int compression_mode(const char *str, void **result) {
	int length = strlen(str);
	if (length > 1)
		return -1;
	switch (str[0]) {
		case 'n':
			*result = (void *)0;
			break;
		case 'h':
			*result = (void *)1;
			break;
		default:
			return -1;
	}
	return 0;
}

/**
 * Compress the files with the given mode and store them as an archive
 * @return 0 on success,
 * 		   -1 if failed to read all of the files
 * 		   -2 if failed to write output
 * 		   -3 on other errors
 */
int compress(const char **files, const char *output, int compression_mode) {
	renarc *archive = create_renarc(compression_mode);
	int return_code = 0;
	with(archive != NULL, destroy_renarc(archive), {
		for (uint32_t i = 0; files[i] != NULL; i++) {
			FILE *current_file = fopen(files[i], "rb");
			with(current_file != NULL, fclose(current_file), {
				if (fseek(current_file, 0, SEEK_END) != 0)
					break;
				int32_t file_length = ftell(current_file);
				if (file_length == -1 || fseek(current_file, 0, SEEK_SET) != 0)
					break;

				unsigned char *data = (unsigned char *)malloc(file_length);
				if (data == NULL) {
					return_code = -3;
					goto outer_fail;
				}
				with(data != NULL, free(data), {
					if (fread(data, sizeof(unsigned char), file_length, current_file) != file_length)
						break;
					if (renarc_add_file(archive, files[i], file_length, data) != 0) {
                        return_code = -3;
						break;
					}
				});

			});
			if (current_file != NULL && ferror(current_file)) {
				return_code = -2;
			}
		}

		FILE *save_file = fopen(output, "wb");
		if (save_file == NULL)
			return_code = -2;
		if (renarc_write(archive, save_file) != 0)
			return_code = -2;
		fclose(save_file);

		outer_fail:
		break;
	});
	return return_code;
}

/**
 * Decompress the archive file and store the files in the output directory
 * @return 0 on success,
 * 		   -1 if failed to read input file
 * 		   -2 if failed to write output
 * 		   -3 on other errors
 */
int decompress(const char *input, const char *output) {
	renarc *archive = create_renarc(0);
	int return_code = 0;
	with(archive != NULL, destroy_renarc(archive), {
		FILE *input_file = fopen(input, "rb");
		with(input_file != NULL, fclose(input_file), {
			if (renarc_read(archive, input_file) != 0)
				return_code = -1;

			uint32_t directory_length = strlen(output);
			const renarc_file *current_file = archive->first;
			while (current_file != NULL) {
				uint32_t current_name_length = strlen(current_file->name);
				char *current_filepath = (char *)malloc(directory_length + 1 + current_name_length + 2);
				if (current_filepath == NULL) {
					return_code = -3;
					break;
				}
				sprintf(current_filepath, "%s/%s", output, current_file->name);

				unsigned char *file_data = (unsigned char *)malloc(current_file->extracted_length);
				if (file_data == NULL) {
					return_code = -3;
					break;
				}
				if (renarc_extract_file(archive, current_file, file_data) != 0) {
					return_code = -3;
					break;
				}

				FILE *output_file = fopen(current_filepath, "wb");
				if (output_file == NULL) {
					return_code = -2;
					free(file_data);
					break;
				}
				if (fwrite(file_data, sizeof(unsigned char), current_file->extracted_length, output_file)
					!= current_file->extracted_length) {
					return_code = -2;
				}
				free(file_data);
				fclose(output_file);
				if (ferror(output_file))
					break;

				current_file = current_file->next;
			}
		});
	});
	return return_code;
}

int main(int argc, const char *argv[]) {
	// huge chunk of parsing all the arguments
	int result;
	const argument_parser_t *argument_parser = create_parser("renarc",
		"Simple archive utility with options for huffman-based compression.",
		"By @renbou", 1);
	if (!argument_parser) {
		PUT_ERROR("Unable to create argument parser!");
		return -1;
	}

	// first we parse compress/decompress flags in order to understand what the user wants,
	// and then add more flags depending on the choice
	result = parser_add_argument(argument_parser, "compress", BOOLEAN_ARGUMENT,
		"compression mode, meaning create an archive from specified files", 0, NULL, NULL);
	if (result == -5) {
		PUT_ERROR("Unable to add arguments to parser!");
		return -1;
	}
	result = parser_add_argument(argument_parser, "decompress", BOOLEAN_ARGUMENT,
						"decompression mode, meaning extract files from an archive", 0, NULL, NULL);
	if (result == -5) {
		PUT_ERROR("Unable to add arguments to parser!");
		return -1;
	}

	result = parser_parse_arguments(argument_parser, argc, argv);
	if (result == -5) {
		PUT_ERROR("Unable to parse arguments!");
		return -1;
	}

	int compression = (int)get_entry_value(
		argument_parser->parsed_arguments->get(argument_parser->parsed_arguments, "compress"));
	int decompression = (int)get_entry_value(
		argument_parser->parsed_arguments->get(argument_parser->parsed_arguments, "decompress"));
	if (!compression && !decompression) {
		printf("Compression or decompression mode should be specified.\n");
		parser_print_help(argument_parser);
		return -1;
	}
	if (compression && decompression) {
		printf("Only one of \"compress\" and \"decompress\" should be specified.\n");
		parser_print_help(argument_parser);
		return -1;
	}

	result = parser_add_argument(argument_parser, "-o", VALUE_ARGUMENT,
								 "archive path for compression mode, directory path for decompression mode",
								 1, "str", NULL);
	if (result == -5) {
		PUT_ERROR("Unable to add arguments to parser!");
		return -1;
	}

	if (compression) {
		result = parser_add_argument(argument_parser, "-f", VALUE_ARGUMENT,
									 "only needed for compression mode, specify the files "
									 "to compress in JSON-like format [\"path1\",\"path2\"]",
									 1, "list[str]", parse_array);
		if (result == -5) {
			PUT_ERROR("Unable to add arguments to parser!");
			return -1;
		}

		result = parser_add_argument(argument_parser, "-m", VALUE_ARGUMENT,
									 "the mode of compression, possible values: n - none (by default), h - huffman coding",
									 0, "int", compression_mode);
		if (result == -5) {
			PUT_ERROR("Unable to add arguments to parser!");
			return -1;
		}
	} else {
		result = parser_add_argument(argument_parser, "-i", VALUE_ARGUMENT,
									 "path of the archive to decompress",
									 1, "str", NULL);
		if (result == -5) {
			PUT_ERROR("Unable to add arguments to parser!");
			return -1;
		}
	}

	result = parser_parse_arguments(argument_parser, argc, argv);
	if (result == -3 || result == -4) {
		parser_print_help(argument_parser);
		return -1;
	}
	if (result == -5) {
		PUT_ERROR("Unable to parse arguments!");
		return -1;
	}

	const char *output = (const char *)get_entry_value(
		argument_parser->parsed_arguments->get(argument_parser->parsed_arguments, "-o"));

	// finally either compress or decompress depending on the choice

	if (compression) {
		int compression_mode = 0;
		const map_entry_t *tmp_entry = argument_parser->parsed_arguments->get(argument_parser->parsed_arguments, "-m");
		if (tmp_entry) {
			compression_mode = (int)get_entry_value(tmp_entry);
		}
		const char **files = (const char **)get_entry_value(
			argument_parser->parsed_arguments->get(argument_parser->parsed_arguments, "-f"));

		compress(files, output, compression_mode);
		free(files);
	} else {
		const char *input = (const char *)get_entry_value(
			argument_parser->parsed_arguments->get(argument_parser->parsed_arguments, "-i"));
		decompress(input, output);
	}
}

