# Copyright (c) 2016-2022 Knuth Project developers.
# Distributed under the MIT software license, see the accompanying
# file COPYING or http://www.opensource.org/licenses/mit-license.php.

import os
from conans import CMake
from kthbuild import option_on_off, march_conan_manip, pass_march_to_compiler
from kthbuild import KnuthConanFile

class KnuthDomainConan(KnuthConanFile):
    def recipe_dir(self):
        return os.path.dirname(os.path.abspath(__file__))

    name = "domain"
    # version = get_version()
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
               "microarchitecture": "ANY",
               "fix_march": [True, False],
               "march_id": "ANY",
               "verbose": [True, False],
               "keoken": [True, False],
               "cached_rpc_data": [True, False],
               "cxxflags": "ANY",
               "cflags": "ANY",
               "glibcxx_supports_cxx11_abi": "ANY",
               "cmake_export_compile_commands": [True, False],
               "log": ["boost", "spdlog", "binlog"],
               "disable_get_blocks": [True, False],
    }

    #    "with_png": [True, False],

    default_options = {
        "shared": False,
        "fPIC": True,
        "with_icu": False,
        "with_qrencode": False,
        "tests": False,
        "examples": False,
        "currency": "BCH",
        "microarchitecture": "_DUMMY_",
        "fix_march": False,
        "march_id": "_DUMMY_",
        "verbose": False,
        "keoken": False,
        "cached_rpc_data": False,
        "cxxflags": "_DUMMY_",
        "cflags": "_DUMMY_",
        "glibcxx_supports_cxx11_abi": "_DUMMY_",
        "cmake_export_compile_commands": False,
        "log": "spdlog",
        "disable_get_blocks": False,
    }

        # "with_png=False", \

    generators = "cmake"
    exports = "conan_*", "ci_utils/*"
    exports_sources = "src/*", "CMakeLists.txt", "cmake/*", "kth-domainConfig.cmake.in", "include/*", "test/*", "examples/*", "test_new/*"

    package_files = "build/lkth-domain.a"
    build_policy = "missing"

    @property
    def is_keoken(self):
        return self.options.currency == "BCH" and self.options.get_safe("keoken")

    def requirements(self):
        self.requires("algorithm/0.1.239@tao/stable")
        self.requires("secp256k1/0.X@%s/%s" % (self.user, self.channel))
        self.requires("infrastructure/0.X@%s/%s" % (self.user, self.channel))
        # self.requires("crypto/0.X@%s/%s" % (self.user, self.channel))

        if self.options.tests:
            self.requires("catch2/2.13.7")

        if self.options.currency == "LTC":
            #TODO(fernando): check if a newer version exists
            self.requires("OpenSSL/1.0.2l@conan/stable")

        if self.options.with_qrencode:
            self.requires("libqrencode/4.0.0@kth/stable")

    def config_options(self):
        KnuthConanFile.config_options(self)

    def configure(self):
        KnuthConanFile.configure(self)

        if self.options.keoken and self.options.currency != "BCH":
            self.output.warn("For the moment Keoken is only enabled for BCH. Building without Keoken support...")
            del self.options.keoken
        else:
            self.options["*"].keoken = self.options.keoken

        self.options["*"].cached_rpc_data = self.options.cached_rpc_data

        self.options["*"].log = self.options.log
        self.output.info("Compiling with log: %s" % (self.options.log,))

    def package_id(self):
        KnuthConanFile.package_id(self)

    def build(self):
        cmake = self.cmake_basis()
        cmake.definitions["WITH_CACHED_RPC_DATA"] = option_on_off(self.options.cached_rpc_data)
        cmake.definitions["WITH_KEOKEN"] = option_on_off(self.is_keoken)
        cmake.definitions["WITH_ICU"] = option_on_off(self.options.with_icu)
        cmake.definitions["WITH_QRENCODE"] = option_on_off(self.options.with_qrencode)
        # cmake.definitions["WITH_PNG"] = option_on_off(self.options.with_png)
        cmake.definitions["WITH_PNG"] = option_on_off(self.options.with_qrencode)
        cmake.definitions["LOG_LIBRARY"] = self.options.log

        cmake.configure(source_dir=self.source_folder)
        if not self.options.cmake_export_compile_commands:
            cmake.build()
            #Note: Cmake Tests and Visual Studio doesn't work
            if self.options.tests:
                cmake.test()
                # cmake.test(target="tests")

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
        self.cpp_info.libs = ["kth-domain"]

        if self.settings.os == "Linux" or self.settings.os == "FreeBSD":
            self.cpp_info.libs.append("pthread")

        if self.settings.os == "Windows" and self.settings.compiler == "gcc": # MinGW
            self.cpp_info.libs.append("ws2_32")
            self.cpp_info.libs.append("wsock32")

        if not self.is_shared:
            self.cpp_info.defines.append("KD_STATIC")


