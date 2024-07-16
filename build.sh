#!/bin/bash
conan install -of=third-party --build=missing --profile=ap_debug .

if [ ! -d build ]; then
    mkdir build
fi

cd build
cmake -G "Ninja" -DCMAKE_BUILD_TYPE="Debug" -DCMAKE_TOOLCHAIN_FILE=x86_64_toolchain ..
cmake --build . -j16