project(core)

add_library(core SHARED
 "error_code.cpp"
 "core_error_domain.cpp"
 "future_error_domain.cpp"
 "initialization.cpp")

set_target_properties(core PROPERTIES LINKER_LANGUAGE CXX)

target_include_directories(core PUBLIC ${CMAKE_SOURCE_DIR}/include/public)