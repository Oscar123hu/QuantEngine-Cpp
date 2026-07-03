#include <iostream>
#include <vector>
#include "OrderBook.h"
#include "MarketDataParser.h"

int main() {
    OrderBook book;

    std::cout << "========= 零拷贝流式 CSV 行情解析测试 =========" << std::endl;

    // 模拟从大型历史行情文件（CSV）中读取出来的流式行文本
    std::vector<std::string_view> mockCsvFile = {
        "1672531199000,1001,100.50,10,SELL",
        "1672531199005,1002,100.30,5,SELL",
        "1672531199010,2001,100.60,12,BUY"
    };

    TickRecord record;
    for (std::string_view line : mockCsvFile) {
        std::cout << "\n[网络行情流滚入] 原始行: " << line << std::endl;

        // 1. 触发零拷贝解析
        if (MarketDataParser::parseLine(line, record)) {
            std::cout << "  └─ [解析成功] 零拷贝时间戳: " << record.timestamp
                << " | 价格: " << record.price
                << " | 数量: " << record.qty << std::endl;

            // 2. 直接将解析出来的结构体喂进撮合内核
            book.submitOrder({ record.orderId, record.price, record.qty, record.side });
        }
        else {
            std::cerr << "  └─ [解析失败] 数据行格式损坏！" << std::endl;
        }

        // 3. 打印实时动态盘口
        book.printBook();
    }

    std::cout << "========= 历史数据回测流运行结束 =========" << std::endl;
    return 0;
}