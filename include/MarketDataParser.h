#pragma once
#include <string_view>
#include <vector>
#include "OrderBook.h"

// 每一行 Tick 行情流转的原始记录
struct TickRecord {
    std::string_view timestamp; // 零拷贝保留时间戳字符串
    uint64_t orderId;
    double price;
    uint32_t qty;
    Side side;
};

class MarketDataParser {
public:
    // 高性能流式解析单行 CSV 文本，成功返回 true 并填充 record
    static bool parseLine(std::string_view line, TickRecord& record);
};