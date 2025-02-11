
# 一、最简单的demo

使用 `pybind11` 可以让你轻松地将 C++ 代码与 Python 进行绑定，从而在 Python 中使用 C++ 写的高性能代码。下面，我将为你提供一个简单的 `pybind11` 示例，帮助你理解它的工作流程。

### **Pybind11 工作流程**

1. **编写 C++ 代码** ：你首先需要编写一个 C++ 文件，其中包含你想要在 Python 中使用的类、函数等。
2. **创建 Pybind11 绑定代码** ：然后，你需要写一段绑定代码，它将 C++ 类或函数暴露给 Python。
3. **编译 C++ 代码** ：你需要使用编译器将 C++ 代码与 pybind11 进行编译，并生成一个 Python 可用的扩展模块（共享库）。
4. **在 Python 中使用** ：通过 Python `import` 语句导入生成的扩展模块，就像使用普通 Python 库一样。

### **安装 pybind11**

如果你没有安装 `pybind11`，可以通过以下命令安装：

```bash
pip install pybind11
```

或者，如果你使用的是 `conda` 环境，可以通过 `conda` 安装：

```bash
conda install pybind11
```

### **步骤 1：编写 C++ 代码**

假设我们创建一个简单的 C++ 类 `Calculator`，它实现了加法和减法功能。

**calculator.cpp**

```cpp
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

// 绑定代码，暴露 Calculator 类到 Python
PYBIND11_MODULE(calculator, m) {
    py::class_<Calculator>(m, "Calculator")
        .def(py::init<>())  // 默认构造函数
        .def("add", &Calculator::add)
        .def("subtract", &Calculator::subtract);
}
```

在这段代码中：

* 我们首先定义了一个 `Calculator` 类，它有两个成员函数 `add` 和 `subtract`。
* 然后，我们使用 `PYBIND11_MODULE` 宏将这个类暴露到 Python 中。这里创建了一个名为 `calculator` 的 Python 扩展模块。

### **步骤 2：编写 CMake 构建文件**

为了编译 C++ 代码并生成 Python 扩展模块，我们使用 `CMake`。下面是一个简单的 `CMakeLists.txt` 文件，用于构建这个项目：

**CMakeLists.txt**

```cmake
cmake_minimum_required(VERSION 3.12)
project(CalculatorModule)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 11)

# 查找 pybind11
find_package(pybind11 REQUIRED)

# 创建 Python 扩展模块
pybind11_add_module(calculator calculator.cpp)
```

### **步骤 3：构建 C++ 代码**

1. **创建构建目录** ：
   在你的项目目录下，创建一个 `build` 目录用于构建：

```bash
   mkdir build
   cd build
```

1. **生成 Makefile** ：
   使用 `cmake` 生成构建文件：

```bash
   cmake ..
```

1. **构建扩展模块** ：
   使用 `make` 编译 C++ 代码并生成 Python 扩展：

```bash
   make
```

   这时，`calculator` 模块的 `.so` 或 `.pyd` 文件（具体取决于操作系统）将被生成。

### **步骤 4：在 Python 中使用**

生成模块后，你可以在 Python 中导入并使用它。假设你在当前目录下有 `calculator` 模块：

**test.py**

```python
import calculator

# 创建 Calculator 对象
calc = calculator.Calculator()

# 使用 add 和 subtract 方法
print(calc.add(3, 5))        # 输出 8
print(calc.subtract(10, 4))   # 输出 6
```

### **运行示例**

在命令行中执行：

```bash
python test.py
```

你应该会看到输出：

```
8
6
```

### **总结**

1. **C++ 代码** ：你首先定义一个 C++ 类或函数。
2. **Pybind11 绑定代码** ：然后，你使用 `pybind11` 编写绑定代码，将 C++ 代码暴露给 Python。
3. **编译** ：使用 `CMake` 或类似的构建工具编译 C++ 代码并生成 Python 扩展模块。
4. **Python 使用** ：最后，你可以像普通 Python 库一样使用生成的扩展模块。

### **扩展思路**

* **C++ 类的继承和多态** ：你可以将 C++ 中的类继承和多态机制暴露到 Python 中。
* **C++ STL 容器** ：你可以将 `std::vector`、`std::map` 等容器暴露给 Python，并在 Python 中进行操作。
* **C++ 性能优化** ：通过将性能瓶颈部分用 C++ 编写并暴露给 Python，你可以在不牺牲 Python 简单性的前提下提升性能。

