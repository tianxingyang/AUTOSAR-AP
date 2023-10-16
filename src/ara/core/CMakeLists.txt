project(core)

add_library(core OBJECT
 error_code.cpp
)

set_target_properties(core PROPERTIES LINKER_LANGUAGE CXX)

target_include_directories(core PUBLIC ${CMAKE_SOURCE_DIR}/include/public)