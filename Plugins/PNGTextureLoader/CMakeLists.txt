# BMPTextureLoader Plugin Project.
project(PNGTextureLoaderPlugin LANGUAGES CXX)

# Enables C++ 11 flags.
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

# Sources files
file(GLOB sources "${GRE_ROOT_DIRECTORY}/plugins/PNGTextureLoader/inc/*.h")
file(GLOB headers "${GRE_ROOT_DIRECTORY}/plugins/PNGTextureLoader/src/*.cpp")

# Adds includes
include_directories( PUBLIC
	"${GRE_ROOT_DIRECTORY}/Engine/inc"
	"${GRE_ROOT_DIRECTORY}/plugins/PNGTextureLoader/inc"
	"$<INSTALL_INTERFACE:include>"
	PRIVATE src 
)

# Creates plugin target
add_library(PNGTextureLoader SHARED ${sources} ${headers})
target_link_libraries(PNGTextureLoader gre)

# Find libpng
find_package (PNG) 
if (PNG_FOUND)
    include_directories(${PNG_INCLUDE_DIRS})
    target_link_libraries(PNGTextureLoader ${PNG_LIBRARIES})
endif (PNG_FOUND)

set_target_properties( PNGTextureLoader
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

    set_xcode_property(PNGTextureLoader CLANG_CXX_LANGUAGE_STANDARD "c++11")
    set_xcode_property(PNGTextureLoader CLANG_CXX_LIBRARY "libc++")

    set_property(TARGET PNGTextureLoader PROPERTY CXX_STANDARD 11)
    set_property(TARGET PNGTextureLoader PROPERTY CXX_STANDARD_REQUIRED ON)

else()

    set_property(TARGET PNGTextureLoader PROPERTY CXX_STANDARD 11)
    set_property(TARGET PNGTextureLoader PROPERTY CXX_STANDARD_REQUIRED ON)

endif(${CMAKE_GENERATOR} MATCHES "Xcode") 