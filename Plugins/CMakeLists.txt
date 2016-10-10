# At least we have CMake 3.
cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
project(GreSdkPlugins VERSION 1 LANGUAGES CXX)

# Insert here your plugins
add_subdirectory(DarwinGlRenderer)
add_subdirectory(OsXWindow)