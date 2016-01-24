# Install script for directory: /opt/Modoki/openbr/3rdparty/stasm4.0.0

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "0")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE FILE FILES
    "/opt/Modoki/openbr/3rdparty/stasm4.0.0/stasm/hat.h"
    "/opt/Modoki/openbr/3rdparty/stasm4.0.0/stasm/basedesc.h"
    "/opt/Modoki/openbr/3rdparty/stasm4.0.0/stasm/stasmcascadeclassifier.h"
    "/opt/Modoki/openbr/3rdparty/stasm4.0.0/stasm/eyedet.h"
    "/opt/Modoki/openbr/3rdparty/stasm4.0.0/stasm/err.h"
    "/opt/Modoki/openbr/3rdparty/stasm4.0.0/stasm/print.h"
    "/opt/Modoki/openbr/3rdparty/stasm4.0.0/stasm/shapemod.h"
    "/opt/Modoki/openbr/3rdparty/stasm4.0.0/stasm/convshape.h"
    "/opt/Modoki/openbr/3rdparty/stasm4.0.0/stasm/asm.h"
    "/opt/Modoki/openbr/3rdparty/stasm4.0.0/stasm/stasm_lib.h"
    "/opt/Modoki/openbr/3rdparty/stasm4.0.0/stasm/pinstart.h"
    "/opt/Modoki/openbr/3rdparty/stasm4.0.0/stasm/startshape.h"
    "/opt/Modoki/openbr/3rdparty/stasm4.0.0/stasm/hatdesc.h"
    "/opt/Modoki/openbr/3rdparty/stasm4.0.0/stasm/eyedist.h"
    "/opt/Modoki/openbr/3rdparty/stasm4.0.0/stasm/faceroi.h"
    "/opt/Modoki/openbr/3rdparty/stasm4.0.0/stasm/stasmhash.h"
    "/opt/Modoki/openbr/3rdparty/stasm4.0.0/stasm/stasm_landmarks.h"
    "/opt/Modoki/openbr/3rdparty/stasm4.0.0/stasm/landmarks.h"
    "/opt/Modoki/openbr/3rdparty/stasm4.0.0/stasm/shape17.h"
    "/opt/Modoki/openbr/3rdparty/stasm4.0.0/stasm/classicdesc.h"
    "/opt/Modoki/openbr/3rdparty/stasm4.0.0/stasm/stasm_lib_ext.h"
    "/opt/Modoki/openbr/3rdparty/stasm4.0.0/stasm/shapehacks.h"
    "/opt/Modoki/openbr/3rdparty/stasm4.0.0/stasm/misc.h"
    "/opt/Modoki/openbr/3rdparty/stasm4.0.0/stasm/stasm.h"
    "/opt/Modoki/openbr/3rdparty/stasm4.0.0/stasm/MOD_1/yaw00.h"
    "/opt/Modoki/openbr/3rdparty/stasm4.0.0/stasm/MOD_1/initasm.h"
    "/opt/Modoki/openbr/3rdparty/stasm4.0.0/stasm/MOD_1/facedet.h"
    )
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libstasm.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libstasm.so")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libstasm.so"
         RPATH "/usr/local/lib:/usr/lib")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES "/opt/Modoki/openbr/3rdparty/stasm4.0.0/build/libstasm.so")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libstasm.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libstasm.so")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libstasm.so"
         OLD_RPATH ":::::::::::::::::::::::"
         NEW_RPATH "/usr/local/lib:/usr/lib")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libstasm.so")
    endif()
  endif()
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("/opt/Modoki/openbr/3rdparty/stasm4.0.0/build/stasm/cmake_install.cmake")

endif()

