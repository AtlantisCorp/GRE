# At least we have CMake 3.
cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
project(GreExample VERSION 1 LANGUAGES CXX)

# Sources files.
add_executable( example
        main.cpp
        Example1.cpp
        Example2.cpp)
target_link_libraries( example gre )

# Headers files.
include_directories(PUBLIC
        ${GRE_ROOT_DIRECTORY}/Engine/inc
        $<INSTALL_INTERFACE:include>
        PRIVATE src)

set_target_properties( example
        PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY ${GRE_ROOT_DIRECTORY}/Sdk
        LIBRARY_OUTPUT_DIRECTORY ${GRE_ROOT_DIRECTORY}/Sdk
        RUNTIME_OUTPUT_DIRECTORY ${GRE_ROOT_DIRECTORY}/Sdk
        )

set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -std=c++11")