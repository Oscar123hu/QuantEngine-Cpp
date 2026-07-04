#include "MarketDataParser.h"
#include <charconv>

bool MarketDataParser::parseLine(std::string_view line, TickRecord& record) {
    if (line.empty()) return false;

    // 存储切分出来的 5 个 CSV 字段
    std::string_view fields[5];
    size_t start = 0;
    int fieldIdx = 0;

    // 按逗号循环切分前 4 个字段
    while (fieldIdx < 4) {
        size_t end = line.find(',', start);
        if (end == std::string_view::npos) return false; // 逗号数量不匹配则返回失败

        fields[fieldIdx++] = line.substr(start, end - start);
        start = end + 1;
    }
    // 最后一个字段（Side）切分到行尾
    fields[fieldIdx] = line.substr(start);

    // 1. 映射时间戳字段
    record.timestamp = fields[0];

    // 2. 解析订单ID (uint64_t)
    auto [ptr1, ec1] = std::from_chars(fields[1].data(), fields[1].data() + fields[1].size(), record.orderId);
    if (ec1 != std::errc()) return false;

    // 3. 解析价格 (double)
    auto [ptr2, ec2] = std::from_chars(fields[2].data(), fields[2].data() + fields[2].size(), record.price);
    if (ec2 != std::errc()) return false;

    // 4. 解析数量 (uint32_t)
    auto [ptr3, ec3] = std::from_chars(fields[3].data(), fields[3].data() + fields[3].size(), record.qty);
    if (ec3 != std::errc()) return false;

    // 5. 映射买卖方向
    if (fields[4] == "BUY") {
        record.side = Side::BUY;
    }
    else if (fields[4] == "SELL") {
        record.side = Side::SELL;
    }
    else {
        return false;
    }

    return true;
}