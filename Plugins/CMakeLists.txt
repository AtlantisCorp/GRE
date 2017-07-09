# At least we have CMake 3.
cmake_minimum_required(VERSION 3.0 FATAL_ERROR)

# Insert here your plugins
add_subdirectory(macOSWindow)
add_subdirectory(OpenGlRenderer)
add_subdirectory(BMPTextureLoader)
add_subdirectory(PNGTextureLoader)
add_subdirectory(DefaultControllers)
add_subdirectory(X11OpenGlWindow)
add_subdirectory(BasicDefinitions)