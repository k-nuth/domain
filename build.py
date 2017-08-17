# import os
from conan.packager import ConanMultiPackager

def is_clang_or_gcc5(sett):
    # precondition: sett["compiler"] and sett["compiler.version"] are valid
    return sett["compiler"] == "clang" or (sett["compiler"] == "gcc" and float(sett["compiler.version"]) >= 5)

def replace_libcxx(sett):
    if "compiler" in sett and "compiler.libcxx" in sett and "compiler.version" in sett and is_clang_or_gcc5(sett) and sett["compiler.libcxx"] == "libstdc++":
        sett["compiler.libcxx"] = "libstdc++11"
    return sett

if __name__ == "__main__":
    builder = ConanMultiPackager(username="bitprim", channel="testing",
                                 remotes="https://api.bintray.com/conan/bitprim/bitprim")

    builder.add_common_builds(shared_option_name="bitprim-core:shared")
    # builder.password = os.getenv("CONAN_PASSWORD")

    filtered_builds = []
    for settings, options, env_vars, build_requires in builder.builds:
        if settings["build_type"] == "Release" \
                and settings["arch"] == "x86_64" \
                and options["bitprim-core:shared"] == False:

            settings = replace_libcxx(settings)
            filtered_builds.append([settings, options, env_vars, build_requires])

    builder.builds = filtered_builds
    builder.run()
