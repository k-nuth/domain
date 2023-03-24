# Copyright (c) 2016-2023 Knuth Project developers.
# Distributed under the MIT software license, see the accompanying
# file COPYING or http://www.opensource.org/licenses/mit-license.php.

import os
from conan import ConanFile
from conan.tools.build.cppstd import check_min_cppstd
from conan.tools.cmake import CMake, CMakeDeps, CMakeToolchain, cmake_layout
from conan.tools.files import copy #, apply_conandata_patches, export_conandata_patches, get, rm, rmdir
from kthbuild import option_on_off, march_conan_manip, pass_march_to_compiler
from kthbuild import KnuthConanFileV2

required_conan_version = ">=2.0"

class KnuthDomainConan(KnuthConanFileV2):
    name = "domain"
    license = "http://www.boost.org/users/license.html"
    url = "https://github.com/k-nuth/domain"
    description = "Crypto Cross-Platform C++ Development Toolkit"
    settings = "os", "compiler", "build_type", "arch"

    options = {"shared": [True, False],
               "fPIC": [True, False],
               "with_icu": [True, False],
               "with_qrencode": [True, False],
               "tests": [True, False],
               "examples": [True, False],
               "currency": ['BCH', 'BTC', 'LTC'],

               "march_id": ["ANY"],
               "march_strategy": ["download_if_possible", "optimized", "download_or_fail"],

               "verbose": [True, False],
               "cached_rpc_data": [True, False],
               "cxxflags": ["ANY"],
               "cflags": ["ANY"],
               "cmake_export_compile_commands": [True, False],
               "log": ["boost", "spdlog", "binlog"],
               "disable_get_blocks": [True, False],
    }

    default_options = {
        "shared": False,
        "fPIC": True,
        "with_icu": False,
        "with_qrencode": False,
        "tests": False,
        "examples": False,
        "currency": "BCH",

        "march_strategy": "download_if_possible",

        "verbose": False,
        "cached_rpc_data": False,
        "cmake_export_compile_commands": False,
        "log": "spdlog",
        "disable_get_blocks": False,
    }

    exports_sources = "src/*", "CMakeLists.txt", "ci_utils/cmake/*", "cmake/*", "include/*", "test/*", "examples/*", "test_new/*"

    def build_requirements(self):
        if self.options.tests:
            self.test_requires("catch2/3.3.1")

    def requirements(self):
        self.requires("infrastructure/0.26.0")

        self.requires("boost/1.81.0")
        self.requires("fmt/9.1.0")

        if self.options.currency == "LTC":
            self.requires("OpenSSL/1.0.2l@conan/stable")

        if self.options.with_qrencode:
            self.requires("libqrencode/4.0.0@kth/stable")

    def validate(self):
        KnuthConanFileV2.validate(self)
        if self.info.settings.compiler.cppstd:
            check_min_cppstd(self, "20")

    def config_options(self):
        KnuthConanFileV2.config_options(self)

    def configure(self):
        KnuthConanFileV2.configure(self)
        self.options["*"].cached_rpc_data = self.options.cached_rpc_data
        self.options["*"].log = self.options.log
        self.output.info("Compiling with log: %s" % (self.options.log,))

    def package_id(self):
        KnuthConanFileV2.package_id(self)

    def layout(self):
        cmake_layout(self)

    def generate(self):
        tc = self.cmake_toolchain_basis()
        # tc.variables["CMAKE_VERBOSE_MAKEFILE"] = True
        tc.variables["WITH_CACHED_RPC_DATA"] = option_on_off(self.options.cached_rpc_data)
        tc.variables["WITH_ICU"] = option_on_off(self.options.with_icu)
        tc.variables["WITH_QRENCODE"] = option_on_off(self.options.with_qrencode)
        # tc.variables["WITH_PNG"] = option_on_off(self.options.with_png)
        tc.variables["WITH_PNG"] = option_on_off(self.options.with_qrencode)
        tc.variables["LOG_LIBRARY"] = self.options.log
        tc.variables["CONAN_DISABLE_CHECK_COMPILER"] = option_on_off(True)

        tc.generate()
        tc = CMakeDeps(self)
        tc.generate()

    def build(self):
        cmake = CMake(self)
        cmake.configure()

        if not self.options.cmake_export_compile_commands:
            cmake.build()
            #Note: Cmake Tests and Visual Studio doesn't work
            if self.options.tests:
                cmake.test()
                # cmake.test(target="tests")

    # def imports(self):
    #     self.copy("*.h", "", "include")

    def package(self):
        cmake = CMake(self)
        cmake.install()
        # rmdir(self, os.path.join(self.package_folder, "lib", "cmake"))
        # rmdir(self, os.path.join(self.package_folder, "lib", "pkgconfig"))
        # rmdir(self, os.path.join(self.package_folder, "res"))
        # rmdir(self, os.path.join(self.package_folder, "share"))

    def package_info(self):
        self.cpp_info.includedirs = ['include']
        self.cpp_info.libs = ["domain"]

        if self.settings.os == "Linux" or self.settings.os == "FreeBSD":
            self.cpp_info.system_libs.append("pthread")

        if self.settings.os == "Windows" and self.settings.compiler == "gcc": # MinGW
            self.cpp_info.system_libs.append("ws2_32")
            self.cpp_info.system_libs.append("wsock32")

        if not self.is_shared:
            self.cpp_info.defines.append("KD_STATIC")


