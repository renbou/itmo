// simple but extendable argument argument_parser_t for C
// Copyright (C) 2020, Artem Mikheev <c@renbou.ru>.
// Licensed under the Apache License, Version 2.0.

#ifndef CREN_ARGUMENT_PARSER_H
#define CREN_ARGUMENT_PARSER_H

#include <stdint.h>
#include "map.h"

/**
 * Enum representing the different kinds of arguments you might want from an argument parser.
 * VALUE describes arguments in format "name" "value"
 * BOOLEAN describes arguments in format "name"
 */
typedef enum argument_type_e {VALUE_ARGUMENT, BOOLEAN_ARGUMENT} argument_type_t;

/**
 * Struct describing the internal format in which added arguments (unparsed) are stored.
 */
typedef struct {
	// The name of the argument
	const char *name;
	// The kind of argument we are working with
	argument_type_t argument_type;
	// The name of the type with which to save the argument to the argument map
	const char *type_name;
	// A function which should convert a string to the wanted type, should store the conversion result in result,
	// should return 0 on success and negative number on fail
	int (*type_converter)(const char *str, void **result);
	// An optional description for the argument, used when printing the help
	const char *description;
	// 0 if the argument is optional, 1 if the argument *must* exist
	int required;
} parser_argument_t;

typedef struct {
	const char *program_name;
	// What to print after the usage format and before the option descriptions. Optional.
	const char *help_header;
	// What to print after everything else. Optional.
	const char *help_footer;
	// A map of parser_argument_t, which is then used to convert the given arguments to the wanted
	// types and stored in the parsed_arguments map
	const map_t *added_arguments;
	// The map which is initialized after adding all the wanted arguments and then parsing the actual arguments
	const map_t *parsed_arguments;
} argument_parser_t;

/**
 * Create a fresh, new argument parser. This uses malloc to allocate memory for the argument parser.
 * @param add_help_flags should be 1 if you want -h and --help to be added as arguments
 * @return NULL if memory allocation for the argument parser failed, otherwise a pointer to a new argument parser
 */
const argument_parser_t *create_parser(const char *program_name,
	const char *help_header, const char *help_footer, int add_help_flags);

/**
 * Destroy an argument parser after we are done parsing arguments and what-not.
 * @return 0 if the argument parser has been successfully destroyed, i.e. removed from memory
 * 		   -1 if a null argument parser was passed
 * 		   -2 if an invalid argument parser was passed
 */
int destroy_parser(const argument_parser_t* _parser);

/**
 * Function which adds an argument to an argument parser.
 * @param name the name of the argument to add
 * @param required 0 to indicate that the argument is optional,
 * 				   1 to indicate that parsing args should fail if this argument isn't present
 * @param value_type the string representation of the type of the argument's value
 * @param value_type_converter function which should convert strings to the desired type
 * @return 0 on success, otherwise:
 * 		   -1 if _parser or name is null
 * 		   -1 if the argument type isn't boolean but the value type or converter are NULL
 * 		   -2 if an invalid argument parser was passed
 * 		   -3 if an argument with such name is already added
 * 		   -4 if argument_type is not a valid argument type
 * 		   -5 if the argument couldn't be set for some reason, in which case this argument parser shouldn't be used anymore
 */
int parser_add_argument(const argument_parser_t *_parser, const char *name, argument_type_t argument_type,
						const char *description, int required, const char *value_type,
						int (*value_type_converter)(const char *str, void **result));

/**
 * Parse arguments after initializing the argument parser with all of the needed arguments.
 * WARNING! Currently this edits argv, so you should make a copy if needed...
 * @return 0 if the arguments have been successfully parsed
 * 		   -1 if _parser is null
 * 		   -2 if an invalid argument parser was passed
 * 		   -3 if not all of the required arguments have been parsed
 * 		   -4 if an argument's value is of the wrong type
 * 		   -5 if parsing failed for some reason, or the parsed arguments map couldn't be updated, meaning the parser is broken
 */
int parser_parse_arguments(const argument_parser_t *_parser, int argc, const char *argv[]);

/**
 * Print a help message constructed from the parser and its arguments.
 */
void parser_print_help(const argument_parser_t *_parser);

#endif //CREN_ARGUMENT_PARSER_H
