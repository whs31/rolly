# Leaf
<h3 align="center">
    <img src="docs/images/logo.svg" alt=""/><br/>
    <img src="https://raw.githubusercontent.com/catppuccin/catppuccin/main/assets/misc/transparent.png" height="30" width="0px"/>
    Lightweight essentials and functions library
    <img src="https://raw.githubusercontent.com/catppuccin/catppuccin/main/assets/misc/transparent.png" height="30" width="0px"/><br/>
    <img src="https://raw.githubusercontent.com/catppuccin/catppuccin/main/assets/misc/transparent.png" height="30" width="0px"/><br/>
    <img src="https://img.shields.io/badge/C%2B%2B-00599C?style=for-the-badge&logo=c%2B%2B&logoColor=white" alt="cpp_badge"/><img src="https://img.shields.io/badge/CMake-064F8C?style=for-the-badge&logo=cmake&logoColor=white" alt="cmake_badge"/>
</h3>

### Использование
Сборка и установки библиотеки в системные директории:
- `mkdir target`
- `cd target`
- `cmake -DCMAKE_BUILD_TYPE=Release ..`
- `cmake --build .`
- `sudo cmake --install .`
Или:
- `chmod +x install.sh && ./install.sh`

Подключение в свой проект:
```cmake
cmake_minimum_required(VERSION 3.10)
project(test)

set(CMAKE_CXX_STANDARD 20)

find_package(leaf 0.3.1 REQUIRED)

add_executable(test main.cpp)
target_link_libraries(test leaf::leaf)
```

### Зависимости
##### Системные библиотеки:
- `spdlog` - библиотека для логгинга сообщений в файл/stdout
- `fmtlib` - библиотека для форматирования строк