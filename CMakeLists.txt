cmake_minimum_required(VERSION 3.15)
project(QuantEngine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 包含头文件目录
include_directories(include)

# 核心：必须精确指定这两个源文件参与编译！
add_executable(QuantEngine src/main.cpp src/OrderBook.cpp src/MarketDataParser.cpp)