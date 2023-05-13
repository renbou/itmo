// ITMO 09.03.02 2020
// Artem Mikheev M3101
// variant 17
// lab 8

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

typedef uint32_t uint32;
typedef int32_t int32;

#define xalloc(size) \
  calloc(1ul, (size_t)size)
#define xfree(data) \
  free((void *)data)
#define min(a, b) \
  (a < b ? a : b)

//#define not_whitespace(c) \
  (c != ' ' && c != '\n' && c != '\t' && c != '\r' && c != '\v' && c != '\f')

#define not_whitespace(c) \
  (c != '\n' && c != '\r')

#define DEFAULT_STRING_BUFFER_SIZE 0x10
typedef struct string_t {
	size_t length, max_length;
	char *data;
} string;
typedef string * string_p;

// make empty string structure
string_p string_init_() {
	return (string_p)( xalloc(sizeof(string)) );
}

// create a string with custom buffer size
string_p string_make_(const size_t buffer_size) {
	string_p temporary = string_init_();
	if (temporary == NULL)
		return NULL;
	temporary->data = (char *)( xalloc(buffer_size) );
	if (temporary->data == NULL) {
		xfree(temporary);
		return NULL;
	}
	temporary->max_length = buffer_size;
	return temporary;
}

// create a string with default buffer size
string_p string_make() {
	return string_make_(DEFAULT_STRING_BUFFER_SIZE);
}

void string_free(const string_p str) {
	xfree(str->data);
	xfree(str);
}

size_t string_length(const string_p str) {
	return str->length;
}

// expand the string by 2 times
// note - expands by max_length, so if you haven't filled ur string
// then this makes no sense
// IF WE FAIL, THEN WE DON'T FREE THE OLD STRING!!!
string_p string_expand(const string_p str) {
	const size_t new_length = str->max_length * 2;
	// if we got an overflow then die
	if (new_length < str->max_length) {
		return NULL;
	}
	string_p new = string_make_(new_length);
	if (new == NULL)
		return NULL;
	new->length = str->length;
	// use memcpy so that compiler optimizes it to qmemcpy
	memcpy(new->data, str->data, new->length);
	// free previous string
	string_free(str);
	return new;
}

// function that creates and reads a string
// after you're done using it, you should free the string
string_p string_read() {
	string_p str = string_make();
	if (str == NULL)
		return NULL;
	size_t num_read, available;
	char c = 0;
	c = getc(stdin);
	while(1) {
		num_read = 0;
		available = str->max_length - str->length;
		while(not_whitespace(c) && (num_read < available)) {
			str->data[str->length + num_read] = c;
			num_read++;
			c = getc(stdin);
		}
		str->length += num_read;
		if (str->length == str->max_length) {
			str = string_expand(str);
			if (str == NULL)
				return NULL;
		} else {
			break;
		}
	}
	return str;
}

// appends one char to the string, resizing if needed, and returns the new pointer
// always check if this returns null!
string_p string_append_char(string_p str, unsigned char c) {
	if (str->length == str->max_length) {
		str = string_expand(str);
		if (str == NULL)
			return NULL;
	}
	str->data[str->length++] = c;
	return str;
}

// simply print string to stdout
void string_print(string_p str) {
	fwrite(str->data, sizeof(char), str->length, stdout);
}

// custom printf which prints passed string instead of %$
void string_printf(const char *format_string, ...) {
	if (!format_string)
		return;

	size_t format_size = strlen(format_string);
	char *true_format = xalloc(format_size + 1);
	strncpy(true_format, format_string, format_size);

	va_list args, saved_args;
	va_start(args, format_string);
	string_p str;
	// parse different normal printf format strings
	char *current_fmt = true_format, *ptr = true_format;
	while (*(current_fmt) && *(ptr)) {
		// save current arguments
		va_copy(saved_args, args);
		ptr = current_fmt + 1;
		// find our next string
		while ( (*(ptr)) && !((*(ptr - 1) == '%') && (*(ptr) == '$')) )
			ptr++;
		if ( !*(ptr) )
			continue;

		// when we find our string, print the previous format
		// the update the args after printf, and print our string
		ptr--;
		memset(ptr, 0, 2);
		vprintf(current_fmt, saved_args);
		va_end(args);
		va_copy(args, saved_args);
		str = va_arg(args, string_p);
		string_print(str);
		// advance current_fmt only if we got to check a new one
		// so that after all %$ args current_fmt will be saved and printed
		ptr += 2;
		current_fmt = ptr;
		va_end(saved_args);
	}
	if ( *(current_fmt) )
		vprintf(current_fmt, args);
	// clear up
	va_end(args);
	xfree(true_format);
}

