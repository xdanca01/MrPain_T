set(TEST_NAME ${PROJECT_NAME}_tests)

add_executable(${TEST_NAME}
    ./tests.cpp
    ../application.hpp
    ../application.cpp
    ../application_details.cpp
    )

add_test(NAME ${TEST_NAME}
         COMMAND ${TEST_NAME})

set_target_properties(${TEST_NAME} PROPERTIES 
    CXX_STANDARD 20             
    CXX_EXTENSIONS OFF
)

target_link_libraries(${TEST_NAME}
    PRIVATE    
        FRAMEWORK_CORE FRAMEWORK_OPENGL_3_3
)

file(
    GENERATE
    OUTPUT "$<TARGET_FILE_DIR:${TEST_NAME}>/configuration.toml"
    CONTENT "data = \"${CMAKE_SOURCE_DIR}/pa010_projects/noise/tests/data\""
)