#include "requests.h"

#include <iomanip>
#include <iostream>

namespace {

template <class T>
void print_binary(const T & x)
{
    size_t i = 0;
    const auto print8 = [&x, &i]() {
        for (size_t j = 0; i < x.size() && j < 8; ++i, ++j) {
            if (j != 0) {
                std::cout << ' ';
            }
            std::cout << std::hex << std::setfill('0') << std::setw(2) << static_cast<int>(x[i]);
        }
    };
    while (i < x.size()) {
        print8();
        if (i < x.size()) {
            std::cout << "  ";
            print8();
        }
        std::cout << std::endl;
    }
}

} // anonymous namespace

int main()
{
    const auto new_order_msg = create_new_order_request(1,
                                                        "ORD1001",
                                                        Side::Buy,
                                                        100,
                                                        12.505,
                                                        OrdType::Limit,
                                                        TimeInForce::Day,
                                                        10,
                                                        "AAPl",
                                                        Capacity::Principal,
                                                        "ACC331");
    print_binary(new_order_msg);

    const std::vector<unsigned char> message = {
            0xBA, 0xBA, 0x5A, 0x00, 0x2C, 0x03, 0x64, 0x00, 0x00, 0x00, 0xE0, 0xFA, 0x20, 0xF7, 0x36, 0x71, 0xF8, 0x11, 0x41, 0x42, 0x43, 0x31, 0x32, 0x33, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xF0, 0xB7, 0xD9, 0x71, 0x21, 0x00, 0x00, 0x64, 0x00, 0x00, 0x00, 0x08, 0xE2, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x41, 0x00, 0x42, 0x41, 0x54, 0x53, 0x00, 0x08, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x80, 0x01, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x32, 0x58, 0x53, 0x54, 0x4F, 0x52, 0x47};

    ExecutionDetails decodedData = decode_order_execution(message);
    std::cout << "cl_ord_id: " << decodedData.cl_ord_id << std::endl;
    std::cout << "exec_id: " << decodedData.exec_id << std::endl;
    std::cout << "filled_volume: " << decodedData.filled_volume << std::endl;
    std::cout << "active_volume: " << decodedData.active_volume << std::endl;
    std::cout << "price: " << decodedData.price << std::endl;
    std::cout << "liquidity_indicator: " << static_cast<int>(decodedData.liquidity_indicator) << std::endl;
    std::cout << "symbol: " << decodedData.symbol << std::endl;
    std::cout << "last_mkt: " << decodedData.last_mkt << std::endl;
    std::cout << "fee_code: " << decodedData.fee_code << std::endl;
    return 0;
}
