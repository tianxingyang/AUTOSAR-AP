#!/bin/bash

if [ $# -eq 1 ] && [ $1 = "clean" ]; then
    echo "Cleaning build directory..."
    rm -rf build
    exit 0
fi

build_type="Debug"
#build_type="Release"

conan install -of=third-party --build=missing --profile=./ap_$build_type .

if [ ! -d build ]; then
    mkdir build
fi

cd build
cmake -G "Ninja" -DCMAKE_BUILD_TYPE="$build_type" -DCMAKE_VERBOSE_MAKEFILE=ON -DCMAKE_INSTALL_PREFIX=../install -DCMAKE_TOOLCHAIN_FILE=x86_64_toolchain ..
cmake --build . -j$(nproc)
cmake --install .