project(core)

set(${PROJECT_NAME}_SOURCES 
  "error_code.cpp"
  "core_error_domain.cpp"
  "future_error_domain.cpp"
  "initialization.cpp"
  "abort.cpp"
)

add_project_library(${PROJECT_NAME} ${PROJECT_NAME}_SOURCES)

target_include_directories(${PROJECT_NAME} PRIVATE 
  ${CMAKE_SOURCE_DIR}/include/private
)

target_link_libraries(${PROJECT_NAME} 
  PRIVATE
  fmt::fmt)