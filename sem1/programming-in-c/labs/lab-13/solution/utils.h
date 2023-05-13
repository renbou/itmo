// ITMO 09.03.02 2020
// Artem Mikheev M3101
// Apache License
#ifndef ID3V2_UTILS_H
#define ID3V2_UTILS_H


#include <stdint.h>
#include <stdlib.h>
#include "errors.h"
#include "config.h"

#define write_u64(value, dest) \
	*(uint64_t *)(dest) = (value)
#define write_u32(value, dest) \
	*(uint32_t *)(dest) = (value)
#define write_u16(value, dest) \
	*(uint16_t *)(dest) = (value)
#define write_u8(value, dest) \
	*(uint8_t *)(dest) = (value)

#define read_u64(src) \
	*(uint64_t*)(src)
#define read_u32(src) \
	*(uint32_t*)(src)
#define read_u16(src) \
	*(uint16_t*)(src)
#define read_u8(src) \
	*(uint8_t*)(src)

#define bswap64(value)						 \
	(((value) & 0xFF00000000000000ull) >> 56u) |  \
	(((value) & 0x00FF000000000000ull) >> 40u) | \
	(((value) & 0x0000FF0000000000ull) >> 24u) | \
	(((value) & 0x000000FF00000000ull) >>  8u) | \
	(((value) & 0x00000000FF000000ull) <<  8u) | \
	(((value) & 0x0000000000FF0000ull) << 24u) | \
	(((value) & 0x000000000000FF00ull) << 40u) | \
	(((value) & 0x00000000000000FFull) << 56u)   \

#define bswap32(value)			     \
	(((value) & 0xFF000000ul) >> 24u) |  \
	(((value) & 0x00FF0000ul) >>  8u) | \
	(((value) & 0x0000FF00ul) <<  8u) | \
	(((value) & 0x000000FFul) << 24u)  \

#define bswap16(value)			     \
	(((value) & 0xFF00u) >> 8u) | \
	(((value) & 0x00FFu) << 8u)   \

#define lower16(value) \
	((value) & 0xff)
#define upper16(value) \
	(((value) >> 8) & 0xff)

/**
 * Functions for reading and writing big-endian integers since all values inside ID3v2 tags are big-endian.
 */

void write_bigendian_u64(uint64_t value, void* dest);
uint64_t read_bigendian_u64(void* src);
void write_bigendian_u32(uint32_t value, void* dest);
uint32_t read_bigendian_u32(void* src);
void write_bigendian_u16(uint16_t value, void* dest);
uint16_t read_bigendian_u16(void* src);
#define write_bigendian_u8 write_u8
#define read_bigendian_u8 read_u8

/**
 * Functions for working with memory.
 */

void *xalloc(size_t size);

#endif //ID3V2_UTILS_H
