//
//  Version.h
//  GRE
//
//  Created by Jacques Tronconi on 23/11/2015.
//
//

#ifndef GRE_Version_h
#define GRE_Version_h

// Purpose : Declares some Version macros, also includes some used headers depending
// on platforms.

#if defined _WIN32 || defined __CYGWIN__ || defined _WIN64
#   ifdef BUILDING_DLL
#       ifdef __GNUC__
#           define DLL_PUBLIC __attribute__ ((dllexport))
#       else
#           define DLL_PUBLIC __declspec(dllexport) // Note: actually gcc seems to also supports this syntax.
#       endif
#   else
#       ifdef __GNUC__
#           define DLL_PUBLIC __attribute__ ((dllimport))
#       else
#           define DLL_PUBLIC __declspec(dllimport) // Note: actually gcc seems to also supports this syntax.
#       endif
#   endif
#
#   define DLL_LOCAL
#
#else
#    if __GNUC__ >= 4
#        define DLL_PUBLIC __attribute__ ((visibility ("default")))
#        define DLL_LOCAL  __attribute__ ((visibility ("hidden")))
#    else
#        define DLL_PUBLIC
#        define DLL_LOCAL
#    endif
#endif

#define GRE_BEGIN_NAMESPACE namespace Gre {
#define GRE_END_NAMESPACE   }

// Platforms headers

#   include <iostream>
#   include <memory>
#   include <map>
#   include <new>
#   include <fstream>
#   include <utility>
#   include <vector>
#   include <chrono>

#if defined _WIN32
//  Windows 32 bits
#   define GRE_WINDOWS
#   define GRE_32BITS
#
#   include <Windows.h>
#
#elif defined _WIN64
//  Windows 64 bits
#   define GRE_WINDOWS
#   define GRE_64BITS
#
#   include <Windows.h>
#
#elif defined __APPLE__ 
//  Mac Os X
#   define GRE_OSX
#   if defined __GNUC__ && defined __x86_64__
#       define GRE_64BITS
#   else   
#       define GRE_32BITS
#   endif
#
#   include <unistd.h>
#   include <dirent.h>
#   include <sys/time.h>
#
#else
//  Assume Unix platform
#   define GRE_UNIX
#   if defined __GNUC__ && defined __x86_64__
#       define GRE_64BITS
#   else
#       define GRE_32BITS
#   endif
#
#   include <unistd.h>
#   include <dirent.h>
#   include <sys/time.h>
#endif

#include "glm/vec2.hpp"
#include "glm/vec3.hpp"

GRE_BEGIN_NAMESPACE

// Version Profiles

#define GRE_VERSION_MAJOR 0             ///< @brief GRE Major version.
#define GRE_VERSION_MINOR 0             ///< @brief GRE Minor version.
#define GRE_VERSION_BUILD 8             ///< @brief GRE Build number.

/// @brief Defines the Version structure.
typedef struct Version
{
    unsigned major;
    unsigned minor;
    unsigned build;
} Version;

#define localVersion (Version({ GRE_VERSION_MAJOR , GRE_VERSION_MINOR , GRE_VERSION_BUILD }))
DLL_PUBLIC Version GetLibVersion ();

enum class PrimitiveType
{
    Points                  = 0x1,
    Lines                   = 0x2,
    LinesStrip              = 0x3,
    LinesLoop               = 0x4,
    LinesStripAdjacency     = 0x5,
    LinesAdjacency          = 0x6,
    Triangles               = 0x7,
    TrianglesStrip          = 0x8,
    TrianglesFan            = 0x9,
    TrianglesAdjacency      = 0xA,
    TrianglesStripAdjacency = 0xB,
    Patches                 = 0xC
};

enum class StorageType
{
    UnsignedByte    = 0x01,
    UnsignedShort   = 0x02,
    UnsignedInt     = 0x03
};

GRE_END_NAMESPACE
#endif
