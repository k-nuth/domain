mkdir build
cd build
# rm -rf *
conan install .. -o currency=BTC -o tests=True -s build_type=Debug
conan build ..