`pybind11` 是一个非常强大的工具，它让 Python 和 C++ 的结合变得非常简单。通过这套流程，你可以将高效的 C++ 代码与 Python 的简便性结合起来。


# 二、编出的.so，如何引入python中


生成的 `.so` 文件是你通过 `pybind11` 绑定 C++ 代码后生成的 Python 扩展模块，它是一个共享库（Shared Object），可以在 Python 中导入并使用。接下来，我将为你详细介绍如何处理这个 `.so` 文件。

### **1. 确定 `.so` 文件的位置**

在完成 `make` 构建步骤后，你的 `.so` 文件（例如 `calculator.so`）通常会位于项目的 `build` 目录中，或者是与 `CMakeLists.txt` 中的构建设置相关的目录中。你可以通过以下方式找到 `.so` 文件：

```bash
find . -name "calculator.so"
```

假设 `.so` 文件的路径为 `build/calculator.so`。

### **2. 确保 Python 可以找到 `.so` 文件**

为了在 Python 中导入这个 `.so` 文件，你需要确保 Python 可以找到它。有几种方法可以做到这一点：

#### **方法 1：将 `.so` 文件放入当前目录**

最简单的方法是将生成的 `.so` 文件放置在你的 Python 脚本相同的目录中，或者直接在当前工作目录中。这时，你可以直接导入：

```python
import calculator

calc = calculator.Calculator()
print(calc.add(3, 5))  # 输出 8
```

#### **方法 2：将 `.so` 文件放入 Python 模块路径**

如果你不想将 `.so` 文件放在当前目录，可以将它移动到 Python 的模块搜索路径中。你可以通过以下方法将路径添加到 Python 的模块路径。

1. **将 `.so` 文件放到 Python 的 site-packages 目录** ，通常你可以在安装目录中找到该目录：

```bash
   cp build/calculator.so $(python -c "import site; print(site.getsitepackages()[0])")/calculator.so
```

1. **手动修改 `sys.path`** ：
   你可以在 Python 脚本中通过修改 `sys.path` 来添加 `.so` 文件所在的目录：

```python
   import sys
   sys.path.append("/path/to/your/so/file")  # 替换为实际路径

   import calculator

   calc = calculator.Calculator()
   print(calc.add(3, 5))  # 输出 8
```

#### **方法 3：设置 `PYTHONPATH` 环境变量**

你也可以通过设置 `PYTHONPATH` 环境变量，告诉 Python 解释器在哪里查找 `.so` 文件。假设 `.so` 文件位于 `/path/to/your/so/` 目录，你可以在终端中设置 `PYTHONPATH` 环境变量：

```bash
export PYTHONPATH=$PYTHONPATH:/path/to/your/so/
```

然后，你可以在 Python 脚本中正常导入：

```python
import calculator

calc = calculator.Calculator()
print(calc.add(3, 5))  # 输出 8
```

### **3. 使用 Python 代码调用 `.so` 文件**

当 `.so` 文件可以在 Python 中找到后，你可以像普通的 Python 模块一样导入并使用它。以下是使用刚刚生成的 `calculator.so` 模块的一个简单示例：

**test.py**

```python
import calculator  # 导入 calculator 模块

# 创建 Calculator 对象
calc = calculator.Calculator()

# 调用加法和减法函数
print(calc.add(3, 5))        # 输出 8
print(calc.subtract(10, 4))   # 输出 6
```

### **4. 处理不同平台**

如果你在不同的平台（Linux、macOS、Windows）上使用 `.so` 文件，注意：

* 在 **Linux** 上，文件扩展名通常是 `.so`。
* 在 **macOS** 上，文件扩展名通常是 `.dylib` 或 `.so`。
* 在 **Windows** 上，生成的文件通常是 `.pyd` 文件，它实际上是 Windows 下的 `.dll` 文件。

所以，确保根据你的操作系统生成和使用正确的文件类型。

### **5. 打包为 Python 包**

如果你希望分发你的 C++ 绑定代码，可以将 `.so` 文件和 Python 包一起打包。你可以将 `.so` 文件放到 `python` 包的 `ext_modules` 中，使用 `setuptools` 来打包 Python 包。

例如，你可以通过 `setuptools` 来编译并生成包：

**setup.py**

```python
from setuptools import setup, Extension

module = Extension(
    'calculator',  # 扩展模块的名称
    sources=['calculator.cpp'],  # C++ 源代码
    libraries=['pybind11'],  # 依赖的库
    include_dirs=['/path/to/pybind11/include'],  # pybind11 的头文件路径
    library_dirs=['/path/to/pybind11/lib'],  # pybind11 的库路径
)

setup(
    name='calculator',
    version='1.0',
    description='A Python interface for Calculator (C++ implemented)',
    ext_modules=[module],
)
```

