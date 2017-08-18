from conan.packager import ConanMultiPackager

if __name__ == "__main__":
    builder = ConanMultiPackager(username="bitprim", channel="stable",
                                 remotes="https://api.bintray.com/conan/bitprim/bitprim",
                                 archs=["x86_64"])

    builder.add_common_builds(shared_option_name="bitprim-core:shared", pure_c=False)

    filtered_builds = []
    for settings, options, env_vars, build_requires in builder.builds:
        if settings["build_type"] == "Release" \
                and options["bitprim-core:shared"] == False:

            #settings = replace_libcxx(settings)
            filtered_builds.append([settings, options, env_vars, build_requires])

            # #TODO: Adding gcc 4.9 c++11 build manually until Conan fixes it
            # if settings["compiler"] == "gcc" and settings["compiler.version"] == "4.9":
            #     # new_settings={'compiler.version': '4.9', 'compiler.libcxx': 'libstdc++11', 'arch': 'x86_64', 'build_type': 'Release', 'compiler': 'gcc'}, options={'bitprim-conan-boost:shared': False})
            #     # new_settings = {'compiler.libcxx': settings["compiler.libcxx"], 'compiler.exception': settings["compiler.exception"], 'compiler.threads': settings["compiler.threads"], 'compiler.version': settings["compiler.version"], 'arch': settings["arch"], 'build_type': settings["build_type"], 'compiler': settings["compiler"]}
            #     new_settings = {'compiler.libcxx': 'libstdc++11', 'compiler.exception': settings["compiler.exception"], 'compiler.threads': settings["compiler.threads"], 'compiler.version': settings["compiler.version"], 'arch': settings["arch"], 'build_type': settings["build_type"], 'compiler': settings["compiler"]}
            #     filtered_builds.append([new_settings, options, env_vars, build_requires])

            #     # ERROR: C:\Users\appveyor\.conan\data\mingw_installer\1.0\conan\stable\export\conanfile.py: Error while initializing settings. Invalid setting 'libstdc++11' is not a valid 'settings.compiler.libcxx' value.
            #     # Possible values are ['libstdc++']

            # else:
            #     filtered_builds.append([settings, options, env_vars, build_requires])

    builder.builds = filtered_builds
    builder.run()
