#include "requests.h"

#include <algorithm>

namespace {

void encode_new_order_opt_fields(unsigned char * bitfield_start,
                                 const double price,
                                 const char ord_type,
                                 const char time_in_force,
                                 const unsigned max_floor,
                                 const std::string & symbol,
                                 const char capacity,
                                 const std::string & account)
{
    auto * p = bitfield_start + new_order_bitfield_num();
#define FIELD(name, bitfield_num, bit)                    \
    set_opt_field_bit(bitfield_start, bitfield_num, bit); \
    p = encode_field_##name(p, name);
#include "new_order_opt_fields.inl"
}

void decode_order_execution_opt_fields(const unsigned char * bitfield_start,
                                       const std::string & symbol,
                                       const std::string & last_mkt,
                                       const std::string & fee_code)
{
    auto * p = bitfield_start + order_execution_bitfield_num();
#define FIELD(name, bitfield_num, bit)                               \
    if (get_opt_field_bit(bitfield_start, bitfield_num, bit)) {      \
        p = decode_field_##name(p, const_cast<std::string &>(name)); \
    }
#include "order_execution_opt_fields.inl"
}

uint8_t encode_request_type(const RequestType type)
{
    switch (type) {
    case RequestType::New:
        return 0x38;
    }
    return 0;
}

uint8_t encode_response_type(const ResponseType type)
{
    switch (type) {
    case ResponseType::OrderExecution:
        return 0x2C;
    }
    return 0;
}

unsigned char * add_request_header(unsigned char * start, unsigned length, const RequestType type, unsigned seq_no)
{
    *start++ = 0xBA;
    *start++ = 0xBA;
    start = encode(start, static_cast<uint16_t>(length));
    start = encode(start, encode_request_type(type));
    *start++ = 0;
    return encode(start, static_cast<uint32_t>(seq_no));
}

const unsigned char * check_request_header(const unsigned char * start, unsigned length, const ResponseType type)
{
    bool good = *start++ == 0xBA;
    good = good && *start++ == 0xBA;

    uint16_t message_length;
    start = decode(start, message_length);
    good = good && message_length == (length - 2);

    uint8_t message_type;
    start = decode(start, message_type);
    good = good && message_type == encode_response_type(type);

    // skip matching unit and seq number
    start += 5;
    return good ? start : nullptr;
}

char convert_side(const Side side)
{
    switch (side) {
    case Side::Buy: return '1';
    case Side::Sell: return '2';
    }
    return 0;
}

char convert_ord_type(const OrdType ord_type)
{
    switch (ord_type) {
    case OrdType::Market: return '1';
    case OrdType::Limit: return '2';
    case OrdType::Pegged: return 'P';
    }
    return 0;
}

char convert_time_in_force(const TimeInForce time_in_force)
{
    switch (time_in_force) {
    case TimeInForce::Day: return '0';
    case TimeInForce::IOC: return '3';
    case TimeInForce::GTD: return '6';
    }
    return 0;
}

char convert_capacity(const Capacity capacity)
{
    switch (capacity) {
    case Capacity::Agency: return 'A';
    case Capacity::Principal: return 'P';
    case Capacity::RisklessPrincipal: return 'R';
    }
    return 0;
}

LiquidityIndicator convert_to_liquidity_indicator(const char c)
{
    switch (c) {
    case 'A': return LiquidityIndicator::Added;
    case 'R': return LiquidityIndicator::Removed;
    }
    return LiquidityIndicator();
}

std::string to_base64(uint64_t value)
{
    static const std::string alphabet = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";

    if (value == 0) {
        return std::string(1, alphabet[0]);
    }
    std::string result;
    while (value > 0) {
        result.push_back(alphabet[value % alphabet.size()]);
        value /= alphabet.size();
    }
    std::reverse(result.begin(), result.end());
    return result;
}

const unsigned char * decode_binary4_to_double(const unsigned char * start, double & value)
{
    uint32_t byte_value;
    const unsigned char * result = decode_binary4(start, byte_value);
    value = byte_value;
    return result;
}

} // anonymous namespace

std::array<unsigned char, calculate_size(RequestType::New)> create_new_order_request(const unsigned seq_no,
                                                                                     const std::string & cl_ord_id,
                                                                                     const Side side,
                                                                                     const double volume,
                                                                                     const double price,
                                                                                     const OrdType ord_type,
                                                                                     const TimeInForce time_in_force,
                                                                                     const double max_floor,
                                                                                     const std::string & symbol,
                                                                                     const Capacity capacity,
                                                                                     const std::string & account)
{
    static_assert(calculate_size(RequestType::New) == 78, "Wrong New Order message size");

    std::array<unsigned char, calculate_size(RequestType::New)> msg;
    auto * p = add_request_header(&msg[0], msg.size() - 2, RequestType::New, seq_no);
    p = encode_text(p, cl_ord_id, 20);
    p = encode_char(p, convert_side(side));
    p = encode_binary4(p, static_cast<uint32_t>(volume));
    p = encode(p, static_cast<uint8_t>(new_order_bitfield_num()));
    encode_new_order_opt_fields(p,
                                price,
                                convert_ord_type(ord_type),
                                convert_time_in_force(time_in_force),
                                max_floor,
                                symbol,
                                convert_capacity(capacity),
                                account);
    return msg;
}

ExecutionDetails decode_order_execution(const std::vector<unsigned char> & msg)
{
    ExecutionDetails exec_details;
    auto * p = check_request_header(&msg[0], msg.size(), ResponseType::OrderExecution);
    // skip time, currently don't need it
    p += 8;
    p = decode_text(p, exec_details.cl_ord_id, 20);

    uint64_t exec_id_numeric;
    p = decode(p, reinterpret_cast<int64_t &>(exec_id_numeric));
    exec_details.exec_id = to_base64(exec_id_numeric);

    p = decode_binary4_to_double(p, exec_details.filled_volume);
    p = decode_price(p, exec_details.price);
    p = decode_binary4_to_double(p, exec_details.active_volume);

    char liquidity_indicator_c;
    p = decode_char(p, liquidity_indicator_c);
    exec_details.liquidity_indicator = convert_to_liquidity_indicator(liquidity_indicator_c);

    // skip until optionals
    p += 7;
    decode_order_execution_opt_fields(p,
                                      exec_details.symbol,
                                      exec_details.last_mkt,
                                      exec_details.fee_code);
    return exec_details;
}

std::vector<unsigned char> request_optional_fields_for_message(const ResponseType response)
{
    switch (response) {
    case ResponseType::OrderExecution: {
        std::vector<unsigned char> result(order_execution_bitfield_num());
#define FIELD(_, bitfield_num, bit) result[bitfield_num - 1] |= bit;
#include "order_execution_opt_fields.inl"
        return result;
    }
    }
}