然后，使用以下命令打包和安装：

```bash
python setup.py install
```

### **总结**

1. `.so` 文件是 Python 和 C++ 结合的扩展模块，它提供了高效的 C++ 实现，可以在 Python 中直接调用。
2. 你可以将 `.so` 文件放置在 Python 模块路径中，或者通过修改 `sys.path` 或设置 `PYTHONPATH` 环境变量来让 Python 找到它。
3. 在不同平台上生成的文件扩展名不同，要确保使用与平台匹配的文件类型。
4. 最后，你可以使用 `setuptools` 或其他打包工具来将 `.so` 文件打包成 Python 包，便于分发和安装。

通过这些方法，你可以灵活地将 C++ 代码与 Python 融合，并在 Python 中使用高性能的 C++ 代码。

# 三、pybind11绑定方法总结


在使用 `pybind11` 进行 C++ 与 Python 绑定时，你会遇到很多基础的绑定写法，涉及到类、函数、变量等不同的部分。为了帮助你系统地学习和查找相关内容，下面我将这些绑定写法按照类别进行整理和归类，涵盖常见的用法、常见的绑定模式以及一些进阶的绑定技巧。

---

### **1. 基本的类绑定**

#### **1.1 类的定义和绑定**

要将一个 C++ 类绑定到 Python 中，首先需要用 `pybind11` 创建一个模块，并且使用 `py::class` 进行绑定。

```cpp
#include <pybind11/pybind11.h>

namespace py = pybind11;

class MyClass {
public:
    MyClass() : value(0) {}
    void set_value(int v) { value = v; }
    int get_value() const { return value; }
private:
    int value;
};

PYBIND11_MODULE(example, m) {
    py::class_<MyClass>(m, "MyClass")
        .def(py::init<>())  // 构造函数绑定
        .def("set_value", &MyClass::set_value)  // 成员函数绑定
        .def("get_value", &MyClass::get_value);
}
```

* `py::class_<ClassType>` 用于将 C++ 类绑定到 Python。
* `py::init<>()` 用于绑定构造函数。
* `.def()` 用于绑定成员函数。

#### **1.2 绑定构造函数**

绑定构造函数时，可以指定多个构造函数或使用默认参数：

```cpp
py::class_<MyClass>(m, "MyClass")
    .def(py::init<>())  // 默认构造函数
    .def(py::init<int>(), py::arg("value"))  // 带参数的构造函数
    .def("set_value", &MyClass::set_value)
    .def("get_value", &MyClass::get_value);
```

---

### **2. 静态方法和类方法绑定**

#### **2.1 绑定静态方法**

C++ 中的静态方法可以使用 `.def_static()` 进行绑定。

```cpp
class MyClass {
public:
    static void static_method() {
        std::cout << "This is a static method!" << std::endl;
    }
};

PYBIND11_MODULE(example, m) {
    py::class_<MyClass>(m, "MyClass")
        .def_static("static_method", &MyClass::static_method);
}
```

#### **2.2 绑定类方法**

类方法使用 `.def()` 和类的实例进行绑定。

```cpp
class MyClass {
public:
    static void class_method(MyClass* self) {
        std::cout << "This is a class method with instance!" << std::endl;
    }
};

PYBIND11_MODULE(example, m) {
    py::class_<MyClass>(m, "MyClass")
        .def("class_method", &MyClass::class_method);
}
```

---

### **3. 成员变量绑定**

如果你想在 Python 中访问 C++ 类的成员变量，可以使用 `.def_readwrite()`（可读写）或 `.def_readonly()`（只读）。

#### **3.1 绑定可读写的成员变量**

```cpp
class MyClass {
public:
    int value;
};

PYBIND11_MODULE(example, m) {
    py::class_<MyClass>(m, "MyClass")
        .def_readwrite("value", &MyClass::value);
}
```

#### **3.2 绑定只读成员变量**

```cpp
class MyClass {
public:
    int value;
    int get_value() const { return value; }
};

PYBIND11_MODULE(example, m) {
    py::class_<MyClass>(m, "MyClass")
        .def_readonly("value", &MyClass::value)
        .def("get_value", &MyClass::get_value);
}
```

---

### **4. 绑定普通函数**

如果你有一个不属于类的普通函数，也可以将其绑定到 Python 中。

```cpp
int add(int a, int b) {
    return a + b;
}

PYBIND11_MODULE(example, m) {
    m.def("add", &add, "A function that adds two integers");
}
```

