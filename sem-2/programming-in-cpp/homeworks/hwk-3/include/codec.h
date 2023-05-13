#pragma once

#include <cstdint>
#include <cstring>
#include <string>

inline unsigned char * encode(unsigned char * start, const uint8_t value)
{
    *start = value;
    return start + 1;
}

inline const unsigned char * decode(const unsigned char * start, uint8_t & value)
{
    value = *start;
    return start + 1;
}

inline unsigned char * encode(unsigned char * start, const uint16_t value)
{
    *start++ = static_cast<unsigned char>(value & 0xFF);
    *start++ = static_cast<unsigned char>((value >> 8) & 0xFF);
    return start;
}

inline const unsigned char * decode(const unsigned char * start, uint16_t & value)
{
    value = static_cast<uint16_t>(*start++);
    value |= (static_cast<uint16_t>(*start++) << 8u);
    return start;
}

inline unsigned char * encode(unsigned char * start, const uint32_t value)
{
    *start++ = static_cast<unsigned char>(value & 0xFF);
    *start++ = static_cast<unsigned char>((value >> 8) & 0xFF);
    *start++ = static_cast<unsigned char>((value >> 16) & 0xFF);
    *start++ = static_cast<unsigned char>((value >> 24) & 0xFF);
    return start;
}

inline const unsigned char * decode(const unsigned char * start, uint32_t & value)
{
    value = static_cast<uint32_t>(*start++);
    value |= (static_cast<uint32_t>(*start++) << 8u);
    value |= (static_cast<uint32_t>(*start++) << 16u);
    value |= (static_cast<uint32_t>(*start++) << 24u);
    return start;
}

inline unsigned char * encode(unsigned char * start, int64_t value)
{
    for (int i = 0; i < 8; ++i) {
        *start++ = static_cast<unsigned char>(value & 0xFF);
        value >>= 8;
    }
    return start;
}

inline const unsigned char * decode(const unsigned char * start, int64_t & value)
{
    value = 0;
    for (int i = 0; i < 8; ++i) {
        value |= (static_cast<uint64_t>(*start++) << (8 * i));
    }
    return start;
}

inline unsigned char * encode(unsigned char * start, const std::string & str, const size_t field_size)
{
    size_t i = 0;
    while (i < str.size() && i < field_size) {
        *start++ = str[i];
        ++i;
    }
    while (i < field_size) {
        *start++ = '\0';
        ++i;
    }
    return start;
}

// We want to decode a byte string into a proper string, this means that its the callers problem to make
// sure we have a valid const char * (not unsigned)
inline const unsigned char * decode(const char * start, std::string & str, const size_t field_size)
{
    // if field is completely filled, we might not find the \0 char
    const char * end = reinterpret_cast<const char *>(std::memchr(reinterpret_cast<const void *>(start), '\0', field_size));
    const size_t actual_size = end == 0 ? field_size : end - start;
    str.assign(start, actual_size);
    return reinterpret_cast<const unsigned char *>(start) + field_size;
}