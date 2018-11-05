# Bitprim Domain <a target="_blank" href="http://semver.org">![Version][badge.version]</a> <a target="_blank" href="https://travis-ci.org/bitprim/bitprim-domain">![Travis status][badge.Travis]</a> <a target="_blank" href="https://ci.appveyor.com/project/bitprim/bitprim-domain">![Appveyor status][badge.Appveyor]</a> <a target="_blank" href="https://gitter.im/bitprim/Lobby">![Gitter Chat][badge.Gitter]</a>

> Domain functionality

## Installation Requirements

- 64-bit machine.
- [Conan](https://www.conan.io/) package manager, version 1.4.0 or newer. See [Conan Installation](http://docs.conan.io/en/latest/installation.html#install-with-pip-recommended).

## Installation Procedure

The *Bitprim* libraries can be installed on Linux, macOS, FreeBSD, Windows and others. These binaries are pre-built for the most usual operating system/compiler combinations and hosted in an online repository. If there are no pre-built binaries for your platform, a build from source will be attempted.

So, for any platform, an installation can be performed in 2 simple steps:

1. Configure the Conan remote:
```
conan remote add bitprim https://api.bintray.com/conan/bitprim/bitprim
```

2. Install the appropriate library:

```
conan install bitprim-domain/0.X@bitprim/stable 
```

In you want to tune the installation for better performance, please refer to [this](https://bitprim.github.io/docfx/content/user_guide/installation.html#advanced-installation).


<!-- Links -->
[badge.Appveyor]: https://ci.appveyor.com/api/projects/status/github/bitprim/bitprim-domain?svg=true&branch=dev
[badge.Gitter]: https://img.shields.io/badge/gitter-join%20chat-blue.svg
[badge.Travis]: https://travis-ci.org/bitprim/bitprim-domain.svg?branch=master
[badge.version]: https://badge.fury.io/gh/bitprim%2Fbitprim-domain.svg

[CMake]: http://www.cmake.org
[Doxygen]: http://www.doxygen.org
[eRuby]: http://en.wikipedia.org/wiki/ERuby
[Hana.docs]: http://boostorg.github.io/hana
[Hana.wiki]: https://github.com/boostorg/hana/wiki
[Homebrew formula]: https://github.com/Homebrew/homebrew-domain/blob/master/Formula/hana.rb


