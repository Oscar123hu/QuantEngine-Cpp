#pragma once
#include <iostream>
#include <map>
#include <list>
#include <string>

enum class Side { BUY, SELL };

struct Order {
    uint64_t orderId;
    double price;
    uint32_t qty;
    Side side;
};

class OrderBook {
private:
    std::map<double, std::list<Order>, std::greater<double>> buyOrders;
    std::map<double, std::list<Order>, std::less<double>> sellOrders;

public:
    void submitOrder(Order order);
    void printBook() const;
};