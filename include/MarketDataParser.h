#pragma once
#include <string_view>
#include <cstdint>
#include "OrderBook.h"

/**
 * @brief Representation of a raw tick market data row.
 */
struct TickRecord {
    std::string_view timestamp; // Zero-copy view into origin input buffer
    uint64_t orderId{0};
    double price{0.0};
    uint32_t qty{0};
    Side side{Side::Buy};
};

/**
 * @brief High-performance streaming market data parser.
 */
class MarketDataParser {
public:
    /**
     * @brief Parses a single CSV line into a TickRecord structure.
     * @param line Raw input text line from data source.
     * @param record Output reference to be populated.
     * @return true if parsing succeeded, false otherwise.
     */
    static bool parseLine(std::string_view line, TickRecord& record);
};