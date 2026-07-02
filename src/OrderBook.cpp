#include "OrderBook.h"
#include <algorithm>
#include <iomanip>

// 核心撮合函数
void OrderBook::submitOrder(Order order) {
    if (order.side == Side::BUY) {
        // --- 【买单撮合逻辑】 ---
        // 目标：去卖盘（sellOrders）里找最便宜的挂单成交

        // 外层循环：只要买单还有剩余数量、卖盘不为空、且买入价 >= 卖盘最低要价（跨越价差）
        while (order.qty > 0 && !sellOrders.empty() && order.price >= sellOrders.begin()->first) {

            // 拿到当前卖盘口（卖一）的价格档位和对应的订单链表
            double bestSellPrice = sellOrders.begin()->first;
            auto& bestSellList = sellOrders.begin()->second;

            // 内层循环：遍历该价格档位下的双向链表，按时间优先原则依次吃掉挂单
            while (!bestSellList.empty() && order.qty > 0) {
                auto& sellOrder = bestSellList.front(); // 取出链表最前面（最老）的订单

                // 计算本次撮合的成交数量（谁的数量少，就以谁为准）
                uint32_t matchQty = std::min(order.qty, sellOrder.qty);

                // 扣减各自的剩余数量
                order.qty -= matchQty;
                sellOrder.qty -= matchQty;

                std::cout << "[撮合成功] 买单ID: " << order.orderId
                    << " <=> 卖单ID: " << sellOrder.orderId
                    << " | 价格: " << std::fixed << std::setprecision(2) << bestSellPrice
                    << " | 数量: " << matchQty << std::endl;

                // 如果对方的挂单被完全吃饱（数量变0），从双向链表中踢出
                if (sellOrder.qty == 0) {
                    bestSellList.pop_front(); // O(1) 复杂度
                }
            }

            // 如果这个价格档位的所有卖单全被吃光了，把这个价格节点从红黑树里连根拔除
            if (bestSellList.empty()) {
                sellOrders.erase(sellOrders.begin()); // 迭代器删除，摊还复杂度为 O(1)
            }
        }

        // 如果把对面的卖盘扫完之后，买单自己还有剩余数量，就得老老实实挂入买盘红黑树排队
        if (order.qty > 0) {
            buyOrders[order.price].push_back(order); // 红黑树插入复杂度 O(log N)
        }

    }
    else {
        // --- 【卖单撮合逻辑】 ---
        // 目标：去买盘（buyOrders）里找最贵的买单成交（逻辑与上面完全镜像）

        while (order.qty > 0 && !buyOrders.empty() && order.price <= buyOrders.begin()->first) {
            double bestBuyPrice = buyOrders.begin()->first;
            auto& bestBuyList = buyOrders.begin()->second;

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

        if (order.qty > 0) {
            sellOrders[order.price].push_back(order);
        }
    }
}

// 辅助函数：漂亮地打印当前盘口快照
void OrderBook::printBook() const {
    std::cout << "\n======= CURRENT ORDER BOOK =======" << std::endl;
    std::cout << "--- ASKS (卖盘) ---" << std::endl;
    // 卖盘是从小到大排，为了视觉习惯，我们倒序遍历，让高价在上面
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