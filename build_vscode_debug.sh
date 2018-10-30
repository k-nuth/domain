mkdir build2
cd build2
# rm -rf *
conan install .. -o with_tests=True -s build_type=Debug
conan build ..