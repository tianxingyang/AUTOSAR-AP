project(core)

add_project_library(
  NAME 
    ${PROJECT_NAME}
  SOURCES
    error_code.cpp
    core_error_domain.cpp
    future_error_domain.cpp
    initialization.cpp
    abort.cpp
  PRIVATE_DEPENDENCIES 
    fmt::fmt
  PRIVATE_INCLUDES
    ${CMAKE_SOURCE_DIR}/include/private
)