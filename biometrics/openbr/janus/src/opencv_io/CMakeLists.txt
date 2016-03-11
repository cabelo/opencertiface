add_library(opencv_io SHARED opencv_io.cpp)
set_target_properties(opencv_io PROPERTIES
                      DEFINE_SYMBOL JANUS_LIBRARY
                      VERSION ${JANUS_VERSION_MAJOR}.${JANUS_VERSION_MINOR}.${JANUS_VERSION_PATCH}
                      SOVERSION ${JANUS_VERSION_MAJOR}.${JANUS_VERSION_MINOR})
target_link_libraries(opencv_io opencv_core opencv_highgui)
install(TARGETS opencv_io RUNTIME DESTINATION bin
                          LIBRARY DESTINATION lib
                          ARCHIVE DESTINATION lib)

# Add this to the list of implementations to test
option(JANUS_TEST_OPENCV_IO "Test the OpenCV implementation of the Janus I/O API" ON)
if(${JANUS_TEST_OPENCV_IO})
  set(JANUS_IO_TEST_IMPLEMENTATIONS ${JANUS_IO_TEST_IMPLEMENTATIONS} "opencv_io" PARENT_SCOPE)
endif()
