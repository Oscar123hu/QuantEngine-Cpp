# QuantEngine-Cpp 📈

![Language](https://img.shields.io/badge/Language-C%2B%2B17-blue) ![Build](https://img.shields.io/badge/Build-CMake-orange) ![License](https://img.shields.io/badge/License-MIT-green)

`QuantEngine-Cpp` 是一个基于 *C++17* 标准开发的高性能、低延迟限价订单簿（Limit Order Book, LOB）撮合内核。项目专注于在流式高频数据流入场景下，实现超低延迟的资产撮合逻辑与盘口管理。

---

## ⚙️ 核心架构与数据结构设计

为了满足量化交易系统对低延迟与高吞吐的极端要求，本项目在盘口设计上放弃了全局线性扫描，采用了*“自平衡二叉树（树级） + 双向链表（档位级）”*的复合拓扑架构：

### 1. 多级盘口快速检索 (std::map)
* **买盘 (Bids)**：采用 `std::greater<double>` 排序的红黑树，使市场“买一”价格（最高出价）始终维持在树的最左侧根节点，获取盘口时间复杂度为 $O(1)$。
* **卖盘 (Asks)**：采用 `std::less<double>` 排序的红黑树，使市场“卖一”价格（最低要价）自动置顶，便于极速扫盘。
* 新价格档位的插入与删除，其最坏时间复杂度严格控制在 $O(\log N)$。

### 2. 流式队列与时间优先原则 (std::list)
* 每个独立的价格档位内部嵌套一个双向链表。新订单通过 `.push_back()` 追加至队尾，撮合时通过 `.front()` 从队头依次消费，严格遵循 **FIFO（先进先出）** 的时间优先原则。
* **$O(1)$ 级无痕撤单**：高频交易中撤单行为极多。双向链表支持在持有迭代器指针的情况下，在 $O(1)$ 时间内将任意委托单从队列任意位置中拔除，避免了数组搬移开销。

### 3. 类型安全
* 核心交易方向（BUY/SELL）采用 `enum class` 强类型枚举进行隔离，在编译期杜绝了传统枚举常出现的命名空间污染与隐式类型转换漏洞。

---

## 🛠️ 构建与运行指南

本项目采用标准的 CMake 构建系统，无任何第三方库依赖，具备极强的通用性。

### 1. 编译构建
```powershell
# 创建并进入构建沙盒
cmake -B build

# 执行全量编译
cmake --build build
