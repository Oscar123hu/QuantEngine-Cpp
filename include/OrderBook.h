#pragma once
#include <map>
#include <list>
#include <cstdint>

// 交易方向：买入或卖出
enum class Side { BUY, SELL };

// 单条订单基础数据
struct Order {
    uint64_t orderId;
    double price;
    uint32_t qty;
    Side side;
};

// 限价订单簿核心类
class OrderBook {
private:
    // 买单队列：价格从高到低排序 (std::greater)
    std::map<double, std::list<Order>, std::greater<double>> buyOrders;

    // 卖单队列：价格从低到高排序 (std::less)
    std::map<double, std::list<Order>, std::less<double>> sellOrders;

public:
    // 提交新订单并触发撮合逻辑
    void submitOrder(Order order);

    // 打印当前的买卖盘口数据
    void printBook() const;
};