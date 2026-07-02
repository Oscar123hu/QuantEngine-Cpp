#include "OrderBook.h"

int main() {
    OrderBook book;

    std::cout << "========= 量化本地测试 =========" << std::endl;

    // 场景 1：初始化市场，有两位卖家在排队挂单
    std::cout << "\n[动作] 散户 A 挂单：以 100.50 元 卖出 10 手" << std::endl;
    book.submitOrder({ 1001, 100.5, 10, Side::SELL });

    std::cout << "[动作] 散户 B 挂单：以 100.30 元 卖出 5 手 (更低更优质的要价)" << std::endl;
    book.submitOrder({ 1002, 100.3, 5, Side::SELL });

    // 观察初始盘口
    book.printBook();

    // 场景 2：重头戏！来了一个好战的机构大买单，出价极高，企图扫盘
    std::cout << "\n[动作] 机构 C 砸单：以 100.60 元 强行买入 12 手" << std::endl;
    book.submitOrder({ 2001, 100.6, 12, Side::BUY });

    // 再次观察撮合后的盘口快照
    book.printBook();

    std::cout << "========= 测试结束，状态完美 =========" << std::endl;
    return 0;
}