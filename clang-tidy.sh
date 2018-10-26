export PATH=/usr/local/Cellar/llvm/6.0.0/bin/:$PATH
export PATH=/Users/fernando/Downloads/clang+llvm-7.0.0-x86_64-apple-darwin/bin:$PATH



python ../ci_utils/run-clang-tidy.py -checks='*' -warnings-as-errors='*' -header-filter=/Users/fernando/dev/bitprim-domain/include

python ../ci_utils/run-clang-tidy.py -checks='*,-fuchsia*,-llvm-header-guard,-modernize-pass-by-value,-cert-dcl21-cpp,-google-build-using-namespace,-google-runtime-references,-cert-err58-cpp,-cppcoreguidelines-pro-bounds-array-to-pointer-decay,-hicpp-no-array-decay' -warnings-as-errors='*' -header-filter=/home/fernando/dev/bitprim-domain/include


python ../ci_utils/run-clang-tidy.py -checks='modernize*' -warnings-as-errors='*' -header-filter=/Users/fernando/dev/bitprim-domain/include
python ../ci_utils/run-clang-tidy.py -checks='modernize-use-using*' -warnings-as-errors='*' -header-filter=/Users/fernando/dev/bitprim-domain/include/
python ../ci_utils/run-clang-tidy.py -checks='*,-llvm-header-guard,-fuchsia*,-google-runtime-references,-cert-err58-cpp' -warnings-as-errors='*' -header-filter=/Users/fernando/dev/bitprim-domain/include



