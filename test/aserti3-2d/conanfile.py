# Copyright (c) 2016-2023 Knuth Project developers.
# Distributed under the MIT software license, see the accompanying
# file COPYING or http://www.opensource.org/licenses/mit-license.php.

import os
from conans import CMake
from kthbuild import option_on_off, march_conan_manip, pass_march_to_compiler
from kthbuild import KnuthConanFile

class AsertTestVectorsGenerator(KnuthConanFile):
    def recipe_dir(self):
        return os.path.dirname(os.path.abspath(__file__))

    name = "asert-gen"
    url = "https://github.com/k-nuth/kth"
    description = "aserti3-2e test vectors generator"
    settings = "os", "compiler", "build_type", "arch"

    options = {
        # "currency": ['BCH', 'BTC', 'LTC'],

        "microarchitecture": "ANY",
        "fix_march": [True, False],
        "march_id": "ANY",

        "verbose": [True, False],

        "cxxflags": "ANY",
        "cflags": "ANY",
        "glibcxx_supports_cxx11_abi": "ANY",
        "cmake_export_compile_commands": [True, False],
    }

    default_options = {
        # "currency": "BCH",

        "microarchitecture": "_DUMMY_",
        "fix_march": False,
        "march_id": "_DUMMY_",

        "verbose": False,

        "cxxflags": "_DUMMY_",
        "cflags": "_DUMMY_",
        "glibcxx_supports_cxx11_abi": "_DUMMY_",
        "cmake_export_compile_commands": False,
    }

    generators = "cmake"
    exports = "conan_*"
    exports_sources = "CMakeLists.txt", "genenerate_test_vectors.cpp"

    build_policy = "missing"

    def requirements(self):
        self.requires("domain/0.X@%s/%s" % (self.user, self.channel))

    def config_options(self):
        KnuthConanFile.config_options(self)

    def configure(self):
        KnuthConanFile.configure(self)
        self.options["*"].currency = "BCH"

    def package_id(self):
        KnuthConanFile.package_id(self)

    def build(self):
        cmake = self.cmake_basis()
        cmake.configure(source_dir=self.source_folder)
        if not self.options.cmake_export_compile_commands:
            cmake.build()
            # if self.options.tests:
            #     cmake.test()

    def package(self):
        self.copy("generator.exe", dst="bin", src="bin")  # Windows
        self.copy("generator", dst="bin", src="bin")      # Unixes

    def deploy(self):
        self.copy("generator.exe", src="bin")     # copy from current package
        self.copy("generator", src="bin")         # copy from current package
        # self.copy_deps("*.dll") # copy from dependencies
