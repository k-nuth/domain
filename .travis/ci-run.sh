#!/bin/bash

set -x

sudo apt-get update

# apt-get install clang-tidy-6.0
#PATH=/usr/lib/llvm-6.0/bin/:$PATH

sudo apt-get install clang-tidy-7
PATH=/usr/lib/llvm-7/bin/:$PATH

sudo find / -name "clang-tidy"
clang-tidy --version

conan user
conan --version
# conan remote add bitprim_temp https://api.bintray.com/conan/bitprim/bitprim

cd /home/conan/project

# git clone --depth=50 --branch=dev https://github.com/bitprim/bitprim-domain.git bitprim/bitprim-domain
# cd bitprim/bitprim-domain
# git submodule update --init --recursive

mkdir build
cd build
conan install .. 
cmake .. -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DUSE_CONAN=ON -DENABLE_TESTS=OFF -DWITH_TESTS=OFF -DWITH_TOOLS=OFF -DCMAKE_BUILD_TYPE=Release -DJUST_BITPRIM_SOURCES=ON -DWITH_KEOKEN=ON
python ../ci_utils/run-clang-tidy.py -checks='*,-android-*,-modernize-use-using,-hicpp-deprecated-headers,-modernize-deprecated-headers,-cppcoreguidelines-pro-type-vararg,-cppcoreguidelines-owning-memory,-cppcoreguidelines-pro-bounds-pointer-arithmetic,-hicpp-vararg,-hicpp-no-malloc,-fuchsia-default-arguments,-google-runtime-references,-cppcoreguidelines-no-malloc,-hicpp-no-malloc,-llvm-header-guard,-android-cloexec-fopen,-cppcoreguidelines-pro-type-union-access,-cppcoreguidelines-pro-type-const-cast,-hicpp-braces-around-statements,-hicpp-special-member-functions,-readability-braces-around-statements,-coreguidelines-special-member-function,-cppcoreguidelines-special-member-functions,-clang-diagnostic-static-inline-explicit-instantiation,-fuchsia-overloaded-operator,-cppcoreguidelines-pro-type-member-init,-hicpp-member-init' -warnings-as-errors='*' -header-filter=/home/conan/project/include/bitprim
