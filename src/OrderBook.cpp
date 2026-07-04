#include "OrderBook.h"
#include <iostream>
#include <algorithm>
#include <iomanip>

// 核心订单提交与撮合函数
void OrderBook::submitOrder(Order order) {
    if (order.side == Side::BUY) {
        // --- 买单撮合逻辑 ---
        // 循环条件：买单有剩余数量、卖盘不为空、且买入价 >= 卖盘最低要价
        while (order.qty > 0 && !sellOrders.empty() && order.price >= sellOrders.begin()->first) {

            // 获取当前最低卖价档位及其订单链表
            double bestSellPrice = sellOrders.begin()->first;
            auto& bestSellList = sellOrders.begin()->second;

            // 遍历该价格档位下的所有卖单（先进先出原则）
            while (!bestSellList.empty() && order.qty > 0) {
                auto& sellOrder = bestSellList.front();

                // 取买卖双方数量的较小值作为本次成交量
                uint32_t matchQty = std::min(order.qty, sellOrder.qty);

                // 扣减双方的未成交数量
                order.qty -= matchQty;
                sellOrder.qty -= matchQty;

                std::cout << "[撮合成功] 买单ID: " << order.orderId
                    << " <=> 卖单ID: " << sellOrder.orderId
                    << " | 价格: " << std::fixed << std::setprecision(2) << bestSellPrice
                    << " | 数量: " << matchQty << std::endl;

                // 若挂单完全成交，从链表中移除
                if (sellOrder.qty == 0) {
                    bestSellList.pop_front();
                }
            }

            // 若该价格档位已无订单，从 map 中清除该节点
            if (bestSellList.empty()) {
                sellOrders.erase(sellOrders.begin());
            }
        }

        // 若买单仍有剩余数量，挂入买盘队列排队
        if (order.qty > 0) {
            buyOrders[order.price].push_back(order);
        }

    }
    else {
        // --- 卖单撮合逻辑 ---
        // 循环条件：卖单有剩余数量、买盘不为空、且卖出价 <= 买盘最高出价
        while (order.qty > 0 && !buyOrders.empty() && order.price <= buyOrders.begin()->first) {

            // 获取当前最高买价档位及其订单链表
            double bestBuyPrice = buyOrders.begin()->first;
            auto& bestBuyList = buyOrders.begin()->second;

            // 遍历该价格档位下的所有买单
            while (!bestBuyList.empty() && order.qty > 0) {
                auto& buyOrder = bestBuyList.front();

                uint32_t matchQty = std::min(order.qty, buyOrder.qty);
                order.qty -= matchQty;
                buyOrder.qty -= matchQty;

                std::cout << "[撮合成功] 卖单ID: " << order.orderId
                    << " <=> 买单ID: " << buyOrder.orderId
                    << " | 价格: " << std::fixed << std::setprecision(2) << bestBuyPrice
                    << " | 数量: " << matchQty << std::endl;

                if (buyOrder.qty == 0) {
                    bestBuyList.pop_front();
                }
            }

            if (bestBuyList.empty()) {
                buyOrders.erase(buyOrders.begin());
            }
        }

        // 若卖单仍有剩余数量，挂入卖盘队列排队
        if (order.qty > 0) {
            sellOrders[order.price].push_back(order);
        }
    }
}

// 打印当前买卖盘口快照
void OrderBook::printBook() const {
    std::cout << "\n======= CURRENT ORDER BOOK =======" << std::endl;
    std::cout << "--- ASKS (卖盘) ---" << std::endl;
    for (auto it = sellOrders.rbegin(); it != sellOrders.rend(); ++it) {
        uint32_t totalQty = 0;
        for (const auto& ord : it->second) totalQty += ord.qty;
        std::cout << "  价格: " << std::fixed << std::setprecision(2) << it->first
            << " | 总数量: " << totalQty << " (" << it->second.size() << " 笔单)" << std::endl;
    }

    std::cout << "----------------------------------" << std::endl;
    std::cout << "--- BIDS (买盘) ---" << std::endl;
    for (const auto& pair : buyOrders) {
        uint32_t totalQty = 0;
        for (const auto& ord : pair.second) totalQty += ord.qty;
        std::cout << "  价格: " << std::fixed << std::setprecision(2) << pair.first
            << " | 总数量: " << totalQty << " (" << pair.second.size() << " 笔单)" << std::endl;
    }
    std::cout << "==================================\n" << std::endl;
}