//////////////////////////////////////////////////////////////////////
//
//  Version.h
//  This source file is part of Gre
//		(Gang's Resource Engine)
//
//  Copyright (c) 2015 - 2017 Luk2010
//  Created on 23/11/2015.
//
//////////////////////////////////////////////////////////////////////
/*
 -----------------------------------------------------------------------------
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 -----------------------------------------------------------------------------
 */

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

#define GreBeginNamespace namespace Gre {
#define GreEndNamespace   }

/// @brief Defines if Gre is in Debug mode.
/// Debug mode enables many warnings, that slows the engine. You should
/// always undef it (comment this line) when you are developping in Release
/// mode.
#define GreIsDebugMode

/// @brief Defines this to enable exception throwing.
// #define GreHasExceptions

/// @brief If enabled, use of Extra Macros to define Class attributes are
/// allowed.
#define GreExtraMacros

/// @brief Defines this Macro if you do not want any deprecated features.
// #define GreWithoutDeprecatedFunctions

/// @brief Defines this if you want extra care with Resource objects.
// #define GreExtraResourceHolder

// Platforms headers

#   include <iostream>
#   include <memory>
#   include <map>
#   include <new>
#   include <fstream>
#   include <utility>
#   include <vector>
#   include <chrono>
#   include <exception>
#   include <queue>
#   include <deque>
#   include <thread>
#   include <atomic>
#   include <mutex>
#   include <list>
#   include <streambuf>
#   include <bitset>
#   include <stack>

#if defined _WIN32
//  Windows 32 bits
#   define GrePlatformWindows
#   define GrePlatformBits32
#
#   include <Windows.h>
#
#elif defined _WIN64
//  Windows 64 bits
#   define GrePlatformWindows
#   define GrePlatformBits64
#
#   include <Windows.h>
#
#elif defined __APPLE__
//  Mac Os X
#   define GrePlatformDarwin
#   if defined __GNUC__ && defined __x86_64__
#       define GrePlatformBits64
#   else
#       define GrePlatformBits32
#   endif
#
#   define GrePosix
#
#   include <unistd.h>
#   include <dirent.h>
#   include <sys/time.h>
#
#else
//  Assume Unix platform
#   define GrePlatformUnix
#   if defined __GNUC__ && defined __x86_64__
#       define GrePlatformBits64
#   else
#       define GrePlatformBits32
#   endif
#
#   define GrePosix
#
#   include <unistd.h>
#   include <dirent.h>
#   include <sys/time.h>
#endif

// The glm library should always be present when compiling GRE. On linux distributions, we could note that
// 'glm::tvec3' classes where 'glm::details::tvec3' and so on for every templated classes.

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/ext.hpp>

GreBeginNamespace

#ifdef GrePlatformUnix
    template<typename T> using TVector2 = glm::detail::tvec2<T, glm::highp>;
    template<typename T> using TVector3 = glm::detail::tvec3<T, glm::highp>;
    template<typename T> using TVector4 = glm::detail::tvec4<T, glm::highp>;
#else
    template<typename T> using TVector2 = glm::tvec2<T, glm::highp>;
    template<typename T> using TVector3 = glm::tvec3<T, glm::highp>;
    template<typename T> using TVector4 = glm::tvec4<T, glm::highp>;
#endif

typedef TVector2<float> Vector2;
typedef TVector3<float> Vector3;
typedef TVector4<float> Vector4;

typedef TVector2 < int > IVector2 ;
typedef TVector3 < int > IVector3 ;
typedef TVector4 < int > IVector4 ;

typedef glm::mat2 Matrix2 ;
typedef glm::mat3 Matrix3 ;
typedef glm::mat4 Matrix4 ;
typedef glm::quat Quaternion;

// Times definition .

/// @brief The Time definition.
typedef std::chrono::steady_clock Time ;

/// @brief A Time Point structure.
typedef Time::time_point TimePoint ;

/// @brief Delta of two Time , in seconds.
typedef std::chrono::duration < float > Duration ;

/// @brief Delta of two Time , in milliseconds.
typedef std::chrono::milliseconds DurationMilli ;

GreEndNamespace

// This file is here to includes some third-party code help.
#include "ThirdParty.h"
#include "Exceptions.h"
#include "Debug.h"

GreBeginNamespace

// Version Profiles

#define GreVersionMajor 0             ///< @brief GRE Major version.
#define GreVersionMinor 0             ///< @brief GRE Minor version.
#define GreVersionBuild 26            ///< @brief GRE Build number.

/// @brief Defines the Version structure.
typedef struct Version
{
    unsigned major;
    unsigned minor;
    unsigned build;
} Version;

#define localVersion (Version({ GreVersionMajor , GreVersionMinor , GreVersionBuild }))
DLL_PUBLIC Version GetLibVersion ();

/// @brief Defines the Primitives type.
/// @note This has no use for now.
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

