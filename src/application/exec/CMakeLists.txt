project(execution_manager)

ADD_PROJECT_EXECUTABLE(
  NAME
    ${PROJECT_NAME}
  SOURCES
    execution_manager.cpp
  DEPENDENCIES
    core
    log
)

ADD_PROJECT_LIBRARY(
  NAME
    execution_client
  SOURCES
    execution_client.cpp
    exec_error_domain.cpp
  PRIVATE_DEPENDENCIES
    core
    log
)