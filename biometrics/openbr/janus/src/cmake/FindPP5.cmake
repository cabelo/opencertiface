# ==================================================== #
# The PittPatt 5.x.x CMake configuration file          #
#                                                      #
# Usage from an external project:                      #
#   In your CMakeLists.txt, add these lines:           #
#                                                      #
#   find_package(PP5 REQUIRED)                         #
#   target_link_libraries(MY_TARGET ${PP5_LIBS})       #
# ==================================================== #

include(FindPackageHandleStandardArgs)
find_path(PP5_DIR include/pittpatt_sdk.h)
find_package_handle_standard_args(PP5 DEFAULT_MSG PP5_DIR)
set(PP5_FOUND ${PP5_FOUND})
if(PP5_FOUND)
  include_directories(${PP5_DIR}/include)
  link_directories(${PP5_DIR}/lib)
  set(PP5_LIBS pittpatt_raw_image
               pittpatt_raw_image_io
               pittpatt_sdk
               pittpatt_video_io)
endif()
