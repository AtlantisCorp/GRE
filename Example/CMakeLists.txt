# At least we have CMake 3.
cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
project(GreExample VERSION 1 LANGUAGES CXX)

# Sources files.
add_executable( example
        Example3.cpp )
target_link_libraries( example gre )

# Headers files.
include_directories(PUBLIC
        ${GRE_ROOT_DIRECTORY}/Engine/inc
        $<INSTALL_INTERFACE:include>
        PRIVATE src)

set_target_properties( example
        PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY ${GRE_ROOT_DIRECTORY}/SDK
        LIBRARY_OUTPUT_DIRECTORY ${GRE_ROOT_DIRECTORY}/SDK
        RUNTIME_OUTPUT_DIRECTORY ${GRE_ROOT_DIRECTORY}/SDK
        )

set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -std=c++11")