// returns 0 if strings match, otherwise 1
int32 string_compare(const string_p str1, const string_p str2) {
	if (str1->length != str2->length) {
		return 1;
	}
	size_t until = min(str1->length, str2->length);
	for (size_t i = 0; i < until; i++) {
		if (str1->data[i] - str2->data[i]) {
			return 1;
		}
	}
	return 0;
}

// compares the first n characters of the strings,
// returns 0 if they match, otherwise 1
int32 string_compare_n(const string_p str1, const string_p str2, const size_t n) {
	size_t until = min(n, min(str1->length, str2->length));
	for (size_t i = 0; i < until; i++) {
		if (str1->data[i] - str2->data[i]) {
			return 1;
		}
	}
	return 0;
}

// struct which acts as a set of chars
// basically splits the 256 values into 64-value blocks,
// each represented by a bit
typedef struct {
	uint64_t chars[4];
} byte_finder;

// adds a char to the set
static inline void byte_finder_add(byte_finder *bf, uint8_t value) {
	bf->chars[value >> 6] |= (1ULL << (value & 63));
}

// checks if char is in the set
static inline uint64_t byte_finder_check(byte_finder *bf, uint8_t value) {
	return bf->chars[value >> 6] & (1ULL << (value & 63));
}

// removes the char from the set
static inline void byte_finder_remove(byte_finder *bf, uint8_t value) {
	bf->chars[value >> 6] &= (~(1ULL << (value & 63)));
}

// finds the chars of the second string in the first string
string_p string_shared(const string_p str1, const string_p str2) {
	byte_finder bf = {0};
	// sets the flag in in_first that some character is present in str2
	for (size_t i = 0; i < str2->length; i++) {
		byte_finder_add(&bf, str2->data[i]);
	}
	string_p result = string_make();
	for (size_t i = 0; i < str1->length; i++) {
		uint8_t cur_value = str1->data[i];
		if (byte_finder_check(&bf, cur_value)) {
			result = string_append_char(result, str1->data[i]);
			// remove the value, cause we already added it to the answer
			byte_finder_remove(&bf, cur_value);
			if (result == NULL) {
				return NULL;
			}
		}
	}
	return result;
}

// function that splits str1 into tokens defined by separators in str2
// stores the resulting array in tokens and the size in num_tokens, which
// must be passed as references
void string_tokens(const string_p str1, const string_p str2, string_p **tokens, size_t *num_tokens) {
	if (tokens == NULL || num_tokens == NULL) {
		return;
	}
	*tokens = NULL, *num_tokens = 0;
	// same logic as in string_shared
	byte_finder bf = {0};
	for (size_t i = 0; i < str2->length; i++) {
		byte_finder_add(&bf, str2->data[i]);
	}
	// initialize the tokens array with 4 strings first, then expand as needed
	uint32 current_max = 4;
	(*tokens) = xalloc(current_max * sizeof(string_p));
	if (*tokens == NULL) {
		return;
	}

	for (size_t i = 0; i < str1->length;) {
		string_p current_string = string_make();
		while (i < str1->length && !byte_finder_check(&bf, str1->data[i])) {
			current_string = string_append_char(current_string, str1->data[i]);
			if (current_string == NULL) {
				xfree(*tokens);
				*tokens = NULL;
				*num_tokens = 0;
				return;
			}
			i++;
		}
		if (current_string->length) {
			// need to expand token vector
			if (*num_tokens == current_max) {
				string_p *new_tokens = xalloc(current_max * 2 * sizeof(string_p));
				if (new_tokens == NULL) {
					xfree(*tokens);
					*tokens = NULL;
					*num_tokens = 0;
					return;
				}
				memcpy(new_tokens, *tokens, current_max * sizeof(string_p));
				current_max *= 2;
				xfree(*tokens);
				*tokens = new_tokens;
			}
			(*tokens)[(*num_tokens)++] = current_string;
		}
		while (i < str1->length && byte_finder_check(&bf, str1->data[i])) {
			i++;
		}
	}
}

