# 1. CMake 的最低版本
cmake_minimum_required(VERSION 3.15)

# 2. 项目名称
project(QuantEngine)

# 3.  C++ 语言标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 4. 头文件位置
include_directories(include)

# 5. 核心命令：指定要编译出来的程序名字，以及生产它需要哪些源文件（.cpp）
add_executable(QuantEngine src/main.cpp src/OrderBook.cpp)