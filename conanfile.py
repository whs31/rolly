import os
from conan import ConanFile
from conan.tools.cmake import CMake, CMakeToolchain, cmake_layout, CMakeDeps
from conan.tools.build import check_min_cppstd
from conan.tools.files import copy, rmdir


class SpdlogRecipe(ConanFile):
    requires = "fmt/10.1.1"

    name = "leaf"
    version = "0.4.6"
    description = "Coreutils library for C++"
    author = "whs31 <whs31@github.io>"
    url = "https://github.com/gabime/spdlog"
    topics = ("logging", "header-only")

    settings = "os", "arch", "compiler", "build_type"
    options = {
        "shared": [True, False]
    }
    default_options = {
        "shared": True
    }

    exports_sources = "*"

    def layout(self):
        cmake_layout(self)

    def validate(self):
        if self.settings.get_safe("compiler.cppstd"):
            check_min_cppstd(self, 14)
        deps = CMakeDeps(self)

    def generate(self):
        deps = CMakeDeps(self)
        deps.generate()
        tc = CMakeToolchain(self)
        tc.generate()

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def package(self):
        copy(self, pattern="LICENSE", src=self.source_folder, dst=os.path.join(self.package_folder, "licenses"))
        cmake = CMake(self)
        cmake.install()
        rmdir(self, os.path.join(self.package_folder, "lib", "cmake"))
        rmdir(self, os.path.join(self.package_folder, "lib", "pkgconfig"))
        rmdir(self, os.path.join(self.package_folder, "res"))
        rmdir(self, os.path.join(self.package_folder, "share"))

    def package_info(self):
        target = "spdlog"
        self.cpp_info.set_property("cmake_file_name", "spdlog")
        self.cpp_info.set_property("cmake_target_name", f"spdlog::{target}")
        self.cpp_info.set_property("pkg_config_name",  "spdlog")