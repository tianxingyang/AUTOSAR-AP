project(execution_manager)

set(EXEC_NAME execution_manager)

add_executable(${EXEC_NAME} execution_manager.cpp)

target_link_libraries(${EXEC_NAME} PRIVATE 
  core
)