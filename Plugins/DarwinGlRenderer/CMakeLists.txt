# At least we have CMake 3.
cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
project(DarwinGlRendererPlugin VERSION 1 LANGUAGES CXX)

# Sources files.
file(GLOB_RECURSE DarwinGlRendererSources "*.h" "*.cpp")
add_library(DarwinGlRenderer SHARED ${DarwinGlRendererSources})

INCLUDE_DIRECTORIES ( /System/Library/Frameworks )
FIND_LIBRARY(OpenGL_LIBRARY OpenGL )
FIND_LIBRARY(COCOA_LIBRARY Cocoa)
MARK_AS_ADVANCED (OpenGL_LIBRARY)
SET(EXTRA_LIBS gre ${OpenGL_LIBRARY})

target_link_libraries(DarwinGlRenderer ${EXTRA_LIBS})

# Headers files.
include_directories(PUBLIC
        ${GRE_ROOT_DIRECTORY}/Engine/inc
	inc
        $<INSTALL_INTERFACE:include>
        PRIVATE src)

set_target_properties( DarwinGlRenderer
        PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY ${GRE_ROOT_DIRECTORY}/Sdk/Plugins
        LIBRARY_OUTPUT_DIRECTORY ${GRE_ROOT_DIRECTORY}/Sdk/Plugins
        RUNTIME_OUTPUT_DIRECTORY ${GRE_ROOT_DIRECTORY}/Sdk/Plugins
        )

set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -std=c++11")