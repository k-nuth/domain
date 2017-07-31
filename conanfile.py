from conans import ConanFile, CMake


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
    exports_sources = "src/*", "CMakeLists.txt", "cmake/*"
    package_files = "build/lbitprim-core.a"




    requires = (("bitprim-conan-boost/1.64.0@bitprim/stable"),
               ("secp256k1/0.1@bitprim/stable"))

#    def build(self):
#        cmake = CMake(self)
#        cmake.configure(source_dir=self.conanfile_directory)
#        cmake.build()

    def package(self):
        self.copy("*.h", dst="include", src="src")
        self.copy("*.lib", dst="lib", keep_path=False)
        self.copy("*.dll", dst="bin", keep_path=False)
        self.copy("*.dylib*", dst="lib", keep_path=False)
        self.copy("*.so", dst="lib", keep_path=False)
        self.copy("*.a", dst="lib", keep_path=False)

    def package_info(self):
        self.cpp_info.libs = ["bitprim-core"]
