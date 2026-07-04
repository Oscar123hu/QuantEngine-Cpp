#pragma once
#include <string_view>
#include <cstdint>
#include "OrderBook.h"

struct TickRecord {
    std::string_view timestamp;
    uint64_t orderId{ 0 };
    double price{ 0.0 };
    uint32_t qty{ 0 };
    Side side{ Side::BUY }; 
};

class MarketDataParser {
public:
    static bool parseLine(std::string_view line, TickRecord& record);
};