// questionable library for working with various error-like conditions in C
// Copyright (C) 2020, Artem Mikheev <c@renbou.ru>.
// Licensed under the Apache License, Version 2.0.
#ifndef CREN_ERRORS_H
#define CREN_ERRORS_H

#define with(PREREQUISITES, FINALIZER, ...) do {\
	if ((PREREQUISITES)) do { __VA_ARGS__; } while(0); if ((PREREQUISITES)) do { FINALIZER; } while(0); \
} while(0)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

void error(const char *type, const char *reason, const char *file, const char *func_name, unsigned int line);

#define PUT_LOG(type, reason) \
	error(type, reason, __FILE__, __FUNCTION__, __LINE__)
#define PUT_ERROR(reason) \
	PUT_LOG("ERROR", reason)
#define ERROR() \
	PUT_ERROR(strerror(errno))
#define FATAL(reason) \
	{ PUT_ERROR(reason); exit(-1); }
#define PUTF_LOG(type, ...) \
	{ snprintf(ERROR_STRING_BUFFER, 8192, ##__VA_ARGS__); PUT_LOG(type, ERROR_STRING_BUFFER); }
#define PUTF_ERROR(...) \
	PUTF_LOG("ERROR", ##__VA_ARGS__)

int file_eof(FILE *file, const char *filename, const char *extra_info);

int file_error(FILE * file);

#define BAD_FILE(file, filename, info) \
	file_error(file) | file_eof(file, filename, info)

#endif //CREN_ERRORS_H
