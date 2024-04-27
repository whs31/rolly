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
#### Сборка вручную
Сборка и установка библиотеки в системные папки:
```shell
cmake -B target -S . -DCMAKE_BUILD_TYPE="Release" -DBUILD_SHARED_LIBS=ON
cmake --build target --config "Release"
sudo cmake --install target
```

#### Conan
По умолчанию будет использоваться динамическая библиотека.
```py
def requirements(self):
    self.requires("leaf/[^0.6.0]", transitive_headers = True, transitive_libs=True)
```

#### CMake
Подключение в свой проект:
```cmake
cmake_minimum_required(VERSION 3.10)
project(test)

set(CMAKE_CXX_STANDARD 20)

find_package(leaf REQUIRED)

add_executable(test main.cpp)
target_link_libraries(test leaf::leaf)
```