/// @brief Storage type in a generic buffer.
enum class StorageType
{
    UnsignedByte    = 0x01,
    UnsignedShort   = 0x02,
    UnsignedInt     = 0x03
};

/// @brief Intersection possible results.
enum class IntersectionResult
{
    Outside, Inside, Between
};

//////////////////////////////////////////////////////////////////////
/// @brief Briefly describes a surface with Top, Left, Width, and
/// Height.
//////////////////////////////////////////////////////////////////////
struct Surface
{
    int top;
    int left;
    int width ;
    int height ;

    /// @brief A Zero'd surface.
    static Surface Null;
};

//////////////////////////////////////////////////////////////////////
/// @brief Defines an Orthogonal Volume.
struct OrthogonalVolume
{
    float bottom ;
    float top ;
    float left ;
    float right ;
};

inline bool operator < ( const Surface& s1, const Surface& s2 ) {
    return (s1.width*s1.top) < (s2.width*s2.top) ;
}

/// @brief Common egality operator.
extern bool operator == (const Surface& lhs, const Surface& rhs);

/// @brief Common different operator.
extern bool operator != (const Surface& lhs, const Surface& rhs);

//////////////////////////////////////////////////////////////////////
/// @brief Describe when the ResourceManager should stop the Main Loop.
//////////////////////////////////////////////////////////////////////
enum class CloseBehaviour
{
    /// @brief Close only when user decides it.
    Manual,

    /// @brief Closes when every Window has closed.
    AllWindowClosed
};

typedef std::exception GreException;

class GreExceptionWithText : public GreException
{
public:

    GreExceptionWithText(const std::string& reason) : _reason(reason) { }
    ~GreExceptionWithText() { }

    const char* what() const throw() { return _reason.c_str(); }

private:

    std::string _reason;
};

typedef GreExceptionWithText GreUnsupportedOperation;

/// @brief Exception called when a call from ResourceUser fails.
typedef GreExceptionWithText GreInvalidUserException;

//////////////////////////////////////////////////////////////////////
/// @brief Exception that define Allocation Problems.
class AllocationBadMemory : public GreException
{
public:

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    AllocationBadMemory ( const std::string & classname , size_t sz ) ;

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    const char * what () const throw () ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the allocation size that failed to be allocated.
    //////////////////////////////////////////////////////////////////////
    size_t size () const ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the class name of the allocation.
    //////////////////////////////////////////////////////////////////////
    const std::string & name () const ;

    /// @brief Complete 'what' sentence.
    std::string iWhat ;

    /// @brief Class's name.
    std::string iClassname ;

    /// @brief Class size.
    size_t iClasssize ;
};

//////////////////////////////////////////////////////////////////////
/// @brief Describes an Error where a Constructor has failed to
/// initialize.
//////////////////////////////////////////////////////////////////////
class GreConstructorException : public GreException
{
public:

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    GreConstructorException(const char* constructor);

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    GreConstructorException(const char* constructor, const char* error);

    //////////////////////////////////////////////////////////////////////
    /// @brief Tells which was the Constructor involved, and the error.
    //////////////////////////////////////////////////////////////////////
    const char* what() const throw();

private:

    /// @brief Constructor involved.
    std::string iConstructor;

    /// @brief Error involved.
    std::string iError;
};

//////////////////////////////////////////////////////////////////////
/// @brief Exception from Invalid Index.
//////////////////////////////////////////////////////////////////////
class GreIndexException : public GreException
{
public:

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    GreIndexException(const char* emitter, size_t idx, size_t max);

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    ~GreIndexException();

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    const char* what() const throw();

public:

    /// @brief Emitter who send this Exception.
    std::string iEmitter;

    /// @brief Index given.
    size_t iIndex;

    /// @brief Maximum index that could be reached.
    size_t iMaxIndex;

    /// @brief Sentence returned by 'what' function.
    std::string iWhat;
};

typedef std::vector<std::string> StringList;

class MatrixUtils
{
public:
    static Matrix4 Zero4;
    static Matrix4 Identity;
};

/// @brief Prints a numeroted list in a string.
std::string DebugListNumeroted(const StringList& list);

/// @brief Gets a number inserted by the user using std::cin.
int DebugGetNumber();

/// @brief Holds a volume in 3D.
struct Volume
{
    union
    {
        struct
        {
            float width;
            float height;
            float depth;
        };

        struct
        {
            float x;
            float y;
            float z;
        };

        Vector3 vec3;
    };
};

/// @brief Types that define a Radian angle.
typedef float Radian;

/// @brief Simply, a plane is a Vector4 component.
typedef Vector4 Plane;

//////////////////////////////////////////////////////////////////////
/// @brief Flags to clear buffers.
enum class ClearBuffer : int
{
    Color   = 0 ,
    Depth   = 1 ,
    Stencil = 2 ,

    Size    = 3
};

/// @brief bitset to manipulate clear buffers.
typedef std::bitset < (size_t) ClearBuffer::Size > ClearBuffers ;

GreEndNamespace
#endif