int main() {
	printf(
		"[1.1] Enter two strings to compare\n"
		": ");
	fflush(stdout);
	string_p first = string_read();
	if (first == NULL) {
		printf("\n{ERROR} Can't read first string.\n");
		return 0;
	}
	printf(": ");
	fflush(stdout);
	string_p second = string_read();
	if (second == NULL) {
		printf("\n{ERROR} Can't read second string.\n");
		return 0;
	}
	string_printf(
		"[1.2] \"%$\" %s \"%$\"\n\n",
		first, ( string_compare(first, second) ? "!=" : "==" ), second);
	string_free(first);
	string_free(second);

	printf(
		"[2.1] Enter two strings to compare first n chars of\n"
		": ");
	fflush(stdout);
	first = string_read();
	if (first == NULL) {
		printf("\n{ERROR} Can't read first string.\n");
		return 0;
	}
	printf(": ");
	fflush(stdout);
	second = string_read();
	if (second == NULL) {
		printf("\n{ERROR} Can't read second string.\n");
		return 0;
	}
	printf("[2.2] Enter n: ");
	fflush(stdout);
	size_t n;
	scanf("%u%*c", &n);
	string_printf("[2.3] first %u chars of \"%$\" and \"%$\" are %s\n\n",
				  n, first, second, ( string_compare_n(first, second, n) ? "not equal" : "equal" ));
	string_free(first);
	string_free(second);

	printf(
		"[3.1] Enter string to get its length\n"
		": ");
	fflush(stdout);
	first = string_read();
	if (first == NULL) {
		printf("\n{ERROR} Can't read first string.\n");
		return 0;
	}
	string_printf("[3.2] \"%$\" is %u chars long\n\n",
				  first, string_length(first));
	string_free(first);

	printf(
		"[4.1] Enter two strings to find any char of second string in first\n"
		": ");
	fflush(stdout);
	first = string_read();
	if (first == NULL) {
		printf("\n{ERROR} Can't read first string.\n");
		return 0;
	}
	printf(": ");
	fflush(stdout);
	second = string_read();
	if (second == NULL) {
		printf("\n{ERROR} Can't read second string.\n");
		return 0;
	}
	string_p shared_chars = string_shared(first, second);
	if (shared_chars == NULL) {
		printf("\n{ERROR} Unable to create string of shared characters\n");
		return 0;
	}
	string_printf(
		shared_chars->length ? "[4.2] Found shared chars \"%$\" from \"%$\" in \"%$\"\n\n" :
		"[4.2]%cNo shared characters from \"%$\" found in \"%$\"\n\n",
		shared_chars, second, first);
	string_free(first);
	string_free(second);
	string_free(shared_chars);

	printf(
		"[5.1] Enter string to tokenize\n"
		": ");
	fflush(stdout);
	first = string_read();
	if (first == NULL) {
		printf("\n{ERROR} Can't read second string.\n");
		return 0;
	}
	first->data[first->length] = '\0';
	printf(
		"[5.2] Enter delimiters as a string\n"
		": ");
	fflush(stdout);
	second = string_read();
	if (second == NULL) {
		printf("\n{ERROR} Can't read second string.\n");
		return 0;
	}
	printf("[5.3] Tokens:\n");
	string_p *tokens;
	size_t num_tokens;
	string_tokens(first, second, &tokens, &num_tokens);
	for (size_t i = 0; i < num_tokens; i++) {
		string_printf("- %$\n", tokens[i]);
		string_free(tokens[i]);
	}
	string_free(first);
	string_free(second);
	return 0;
}