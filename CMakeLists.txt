cmake_minimum_required(VERSION 3.19)

set(CMAKE_EXPORT_COMPILE_COMMANDS on)
set(CMAKE_COMPILE_WARNING_AS_ERROR on)

include(cmake/vito_ap.cmake)

project(VitoAp VERSION 0.0.1)

add_compile_options(-Wall)

set(CMAKE_CXX_STANDARD 20)

set(CONAN_GEN_DIR ${CMAKE_SOURCE_DIR}/third-party/build/${CMAKE_BUILD_TYPE}/generators)
message(STATUS "CONAN_GEN_DIR: ${CONAN_GEN_DIR}")

find_package(fmt REQUIRED
  PATHS ${CONAN_GEN_DIR}
)

find_package(asio REQUIRED
  PATHS ${CONAN_GEN_DIR}
)

find_package(nlohmann_json REQUIRED
  PATHS ${CONAN_GEN_DIR}
)

add_subdirectory(src)