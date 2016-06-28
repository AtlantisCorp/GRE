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

#define GreBeginNamespace namespace Gre {
#define GreEndNamespace   }

/// @brief Defines if Gre is in Debug mode.
/// Debug mode enables many warnings, that slows the engine. You should
/// always undef it (comment this line) when you are developping in Release
/// mode.
#define GreIsDebugMode

/// @brief If enabled, use of Extra Macros to define Class attributes are
/// allowed.
#define GreExtraMacros

/// @brief Defines this Macro if you do not want any deprecated features.
// #define GreWithoutDeprecatedFunctions

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
#   include <unistd.h>
#   include <dirent.h>
#   include <sys/time.h>
#endif

// The glm library should always be present when compiling the
// Gre Engine.

#include <glm/glm.hpp>
#include <glm/ext.hpp>

// This file is here to includes some third-party code help.
#include "ThirdParty.h"

GreBeginNamespace

// Version Profiles

#define GreVersionMajor 0             ///< @brief GRE Major version.
#define GreVersionMinor 0             ///< @brief GRE Minor version.
#define GreVersionBuild 15            ///< @brief GRE Build number.

/// @brief Defines the Version structure.
typedef struct Version
{
    unsigned major;
    unsigned minor;
    unsigned build;
} Version;

#define localVersion (Version({ GreVersionMajor , GreVersionMinor , GreVersionBuild }))
DLL_PUBLIC Version GetLibVersion ();

/// @brief Debug using an intro (should use __COMPACT_PRETTY_FUNCTION__ macro) and the body message.
DLL_PUBLIC std::ostream& GreDebug(const std::string& func);
#define GreDebugPretty() GreDebug( __COMPACT_PRETTY_FUNCTION__ )

#define _GreDebugNotImplemented( message , arg ) \
    static std::string __message##__arg = std::string( message ); \
    static bool __already_passed##__arg = false; \
    if( __already_passed##__arg ) { GreDebugPretty() << message << std::endl; __already_passed##__arg = true; }

#define GreDebugNotImplemented( message ) _GreDebugNotImplemented( message , __LINE__ )
#define GreDebugFunctionNotImplemented() GreDebugNotImplemented( std::string ( __COMPACT_PRETTY_FUNCTION__ ) + " : Not implemented !" )

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

//////////////////////////////////////////////////////////////////////
/// @brief Briefly describes a surface with Top, Left, Width, and
/// Height.
//////////////////////////////////////////////////////////////////////
typedef struct
{
    int top;
    int left;
    int width;
    int height;
    
} Surface;

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

typedef std::vector<std::string> StringList;
typedef glm::vec2 Vector2;
typedef glm::vec3 Vector3;
typedef glm::vec4 Vector4;
typedef glm::mat4 Matrix4;
typedef glm::quat Quaternion;

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

GreEndNamespace
#endif
