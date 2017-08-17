from conans import ConanFile, CMake
import os

class BitprimcoreConan(ConanFile):
    name = "bitprim-core"
    version = "0.1"
    license = "http://www.boost.org/users/license.html"
    url = "https://github.com/bitprim/bitprim-core"
    description = "Bitcoin Cross-Platform C++ Development Toolkit"
    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [True, False]}
    default_options = "shared=False"
    generators = "cmake"
    exports_sources = "src/*", "CMakeLists.txt", "cmake/*", "bitprim-coreConfig.cmake.in", "include/*", "test/*"
    package_files = "build/lbitprim-core.a"
    build_policy = "missing"

    requires = (("bitprim-conan-boost/1.64.0@bitprim/stable"),
               ("secp256k1/0.1@bitprim/testing"))


# def is_clang_or_gcc5(sett):
#     # precondition: sett["compiler"] and sett["compiler.version"] are valid
#     return sett["compiler"] == "clang" or (sett["compiler"] == "gcc" and float(sett["compiler.version"]) >= 5)

# def replace_libcxx(sett):
#     if "compiler" in sett and "compiler.libcxx" in sett and "compiler.version" in sett and is_clang_or_gcc5(sett) and sett["compiler.libcxx"] == "libstdc++":
#         sett["compiler.libcxx"] = "libstdc++11"
#     return sett


    def build(self):
        cmake = CMake(self)
        cmake.definitions["CMAKE_VERBOSE_MAKEFILE"] = "ON"

        if self.settings.compiler == "gcc":
            if float(self.settings.compiler.version) >= 5):
                cmake.definitions["_GLIBCXX_USE_CXX11_ABI"] = "1"
            else:
                cmake.definitions["_GLIBCXX_USE_CXX11_ABI"] = "0"

        cmake.configure(source_dir=self.conanfile_directory)
        cmake.build()

    def imports(self):
        self.copy("*.h", "", "include")

    def package(self):
        self.copy("*.h", dst="include", src="include")
        self.copy("*.hpp", dst="include", src="include")
        self.copy("*.ipp", dst="include", src="include")
        self.copy("*.lib", dst="lib", keep_path=False)
        self.copy("*.dll", dst="bin", keep_path=False)
        self.copy("*.dylib*", dst="lib", keep_path=False)
        self.copy("*.so", dst="lib", keep_path=False)
        self.copy("*.a", dst="lib", keep_path=False)

    def package_info(self):
        self.cpp_info.includedirs = ['include']
        self.cpp_info.libs = ["bitprim-core"]

        if self.settings.os == "Linux":
            self.cpp_info.libs.append("pthread")