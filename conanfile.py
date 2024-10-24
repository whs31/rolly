import os
from conan import ConanFile
from conan.tools.cmake import CMake, CMakeToolchain, cmake_layout, CMakeDeps
from conan.tools.build import check_min_cppstd
from conan.tools.files import rmdir


class RollyRecipe(ConanFile):
    name = "rolly"
    version = "2.0.1"
    description = "Radar open-source library"
    author = "whs31 <whs31@github.io>"
    topics = ("coreutils", "utility")

    settings = "os", "arch", "compiler", "build_type"
    options = {
        "shared": [True, False],
        "test": [True, False],
        "compat": [True, False]
    }
    default_options = {
        "shared": True,
        "test": False,
        "compat": False
    }

    exports_sources = "*"

    @property
    def _min_cppstd(self):
        return "20" if self.options.compat else "17"

    def requirements(self):
        self.requires("fmt/[>=10.0.0]", transitive_headers=True, transitive_libs=True)
        if self.options.test:
            self.requires("gtest/1.14.0")
            self.requires("tomlplusplus/[^3.0.0]", transitive_headers=True, transitive_libs=True)

    def layout(self):
        cmake_layout(self)

    def validate(self):
        if self.settings.get_safe("compiler.cppstd"):
            check_min_cppstd(self, self._min_cppstd)

    def configure(self):
        self.options["fmt/*"].shared = True

    def generate(self):
        deps = CMakeDeps(self)
        deps.generate()
        tc = CMakeToolchain(self)
        tc.cache_variables["BUILD_SHARED_LIBS"] = self.options.shared
        tc.cache_variables["TESTS"] = self.options.test
        tc.cache_variables["ROLLY_COMPAT"] = self.options.compat
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
        self.cpp_info.set_property("cmake_file_name", "rolly")
        self.cpp_info.set_property("cmake_target_name", "rolly::rolly")
        self.cpp_info.libs = ["rolly"]
        self.cpp_info.requires = ["fmt::fmt"]
        if self.options.test:
            self.cpp_info.requires.append("gtest::gtest")
            self.cpp_info.requires.append("tomlplusplus::tomlplusplus")
