#pragma once

#include "fields.h"

#include <algorithm>
#include <array>
#include <string>
#include <vector>

/*
 * New Order
 *  Price(1,4)
 *  OrdType(1,16)
 *  TimeInForce(1,32)
 *  MaxFloor(1,128)
 *  Symbol(2,1)
 *  Capacity(2,64)
 *  Account(3,1)
 */

constexpr size_t new_order_bitfield_num()
{
    return std::max({0
#define FIELD(_, n, __) , n
#include "new_order_opt_fields.inl"
    });
}

constexpr size_t new_order_opt_fields_size()
{
    return 0
#define FIELD(name, _, __) +name##_field_size
#include "new_order_opt_fields.inl"
            ;
}

enum class RequestType
{
    New
};

constexpr size_t calculate_size(const RequestType type)
{
    switch (type) {
    case RequestType::New:
        return 36 + new_order_bitfield_num() + new_order_opt_fields_size();
    }
}

enum class Side
{
    Buy,
    Sell
};

enum class OrdType
{
    Market,
    Limit,
    Pegged
};

enum class TimeInForce
{
    Day,
    IOC,
    GTD
};

enum class Capacity
{
    Agency,
    Principal,
    RisklessPrincipal
};

std::array<unsigned char, calculate_size(RequestType::New)> create_new_order_request(
        unsigned seq_no,
        const std::string & cl_ord_id,
        Side side,
        double volume,
        double price,
        OrdType ord_type,
        TimeInForce time_in_force,
        double max_floor,
        const std::string & symbol,
        Capacity capacity,
        const std::string & account);

/*
 * Order Execution
 */

constexpr size_t order_execution_bitfield_num()
{
    return std::max({0
#define FIELD(_, n, __) , n
#include "order_execution_opt_fields.inl"
    });
}

enum class ResponseType
{
    OrderExecution
};

std::vector<unsigned char> request_optional_fields_for_message(ResponseType);

enum class LiquidityIndicator
{
    Added,
    Removed
};

struct ExecutionDetails
{
    std::string cl_ord_id;
    std::string exec_id;
    double filled_volume;
    double active_volume;
    double price;
    LiquidityIndicator liquidity_indicator;
    std::string symbol;
    std::string last_mkt;
    std::string fee_code;
};

ExecutionDetails decode_order_execution(const std::vector<unsigned char> & message);
