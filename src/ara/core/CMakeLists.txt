project(core)

set(${PROJECT_NAME}_SOURCES 
  "error_code.cpp"
  "core_error_domain.cpp"
  "future_error_domain.cpp"
  "initialization.cpp"
  "abort.cpp")

add_project_library(core ${PROJECT_NAME}_SOURCES)

target_link_libraries(${PROJECT_NAME} 
  PRIVATE
  fmt::fmt)