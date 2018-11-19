#
# Copyright (c) 2017-2018 Bitprim Inc.
#
# This file is part of Bitprim.
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
from conans import CMake
from ci_utils import option_on_off, march_conan_manip, pass_march_to_compiler, glibcxx_supports_cxx11_abi
from ci_utils import BitprimConanFile #, BitprimCxx11ABIFixer

class BitprimDomainConan(BitprimConanFile):
# class BitprimDomainConan(BitprimCxx11ABIFixer):

    
    name = "bitprim-domain"
    # version = get_version()
    license = "http://www.boost.org/users/license.html"
    url = "https://github.com/bitprim/bitprim-domain"
    description = "Bitcoin Cross-Platform C++ Development Toolkit"
    settings = "os", "compiler", "build_type", "arch"

    # if Version(conan_version) < Version(get_conan_req_version()):
    #     raise Exception ("Conan version should be greater or equal than %s. Detected: %s." % (get_conan_req_version(), conan_version))

    options = {"shared": [True, False],
               "fPIC": [True, False],
               "with_icu": [True, False],
               "with_qrencode": [True, False],
               "with_tests": [True, False],
               "with_examples": [True, False],
               "currency": ['BCH', 'BTC', 'LTC'],
               "microarchitecture": "ANY", #["x86_64", "haswell", "ivybridge", "sandybridge", "bulldozer", ...]
               "fix_march": [True, False],
               "verbose": [True, False],
               "keoken": [True, False],
               "cached_rpc_data": [True, False],
               "cxxflags": "ANY",
               "cflags": "ANY",
               "dont_fix_glibcxx_abi": [True, False],
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
        "verbose=False", \
        "keoken=False", \
        "cached_rpc_data=False", \
        "cxxflags=_DUMMY_", \
        "cflags=_DUMMY_", \
        "dont_fix_glibcxx_abi=False"

        # "with_png=False", \

    generators = "cmake"
    exports = "conan_*", "ci_utils/*"
    exports_sources = "src/*", "CMakeLists.txt", "cmake/*", "bitprim-domainConfig.cmake.in", "include/*", "test/*", "examples/*", "test_new/*"

    package_files = "build/lbitprim-domain.a"
    build_policy = "missing"

    @property
    def is_keoken(self):
        return self.options.currency == "BCH" and self.options.get_safe("keoken")

    def requirements(self):
        self.requires("boost/1.68.0@bitprim/stable")
        self.requires("secp256k1/0.X@%s/%s" % (self.user, self.channel))
        self.requires("bitprim-infrastructure/0.X@%s/%s" % (self.user, self.channel))
        # self.requires("bitprim-crypto/0.X@%s/%s" % (self.user, self.channel))

        # if self.options.with_png:
        #     self.requires("libpng/1.6.34@bitprim/stable")
            
        if self.options.currency == "LTC":
            self.requires("OpenSSL/1.0.2l@conan/stable")

        if self.options.with_qrencode:
            self.requires("libqrencode/4.0.0@bitprim/stable")

        # self.bitprim_requires(["secp256k1/0.X@%s/%s"])

    def config_options(self):
        if self.settings.arch != "x86_64":
            self.output.info("microarchitecture is disabled for architectures other than x86_64, your architecture: %s" % (self.settings.arch,))
            self.options.remove("microarchitecture")
            self.options.remove("fix_march")

        if self.settings.compiler == "Visual Studio":
            self.options.remove("fPIC")
            if self.options.shared and self.msvc_mt_build:
                self.options.remove("shared")


    def configure(self):
        # super(Foo, self).baz(arg)
        # BitprimConanFile.configure(self)

        # self.output.info("glibcxx_supports_cxx11_abi: %s" % (glibcxx_supports_cxx11_abi(),))

        # if self.settings.compiler == "gcc" or self.settings.compiler == "clang":
        #     if str(self.settings.compiler.libcxx) == "libstdc++" or str(self.settings.compiler.libcxx) == "libstdc++11":
        #         if not glibcxx_supports_cxx11_abi():
        #             raise Exception ("C++ Standard library implementation not supported, run with option `--build`.")


        if self.settings.arch == "x86_64" and self.options.microarchitecture == "_DUMMY_":
            del self.options.fix_march
            # self.options.remove("fix_march")
            # raise Exception ("fix_march option is for using together with microarchitecture option.")

        if self.settings.arch == "x86_64":
            march_conan_manip(self)
            self.options["*"].microarchitecture = self.options.microarchitecture

        if self.options.keoken and self.options.currency != "BCH":
            self.output.warn("For the moment Keoken is only enabled for BCH. Building without Keoken support...")
            del self.options.keoken
        else:
            self.options["*"].keoken = self.options.keoken

        self.options["*"].currency = self.options.currency
        self.output.info("Compiling for currency: %s" % (self.options.currency,))

    def package_id(self):
        BitprimConanFile.package_id(self)

        self.info.options.with_tests = "ANY"
        self.info.options.with_examples = "ANY"
        self.info.options.verbose = "ANY"
        self.info.options.fix_march = "ANY"
        self.info.options.cxxflags = "ANY"
        self.info.options.cflags = "ANY"
        
        self.output.info("libcxx: %s" % (str(self.settings.compiler.libcxx),))
        self.output.info("libcxx: %s" % (str(self.info.settings.compiler.libcxx),))

        # #For Bitprim Packages libstdc++ and libstdc++11 are the same
        # if self.settings.compiler == "gcc" or self.settings.compiler == "clang":
        #     if str(self.settings.compiler.libcxx) == "libstdc++" or str(self.settings.compiler.libcxx) == "libstdc++11":
        #         self.info.settings.compiler.libcxx = "ANY"

    def build(self):
        # for dep in self.deps_cpp_info.deps:
        #     # self.output.warn(self.deps_cpp_info["MyLib"].libdirs)
        #     print(dep)
        #     print(self.options[dep])
        #     # self.options["boost"]

        cmake = CMake(self)
        cmake.definitions["USE_CONAN"] = option_on_off(True)
        cmake.definitions["NO_CONAN_AT_ALL"] = option_on_off(False)
        cmake.verbose = self.options.verbose
        cmake.definitions["ENABLE_SHARED"] = option_on_off(self.is_shared)
        cmake.definitions["ENABLE_POSITION_INDEPENDENT_CODE"] = option_on_off(self.fPIC_enabled)

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

        if self.settings.compiler != "Visual Studio":
            # cmake.definitions["CONAN_CXX_FLAGS"] += " -Wno-deprecated-declarations"
            cmake.definitions["CONAN_CXX_FLAGS"] = cmake.definitions.get("CONAN_CXX_FLAGS", "") + " -Wno-deprecated-declarations"

        if self.settings.compiler == "Visual Studio":
            cmake.definitions["CONAN_CXX_FLAGS"] = cmake.definitions.get("CONAN_CXX_FLAGS", "") + " /DBOOST_CONFIG_SUPPRESS_OUTDATED_MESSAGE"

        if self.options.cxxflags != "_DUMMY_":
            cmake.definitions["CONAN_CXX_FLAGS"] = cmake.definitions.get("CONAN_CXX_FLAGS", "") + " " + str(self.options.cxxflags)
        if self.options.cflags != "_DUMMY_":
            cmake.definitions["CONAN_C_FLAGS"] = cmake.definitions.get("CONAN_C_FLAGS", "") + " " + str(self.options.cflags)


        cmake.definitions["MICROARCHITECTURE"] = self.options.microarchitecture
        cmake.definitions["BITPRIM_PROJECT_VERSION"] = self.version

        if self.settings.compiler == "gcc":
            if float(str(self.settings.compiler.version)) >= 5:
                cmake.definitions["NOT_USE_CPP11_ABI"] = option_on_off(False)
            else:
                cmake.definitions["NOT_USE_CPP11_ABI"] = option_on_off(True)
        elif self.settings.compiler == "clang":
            if str(self.settings.compiler.libcxx) == "libstdc++" or str(self.settings.compiler.libcxx) == "libstdc++11":
                cmake.definitions["NOT_USE_CPP11_ABI"] = option_on_off(False)

        pass_march_to_compiler(self, cmake)

        cmake.configure(source_dir=self.source_folder)

        # self.output.info("CONAN_CXX_FLAGS: %s" % (cmake.definitions["CONAN_CXX_FLAGS"], ))
        # self.output.info("cmake.command_line: %s" % (cmake.command_line, ))

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
        self.cpp_info.libs = ["bitprim-domain"]

        if self.settings.os == "Linux" or self.settings.os == "FreeBSD":
            self.cpp_info.libs.append("pthread")

        if self.settings.os == "Windows" and self.settings.compiler == "gcc": # MinGW
            self.cpp_info.libs.append("ws2_32")
            self.cpp_info.libs.append("wsock32")

        if not self.is_shared:
            self.cpp_info.defines.append("BC_STATIC")


