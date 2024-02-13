# Set the compiler options
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "--coverage -g -O0 -Wall -fprofile-arcs -ftest-coverage")
set(CMAKE_CXX_OUTPUT_EXTENSION_REPLACE ON)

# Create OBJECT_DIR variable
set(OBJECT_DIR ${CMAKE_BINARY_DIR}/test/CMakeFiles/${PROJECT_NAME}_tests.dir)
message("-- Object files will be output to: ${OBJECT_DIR}")


include(CMakePrintHelpers)
include(FetchContent)
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG        v1.14.0
)
FetchContent_MakeAvailable(googletest)

set(TEST_NAME ${PROJECT_NAME}_tests)
add_executable(${TEST_NAME} client_test.cpp)
target_link_libraries(${TEST_NAME}
        gtest_main
)

enable_testing()
include(GoogleTest)
gtest_discover_tests(${TEST_NAME})

# Create the gcov target. Run coverage tests with 'make gcov'
add_custom_target(gcov
        COMMAND mkdir -p gcoverage
        COMMAND ${CMAKE_MAKE_PROGRAM} test
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        )
add_custom_command(TARGET gcov
        COMMAND echo "=================== GCOV ===================="
        COMMAND gcov -b ${CMAKE_SOURCE_DIR}/test/*.cpp -o ${OBJECT_DIR} | grep -A 5 "Utils_test" > CoverageSummary.tmp
        COMMAND cat CoverageSummary.tmp
        COMMAND echo "-- Coverage files have been output to ${CMAKE_BINARY_DIR}/gcoverage"
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/gcoverage
        )

add_custom_target(coverage
        COMMAND mkdir -p gcoverage
        COMMAND ${CMAKE_MAKE_PROGRAM} test
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        )
add_custom_command(TARGET coverage
        COMMAND echo "=================== GCOV ===================="
        COMMAND gcovr -r ${CMAKE_SOURCE_DIR} --exclude ${CMAKE_SOURCE_DIR}/build/_deps --exclude ${CMAKE_SOURCE_DIR}/test --html-details -o ${CMAKE_BINARY_DIR}/gcoverage/coverage.html
        COMMAND echo "-- Coverage files have been output to ${CMAKE_BINARY_DIR}/gcoverage"
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/gcoverage
        )

# Make sure to clean up the coverage folder
set_property(DIRECTORY APPEND PROPERTY ADDITIONAL_MAKE_CLEAN_FILES coverage)

# Create the gcov-clean target. This cleans the build as well as generated
# .gcda and .gcno files.
add_custom_target(scrub
        COMMAND ${CMAKE_MAKE_PROGRAM} clean
        COMMAND rm -f ${OBJECT_DIR}/*.gcno
        COMMAND rm -f ${OBJECT_DIR}/*.gcda
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        )

