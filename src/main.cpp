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

    std::cout << "========= 阶段三终极合体：流式量化回测系统启动 =========" << std::endl;

    // 打开真实的磁盘行情 CSV 文件（如果 VS 提示找不到，请自行改为绝对路径）
    std::ifstream file("D:\\QuantEngine-Cpp\\market_data.csv");
    if (!file.is_open()) {
        std::cerr << "[严重错误] 无法打开行情文件！请确认路径是否正确。" << std::endl;
        return -1;
    }

    std::string line;
    TickRecord record;
    uint64_t lineCount = 0;

    // 流式吞噬循环开始
    while (std::getline(file, line)) {
        lineCount++;
        std::string_view lineView(line);

        // 1. 触发零拷贝解析
        if (MarketDataParser::parseLine(lineView, record)) {
            std::cout << "\n--------------------------------------------------" << std::endl;
            std::cout << "[数据流激滚入] 行号: " << lineCount
                << " | 时间戳: " << record.timestamp
                << " | 方向: " << (record.side == Side::BUY ? "BUY" : "SELL")
                << " | 价格: " << record.price
                << " | 数量: " << record.qty << std::endl;

            // 2. 喂给策略大脑，实时增量更新数学指标并尝试激发信号
            strategy.onTick(record);

            // 3. 喂给订单簿心脏，触发限价撮合流
            book.submitOrder({ record.orderId, record.price, record.qty, record.side });

            // 4. 打印当前最新的动态盘口
            book.printBook();
        }
        else {
            std::cerr << "[警告] 第 " << lineCount << " 行数据格式损坏，自动跳过。" << std::endl;
        }
    }

    std::cout << "\n==================================================" << std::endl;
    std::cout << "========= 回测流运行结束，系统状态完美！ =========" << std::endl;
    file.close();
    return 0;
}