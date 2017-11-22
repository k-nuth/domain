#!/bin/bash
#
# Copyright (c) 2017 Bitprim developers (see AUTHORS)
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

set -e
set -x


###############################################################
# Install
###############################################################

sudo apt-get update
sudo apt-get --yes install python3.6
sudo apt-get --yes install python3.6-dev

sudo apt-get  --yes install vera++
sudo apt-get  --yes install pcregrep
sudo apt-get  --yes install clang-tidy # || true
# sudo apt-get  --yes install clang-tidy-5.0 || true
# sudo apt-get  --yes install clang-tidy-4.0 || true
# sudo apt-get  --yes install clang-tidy-3.9 || true
sudo apt-get  --yes install clang-format # || true
# sudo apt-get  --yes install clang-format-5.0 || true
# sudo apt-get  --yes install clang-format-4.0 || true
# sudo apt-get  --yes install clang-format-3.9 || true


wget https://bootstrap.pypa.io/get-pip.py --no-check-certificate
sudo python3.6 get-pip.py
export BITPRIM_PIP=pip3.6

sudo pip3.6 install --upgrade pip
sudo pip3.6 install --upgrade conan
# sudo pip3.6 install --upgrade conan_package_tools


###############################################################
# Checks
###############################################################

# For the moment, these checks are disabled

# # Find non-ASCII characters in headers
# # hpps=$(find include doc -name \*\.hpp)
# # cpps=$(find src test example -name \*\.cpp)
# hpps=$(find include -name \*\.hpp)
# cpps=$(find src -name \*\.cpp)
# pcregrep --color='auto' -n "[\x80-\xFF]" ${hpps} ${cpps}
# if [[ $? -ne 1 ]]; then exit 1; fi

# F001: Source files should not use the '\r' (CR) character
# L001: No trailing whitespace at the end of lines
# L002: Don't use tab characters
# find include -name \*\.hpp | vera++ --rule F001 --rule L001 --rule L002 --error
# find include -name \*\.?pp | vera++ --rule F001 --rule L001 --rule L002 --error



sudo find / -name "run-clang-tidy.py"
clang-tidy --version
# /usr/local/Cellar/llvm/5.0.0/share/clang/run-clang-tidy.py -header-filter='.*' -checks='-*,modernize-use-override'


cd ${TRAVIS_BUILD_DIR}

conan user
conan remote add bitprim_temp https://api.bintray.com/conan/bitprim/bitprim
cd /home/conan/project

mkdir build
cd build

conan install ..
cmake .. -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DENABLE_TESTS=OFF -DWITH_TESTS=OFF -DWITH_TOOLS=OFF -DCMAKE_BUILD_TYPE=$BUILD_TYPE