---

### **5. 绑定 lambda 和函数对象**

`pybind11` 支持将 C++ 中的 lambda 函数绑定到 Python。

#### **5.1 绑定普通的 lambda 函数**

```cpp
PYBIND11_MODULE(example, m) {
    m.def("lambda_example", [](int x, int y) { return x + y; });
}
```

#### **5.2 绑定自定义函数对象**

你可以将函数对象（例如 `std::function`）传递给 Python。

```cpp
#include <functional>

void process(const std::function<void(int)>& f) {
    f(42);
}

PYBIND11_MODULE(example, m) {
    m.def("process", &process);
}
```

---

### **6. 异常处理**

在 C++ 中，您可以通过 `pybind11` 抛出 Python 异常。

#### **6.1 抛出 Python 异常**

```cpp
#include <pybind11/pybind11.h>

namespace py = pybind11;

void my_function(int x) {
    if (x < 0) {
        throw py::value_error("x cannot be negative");
    }
}

PYBIND11_MODULE(example, m) {
    m.def("my_function", &my_function);
}
```

---

### **7. 绑定模板类**

如果你有模板类，`pybind11` 允许你将它们也绑定到 Python。

#### **7.1 绑定模板类**

```cpp
template <typename T>
class MyClass {
public:
    T value;
    MyClass(T v) : value(v) {}
    T get_value() const { return value; }
};

PYBIND11_MODULE(example, m) {
    py::class_<MyClass<int>>(m, "MyClassInt")
        .def(py::init<int>())
        .def("get_value", &MyClass<int>::get_value);
}
```

---

### **8. 自定义类型转换**

如果你有自定义的类型，并且希望将其自动转换为 Python 对象，可以定义转换函数。

#### **8.1 定义类型转换**

```cpp
class MyClass {
public:
    int value;
    MyClass(int v) : value(v) {}
};

namespace pybind11 { namespace detail {
    template <> struct type_caster<MyClass> {
    public:
        PYBIND11_TYPE_CASTER(MyClass, _("MyClass"));

        bool load(py::handle src, bool) {
            py::str s = src.cast<py::str>();
            value = std::stoi(s);
            return true;
        }

        static py::handle cast(MyClass src) {
            return py::str(std::to_string(src.value)).release();
        }
    };
}}
```

---

### **9. 其他常用功能**

#### **9.1 绑定迭代器**

你可以将 C++ 中的迭代器转换为 Python 中的可迭代对象。

```cpp
std::vector<int> get_vector() {
    return {1, 2, 3, 4};
}

PYBIND11_MODULE(example, m) {
    m.def("get_vector", &get_vector);
}
```

#### **9.2 绑定枚举类型**

```cpp
enum class MyEnum { Foo = 0, Bar = 1 };

PYBIND11_MODULE(example, m) {
    py::enum_<MyEnum>(m, "MyEnum")
        .value("Foo", MyEnum::Foo)
        .value("Bar", MyEnum::Bar)
        .export_values();
}
```

#### **9.3 绑定 `std::shared_ptr`**

`pybind11` 自动支持 `std::shared_ptr` 和 `std::unique_ptr`。

```cpp
std::shared_ptr<MyClass> create_object() {
    return std::make_shared<MyClass>();
}

PYBIND11_MODULE(example, m) {
    m.def("create_object", &create_object);
}
```

---

### **总结**

以上是 `pybind11` 常见的 C++ 和 Python 绑定的基础写法，按照功能分类：

1. **类绑定** ：包括构造函数、成员函数、静态方法、类方法、成员变量绑定。
2. **普通函数绑定** ：包括普通函数、lambda 函数、函数对象。
3. **异常处理** ：通过 `pybind11` 抛出 Python 异常。
4. **模板类绑定** ：模板类的绑定。
5. **类型转换** ：自定义类型和 Python 对象之间的转换。
6. **其他** ：如迭代器、枚举、`std::shared_ptr` 和 `std::unique_ptr` 的绑定。

通过这些常见的写法，你可以有效地将 C++ 代码暴露给 Python 使用，并灵活地处理各种复杂的绑定需求。

# End.报错

## 1、使用 `pip install pybind11`之后，cmake 报错

```
参照下面的方法：
1、使用下面的命令，输出pybind11的包路径
python -m pybind11 --cmake

2、在CMakeLists.txt中，设置pybind11包路径
set(pybind11_DIR "/home/feng/miniconda3/lib/python3.12/site-packages/pybind11/share/cmake/pybind11")

```


