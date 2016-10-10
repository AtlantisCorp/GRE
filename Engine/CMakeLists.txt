# At least we have CMake 3.
cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
project(libgre VERSION 0.0 LANGUAGES CXX)

# Sources files.
file(GLOB_RECURSE GRE_SOURCES_FILES "*.cpp" "*.h")
add_library(gre SHARED ${GRE_SOURCES_FILES})

# Headers files.
include_directories(PUBLIC
        ${GRE_ROOT_DIRECTORY}/Engine/inc
        $<INSTALL_INTERFACE:include>
        PRIVATE src)

# Link with used libraries.
target_link_libraries(gre
        dl)

set_target_properties( gre
        PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY ${GRE_ROOT_DIRECTORY}/Sdk
        LIBRARY_OUTPUT_DIRECTORY ${GRE_ROOT_DIRECTORY}/Sdk
        RUNTIME_OUTPUT_DIRECTORY ${GRE_ROOT_DIRECTORY}/Sdk
)

set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -std=c++11")