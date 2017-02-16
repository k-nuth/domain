[![Build Status](https://travis-ci.org/bitprim/bitprim-core.svg?branch=master)](https://travis-ci.org/bitprim/bitprim-core)

# Bitprim Core

*Bitprim Core*

**License Overview**

All files in this repository fall under the license specified in [COPYING](COPYING). The project is licensed as [AGPL with a lesser clause](https://wiki.unsystem.net/en/index.php/Libbitcoin/License). It may be used within a proprietary project, but the core library and any changes to it must be published online. Source code for this library must always remain free for everybody to access.

**About Bitprim**

The Bitprim toolkit is a set of cross platform C++ libraries for building bitcoin applications. The toolkit consists of several libraries, most of which depend on the foundational [bitprim-core](https://github.com/bitprim/bitprim-core) library. Each library's repository can be cloned and built using common [automake](http://www.gnu.org/software/automake) 1.14+ instructions. 

## Installation

A minimal bitprim build requires boost and libsecp256k1. The [bitprim/secp256k1](https://github.com/bitprim/secp256k1) repository is forked from [bitcoin-core/secp256k1](https://github.com/bitcoin-core/secp256k1) in order to control for changes and to incorporate the necessary Visual Studio build. The original repository can be used directly but recent changes to the public interface may cause build breaks. The `--enable-module-recovery` switch is required.

Detailed instructions are provided below.
  * [Debian/Ubuntu](#debianubuntu)
  * [Macintosh](#macintosh)
  * [Windows](#windows)

### Debian/Ubuntu

Bitprim requires a C++11 compiler, currently minimum [GCC 4.8.0](https://gcc.gnu.org/projects/cxx0x.html) or Clang based on [LLVM 3.5](http://llvm.org/releases/3.5.0/docs/ReleaseNotes.html).

To see your GCC version:
```sh
$ g++ --version
```
```
g++ (Ubuntu 4.8.2-19ubuntu1) 4.8.2
Copyright (C) 2013 Free Software Foundation, Inc.
This is free software; see the source for copying conditions.  There is NO
warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
```
If necessary, upgrade your compiler as follows:
```sh
$ sudo apt-get install g++-4.8
$ sudo update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-4.8 50
$ sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-4.8 50
$ sudo update-alternatives --install /usr/bin/gcov gcov /usr/bin/gcov-4.8 50
```
Next install the [build system](http://wikipedia.org/wiki/GNU_build_system) (Automake minimum 1.14) and git:
```sh
$ sudo apt-get install build-essential autoconf automake libtool pkg-config git 
```

If you intend to use libbbitcoin-consensus (this is the default), you will also need libcrypto:
```sh
$ sudo apt-get install libssl-dev
```

Next install the [Boost](http://www.boost.org) (minimum 1.56.0) development package:
```sh
$ sudo apt-get install libboost-all-dev
```
Next install the [ZeroMQ](http://zeromq.org/) (minimum 4.2.0) development package:
```sh
$ git clone https://github.com/zeromq/libzmq
$ cd libzmq
$ ./autogen.sh && ./configure && make -j 4
$ make check
$ sudo make install
$ sudo ldconfig
```

Next install the [Protobuff](https://github.com/google/protobuf) (minimum 3.0.0) development package:
```sh
$ sudo apt-get install autoconf automake libtool curl make g++ unzip
$ git clone https://github.com/google/protobuf
$ cd protobuf
$ ./autogen.sh 
$ ./configure
$ make
$ make check
$ sudo make install
$ sudo ldconfig
```

Next install the [CMake](https://cmake.org/) (minimum 3.7.0-rc1) development package:
```sh
$ wget https://cmake.org/files/v3.7/cmake-3.7.0-rc1.tar.gz
$ tar -xvzf cmake-3.7.0-rc1.tar.gz
$ cd cmake-3.7.0-rc1
$ ./bootstrap
$ make 
$ sudo make install
$ sudo ln -s /usr/local/bin/cmake /usr/bin/cmake
```

Finally, install bitprim-build:
```sh
$ git clone --recursive https://github.com/bitprim/bitprim-build/
$ cd bitprim-build
$ mkdir build
$ cd build
$ cmake .. -G "Unix Makefiles" -DENABLE_TESTS=OFF -DWITH_TESTS=OFF -DWITH_TOOLS=OFF -DCMAKE_BUILD_TYPE=Release
$ make
$ sudo make install
$ sudo ldconfig
```
Bitprim is now installed in `/usr/local/`.

### Macintosh

The OSX installation differs from Linux in the installation of the compiler and packaged dependencies. Bitprim supports both [Homebrew](http://brew.sh) and [MacPorts](https://www.macports.org) package managers. Both require Apple's [Xcode](https://developer.apple.com/xcode) command line tools. Neither requires Xcode as the tools may be installed independently.

Bitprim compiles with Clang on OSX and requires C++11 support. Installation has been verified using Clang based on [LLVM 3.5](http://llvm.org/releases/3.5.0/docs/ReleaseNotes.html). This version or newer should be installed as part of the Xcode command line tools.

To see your Clang/LLVM  version:
```sh
$ clang++ --version
```
```
Apple LLVM version 6.0 (clang-600.0.54) (based on LLVM 3.5svn)
Target: x86_64-apple-darwin14.0.0
Thread model: posix
```
If required update your version of the command line tools as follows:
```sh
$ xcode-select --install
```

#### Using Homebrew

First install Homebrew. Installation requires [Ruby](https://www.ruby-lang.org/en) and [cURL](http://curl.haxx.se), which are pre-installed on OSX.
```sh
$ ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)"
```
You may encounter a prompt to install the Xcode command line developer tools, in which case accept the prompt.

Next install the [build system](http://wikipedia.org/wiki/GNU_build_system) (Automake minimum 1.14) and [wget](http://www.gnu.org/software/wget):
```sh
$ brew install autoconf automake libtool pkgconfig wget
```
Next install the [Boost](http://www.boost.org) (1.56.0 or newer) development package:
```sh
$ brew install boost
```
Next install the [ZeroMQ](http://zeromq.org/) (minimum 4.2.0) development package:
```
TODO: ZeroMQ Mac Install
```
Next install the [Protobuff](https://github.com/google/protobuf) (minimum 3.0.0) development package:
```
TODO: Protobuff Mac Install
```
Finally install bitprim:
```
TODO: Mac Install
```
Bitprim is now installed in `/usr/local/`.

#### Using MacPorts

First install [MacPorts](https://www.macports.org/install.php).

Next install the [build system](http://wikipedia.org/wiki/GNU_build_system) (Automake minimum 1.14) and [wget](http://www.gnu.org/software/wget):
```sh
$ sudo port install autoconf automake libtool pkgconfig wget
```
Next install the [Boost](http://www.boost.org) (1.56.0 or newer) development package. The `-` options remove MacPort defaults that are not Boost defaults:
```sh
$ sudo port install boost -no_single -no_static -python27
```
Next install the [ZeroMQ](http://zeromq.org/) (minimum 4.2.0) development package:
```
TODO: ZeroMQ MacPorts Install
```
Next install the [Protobuff](https://github.com/google/protobuf) (minimum 3.0.0) development package:
```
TODO: Protobuff MacPorts Install
```
Finally install bitprim:
```
TODO: MacPorts Install
```
Bitprim is now installed in `/usr/local/`.

#### Notes

The install script itself is commented so that the manual build steps for each dependency can be inferred by a developer.

You can run the install script from any directory on your system. By default this will build bitprim in a subdirectory named `build-bitprim` and install it to `/usr/local/`. The install script requires `sudo` only if you do not have access to the installation location, which you can change using the `--prefix` option on the installer command line.

The build script clones, builds and installs two unpackaged repositories, namely:

- [bitprim/secp256k1](https://github.com/bitprim/secp256k1)
- [bitprim/bitprim-core](https://github.com/bitprim/bitprim-core)

The script builds from the head of their `version4` and `version3` branches respectively. The `master` branch is a staging area for changes. The version branches are considered release quality.

### Windows
```
TODO:Windows Install
```
