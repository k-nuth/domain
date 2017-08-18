# import os
from conan.packager import ConanMultiPackager

#def is_clang_or_gcc5(sett):
#    # precondition: sett["compiler"] and sett["compiler.version"] are valid
#    return sett["compiler"] == "clang" or (sett["compiler"] == "gcc" and float(sett["compiler.version"]) >= 5)

#def replace_libcxx(sett):
#    if "compiler" in sett and "compiler.libcxx" in sett and "compiler.version" in sett and is_clang_or_gcc5(sett) and sett["compiler.libcxx"] == "libstdc++":
#        sett["compiler.libcxx"] = "libstdc++11"
#    return sett

if __name__ == "__main__":
    builder = ConanMultiPackager(username="bitprim", channel="testing",
                                 remotes="https://api.bintray.com/conan/bitprim/bitprim",
                                 archs=["x86_64"])

    builder.add_common_builds(shared_option_name="bitprim-core:shared", pure_c=False)

    filtered_builds = []
    add_special_case = False
    for settings, options, env_vars, build_requires in builder.builds:
        if settings["build_type"] == "Release" \
                and options["bitprim-core:shared"] == False:

            #settings = replace_libcxx(settings)
            #filtered_builds.append([settings, options, env_vars, build_requires])

            #TODO: Adding gcc 4.9 c++11 build manually until Conan fixes it
            if settings["compiler"] == "gcc" and settings["compiler.version"] == "4.9":
                add_special_case = True
            else:
                if not "compiler.libcxx" in settings or settings["compiler"] == "apple-clang" or ("compiler.libcxx" in settings and settings["compiler.libcxx"] == "libstdc++"):
                    filtered_builds.append([settings, options, env_vars, build_requires])

    builder.builds = filtered_builds
    if add_special_case:
        builder.add(settings={'compiler.version': '4.9', 'compiler.libcxx': 'libstdc++11', 'arch': 'x86_64', 'build_type': 'Release', 'compiler': 'gcc'}, options={'bitprim-conan-boost:shared': False})
    builder.run()
