# At least we have cmake 3.
cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
project(GreSdk VERSION 1 LANGUAGES CXX)

# Set the GRE_ROOT_DIRECTORY variable.
set(GRE_ROOT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
set(GRE_LIB_DIRECTORY "${GRE_ROOT_DIRECTORY}/SDK")
set(GRE_PLUGIN_DIRECTORY "${GRE_ROOT_DIRECTORY}/SDK/Plugins")

# Add here any external libraries we have to use.

# Targets we have to make.
add_subdirectory(Engine)
add_subdirectory(Plugins)
add_subdirectory(Example)
