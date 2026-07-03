#include "MarketDataParser.h"
#include <charconv>
#include <iostream>

bool MarketDataParser::parseLine(std::string_view line, TickRecord& record) {
    if (line.empty()) return false;

    // 存储切分出来的 5 个字段的 string_view
    std::string_view fields[5];
    size_t start = 0;
    int fieldIdx = 0;

    // --- 零拷贝切片核心：利用指针移动和 substr，全程不发生堆内存开辟 ---
    while (fieldIdx < 4) {
        size_t end = line.find(',', start);
        if (end == std::string_view::npos) return false; // 逗号数量不对，格式错误

        fields[fieldIdx++] = line.substr(start, end - start);
        start = end + 1;
    }
    // 最后一个字段（Side）到行尾
    fields[fieldIdx] = line.substr(start);

    // --- 字段映射与底层极速类型转换 ---
    // 1. Timestamp
    record.timestamp = fields[0];

    // 2. OrderId (uint64_t)
    // std::from_chars 接收区间：[首地址, 尾地址)
    auto [ptr1, ec1] = std::from_chars(fields[1].data(), fields[1].data() + fields[1].size(), record.orderId);
    if (ec1 != std::errc()) return false; // 转换失败

    // 3. Price (double)
    auto [ptr2, ec2] = std::from_chars(fields[2].data(), fields[2].data() + fields[2].size(), record.price);
    if (ec2 != std::errc()) return false;

    // 4. Qty (uint32_t)
    auto [ptr3, ec3] = std::from_chars(fields[3].data(), fields[3].data() + fields[3].size(), record.qty);
    if (ec3 != std::errc()) return false;

    // 5. Side (方向映射)
    if (fields[4] == "BUY") {
        record.side = Side::BUY;
    }
    else if (fields[4] == "SELL") {
        record.side = Side::SELL;
    }
    else {
        return false; // 非法方向
    }

    return true;
}