#!/usr/bin/env bash
# bootstrap.sh : Install and build every components for the GRE Sdk.

# First , run cmake with given generator.
cmake -H. -BCMakeFiles -G "$1"

# Then , make the Engine.
cd CMakeFiles
make
cd ..

# And we are done !