cmake_minimum_required(VERSION 3.15)
project(asabru_client)
# Enables generation of compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(CMakePrintHelpers)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Add compiler warnings
if (CMAKE_COMPILER_IS_GNUCXX)
    add_compile_options(-Wall)
endif ()

# add dependencies
include(../cmake/CPM.cmake)

# Set DEBUG preprocessor flag
add_compile_definitions(DEBUG)

# Collect all source files
file(GLOB_RECURSE SOURCE_FILES "./*.cpp")

# Add build directory
set(BUILD_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/build")

# Set library directory
set(LIB_DIRECTORY "lib")

MACRO(HEADER_DIRECTORIES search_path return_list)
    FILE(GLOB_RECURSE new_list ${search_path})
    SET(dir_list "")
    FOREACH(file_path ${new_list})
        GET_FILENAME_COMPONENT(dir_path ${file_path} PATH)
        SET(dir_list ${dir_list} ${dir_path})
    ENDFOREACH()
    LIST(REMOVE_DUPLICATES dir_list)
    SET(${return_list} ${dir_list})
ENDMACRO()

HEADER_DIRECTORIES("../../asabru-commons/src/*.h" header_dir_list)
include_directories(${header_dir_list})
HEADER_DIRECTORIES("../../asabru-engine/src/*.h" header_dir_list)
include_directories(${header_dir_list})
include_directories(../../asabru-parsers/src)
include_directories(../../nlohmann_json/single_include)
# Find SqlLite3
find_package (SQLite3 REQUIRED)
include_directories(${SQLite3_INCLUDE_DIRS})

#dependencies
set( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake/modules" )

## Find the readline package
find_package(Readline REQUIRED)
include_directories(${Readline_INCLUDE_DIRS})
# Create a library target
add_executable(${PROJECT_NAME} ${SOURCE_FILES})

# Set output directory for libraries
set_target_properties(${PROJECT_NAME}
        PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${LIB_DIRECTORY}
        )

if (ASABRU_COMMONS_BUILD STREQUAL "GIT_TAG")
    CPMAddPackage(
            NAME asabru_commons
            GITHUB_REPOSITORY ChistaDATA/asabru-commons
            GIT_TAG ${ASABRU_COMMONS_GIT_TAG}
    )
elseif (ASABRU_COMMONS_BUILD STREQUAL "LOCAL_DIR")
    # For development purpose
    CPMAddPackage(
            NAME asabru_commons
            SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../asabru-commons/src
    )
else()
    CPMAddPackage(
            NAME asabru_commons
            GITHUB_REPOSITORY ChistaDATA/asabru-commons
            VERSION 0.0.2
    )
endif()

if (ASABRU_ENGINE_BUILD STREQUAL "GIT_TAG")
    CPMAddPackage(
            NAME asabru_engine
            GITHUB_REPOSITORY ChistaDATA/asabru-engine
            GIT_TAG ${ASABRU_ENGINE_GIT_TAG}
    )
elseif (ASABRU_ENGINE_BUILD STREQUAL "LOCAL_DIR")
    # For development purpose
    CPMAddPackage(
            NAME asabru_engine
            SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../asabru-engine/src
    )
else()
    CPMAddPackage(
            NAME asabru_engine
            GITHUB_REPOSITORY ChistaDATA/asabru-engine
            VERSION 0.0.2
    )
endif()

if (ASABRU_PARSERS_BUILD STREQUAL "GIT_TAG")
    CPMAddPackage(
            NAME asabru_parsers
            GITHUB_REPOSITORY ChistaDATA/asabru-parsers
            GIT_TAG ${ASABRU_PARSERS_GIT_TAG}
    )
elseif (ASABRU_PARSERS_BUILD STREQUAL "LOCAL_DIR")
    # For development purpose
    CPMAddPackage(
            NAME asabru_parsers
            SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../asabru-parsers/src
    )
else()
    CPMAddPackage(
            NAME asabru_parsers
            GITHUB_REPOSITORY ChistaDATA/asabru-parsers
            VERSION 0.0.1
    )
endif()

target_link_libraries(
        ${PROJECT_NAME}
        PUBLIC ${SQLite3_LIBRARIES}
        asabru_commons
        asabru_parsers
        asabru_engine
        readline
)
