#
# Copyright (c) 2016-2020 Knuth Project.
#
# This file is part of Knuth Project.
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU Affero General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Affero General Public License for more details.
#
# You should have received a copy of the GNU Affero General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#
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
    description = "Bitcoin Cross-Platform C++ Development Toolkit"
    settings = "os", "compiler", "build_type", "arch"

    options = {"shared": [True, False],
               "fPIC": [True, False],
               "with_icu": [True, False],
               "with_qrencode": [True, False],
               "with_tests": [True, False],
               "with_examples": [True, False],
               "currency": ['BCH', 'BTC', 'LTC'],
               "microarchitecture": "ANY", #["x86_64", "haswell", "ivybridge", "sandybridge", "bulldozer", ...]
               "fix_march": [True, False],
               "march_id": "ANY",
               "verbose": [True, False],
               "keoken": [True, False],
               "cached_rpc_data": [True, False],
               "cxxflags": "ANY",
               "cflags": "ANY",
               "glibcxx_supports_cxx11_abi": "ANY",
    }

    #    "with_png": [True, False],

    default_options = "shared=False", \
        "fPIC=True", \
        "with_icu=False", \
        "with_qrencode=False", \
        "with_tests=False", \
        "with_examples=False", \
        "currency=BCH", \
        "microarchitecture=_DUMMY_",  \
        "fix_march=False", \
        "march_id=_DUMMY_",  \
        "verbose=False", \
        "keoken=False", \
        "cached_rpc_data=False", \
        "cxxflags=_DUMMY_", \
        "cflags=_DUMMY_", \
        "glibcxx_supports_cxx11_abi=_DUMMY_"

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
        self.requires("boost/1.72.0@kth/stable")
        self.requires("secp256k1/0.X@%s/%s" % (self.user, self.channel))
        self.requires("infrastructure/0.X@%s/%s" % (self.user, self.channel))
        # self.requires("crypto/0.X@%s/%s" % (self.user, self.channel))

        # if self.options.with_png:
        #     self.requires("libpng/1.6.34@kth/stable")
            
        if self.options.currency == "LTC":
            #TODO(fernando): check if a newer version exists
            self.requires("OpenSSL/1.0.2l@conan/stable")

        if self.options.with_qrencode:
            self.requires("libqrencode/4.0.0@bitprim/stable")

        # self.bitprim_requires(["secp256k1/0.X@%s/%s"])

    def config_options(self):
        KnuthConanFile.config_options(self)

    def configure(self):
        # super(Foo, self).baz(arg)
        KnuthConanFile.configure(self)

        if self.options.keoken and self.options.currency != "BCH":
            self.output.warn("For the moment Keoken is only enabled for BCH. Building without Keoken support...")
            del self.options.keoken
        else:
            self.options["*"].keoken = self.options.keoken

        self.options["*"].currency = self.options.currency
        self.output.info("Compiling for currency: %s" % (self.options.currency,))

    def package_id(self):
        KnuthConanFile.package_id(self)

        self.info.options.with_tests = "ANY"
        self.info.options.with_examples = "ANY"
        
    def build(self):
        cmake = self.cmake_basis()

        #TODO(fernando): see what to put un cmake_basis()
        cmake.definitions["WITH_CACHED_RPC_DATA"] = option_on_off(self.options.cached_rpc_data)
        cmake.definitions["WITH_KEOKEN"] = option_on_off(self.is_keoken)
        cmake.definitions["WITH_TESTS"] = option_on_off(self.options.with_tests)
        cmake.definitions["WITH_TESTS_NEW"] = option_on_off(self.options.with_tests)
        cmake.definitions["WITH_EXAMPLES"] = option_on_off(self.options.with_examples)
        cmake.definitions["WITH_ICU"] = option_on_off(self.options.with_icu)
        cmake.definitions["WITH_QRENCODE"] = option_on_off(self.options.with_qrencode)
        # cmake.definitions["WITH_PNG"] = option_on_off(self.options.with_png)
        cmake.definitions["WITH_PNG"] = option_on_off(self.options.with_qrencode)

        cmake.definitions["CURRENCY"] = self.options.currency

        cmake.configure(source_dir=self.source_folder)
        cmake.build()

        #Note: Cmake Tests and Visual Studio doesn't work
        if self.options.with_tests:
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
            self.cpp_info.defines.append("BC_STATIC")