这个错误表示 `CMake` 没有找到 `pybind11` 的配置文件。`pybind11` 的安装可能没有正确配置到 CMake 路径中。解决这个问题有几个方法，下面列出几种可能的解决方案。

### **解决方案 1：使用 `pybind11` CMake 包**

`pybind11` 通常会提供一个 CMake 模块，可以通过 `find_package` 来找到并配置它。如果你使用的是 `pybind11` 的系统安装版，它可能没有包括 CMake 配置文件，或者 CMake 配置文件的路径没有被正确设置。

#### 步骤：

1. **查找 `pybind11Config.cmake` 的位置** ：
   你需要知道 `pybind11Config.cmake` 文件的位置，可以通过以下命令查找：

```bash
   find / -name pybind11Config.cmake
```

   或者你可以检查安装目录，通常 `pybind11` 配置文件应该在以下路径之一：

```bash
   /usr/local/lib/cmake/pybind11/
   /home/feng/miniconda3/lib/python3.12/site-packages/pybind11/share/cmake/pybind11/
```

1. **告诉 CMake `pybind11` 的路径** ：
   假设你找到 `pybind11Config.cmake` 文件的位置，例如 `/home/feng/miniconda3/lib/python3.12/site-packages/pybind11/share/cmake/pybind11/`，你需要将这个路径添加到 `CMAKE_PREFIX_PATH` 环境变量中，或者在 `CMakeLists.txt` 中明确指定。

* **使用环境变量** ：
  设置 `CMAKE_PREFIX_PATH` 环境变量，告诉 `CMake` 找到 `pybind11` 配置文件：
  ``bash export CMAKE_PREFIX_PATH=/home/feng/miniconda3/lib/python3.12/site-packages/pybind11/share/cmake/pybind11/:$CMAKE_PREFIX_PATH ``
* **修改 `CMakeLists.txt`** ：
  你可以在 `CMakeLists.txt` 文件中手动设置 `pybind11_DIR`，指向配置文件的目录：
  ``cmake set(pybind11_DIR "/home/feng/miniconda3/lib/python3.12/site-packages/pybind11/share/cmake/pybind11/") find_package(pybind11 REQUIRED) ``

   这样，CMake 就能找到 `pybind11` 的 CMake 配置文件。

### **解决方案 2：使用 `pybind11` 的 CMake 模块**

如果你没有找到 `pybind11Config.cmake` 文件，另一种方法是直接通过 `pybind11` 包的 CMake 模块来集成。

1. **克隆 `pybind11` 仓库** ：
   你可以直接从 GitHub 克隆 `pybind11` 仓库，并将其包含到项目中：

```bash
   git clone https://github.com/pybind/pybind11.git
```

1. **修改 `CMakeLists.txt`** ：
   在 `CMakeLists.txt` 中通过 `add_subdirectory` 引入 `pybind11`，这样你可以直接使用它：

```cmake
   # 假设 pybind11 存在于项目的 pybind11 目录下
   add_subdirectory(pybind11)

   pybind11_add_module(calculator calculator.cpp)
```

1. **重新运行 CMake** ：
   现在重新运行 `cmake` 和 `make`：

```bash
   cmake ..
   make
```

### **解决方案 3：使用 `pip` 安装 `pybind11` CMake 配置文件**

如果你已经通过 `pip` 安装了 `pybind11`，但仍然找不到配置文件，可以尝试以下方法确保 CMake 配置文件被正确安装。

1. **使用 `pip` 安装 `pybind11` 的开发版本** ：
   通过 `pip` 安装时，可能需要确保安装了 `pybind11` 的开发文件（包括 CMake 配置文件）。使用以下命令重新安装：

```bash
   pip install pybind11 --upgrade --no-cache-dir
```

1. **检查 `pybind11` 的安装路径** ：
   使用以下命令检查 `pybind11` 是否安装了 CMake 配置文件：

```bash
   python -m pybind11 --cmake
```

   该命令会输出 `pybind11` 的 CMake 配置路径，你可以使用该路径来配置 CMake。

### **解决方案 4：手动下载 CMake 配置文件**

如果上面的方法都无效，考虑手动下载 `pybind11Config.cmake` 文件并放置到正确的路径。你可以从 `pybind11` 的 GitHub 页面（[https://github.com/pybind/pybind11](https://github.com/pybind/pybind11)）下载 CMake 配置文件，并将其放置到正确的 CMake 搜索路径中。

---

尝试这些方法后，你应该能够解决 `CMake` 找不到 `pybind11` 配置文件的问题。
