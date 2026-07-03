#pragma once
#include <iostream>
#include "MarketDataParser.h"

class SimpleStrategy {
private:
    double m_ema = 0.0;          // 核心状态：只保存上一次的 EMA 值，空间复杂度 O(1)
    bool m_isInitialized = false; // 标记是否是第一条数据
    double m_alpha;              // EMA 平滑系数

public:
    // 默认构造一个 5 周期的 EMA 均线策略
    SimpleStrategy(int period = 5) {
        m_alpha = 2.0 / (period + 1);
    }

    // 流式数据注入接口：每流入一个 Tick 价格，顺时触发增量数学计算
    void onTick(const TickRecord& record) {
        double currentPrice = record.price;

        // 1. 递推算法初始化
        if (!m_isInitialized) {
            m_ema = currentPrice; // 第一条数据的 EMA 等于它本身
            m_isInitialized = true;
            std::cout << "  [策略初始化] 均线初始定位至: " << m_ema << std::endl;
            return;
        }

        // 2. 核心数学公式递推：EMA_t = alpha * Price_t + (1 - alpha) * EMA_{t-1}
        // 全程仅需一次乘加运算，速度极快，完美保护 CPU L1/L2 缓存
        double lastEma = m_ema;
        m_ema = m_alpha * currentPrice + (1.0 - m_alpha) * lastEma;

        std::cout << "  [策略计算] 当前价格: " << currentPrice << " | 实时 EMA(5): " << m_ema;

        // 3. 策略信号激发逻辑 (简单的均线交叉/突破策略)
        // 如果是买单流，且当前价格向上突破了 EMA 均线 -> 激发做多信号
        if (record.side == Side::BUY && currentPrice > m_ema && currentPrice >= lastEma) {
            std::cout << " 🔥【策略信号：⚡ BUY ⚡】价格向上突破均线，看多！";
        }
        // 如果是卖单流，且当前价格向下击穿了 EMA 均线 -> 激发做空信号
        else if (record.side == Side::SELL && currentPrice < m_ema && currentPrice <= lastEma) {
            std::cout << " ❄️【策略信号：💥 SELL 💥】价格向下击穿均线，看空！";
        }

        std::cout << std::endl;
    }
};