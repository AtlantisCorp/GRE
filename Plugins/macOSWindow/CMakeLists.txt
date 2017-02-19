# Defines macOS Window project.
project(macOSWindowPlugin LANGUAGES CXX)

# Enables C++ 11 Flags.
set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -std=c++11")

# Sources files.
file(GLOB sources "${GRE_ROOT_DIRECTORY}/plugins/macOSWindow/src/*.cpp")
file(GLOB sourcesobj "${GRE_ROOT_DIRECTORY}/plugins/macOSWindow/src/*.mm")
file(GLOB headers "${GRE_ROOT_DIRECTORY}/plugins/macOSWindow/inc/*.h")

# Adds OpenGL and Cocoa libraries.
include_directories("/System/Library/Frameworks")
find_library(OpenGL_LIBRARY OpenGL )
find_library(COCOA_LIBRARY Cocoa )
mark_as_advanced(OpenGL_LIBRARY)
set(EXTRA_LIBS gre ${OpenGL_LIBRARY} ${COCOA_LIBRARY})

# Adds includes directory.
include_directories( PUBLIC 
	"${GRE_ROOT_DIRECTORY}/Engine/inc"
	"${GRE_ROOT_DIRECTORY}/plugins/macOSWindow/inc"
	"$<INSTALL_INTERFACE:include>"
	PRIVATE src)

# Creates library.
add_library(macOSWindow SHARED ${sources} ${sourcesobj} ${headers})
target_link_libraries(macOSWindow ${EXTRA_LIBS})

set_target_properties( macOSWindow
        PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY ${GRE_PLUGIN_DIRECTORY}
        LIBRARY_OUTPUT_DIRECTORY ${GRE_PLUGIN_DIRECTORY}
        RUNTIME_OUTPUT_DIRECTORY ${GRE_PLUGIN_DIRECTORY}
)

# Try to set C++11 Flags for Xcode Projects.
if(${CMAKE_GENERATOR} MATCHES "Xcode")

    macro (set_xcode_property TARGET XCODE_PROPERTY XCODE_VALUE)
        set_property (TARGET ${TARGET} PROPERTY XCODE_ATTRIBUTE_${XCODE_PROPERTY}
                      ${XCODE_VALUE})
    endmacro (set_xcode_property)

    set_xcode_property(macOSWindow CLANG_CXX_LANGUAGE_STANDARD "c++11")
    set_xcode_property(macOSWindow CLANG_CXX_LIBRARY "libc++")

    set_property(TARGET macOSWindow PROPERTY CXX_STANDARD 11)
    set_property(TARGET macOSWindow PROPERTY CXX_STANDARD_REQUIRED ON)

else()

    set_property(TARGET macOSWindow PROPERTY CXX_STANDARD 11)
    set_property(TARGET macOSWindow PROPERTY CXX_STANDARD_REQUIRED ON)

endif(${CMAKE_GENERATOR} MATCHES "Xcode")