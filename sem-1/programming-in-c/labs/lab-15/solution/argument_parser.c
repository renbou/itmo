// simple but extendable argument argument_parser_t for C
// Copyright (C) 2020, Artem Mikheev <c@renbou.ru>.
// Licensed under the Apache License, Version 2.0.

#include <stdio.h>
#include "argument_parser.h"
#include "linear_map.h"
#include "errors.h"

const argument_parser_t *create_parser(const char *program_name,
		const char *help_header, const char *help_footer, const int add_help_flags) {
	argument_parser_t *new_parser = (argument_parser_t *)malloc(sizeof(argument_parser_t));
	with(new_parser != NULL, free(new_parser), {
		new_parser->added_arguments = create_linear_map();

		with(new_parser->added_arguments != NULL, free(new_parser->added_arguments), {
			new_parser->parsed_arguments = create_linear_map();

			with(new_parser->parsed_arguments != NULL, free(new_parser->parsed_arguments), {
				new_parser->help_footer = help_footer;
				new_parser->help_header = help_header;
				new_parser->program_name = program_name;

				if (add_help_flags) {
					parser_add_argument(new_parser, "-h", BOOLEAN_ARGUMENT, "print the help message", 0, NULL, NULL);
					parser_add_argument(new_parser, "--help", BOOLEAN_ARGUMENT, "print the help message", 0, NULL, NULL);
				}

				return new_parser;
			});
		});
	});
	return NULL;
}

int destroy_parser(const argument_parser_t* _parser) {
	if (_parser == NULL) {
		return -1;
	}

	if (_parser->parsed_arguments == NULL || _parser->added_arguments == NULL) {
		return -2;
	}

	// iterate and clear our added arguments, since they have been created by malloc
	const map_entry_t **current = _parser->added_arguments->first_entry(_parser->added_arguments);
	const map_entry_t **next = NULL;
	while (current != NULL) {
		_parser->added_arguments->iterate(_parser->added_arguments, current, &next);
		free(current);
		current = next;
	}

	// iterate and clear our parsed arguments, since they have been created by malloc
	current = _parser->parsed_arguments->first_entry(_parser->parsed_arguments);
	next = NULL;
	while (current != NULL) {
		_parser->parsed_arguments->iterate(_parser->parsed_arguments, current, &next);
		free(current);
		current = next;
	}

	destroy_linear_map(_parser->added_arguments);
	destroy_linear_map(_parser->parsed_arguments);
	free(_parser);

	return 0;
}

int parser_add_argument(const argument_parser_t *_parser, const char *name, const argument_type_t argument_type,
						const char *description, const int required,
						const char *value_type, int (*value_type_converter)(const char *str, void **result)) {
	if (_parser == NULL || name == NULL) {
		return -1;
	}

	if (argument_type != BOOLEAN_ARGUMENT && (value_type == NULL)) {
		return -1;
	}

	if (_parser->parsed_arguments == NULL || _parser->added_arguments == NULL) {
		return -2;
	}

	if (_parser->added_arguments->get(_parser->added_arguments, name) != NULL) {
		return -3;
	}

	if ((argument_type != BOOLEAN_ARGUMENT && argument_type != VALUE_ARGUMENT)
			|| (argument_type == BOOLEAN_ARGUMENT && required != 0)) {
		return -4;
	}

	parser_argument_t *new_argument = (parser_argument_t *)malloc(sizeof(parser_argument_t));
	with(new_argument != NULL, free(new_argument), {
		new_argument->name = name;
		new_argument->required = required;
		new_argument->description = description;
		new_argument->argument_type = argument_type;
		if (argument_type == VALUE_ARGUMENT) {
			new_argument->type_name = value_type;
			new_argument->type_converter = value_type_converter;
		} else {
			new_argument->type_name = "bool";
			new_argument->type_converter = NULL;
		}

		map_entry_t *new_entry = malloc(sizeof(map_entry_t));
		with(new_entry != NULL, free(new_entry), {
			new_entry->type = "parser_argument";
			new_entry->value = new_argument;
			_parser->added_arguments->set(_parser->added_arguments, name, (void *)new_entry);
			return 0;
		});
	});

	return -5;
}

