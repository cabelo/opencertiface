project(janus)
cmake_minimum_required(VERSION 2.8.6)

set(JANUS_NAME "Janus")
set(JANUS_DESCRIPTION "IARPA Janus Program API")

# Extract version info from header file
macro(janus_get_version VAR)
  file(STRINGS ${CMAKE_CURRENT_SOURCE_DIR}/include/iarpa_janus.h LINE
       REGEX "#define[ ]+${VAR}[ ]+[0-9]+")
  string(REGEX MATCH "[0-9]+" ${VAR} ${LINE})
endmacro(janus_get_version)
janus_get_version(JANUS_VERSION_MAJOR)
janus_get_version(JANUS_VERSION_MINOR)
janus_get_version(JANUS_VERSION_PATCH)
configure_file(include/iarpa_janus.h ${CMAKE_CURRENT_BINARY_DIR}/refreshes_version_when_changed)
set(JANUS_VERSION ${JANUS_VERSION_MAJOR}.${JANUS_VERSION_MINOR}.${JANUS_VERSION_PATCH})

if(UNIX)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -fvisibility=hidden")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -fvisibility=hidden")
endif()

include(CTest)
add_subdirectory(src)
