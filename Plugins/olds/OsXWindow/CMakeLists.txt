# At least we have CMake 3.
cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
project(DarwinGlWindowPlugin VERSION 1 LANGUAGES CXX)

# Sources files.
file(GLOB_RECURSE DarwinGlWindowSources "*.h" "*.cpp" "*.m")
add_library(DarwinGlWindow SHARED ${DarwinGlWindowSources})

INCLUDE_DIRECTORIES ( /System/Library/Frameworks )
FIND_LIBRARY(OpenGL_LIBRARY OpenGL )
FIND_LIBRARY(COCOA_LIBRARY Cocoa)
Find_library(FOUNDATION_LIBRARY Foundation)
SET(EXTRA_LIBS stdc++ objc gre ${OpenGL_LIBRARY} ${COCOA_LIBRARY} ${FOUNDATION_LIBRARY})

target_link_libraries(DarwinGlWindow ${EXTRA_LIBS})

# Headers files.
include_directories(PUBLIC
        ${GRE_ROOT_DIRECTORY}/Engine/inc
        $<INSTALL_INTERFACE:include>
        PRIVATE src)

set_target_properties( DarwinGlWindow
        PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY ${GRE_ROOT_DIRECTORY}/Sdk/Plugins
        LIBRARY_OUTPUT_DIRECTORY ${GRE_ROOT_DIRECTORY}/Sdk/Plugins
        RUNTIME_OUTPUT_DIRECTORY ${GRE_ROOT_DIRECTORY}/Sdk/Plugins
        )