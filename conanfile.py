from conans import ConanFile, CMake
import os

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
    exports_sources = "src/*", "CMakeLists.txt", "cmake/*", "bitprim-coreConfig.cmake.in", "include/*"
    package_files = "build/lbitprim-core.a"

    requires = (("bitprim-conan-boost/1.64.0@bitprim/stable"),
               ("secp256k1/0.1@bitprim/stable"))

    def build(self):
        #print self.deps_env_info["secp256k1"].CPATH
        #print self.deps_env_info["secp256k1"].C_INCLUDE_PATH
        #print self.deps_env_info["secp256k1"].CPLUS_INCLUDE_PATH
        if "CPATH" in os.environ:
            os.environ["CPATH"] += os.pathsep + self.deps_env_info["secp256k1"].CPATH[0]
        else:
            os.environ["CPATH"] = self.deps_env_info["secp256k1"].CPATH[0]
        cmake = CMake(self)
        cmake.configure(source_dir=self.conanfile_directory)
        cmake.build()

    def imports(self):
        #self.copy("*.dll", "", "bin")
        #self.copy("*.dylib", "", "lib")
        self.copy("*.h", "", "include")

    def package(self):
        self.copy("*.h", dst="include", src="src")
        self.copy("*.lib", dst="lib", keep_path=False)
        self.copy("*.dll", dst="bin", keep_path=False)
        self.copy("*.dylib*", dst="lib", keep_path=False)
        self.copy("*.so", dst="lib", keep_path=False)
        self.copy("*.a", dst="lib", keep_path=False)

    def package_info(self):
        self.cpp_info.includedirs = ['include']
        self.cpp_info.libs = ["bitprim-core"]
