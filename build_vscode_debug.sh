mkdir build
cd build
# rm -rf *
conan install .. -o currency=BTC -o with_tests=True -s build_type=Debug
conan build ..