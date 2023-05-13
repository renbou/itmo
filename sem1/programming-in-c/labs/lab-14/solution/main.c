// ITMO 09.03.02 2020
// Artem Mikheev M3101
// Apache License
#include <stdint.h>
#include <math.h>
#include "img.h"
#include "utils.h"
#include "errors.h"

/**
 * Check if the point is in the bounds of the width and height
 */
unsigned in_bounds(int32_t x, int32_t y, uint32_t width, uint32_t height) {
	return x >= 0 && x < width && y >= 0 && y < height;
}

/**
 * Computes the index of a 2d point in the array
 */
uint32_t calc_index(int32_t x, int32_t y, uint32_t width) {
	return y * width + x;
}

/**
 * Does one step of the GameOfLife. This allocates a new array of pixels every time, which could technically be optimized to
 * use a global variable or something but that would make the code dirtier.
 * @param game_state the previous game state
 */
void one_step(MonochromeImage *game_state) {
	unsigned char *new_pixels = xalloc(game_state->width * game_state->height);
	if (new_pixels == NULL) {
		PUT_ERROR("Unable to allocate memory for new iteration of the game of life.");
		exit(-1);
	}
	memset(new_pixels, 0, game_state->width * game_state->height);

	int32_t neighboor_deltas[8][2] = {
		{0, 1}, {0, -1}, {1, 0}, {-1, 0}, {-1, -1}, {1, 1}, {-1, 1}, {1, -1}
	};

	for (int32_t i = 0; i < game_state->height; i++) {
		for (int32_t j = 0; j < game_state->width; j++) {
			uint32_t num_neighboors = 0;
			uint32_t x = j, y = i;
			for (uint8_t neighboor = 0; neighboor < 8; neighboor++) {
				int32_t neighboor_x = x + neighboor_deltas[neighboor][0];
				int32_t neighboor_y = y + neighboor_deltas[neighboor][1];
				if (in_bounds(neighboor_x, neighboor_y, game_state->width, game_state->height) &&
						game_state->pixels[calc_index(neighboor_x, neighboor_y, game_state->width)] == 1) {
					num_neighboors++;
				}
			}
			if (num_neighboors == 3) {
				new_pixels[calc_index(x, y, game_state->width)] = 1;
			} else if (game_state->pixels[calc_index(x, y, game_state->width)] == 1 && num_neighboors == 2) {
				new_pixels[calc_index(x, y, game_state->width)] = 1;
			}
		}
	}

	memcpy(game_state->pixels, new_pixels, game_state->width * game_state->height);
	xfree(new_pixels);
}

/**
 * Dumps the state of a game with the given filename and iteration
 */
int dump_state(MonochromeImage *game_state, const char *output_directory, const char *filename, uint32_t iteration) {
	uint32_t iteration_str_width;
	if (iteration == 0) {
		iteration_str_width = 1;
	} else {
		iteration_str_width = ceil(log10(iteration + 1));
	}

	const uint32_t filename_length = strlen(filename);
	const uint32_t directory_name_length = strlen(output_directory);

	char * iteration_filename = xalloc(directory_name_length + 1 + filename_length + iteration_str_width + 2);

	// if file has .bmp extension then remove it
	if (strstr(filename, ".bmp") != NULL && strcmp(filename + filename_length - 4, ".bmp") == 0) {
		sprintf(iteration_filename, "%s/%.*s_%u.bmp", output_directory, filename_length - 4, filename, iteration);
	} else {
		sprintf(iteration_filename, "%s/%s_%u.bmp", output_directory, filename, iteration);
	}

	return write_bmp(iteration_filename, game_state);
}

/**
 * Helper to find the parameter/argument from argc and argv of the program
 * @param name the name of the parameter of which to get the value
 * @return NULL if the parameter or its value doesn't exist, otherwise a string
 */
const char *get_parameter(int argc, char *argv[], const char *name) {
	for (uint32_t i = 0; i < argc - 1; i++) {
		if (strcmp(argv[i], name) == 0) {
			return argv[i + 1];
		}
	}
	return NULL;
}

const char * const HELP =
	"Usage: %s --input [input filename] --output [output directory path] [options...]\n"
 	"Options:\n"
  	"\t--max-iter [N: int]        The maximum number of iterations to emulate. N > 0.\n"
   	"\t--dump-freq [N: int]       The frequence with which to dump game states to the output directory. N > 0.\n";

int main(int argc, char *argv[]) {
	const char* input_file = get_parameter(argc, argv, "--input");
	const char* output_directory = get_parameter(argc, argv, "--output");

	if (input_file == NULL) {
		printf("Cannot play game of life without an input file.\n");
		printf(HELP, argv[0]);
		return -1;
	}

	if (output_directory == NULL) {
		printf("Cannot play game of life without an output directory.\n");
		printf(HELP, argv[0]);
		return -1;
	}

	uint32_t dump_frequency = 1;
	uint32_t max_iterations = UINT32_MAX;

	const char *dump_frequency_option = get_parameter(argc, argv, "--dump-freq");
	if (dump_frequency_option != NULL) {
		int32_t option_value = strtol(dump_frequency_option, NULL, 10);
		if (option_value <= 0) {
			printf("Invalid dump frequency specified, the value must be a positive integer.\n");
			printf(HELP, argv[0]);
			return -1;
		}
		dump_frequency = option_value;
	}

	const char *max_iterations_option = get_parameter(argc, argv, "--max-iter");
	if (max_iterations_option != NULL) {
		int32_t option_value = strtol(max_iterations_option, NULL, 10);
		if (option_value <= 0) {
			printf("Invalid max iterations specified, the value must be a positive integer.\n");
			printf(HELP, argv[0]);
			return -1;
		}
		max_iterations = option_value;
	}

	if (dump_frequency > max_iterations) {
		printf("Specified dump frequency (%u) is bigger than the maximum number of iterations (%u)\n", dump_frequency, max_iterations);
		printf(HELP, argv[0]);
		return -1;
	}

	MonochromeImage *game_state = read_bmp(input_file);
	if (game_state == NULL) {
		printf("Invalid input image specified, unable to continue.\n");
		printf(HELP, argv[0]);
		return -1;
	}

	for (uint32_t i = 0; i < max_iterations; i++) {
		one_step(game_state);
		if (i % dump_frequency == 0) {
			if (dump_state(game_state, output_directory, input_file, i)) {
				printf("Unable to save game state.\n");
				printf(HELP, argv[0]);
				return -1;
			}
		}
	}

	dump_state(game_state, output_directory, input_file, max_iterations);
	return 0;
}
