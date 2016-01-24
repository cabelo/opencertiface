add_library(pittpatt SHARED ../janus.cpp pittpatt.c pittpatt_io.c)
set_target_properties(pittpatt PROPERTIES
                      DEFINE_SYMBOL JANUS_LIBRARY
                      VERSION ${JANUS_VERSION_MAJOR}.${JANUS_VERSION_MINOR}.${JANUS_VERSION_PATCH}
                      SOVERSION ${JANUS_VERSION_MAJOR}.${JANUS_VERSION_MINOR})
target_link_libraries(pittpatt ${PP5_LIBS})
install(TARGETS pittpatt RUNTIME DESTINATION bin
                         LIBRARY DESTINATION lib
                         ARCHIVE DESTINATION lib)

# Add this to the list of implementations to test
set(JANUS_TEST_IMPLEMENTATION ${JANUS_TEST_IMPLEMENTATIONS} "pittpatt" PARENT_SCOPE)
set(JANUS_TEST_SDK_PATH ${JANUS_TEST_SDK_PATHS} ${PP5_DIR} PARENT_SCOPE)
