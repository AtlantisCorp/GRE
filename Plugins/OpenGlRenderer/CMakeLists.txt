# OpenGlRenderer Plugin Project.
project(OpenGlRendererPlugin LANGUAGES CXX)

# Enables C++ 11 flags.
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

# Sources files
file(GLOB headers "${GRE_ROOT_DIRECTORY}/Plugins/OpenGlRenderer/inc/*.h")
file(GLOB sources "${GRE_ROOT_DIRECTORY}/Plugins/OpenGlRenderer/src/*.cpp")

# Adds includes
include_directories( PUBLIC
	"${GRE_ROOT_DIRECTORY}/Engine/inc"
	"${GRE_ROOT_DIRECTORY}/Plugins/OpenGlRenderer/inc"
	"$<INSTALL_INTERFACE:include>"
	PRIVATE src 
)

# Adds OpenGl depending on platform. 

# =========================================================================
# Enables only on Darwin platform.
if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
# =========================================================================

    include_directories("/System/Library/Frameworks")

    find_library(OpenGL_LIBRARY OpenGL)
    mark_as_advanced(OpenGL_LIBRARY)
    set(EXTRA_LIBS gre ${OpenGL_LIBRARY})

# =========================================================================
endif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
# =========================================================================

# =========================================================================
# Enables only on Linux platform. On Linux , we use GLEW to manage OpenGl 
# Extensions , so link against it.
if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
# =========================================================================

    find_package(GLEW)
    include_directories(${GLEW_INCLUDE_DIRS})
    set(EXTRA_LIBS gre ${GLEW_LIBRARIES})

# =========================================================================
endif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
# =========================================================================

# Creates plugin target
add_library(OpenGlRenderer SHARED ${sources} ${headers})
target_link_libraries(OpenGlRenderer ${EXTRA_LIBS})

set_target_properties( OpenGlRenderer
        PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY ${GRE_PLUGIN_DIRECTORY}
	ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${GRE_PLUGIN_DIRECTORY}
	ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${GRE_PLUGIN_DIRECTORY}
        LIBRARY_OUTPUT_DIRECTORY ${GRE_PLUGIN_DIRECTORY}
	LIBRARY_OUTPUT_DIRECTORY_DEBUG ${GRE_PLUGIN_DIRECTORY}
	LIBRARY_OUTPUT_DIRECTORY_RELEASE ${GRE_PLUGIN_DIRECTORY}
        RUNTIME_OUTPUT_DIRECTORY ${GRE_PLUGIN_DIRECTORY}
	RUNTIME_OUTPUT_DIRECTORY_DEBUG ${GRE_PLUGIN_DIRECTORY}
	RUNTIME_OUTPUT_DIRECTORY_RELEASE ${GRE_PLUGIN_DIRECTORY}
)

# Try to set C++11 Flags for Xcode Projects.
if(${CMAKE_GENERATOR} MATCHES "Xcode")

    macro (set_xcode_property TARGET XCODE_PROPERTY XCODE_VALUE)
        set_property (TARGET ${TARGET} PROPERTY XCODE_ATTRIBUTE_${XCODE_PROPERTY}
                      ${XCODE_VALUE})
    endmacro (set_xcode_property)

    set_xcode_property(OpenGlRenderer CLANG_CXX_LANGUAGE_STANDARD "c++11")
    set_xcode_property(OpenGlRenderer CLANG_CXX_LIBRARY "libc++")

    set_property(TARGET OpenGlRenderer PROPERTY CXX_STANDARD 11)
    set_property(TARGET OpenGlRenderer PROPERTY CXX_STANDARD_REQUIRED ON)

else()

    set_property(TARGET OpenGlRenderer PROPERTY CXX_STANDARD 11)
    set_property(TARGET OpenGlRenderer PROPERTY CXX_STANDARD_REQUIRED ON)

endif(${CMAKE_GENERATOR} MATCHES "Xcode") 
