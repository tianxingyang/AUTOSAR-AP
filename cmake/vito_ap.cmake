function (add_project_library)
  set(options)
  set(oneValueArgs NAME)
  set(multiValueArgs 
    SOURCES 
    PRIVATE_DEPENDENCIES 
    PUBLIC_DEPENDENCIES 
    PRIVATE_INCLUDES 
    PUBLIC_INCLUDES
  )

  cmake_parse_arguments(LIB "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

  add_library(${LIB_NAME} SHARED ${LIB_SOURCES})

  target_include_directories(${LIB_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/include/public)
  target_include_directories(${LIB_NAME} PRIVATE ${LIB_PRIVATE_INCLUDES})

  target_link_libraries(${LIB_NAME} PUBLIC ${LIB_PUBLIC_DEPENDENCIES})
  target_link_libraries(${LIB_NAME} PRIVATE ${LIB_PRIVATE_DEPENDENCIES})
  
  set_target_properties(
    ${LIB_NAME} PROPERTIES 
    LINKER_LANGUAGE CXX
    VERSION ${VitoAp_VERSION}
    SOVERSION ${VitoAp_VERSION_MAJOR}
    OUTPUT_NAME "vito_ap_${LIB_NAME}"
  )

  install(TARGETS ${LIB_NAME} DESTINATION ${CMAKE_INSTALL_LIBDIR})
endfunction(add_project_library)

function(add_project_executable)
  set(options)
  set(oneValueArgs NAME)
  set(multiValueArgs SOURCES DEPENDENCIES)

  cmake_parse_arguments(EXEC "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})
  add_executable(${EXEC_NAME} ${EXEC_SOURCES})
  target_link_libraries(${EXEC_NAME} PRIVATE ${EXEC_DEPENDENCIES})
  install(TARGETS ${EXEC_NAME} DESTINATION ${CMAKE_INSTALL_BINDIR}/${EXEC_NAME}/bin)
  install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/config/ DESTINATION ${CMAKE_INSTALL_BINDIR}/${EXEC_NAME}/etc)
endfunction(add_project_executable)
