// questionable library for working with various error-like conditions in C
// Copyright (C) 2020, Artem Mikheev <c@renbou.ru>.
// Licensed under the Apache License, Version 2.0.

#include "errors.h"

char ERROR_STRING_BUFFER[8192];

void error(const char *type, const char *reason, const char *file, const char *func_name, unsigned int line) {
	fprintf(stderr,
			"<%s>\n"
			"\tFILE: %s\n"
			"\tLINE: %u\n"
			"\tFUNCTION: %s\n"
			"\tMESSAGE: %s\n"
			"<\\%s>\n", type, file, line, func_name, reason, type);
}

int file_eof(FILE *file, const char *filename, const char *extra_info) {
	if (feof(file)) {
		PUTF_LOG("FILE_ERROR", "End of file %s reached to soon - %s", filename, extra_info);
		return 1;
	}
	return 0;
}

int file_error(FILE * file) {
	if (ferror(file)) {
		ERROR();
		return 1;
	}
	return 0;
}
