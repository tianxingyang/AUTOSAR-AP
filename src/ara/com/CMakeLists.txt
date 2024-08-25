project(com)

ADD_PROJECT_LIBRARY(
  NAME
    ${PROJECT_NAME}
  SOURCES
    proxy_base.cpp
    skeleton_base.cpp
    com_error_domain.cpp
    network_binding_factory.cpp
    instance_identifier.cpp
  PRIVATE_DEPENDENCIES
    core
    log
)