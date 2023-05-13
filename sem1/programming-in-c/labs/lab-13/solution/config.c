//
// Created by Artem Mikheev on 11/23/20.
//

#include <stdint.h>
#include <string.h>
#include "config.h"
#include "errors.h"

static ID3v2_Config global_config;

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

/**
 * Helper to find a boolean parameter, without a value. Just checks for name in arguments.
 * @param name the name of the parameter which should be found in argv
 * @return 1 if the parameter is present, 0 otherwise
 */
int boolean_parameter(int argc, char *argv[], const char *name) {
	for (uint32_t i = 0; i < argc; i++) {
		if (strcmp(argv[i], name) == 0) {
			return 1;
		}
	}
	return 0;
}

/**
 * Function to check if the name is a valid tag name.
 * @return 1 if it is a valid tag name (A-Z0-9) 0 if it isn't
 */
int check_tag_name(const char *name) {
	if (strlen(name) != 4) {
		return 0;
	}
	for (uint32_t i = 0; i < 4; i++) {
		if ((name[i] < 'A' || name[i] > 'Z') &&
			(name[i] < '0' || name[i] > '9')) {
			return 0;
		}
	}
	return 1;
}

void initialize_global_config(int argc, char *argv[]) {
	union {
		uint32_t x;
		unsigned char c[4];
	} endianness_test = {0x00000001};
	global_config.is_little_endian = endianness_test.c[0];

	global_config.filename = get_parameter(argc, argv, "--filepath");
	if (global_config.filename == NULL) {
		FATAL("Unable to run without a file. (--filepath option not specified)");
	}

	const char *parameter = NULL;
	if (boolean_parameter(argc, argv, "--show")) {
		global_config.running_mode = SHOW;
	} else if ((parameter = get_parameter(argc, argv, "--get")) != NULL) {
		if (!check_tag_name(parameter)) {
			FATAL("Cannot get tag with invalid name.");
		}
		global_config.running_mode = GET;
		global_config.frame_name = parameter;
	} else if ((parameter = get_parameter(argc, argv, "--set")) != NULL) {
		if (!check_tag_name(parameter)) {
			FATAL("Cannot set tag with invalid name.");
		}
		const char *value = get_parameter(argc, argv, "--value");
		if (value == NULL) {
			FATAL("Unable to set a frame value without a given value.");
		}
		global_config.running_mode = SET;
		global_config.frame_name = parameter;
		global_config.frame_value = value;
	} else {
		FATAL("A mode must be specified from --show, --set and --get.")
	}
}

ID3v2_Config *get_global_config() {
	return &global_config;
}
