# QuantEngine-Cpp 📈
![Language](https://img.shields.io/badge/Language-C%2B%2B17-blue)
![Build](https://img.shields.io/badge/Build-CMake-orange)
![Platform](https://img.shields.io/badge/Platform-Windows%20%7C%20Linux-lightgrey)
![IDE](https://img.shields.io/badge/IDE-VS%202022-purple)
![License](https://img.shields.io/badge/License-MIT-green)
本项目是一个基于 **C++17** 标准开发的高性能、低延迟流式限价订单簿（Limit Order Book, LOB）与核心撮合引擎内核。旨在模拟高频交易场景下的资产极速流转，展现硬核的 408 数据结构底层功底与极致的性能压榨。

---

## ✨ 核心亮点与 408 数据结构设计 (面试必面)

为了在低延迟的量化交易中达到极高的吞吐量，本项目彻底抛弃了低效的全局扫描，采用了**“红黑树（树级）+ 双向链表（档位级）”**的复合型拓扑架构：

1. **多级盘口检索 (std::map / 红黑树)**
   * **买盘 (Bids)**：采用 `std::greater<double>` 排序的红黑树，使“买一”盘口（全场最高买价）永远维持在树的最左侧节点，获取复杂度为 $O(1)$。
   * **卖盘 (Asks)**：采用 `std::less<double>` 排序的红黑树，使“卖一”盘口（全场最优质低卖价）完美暴露，便于极速扫盘。
   * 新价格档位的插入与删除，最坏时间复杂度严格控制在 $O(\log N)$。

2. **流式时间优先队列 (std::list / 双向链表)**
   * 每个价格档位内部嵌套一个双向链表，新单通过 `.push_back()` 挂在队尾排队，撮合时通过 `.front()` 从队头依次消费，完美遵循交易所**“价格优先、时间优先”**的铁律。
   * **$O(1)$ 级撤单**：高频交易中撤单频繁，双向链表支持在持有迭代器指针的情况下，在 $O(1)$ 时间复杂度内将任意委托单从队列中“无痕挖除”。

3. **现代 C++ 工程安全性**
   * 引入 `enum class` 强类型枚举严控交易方向（BUY/SELL），在编译期锁死类型安全漏洞。

---

## 🛠️ 构建与运行指南

本项目采用标准现代 CMake 构建系统，无任何第三方库依赖，具备极强的跨平台通用性。

### 编译步骤
```powershell
# 1. 创建并进入构建沙盒
cmake -B build

# 2. 执行全量编译
cmake --build build
