#pragma once
#include <iostream>
#include "MarketDataParser.h"

class SimpleStrategy {
private:
    double m_ema = 0.0;           // 当前的 EMA 均线值
    bool m_isInitialized = false; // 是否完成首次初始化
    double m_alpha;               // EMA 平滑系数

public:
    // 构造函数，默认初始化 5 周期 EMA
    SimpleStrategy(int period = 5) {
        m_alpha = 2.0 / (period + 1);
    }

    // 接收 Tick 行情流并触发增量数学计算
    void onTick(const TickRecord& record) {
        double currentPrice = record.price;

        // 首次数据输入，完成递推初始化
        if (!m_isInitialized) {
            m_ema = currentPrice;
            m_isInitialized = true;
            std::cout << "[Strategy Init] 均线初始值定位至: " << m_ema << std::endl;
            return;
        }

        // 核心 EMA 递推公式: EMA = alpha * Price + (1 - alpha) * last_EMA
        double lastEma = m_ema;
        m_ema = m_alpha * currentPrice + (1.0 - m_alpha) * lastEma;

        std::cout << "[Strategy Run] 当前价: " << currentPrice << " | 实时 EMA: " << m_ema;

        // 均线突破策略逻辑
        // 买单流且当前价格向上突破 EMA 均线
        if (record.side == Side::BUY && currentPrice > m_ema && currentPrice >= lastEma) {
            std::cout << " -> [SIGNAL: BUY] 价格向上突破均线，看多";
        }
        // 卖单流且当前价格向下击穿 EMA 均线
        else if (record.side == Side::SELL && currentPrice < m_ema && currentPrice <= lastEma) {
            std::cout << " -> [SIGNAL: SELL] 价格向下击穿均线，看空";
        }

        std::cout << std::endl;
    }

    // 获取当前的 EMA 均线值
    double getEma() const { return m_ema; }
};