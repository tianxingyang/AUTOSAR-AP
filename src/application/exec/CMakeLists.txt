project(execution_manager)

set(EXEC_NAME execution_manager)

add_project_executable(
  NAME
    execution_manager
  SOURCES
    execution_manager.cpp
  DEPENDENCIES
    core
    log
)