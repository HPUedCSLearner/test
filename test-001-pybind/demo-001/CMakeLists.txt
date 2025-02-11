cmake_minimum_required(VERSION 3.12)
project(CalculatorModule)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 11)

# 查找 pybind11
set(pybind11_DIR "/home/feng/miniconda3/lib/python3.12/site-packages/pybind11/share/cmake/pybind11")
find_package(pybind11 REQUIRED)

# 创建 Python 扩展模块
pybind11_add_module(calculator calculator.cpp)
