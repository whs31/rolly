import os
from conan import ConanFile
from conan.tools.cmake import CMake, CMakeToolchain, cmake_layout, CMakeDeps
from conan.tools.build import check_min_cppstd
from conan.tools.files import rmdir, copy


class RollyRecipe(ConanFile):
    name = "rolly"
    version = "2.1.3"
    description = "Radar open-source library"
    author = "whs31 <whs31@github.io>"
    topics = ("coreutils", "utility")

    settings = "os", "arch", "compiler", "build_type"
    options = {
        "shared": [True, False],
        "test": [True, False],
        "compat": [True, False],
        "export_folder_name": ["ANY"]
    }
    default_options = {
        "shared": True,
        "test": False,
        "compat": False,
        "export_folder_name": "export"
    }
    exports = "CMakeLists.txt", "conanfile.py", "*.cmake.in"
    exports_sources = "*"

    @property
    def _min_cppstd(self):
        return "17"

    def requirements(self):
        self.requires("fmt/[>=10.0.0]", transitive_headers=True, transitive_libs=True)
        if self.settings.os != "Windows":
            self.requires("libuuid/1.0.3")
        if self.options.test:
            self.requires("catch2/[=3.7.1]")
            self.requires("tomlplusplus/[^3.0.0]", transitive_headers=True, transitive_libs=True)

    def layout(self):
        cmake_layout(self)

    def validate(self):
        if self.settings.get_safe("compiler.cppstd"):
            check_min_cppstd(self, self._min_cppstd)

    def configure(self):
        self.options["fmt/*"].shared = self.options.shared

    def generate(self):
        deps = CMakeDeps(self)
        deps.generate()
        tc = CMakeToolchain(self)
        tc.cache_variables["BUILD_SHARED_LIBS"] = self.options.shared
        tc.cache_variables["ROLLY_TESTS"] = self.options.test
        tc.cache_variables["ROLLY_COMPAT"] = self.options.compat
        tc.generate()

        if not self.options.test:
            for dep in self.dependencies.values():
                self.output.info(f"copying {dep.ref.name} into export folder {str(self.options.export_folder_name)}")
                bin_dest = os.path.join(self.build_folder, str(self.options.export_folder_name), 'bin')
                lib_dest = os.path.join(self.build_folder, str(self.options.export_folder_name), 'lib')
                inc_dest = os.path.join(self.build_folder, str(self.options.export_folder_name), self.cpp.source.includedirs[0])
                self.output.info(f" - bin: {bin_dest}")
                self.output.info(f" - lib: {lib_dest}")
                self.output.info(f" - inc: {inc_dest}")
                bin_extensions = [".exe", ".dll", ".dylib", "*"]               # temporarily copying full folder contents because libfmt.so can
                lib_extensions = [".a", ".lib", ".so", "*"]                    # be a symbolic link to libfmt.so.11.0.2 
                inc_extensions = [".h", ".hh", ".hxx", ".h++", ".cuh", "*"]    # same as above, includes can have cursed extensions
                for ext in bin_extensions:
                    copy(self, f"*{ext}", src=dep.cpp_info.bindirs[0], dst=bin_dest)
                for ext in lib_extensions:
                    copy(self, f"*{ext}", src=dep.cpp_info.libdirs[0], dst=lib_dest)
                for ext in inc_extensions:
                    copy(self, f"*{ext}", src=dep.cpp_info.includedirs[0], dst=inc_dest)

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
        if self.settings.os != "Windows":
            self.cpp_info.requires.append("libuuid::libuuid")
        if self.options.test:
            self.cpp_info.requires.append("catch2::catch2")
            self.cpp_info.requires.append("tomlplusplus::tomlplusplus")
