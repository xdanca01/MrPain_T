# ----------------------------------------------------------------------------
# This is the configuration file for the FRAMEWORK_OPENGL_3_3 module.
# The FRAMEWORK_OPENGL_3_3 module extends the FRAMEWORK_CORE 
# with code supporting OpenGL version 3.3.
# ----------------------------------------------------------------------------

# Sets the minimum required version of cmake.
cmake_minimum_required(VERSION 3.14)

# Sets the name of the project, and stores it in the variable PROJECT_NAME.
project(FRAMEWORK_OPENGL_3_3 CXX)

# Sets up a new target if it was not already registered.
if(NOT TARGET ${PROJECT_NAME})
    # Adds a new library target.
    add_library(${PROJECT_NAME} STATIC)

    # Sets the build properties.
    # CXX_STANDARD - the C++ standard whose features are requested.
    # CXX_EXTENSIONS - no compiler specific extensions are requested.
    set_target_properties(${PROJECT_NAME} PROPERTIES 
        CXX_STANDARD 20             
        CXX_EXTENSIONS OFF
    )

    # Specifies the include directories to use when compiling the library target defined above.
    target_include_directories(${PROJECT_NAME} PUBLIC include)

    # Collects the source files and specified them as target sources.
    target_sources(
        ${PROJECT_NAME} 
        PRIVATE 
            include/geometry_3_3.hpp
            include/texture_3_3.hpp
            src/geometry_3_3.cpp
            src/texture_3_3.cpp
    )

    # Specifies libraries to use when linking the library target defined above.
    target_link_libraries(
        ${PROJECT_NAME}
        PUBLIC
            FRAMEWORK_CORE
    )
endif()