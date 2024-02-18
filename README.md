# Leaf
### Lightweight essentials and functions library

### Использование
Сборка и установки библиотеки в системные директории:
- `mkdir target`
- `cd target`
- `cmake -DCMAKE_BUILD_TYPE=Release ..`
- `cmake --build .`
- `sudo cmake --install .`

Подключение в свой проект:
```cmake
cmake_minimum_required(VERSION 3.10)
project(test)

set(CMAKE_CXX_STANDARD 20)

find_package(leaf 0.3.0 REQUIRED)

add_executable(test main.cpp)
target_link_libraries(test leaf::leaf)
```