/**
 * Helper to find the parameter/argument from argc and argv of the program
 * @param name the name of the parameter of which to get the value
 * @return position of the value of the argument,
 */
int32_t parser_internal_find_value_argument(int argc, const char *argv[], const char *name) {
	for (uint32_t i = 0; i < argc - 1; i++) {
		if (strcmp(argv[i], name) == 0) {
			return i + 1;
		}
	}
	return -1;
}

/**
 * Helper to find BOOLEAN parameter from argc and argv of the program
 * @param name the name of the parameter of which to get the value
 * @return position of the value of the argument,
 */
int32_t parser_internal_find_boolean_argument(int argc, const char *argv[], const char *name) {
	for (uint32_t i = 0; i < argc; i++) {
		if (strcmp(argv[i], name) == 0) {
			return i;
		}
	}
	return -1;
}

/**
 * Helper to add a parsed argument to the parser map
 */
int parser_internal_add_parsed_argument(const argument_parser_t *_parser,
		const char *name, const char *type, void *value) {
	map_entry_t *new_entry = malloc(sizeof(map_entry_t));
	with(new_entry != NULL, free(new_entry), {
		new_entry->type = type;
		new_entry->value = value;
		_parser->parsed_arguments->set(_parser->parsed_arguments, name, (void *)new_entry);
		return 0;
	});
	return -1;
}

int parser_parse_arguments(const argument_parser_t *_parser, const int argc, const char *argv[]) {
	if (_parser == NULL) {
		return -1;
	}

	if (_parser->parsed_arguments == NULL || _parser->added_arguments == NULL) {
		return -2;
	}

	const map_entry_t **current = _parser->added_arguments->first_entry(_parser->added_arguments);
	const map_entry_t **next = NULL;
	while (current != NULL) {
		const parser_argument_t *current_argument = (*current)->value;
		int should_add_to_map = 0;
		void *argument_value;
		if (current_argument->argument_type == BOOLEAN_ARGUMENT) {
			int32_t position = parser_internal_find_boolean_argument(argc, argv, current_argument->name);
			if (position != -1) {
				argument_value = (void *)1;
				argv[position] = "";
			} else {
				argument_value = (void *)0;
			}
			should_add_to_map = 1;
		} else {
			int32_t position = parser_internal_find_value_argument(argc, argv, current_argument->name);
			if (position != -1) {
				int conversion_result;
				if (current_argument->type_converter != NULL) {
					conversion_result = current_argument->type_converter(argv[position], &argument_value);
				} else {
					conversion_result = 0;
					argument_value = (void *)argv[position];
				}

				if (conversion_result != 0) {
					return -4;
				}
				should_add_to_map = 1;
				argv[position] = "";
				argv[position - 1] = "";
			} else if (current_argument->required) {
				return -3;
			} else {
				argument_value = NULL;
			}
		}

		if (should_add_to_map) {
			if (parser_internal_add_parsed_argument(_parser, current_argument->name, current_argument->type_name, argument_value) != 0) {
				return -5;
			}
		}

		_parser->added_arguments->iterate(_parser->added_arguments, current, &next);
		current = next;
	}

	return 0;
}

void parser_print_help(const argument_parser_t *_parser) {
	if (_parser == NULL) {
		return;
	}
	printf("Usage: %s ", _parser->program_name);
	const map_entry_t **current = _parser->added_arguments->first_entry(_parser->added_arguments);
	const map_entry_t **next = NULL;
	while (current != NULL) {
		const parser_argument_t *current_argument = (*current)->value;
		if (current_argument->required) {
			printf("%s ", current_argument->name);
		} else {
			printf("[%s] ", current_argument->name);
		}
		_parser->added_arguments->iterate(_parser->added_arguments, current, &next);
		current = next;
	}
	printf("\n%s\nOptions:\n", _parser->help_header);

	current = _parser->added_arguments->first_entry(_parser->added_arguments);
	next = NULL;
	while (current != NULL) {
		const parser_argument_t *current_argument = (*current)->value;
		printf("\t%s - %s\n", current_argument->name, current_argument->description);
		_parser->added_arguments->iterate(_parser->added_arguments, current, &next);
		current = next;
	}

	printf("%s\n", _parser->help_footer);
}
