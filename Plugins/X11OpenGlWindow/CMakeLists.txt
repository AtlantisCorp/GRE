# =========================================================================
# Enables only on Linux platform.
if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
# =========================================================================

project( X11OpenGlWindowProject LANGUAGES CXX )
set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -std=c++11")

# Sources and Headers.
file(GLOB sources "${GRE_ROOT_DIRECTORY}/Plugins/X11OpenGlWindow/src/*.cpp")
file(GLOB headers "${GRE_ROOT_DIRECTORY}/Plugins/X11OpenGlWindow/inc/*.h")

# Adds OpenGl with GLEW. This facilitates the extensions management.
find_package(GLEW)
include_directories(PUBLIC 
	${GLEW_INCLUDE_DIRS} 
	"${GRE_ROOT_DIRECTORY}/Engine/inc"
	"${GRE_ROOT_DIRECTORY}/Plugins/X11OpenGlWindow/inc"
	"$<INSTALL_INTERFACE:include>"
	PRIVATE src
)

# Adds X11 libraries.
find_package(X11)
include_directories( ${X11_INCLUDE_DIR} )

# Adds every libraries.
set( EXTRA_LIBS gre 
	    ${GLEW_LIBRARIES} 
	    ${X11_LIBRARIES} 
)

# Creates the Library.
add_library( X11OpenGlWindow SHARED ${sources} ${headers} )
target_link_libraries( X11OpenGlWindow ${EXTRA_LIBS} )

set_target_properties( X11OpenGlWindow
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

# =========================================================================
endif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
# =========================================================================
