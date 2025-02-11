#include <pybind11/pybind11.h>

namespace py = pybind11;

class Calculator {
    public:
        Calculator() = default;
    
        // 加法
        int add(int a, int b) {
            return a + b;
        }
    
        // 减法
        int subtract(int a, int b) {
            return a - b;
        }
};

PYBIND11_MODULE(calculator, m) {
    py::class_<Calculator>(m, "Calculator")
        .def(py::init<>()) // 默认构造函数
        .def("add", &Calculator::add) // 加法
        .def("sub", &Calculator::subtract); // 减法
}