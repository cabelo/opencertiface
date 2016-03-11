include_directories(BEFORE ../include)
file(GLOB JANUS_HEADERS ../include/*.h)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

option(JANUS_BUILD_PP5_WRAPPER "Build Janus implementation using PittPatt 5" OFF)
if(${JANUS_BUILD_PP5_WRAPPER})
  find_package(PP5 REQUIRED)
  add_subdirectory(pittpatt)
endif()

option(JANUS_BUILD_OPENCV_WRAPPER "Build Janus I/O implementation using OpenCV" OFF)
if(${JANUS_BUILD_OPENCV_WRAPPER})
  find_package(OpenCV)
  if(OpenCV_FOUND AND PP5_FOUND)
    add_subdirectory(opencv_io)
  endif()
endif()

# Janus API documentation
add_subdirectory(doxygen)

# Janus command line utilities
add_subdirectory(utils)
