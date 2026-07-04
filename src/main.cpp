#include <iostream>
#include <fstream>
#include <string>
#include <string_view>
#include "OrderBook.h"
#include "MarketDataParser.h"
#include "SimpleStrategy.h" 

int main() {
    OrderBook book;
    SimpleStrategy strategy(5);

    std::cout << "[System] 流式量化回测引擎启动..." << std::endl;

    // 打开磁盘行情 CSV 文件
    std::ifstream file("D:\\QuantEngine-Cpp\\market_data.csv");
    if (!file.is_open()) {
        std::cerr << "[Error] 无法打开行情文件，请检查文件路径" << std::endl;
        return -1;
    }

    std::string line;
    TickRecord record;
    uint64_t lineCount = 0;

    // 逐行读取流式行情数据
    while (std::getline(file, line)) {
        lineCount++;
        std::string_view lineView(line);

        // 1. 触发零拷贝数据解析
        if (MarketDataParser::parseLine(lineView, record)) {
            std::cout << "\n--------------------------------------------------" << std::endl;
            std::cout << "[Tick] 行号: " << lineCount
                << " | 时间: " << record.timestamp
                << " | 方向: " << (record.side == Side::BUY ? "BUY" : "SELL")
                << " | 价格: " << record.price
                << " | 数量: " << record.qty << std::endl;

            // 2. 触发策略流式计算
            strategy.onTick(record);

            // 3. 将订单提交至订单簿触发限价撮合
            book.submitOrder({ record.orderId, record.price, record.qty, record.side });

            // 4. 打印当前最新的盘口快照
            book.printBook();
        }
        else {
            std::cerr << "[Warning] 第 " << lineCount << " 行数据格式异常，已跳过。" << std::endl;
        }
    }

    std::cout << "\n--------------------------------------------------" << std::endl;
    std::cout << "[System] 回测流程执行完毕。" << std::endl;
    file.close();
    return 0;
}