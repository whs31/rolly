# Leaf
<h3 align="center">
    <img src="./docs/images/logo2.png" alt=""/><br/>
    <img src="https://raw.githubusercontent.com/catppuccin/catppuccin/main/assets/misc/transparent.png" height="30" width="0px"/>
    Lightweight essentials and functions library
    <img src="https://raw.githubusercontent.com/catppuccin/catppuccin/main/assets/misc/transparent.png" height="30" width="0px"/><br/>
    <img src="https://raw.githubusercontent.com/catppuccin/catppuccin/main/assets/misc/transparent.png" height="30" width="0px"/><br/>
    <img src="https://img.shields.io/badge/C%2B%2B-00599C?style=for-the-badge&logo=c%2B%2B&logoColor=white" alt="cpp_badge"/><img src="https://img.shields.io/badge/CMake-064F8C?style=for-the-badge&logo=cmake&logoColor=white" alt="cmake_badge"/>
</h3>


### [История изменений](./CHANGELOG.md)

### Зависимости

- ![fmt logo](https://avatars.githubusercontent.com/u/7280830?s=16&v=4)[**fmt**](https://github.com/fmtlib/fmt) - библиотека для форматирования строк (proposal для `std::format`) (**MIT**)
- ![spdlog logo](https://avatars.githubusercontent.com/u/6052198?s=16&v=4)[**spdlog**](https://github.com/gabime/spdlog) - реализация гибкого логгера на основе *fmt* (**MIT**)
- ![magic enum logo](https://avatars.githubusercontent.com/u/7997966?s=16&v=4)[**magic_enum**](https://github.com/Neargye/magic_enum) - рефлексия для перечислений (**MIT**)

### Лицензии

Библиотека Leaf распостраняется по лицензии MIT, как и все ее зависимости. См. [LICENSE](./LICENSE)

### Использование

#### Сборка вручную
Сборка и установка библиотеки в системные папки:
```shell
conan install . -b "missing" - s "build_type=Release"
cmake --preset "conan-release" .
cmake --build build/Release
sudo cmake --install build/Release
```

#### Conan
По умолчанию будет использоваться динамическая библиотека.
```py
def requirements(self):
    self.requires("leaf/[>=0.8.0]", transitive_headers = True, transitive_libs=True)
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