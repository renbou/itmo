// ITMO 09.03.02 2020
// Artem Mikheev M3101
// Apache License
#include "utils.h"
#include "config.h"

void write_bigendian_u64(uint64_t value, void* dest) {
	if (get_global_config()->is_little_endian) {
		value = bswap64(value);
	}
	write_u64(value, dest);
}

uint64_t read_bigendian_u64(void* src) {
	uint64_t value = read_u64(src);
	if (get_global_config()->is_little_endian) {
		value = bswap64(value);
	}
	return value;
}

void write_bigendian_u32(uint32_t value, void* dest) {
	if (get_global_config()->is_little_endian) {
		value = bswap32(value);
	}
	write_u32(value, dest);
}

uint32_t read_bigendian_u32(void* src) {
	uint64_t value = read_u32(src);
	if (get_global_config()->is_little_endian) {
		value = bswap32(value);
	}
	return value;
}

void write_bigendian_u16(uint16_t value, void* dest) {
	if (get_global_config()->is_little_endian) {
		value = bswap16(value);
	}
	write_u16(value, dest);
}

uint16_t read_bigendian_u16(void* src) {
	uint64_t value = read_u16(src);
	if (get_global_config()->is_little_endian) {
		value = bswap16(value);
	}
	return value;
}

void *xalloc(size_t size) {
	void *result = malloc(size);
	if (result == NULL) {
		PUT_ERROR("Unable to allocate memory. Program must quit.");
		exit(-1);
	}
	return result;
}