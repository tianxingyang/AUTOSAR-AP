cmake_minimum_required(VERSION 3.22)

project(VitoAp VERSION 0.0.1)

add_compile_options(-Wall)

set(CMAKE_CXX_STANDARD 20)

find_package(fmt REQUIRED)
find_package(asio REQUIRED)

add_subdirectory(src)