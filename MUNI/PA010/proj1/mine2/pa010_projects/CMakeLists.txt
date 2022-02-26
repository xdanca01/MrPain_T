# ----------------------------------------------------------------------------
# This is the main configuration file for the PA010 projects.
# ----------------------------------------------------------------------------

# Adds the required parts of the framework.
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../framework/core ${CMAKE_CURRENT_BINARY_DIR}/framework/core)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../framework/opengl_3_3 ${CMAKE_CURRENT_BINARY_DIR}/framework/opengl_3_3)

# Adds all required projects.
add_subdirectory(subdivision)
