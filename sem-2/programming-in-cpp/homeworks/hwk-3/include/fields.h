#pragma once

#include "codec.h"

#include <cmath>

/*
 * Fields
 *  Account : Text(16)
 *  Capacity : Alpha(1)
 *  ClOrdID : Text(20)
 *  FeeCode: Alphanum(2)
 *  LastMkt: Alphanum(4)
 *  MaxFloor : Binary(4)
 *  OrderQty : Binary(4)
 *  OrdType : Alphanum(1)
 *  Price : BinaryPrice(8)
 *  Side : Alphanum(1)
 *  Symbol : Alphanum(8)
 *  TimeInForce : Alphanum(1)
 */
inline unsigned char * encode_text(unsigned char * start, const std::string & str, const size_t field_size)
{
    return encode(start, str, field_size);
}

inline unsigned char * encode_char(unsigned char * start, const char ch)
{
    return encode(start, static_cast<uint8_t>(ch));
}

inline unsigned char * encode_binary4(unsigned char * start, const uint32_t value)
{
    return encode(start, value);
}

inline unsigned char * encode_price(unsigned char * start, const double value)
{
    const double order = 10000;
    const double epsilon = 1e-5;
    // beware: no precision loss check
    return encode(start, static_cast<int64_t>(value * order + std::copysign(epsilon, value)));
}

inline const unsigned char * decode_text(const unsigned char * start, std::string & str, const size_t field_size)
{
    return decode(reinterpret_cast<const char *>(start), str, field_size);
}

inline const unsigned char * decode_char(const unsigned char * start, char & ch)
{
    return decode(start, reinterpret_cast<uint8_t &>(ch));
}

inline const unsigned char * decode_binary4(const unsigned char * start, uint32_t & value)
{
    return decode(start, value);
}

inline const unsigned char * decode_price(const unsigned char * start, double & value)
{
    const double order = 10000;
    int64_t scaled_value;
    const unsigned char * result = decode(start, scaled_value);
    // won't lose precision during div
    value = static_cast<double>(scaled_value) / order;
    return result;
}

inline constexpr size_t char_size = 1;
inline constexpr size_t binary4_size = 4;
inline constexpr size_t price_size = 8;

#define FIELD(name, protocol_type, ctype)                                                \
    inline unsigned char * encode_field_##name(unsigned char * start, const ctype value) \
    {                                                                                    \
        return encode_##protocol_type(start, value);                                     \
    }

#define VAR_FIELD(name, size)                                                                  \
    inline unsigned char * encode_field_##name(unsigned char * start, const std::string & str) \
    {                                                                                          \
        return encode_text(start, str, size);                                                  \
    }

#include "fields.inl"

#define FIELD(name, protocol_type, ctype)                                                        \
    inline const unsigned char * decode_field_##name(const unsigned char * start, ctype & value) \
    {                                                                                            \
        return decode_##protocol_type(start, value);                                             \
    }

#define VAR_FIELD(name, size)                                                                        \
    inline const unsigned char * decode_field_##name(const unsigned char * start, std::string & str) \
    {                                                                                                \
        return decode_text(start, str, size);                                                        \
    }

#include "fields.inl"

#define FIELD(name, protocol_type, _) inline constexpr size_t name##_field_size = protocol_type##_size;
#define VAR_FIELD(name, size) inline constexpr size_t name##_field_size = size;

#include "fields.inl"

inline void set_opt_field_bit(unsigned char * const bitfield_start, unsigned bitfield_num, unsigned bit)
{
    *(bitfield_start + bitfield_num - 1) |= bit;
}

// check if bit field is set, otherwise don't try parsing it
inline bool get_opt_field_bit(const unsigned char * bitfield_start, unsigned bitfield_num, unsigned bit)
{
    return static_cast<bool>(*(bitfield_start + bitfield_num - 1) & bit);
}
