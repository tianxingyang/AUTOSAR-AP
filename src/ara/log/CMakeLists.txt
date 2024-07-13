project(log)

set(${PROJECT_NAME}_SOURCES 
  "logger.cpp"
  "log_stream.cpp"
  "log_error_domain.cpp"
  "dlt_message.cpp"
  "logger_manager.cpp"
  "logging_handler.cpp"
)

add_project_library(${PROJECT_NAME} ${PROJECT_NAME}_SOURCES)

target_include_directories(${PROJECT_NAME} PRIVATE
  ${CMAKE_SOURCE_DIR}/include/private
)

target_link_libraries(${PROJECT_NAME} PRIVATE
  core
  fmt::fmt)