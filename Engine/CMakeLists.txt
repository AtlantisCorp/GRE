# At least we have CMake 3.
cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
project(libgre VERSION 0.0 LANGUAGES CXX)

# Enables C++ 11 Flags.
set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -std=c++11")

# Sources files.
file(GLOB_RECURSE GRE_SOURCES_FILES "*.cpp" "*.h")
add_library(gre SHARED ${GRE_SOURCES_FILES})

# Headers files.
include_directories(PUBLIC
        ${GRE_ROOT_DIRECTORY}/Engine/inc
        $<INSTALL_INTERFACE:include>
        PRIVATE src)

# Link with used libraries.
target_link_libraries(gre dl)

# =========================================================================
# Enables only on Linux platform. On Linux , we also links against the libuuid.
if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
# =========================================================================

	find_package(PkgConfig REQUIRED)

	pkg_search_module(UUID REQUIRED uuid)
	include_directories(${UUID_INCLUDE_DIRS})
	target_link_libraries(gre ${UUID_LIBRARIES})

	find_package( Threads REQUIRED )
	target_link_libraries(gre ${CMAKE_THREAD_LIBS_INIT})

# =========================================================================
endif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
# =========================================================================

set_target_properties( gre
        PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY ${GRE_LIB_DIRECTORY}
	ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${GRE_LIB_DIRECTORY}
	ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${GRE_LIB_DIRECTORY}
        LIBRARY_OUTPUT_DIRECTORY ${GRE_LIB_DIRECTORY}
	LIBRARY_OUTPUT_DIRECTORY_DEBUG ${GRE_LIB_DIRECTORY}
	LIBRARY_OUTPUT_DIRECTORY_RELEASE ${GRE_LIB_DIRECTORY}
        RUNTIME_OUTPUT_DIRECTORY ${GRE_LIB_DIRECTORY}
	RUNTIME_OUTPUT_DIRECTORY_DEBUG ${GRE_LIB_DIRECTORY}
	RUNTIME_OUTPUT_DIRECTORY_RELEASE ${GRE_LIB_DIRECTORY}
)

# Try to set C++11 Flags for Xcode Projects.
if(${CMAKE_GENERATOR} MATCHES "Xcode")

    macro (set_xcode_property TARGET XCODE_PROPERTY XCODE_VALUE)
        set_property (TARGET ${TARGET} PROPERTY XCODE_ATTRIBUTE_${XCODE_PROPERTY}
                      ${XCODE_VALUE})
    endmacro (set_xcode_property)

    set_xcode_property(gre CLANG_CXX_LANGUAGE_STANDARD "c++11")
    set_xcode_property(gre CLANG_CXX_LIBRARY "libc++")

    set_property(TARGET gre PROPERTY CXX_STANDARD 11)
    set_property(TARGET gre PROPERTY CXX_STANDARD_REQUIRED ON)

else()

    set_property(TARGET gre PROPERTY CXX_STANDARD 11)
    set_property(TARGET gre PROPERTY CXX_STANDARD_REQUIRED ON)

endif(${CMAKE_GENERATOR} MATCHES "Xcode")

