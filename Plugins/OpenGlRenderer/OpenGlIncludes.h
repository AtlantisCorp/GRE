//
//  OpenGlIncludes.h
//  GRE
//
//  Created by Jacques Tronconi on 11/12/2015.
//
//

#ifndef GRE_OpenGlIncludes_h
#define GRE_OpenGlIncludes_h

#include "ResourceManager.h"

#ifdef GRE_OSX

#   include <OpenGl/gltypes.h>
#   include "khrglext.h"

#   include <OpenGl/gl.h>
#   include <OpenGl/gl3.h>
#   include <OpenGl/glu.h>

#elif defined( GRE_WINDOWS )

#   include <GL/gl.h>
#   include <GL/glext.h>
#   include <GL/wglext.h>
#   include <GL/glu.h>

#elif defined( GRE_UNIX )

#   include <GL/gl.h>
#   include <GL/glext.h>
#   include <GL/glx.h>
#   include <GL/glxext.h>
#   include <GL/glu.h>

#else

#   include <GL/gl.h>
#   include <GL/glext.h>
#   include <GL/glu.h>

#endif

#include <sstream>

using namespace Gre;

std::vector<std::string>& split(const std::string &s, char delim, std::vector<std::string> &elems);
std::vector<std::string>  split(const std::string &s, char delim);

class OpenGlUtils
{
public:
    
    static GLenum PrimitiveTypeToGl (PrimitiveType ptype);
    static GLenum StorageTypeToGl (StorageType stype);
};

#ifdef GRE_OSX

#   include <mach-o/dyld.h>
#   include <stdlib.h>
#   include <string.h>
void * MyNSGLGetProcAddress (const char *name);

#   define GlGetProcAddress(name) MyNSGLGetProcAddress( name )

#elif defined( GRE_WINDOWS )

#   define GlGetProcAddress(name) wglGetProcAddress( name )

#elif defined( GRE_UNIX )

#   define GlGetProcAddress(name) glXGetProcAddress( name )

#else

#   define GlGetProcAddress(name) 0

#endif // GlGetProcAddress

// On Apple Platforms, every functions are already functions pointers.
// So there is no need to import every functions. But, as this code will be reusable
// on every platforms, we will do it. And we must use it to know if ARB, EXT, APPLE or others
// should be used.

namespace Gl
{
    /// @brief Find the first occurence of given character in given string.
    int FindFirstOccurence(const std::string& in, char what);
    /// @brief Init the function pointers needed by the Gre Engine.
    void GreOpenGlInit();
    
    extern GLint VersionMajor;
    extern GLint VersionMinor;
    extern std::string Vendor;
    extern std::string Renderer;
    
    extern std::vector<std::string> Extensions;
    bool IsExtensionSupported(const std::string& extension);
    
    template <typename T>
    bool IsFunctionSupported(T func) { return func != nullptr; }
    
    extern bool Initialized;
    
    extern PFNGLGETINTEGERVPROC GetIntegerv;
    extern PFNGLGETSTRINGPROC GetString;
    extern PFNGLGETSTRINGIPROC GetStringi;
    extern PFNGLCLEARCOLORPROC ClearColor;
    extern PFNGLCLEARDEPTHPROC ClearDepth;
    extern PFNGLVIEWPORTPROC Viewport;
    extern PFNGLCLEARPROC Clear;
    extern PFNGLENABLEPROC Enable;
    extern PFNGLDISABLEPROC Disable;
    extern PFNGLFLUSHPROC Flush;
    extern PFNGLDEPTHFUNCPROC DepthFunc;
    
    extern PFNGLGENFRAMEBUFFERSPROC GenFramebuffers;
    extern PFNGLDELETEFRAMEBUFFERSPROC DeleteFramebuffers;
    extern PFNGLBINDFRAMEBUFFERPROC BindFramebuffer;
    extern PFNGLFRAMEBUFFERTEXTUREPROC FramebufferTexture;
    
    extern PFNGLGENVERTEXARRAYSPROC GenVertexArrays;
    extern PFNGLDELETEVERTEXARRAYSPROC DeleteVertexArrays;
    extern PFNGLBINDVERTEXARRAYPROC BindVertexArray;
    
    extern PFNGLVERTEXATTRIBPOINTERPROC VertexAttribPointer;
    extern PFNGLENABLEVERTEXATTRIBARRAYPROC EnableVertexAttribArray;
    extern PFNGLDISABLEVERTEXATTRIBARRAYPROC DisableVertexAttribArray;
    
    // GL_ARB_multitexture || GL_VERSION_1_3
    extern PFNGLACTIVETEXTUREPROC ActiveTexture;
    
    // VersionMajor >= 2.0
    extern PFNGLUNIFORM4FVPROC Uniform4fv;
    extern PFNGLCREATESHADERPROC CreateShader;
    extern PFNGLSHADERSOURCEPROC ShaderSource;
    extern PFNGLCOMPILESHADERPROC CompileShader;
    extern PFNGLUNIFORMMATRIX4FVPROC UniformMatrix4fv;
    extern PFNGLGETSHADERIVPROC GetShaderiv;
    extern PFNGLGETPROGRAMINFOLOGPROC GetProgramInfoLog;
    extern PFNGLDELETESHADERPROC DeleteShader;
    extern PFNGLCREATEPROGRAMPROC CreateProgram;
    extern PFNGLATTACHSHADERPROC AttachShader;
    extern PFNGLLINKPROGRAMPROC LinkProgram;
    extern PFNGLGETPROGRAMIVPROC GetProgramiv;
    extern PFNGLDETACHSHADERPROC DetachShader;
    extern PFNGLDELETEPROGRAMPROC DeleteProgram;
    extern PFNGLUSEPROGRAMPROC UseProgram;
    extern PFNGLGETUNIFORMLOCATIONPROC GetUniformLocation;
    extern PFNGLGETATTRIBLOCATIONPROC GetAttribLocation;
    
    // [...] TODO !
}

#endif // GRE_OpenGlIncludes_h
