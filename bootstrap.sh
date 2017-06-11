#!/usr/bin/env bash
# bootstrap.sh : Install and build every components for the GRE Sdk.

function usage 
{
    echo "CMAKE GRE Generator"
    echo "  -h | --help : Show this help."
    echo "  -g | --generator : Specify a generator for CMAKE. Default is 'Unix Makefiles'."
    echo "  -b | --build : Specify a build type for CMAKE. Default is 'RelWithDebInfo'."
    echo "  -p | --prefix : Specify a directory where to put CMAKE files. Default is './CMakeFiles'."
    echo "  -v | --version : Shows this script version."
}

function version
{
    echo "CMAKE GRE Generator Script v.1"
}

# Main

generator="Unix Makefiles"
build="RelWithDebInfo"
prefix="CMakeFiles"

while [ "$1" != "" ]; do
    case $1 in
        -g | --generator )  shift
                            generator=$1
                            ;;
        -b | --build )      shift
                            build=$1
                            ;;
        -p | --prefix )     shift
                            prefix=$1
                            ;;
        -h | --help )       usage
                            exit 1
                            ;;
        -v | --version )    version
                            exit 1 
                            ;;
    esac
    shift
done

# Launch main script.

version

# Check arguments.

echo "CMAKE Generator = $generator"
echo "CMAKE Build type = $build"
echo "CMAKE Prefix = $prefix"

# Run CMAKE.

cmake -G "$generator" -DCMAKE_BUILD_TYPE=$build -H. -B$prefix
