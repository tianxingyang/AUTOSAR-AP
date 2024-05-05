function (add_project_library lib_name lib_sources)
  add_library(${ARGV0} SHARED ${${ARGV1}})
  
  set_target_properties(
    ${ARGV0} PROPERTIES 
    LINKER_LANGUAGE CXX
    SOVERSION ${VitoAp_VERSION}
    VERSION ${VitoAp_VERSION_MAJOR}
    OUTPUT_NAME "vito_ap_${ARGV0}"
  )

  target_include_directories(${ARGV0} PUBLIC ${CMAKE_SOURCE_DIR}/include/public)
endfunction(add_project_library)