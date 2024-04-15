import os
from conan import ConanFile
from conan.tools.cmake import CMake, CMakeToolchain, cmake_layout, CMakeDeps
from conan.tools.build import check_min_cppstd
from conan.tools.files import copy, rmdir


class LeafRecipe(ConanFile):
    name = "leaf"
    version = "0.5.0"
    description = "Coreutils library for C++"
    author = "whs31 <whs31@github.io>"
    topics = ("logging", "header-only")

    settings = "os", "arch", "compiler", "build_type"
    options = {
        "shared": [True, False]
    }
    default_options = {
        "shared": True
    }

    exports_sources = "*"

    def requirements(self):
        self.requires("fmt/[^10.1.0]", transitive_headers = True, transitive_libs=True)
        self.requires("spdlog/1.13.0", transitive_headers = True, transitive_libs=True)
        self.requires("magic_enum/[^0.9.0]", transitive_libs=True)

    def layout(self):
        cmake_layout(self)

    def validate(self):
        if self.settings.get_safe("compiler.cppstd"):
            check_min_cppstd(self, 20)
        deps = CMakeDeps(self)

    def generate(self):
        deps = CMakeDeps(self)
        deps.generate()
        tc = CMakeToolchain(self)
        tc.cache_variables["BUILD_SHARED_LIBS"] = self.options.shared
        tc.generate()

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def package(self):
        cmake = CMake(self)
        cmake.install()
        rmdir(self, os.path.join(self.package_folder, "lib", "cmake"))
        rmdir(self, os.path.join(self.package_folder, "lib", "pkgconfig"))
        rmdir(self, os.path.join(self.package_folder, "res"))
        rmdir(self, os.path.join(self.package_folder, "share"))

    def package_info(self):
        target = "leaf"
        self.cpp_info.set_property("cmake_file_name", target)
        self.cpp_info.set_property("cmake_target_name", f"{target}::{target}")
        self.cpp_info.set_property("pkg_config_name",  target)