cmake_minimum_required(VERSION 3.19)

include(cmake/vito_ap.cmake)

project(VitoAp VERSION 0.0.1)

add_compile_options(-Wall)

set(CMAKE_CXX_STANDARD 20)

set(CONAN_GEN_DIR ${CMAKE_SOURCE_DIR}/third-party/build/Debug/generators)

find_package(fmt REQUIRED
PATHS ${CONAN_GEN_DIR}
)

find_package(asio REQUIRED
PATHS ${CONAN_GEN_DIR}
)

add_subdirectory(src)