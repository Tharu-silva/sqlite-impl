if [[ $(basename "$PWD") != "build" ]]; then
    cd build
fi
cmake .. -DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Release
make