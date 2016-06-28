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
#include <sstream>

#ifdef GrePlatformDarwin

#   include <OpenGl/gltypes.h>
// 'khrglext.h' is an updated version of 'glext.h', given by Khronos Website.
#   include "khrglext.h"

#   include <OpenGl/gl3.h>
#   include <OpenGl/glu.h>

// <OpenGl/OpenGl.h> includes only the CGL framework.
#   include <OpenGl/OpenGL.h>

#elif defined( GrePlatformWindows )

#   include <GL/gl.h>
#   include <GL/glext.h>
#   include <GL/wglext.h>
#   include <GL/glu.h>

#elif defined( GrePlatformUnix )

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

using namespace Gre;

std::vector<std::string>& split(const std::string &s, char delim, std::vector<std::string> &elems);
std::vector<std::string>  split(const std::string &s, char delim);

class OpenGlUtils
{
public:
    
    static GLenum PrimitiveTypeToGl (PrimitiveType ptype);
    static GLenum StorageTypeToGl (StorageType stype);
};

#ifdef GrePlatformDarwin

#   include <stdlib.h>
#   include <string.h>
void * MyNSGLGetProcAddress (const char *name);

#   define GlGetProcAddress(name) MyNSGLGetProcAddress( name )

#elif defined( GrePlatformWindows )

#   define GlGetProcAddress(name) wglGetProcAddress( name )

#elif defined( GrePlatformUnix )

#   define GlGetProcAddress(name) glXGetProcAddress( name )

#else

#   define GlGetProcAddress(name) 0

#endif // GlGetProcAddress

// On Apple Platforms, every functions are already functions pointers.
// So there is no need to import every functions. But, as this code will be reusable
// on every platforms, we will do it. And we must use it to know if ARB, EXT, APPLE or others
// should be used.

class Gl
{
public:
    
    Gl();
    ~Gl();
    
    /// @brief Converts Gre::TexturePrivate::Type to OpenGl Enum.
    static GLenum TextureTypeToGlType(const TexturePrivate::Type& textureType);
    
    // As some functions prototypes are not defined, deinfe it here.
    typedef void (* PFNGLGENTEXTURESPROC) (GLsizei n, GLuint *textures);
    typedef void (* PFNGLBINDTEXTUREPROC) (GLenum target, GLuint texture);
    typedef void (* PFNGLDELETETEXTURESPROC) (GLsizei n, const GLuint *textures);
    typedef void (* PFNGLTEXSUBIMAGE2DPROC) (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void *pixels);
    
    /// @brief Find the first occurence of given character in given string.
    static int FindFirstOccurence(const std::string& in, char what);
    
    void GreOpenGlVersionInit();
    void GreOpenGl3ExtensionInit();
    
    /// @brief Init the function pointers needed by the Gre Engine.
    void GreOpenGlInit();
    
     GLint VersionMajor;
     GLint VersionMinor;
     std::string Vendor;
     std::string Renderer;
    
     std::vector<std::string> Extensions;
    bool IsExtensionSupported(const std::string& extension);
    
    template <typename T>
    bool IsFunctionSupported(T func) { return func != nullptr; }
    
     bool Initialized;
    
     PFNGLGETINTEGERVPROC GetIntegerv;
     PFNGLGETSTRINGPROC GetString;
     PFNGLGETSTRINGIPROC GetStringi;
     PFNGLCLEARCOLORPROC ClearColor;
     PFNGLCLEARDEPTHPROC ClearDepth;
     PFNGLVIEWPORTPROC Viewport;
     PFNGLCLEARPROC Clear;
     PFNGLENABLEPROC Enable;
     PFNGLDISABLEPROC Disable;
     PFNGLFLUSHPROC Flush;
     PFNGLDEPTHFUNCPROC DepthFunc;
    
     PFNGLGENFRAMEBUFFERSPROC GenFramebuffers;
     PFNGLDELETEFRAMEBUFFERSPROC DeleteFramebuffers;
     PFNGLBINDFRAMEBUFFERPROC BindFramebuffer;
     PFNGLFRAMEBUFFERTEXTUREPROC FramebufferTexture;
    
     PFNGLGENVERTEXARRAYSPROC GenVertexArrays;
     PFNGLDELETEVERTEXARRAYSPROC DeleteVertexArrays;
     PFNGLBINDVERTEXARRAYPROC BindVertexArray;
    
     PFNGLVERTEXATTRIBPOINTERPROC VertexAttribPointer;
     PFNGLENABLEVERTEXATTRIBARRAYPROC EnableVertexAttribArray;
     PFNGLDISABLEVERTEXATTRIBARRAYPROC DisableVertexAttribArray;
    
    // GL_ARB_multitexture || GL_VERSION_1_3
     PFNGLACTIVETEXTUREPROC ActiveTexture;
    
    // VersionMajor >= 2.0
     PFNGLUNIFORM4FVPROC Uniform4fv;
     PFNGLCREATESHADERPROC CreateShader;
     PFNGLSHADERSOURCEPROC ShaderSource;
     PFNGLCOMPILESHADERPROC CompileShader;
     PFNGLUNIFORMMATRIX4FVPROC UniformMatrix4fv;
     PFNGLGETSHADERIVPROC GetShaderiv;
     PFNGLGETPROGRAMINFOLOGPROC GetProgramInfoLog;
     PFNGLDELETESHADERPROC DeleteShader;
     PFNGLCREATEPROGRAMPROC CreateProgram;
     PFNGLATTACHSHADERPROC AttachShader;
     PFNGLLINKPROGRAMPROC LinkProgram;
     PFNGLGETPROGRAMIVPROC GetProgramiv;
     PFNGLDETACHSHADERPROC DetachShader;
     PFNGLDELETEPROGRAMPROC DeleteProgram;
     PFNGLUSEPROGRAMPROC UseProgram;
     PFNGLGETUNIFORMLOCATIONPROC GetUniformLocation;
     PFNGLGETATTRIBLOCATIONPROC GetAttribLocation;
     PFNGLGENTEXTURESPROC GenTextures;
     PFNGLPIXELSTOREIPROC PixelStorei;
     PFNGLBINDTEXTUREPROC BindTexture;
     PFNGLTEXPARAMETERIPROC TexParameteri;
     PFNGLTEXIMAGE2DPROC TexImage2D;
     PFNGLDELETETEXTURESPROC DeleteTextures;
     PFNGLGENBUFFERSPROC GenBuffers;
     PFNGLBINDBUFFERPROC BindBuffer;
     PFNGLDELETEBUFFERSPROC DeleteBuffers;
     PFNGLBUFFERDATAPROC BufferData;
     PFNGLBUFFERSUBDATAPROC BufferSubData;
     PFNGLTEXSUBIMAGE2DPROC TexSubImage2D;
    
    // [...] TODO !
};

class OpenGlRenderContext;
extern std::shared_ptr<OpenGlRenderContext> glGlobalContext;

/// @brief A class to allow simple managing of Multi-Context Ids. Those can be GLuint or GLint.
/// This allow an OpenGl class to be ready and usable for any Contexts.
template<typename uint>
class MultiContextId
{
public:
    
    MultiContextId() {
        
    }
    
    ~MultiContextId() {
    
    }
    
    //////////////////////////////////////////////////////////////////////
    /// @brief This function should construct the Gl Object when it is time, i.e. when
    /// the correct RenderContext object is binded.
    //////////////////////////////////////////////////////////////////////
    virtual void construct() = 0;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief This function should destroy the Gl Object when it is time, i.e. when
    /// the correct RenderContext object is binded.
    //////////////////////////////////////////////////////////////////////
    virtual void destroy() = 0;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns true if the Id has been constructed for given context.
    //////////////////////////////////////////////////////////////////////
    bool hasConstructedForContext(uintptr_t ctxtId) const {
        return _mMappedId.find(ctxtId) != _mMappedId.end();
    }
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the id for given Context.
    //////////////////////////////////////////////////////////////////////
    uint getId(uintptr_t ctxtId) const {
        auto it = _mMappedId.find(ctxtId);
        if(it != _mMappedId.end()) {
            return it->second;
        }
        else {
            return 0;
        }
    }
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Adds a new Id for given context.
    //////////////////////////////////////////////////////////////////////
    void addId(uintptr_t ctxtId, uint glId) {
        _mMappedId[ctxtId] = glId;
    }
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Removes id for given context.
    //////////////////////////////////////////////////////////////////////
    void removeId(uintptr_t ctxtId) {
        auto it = _mMappedId.find(ctxtId);
        if(it != _mMappedId.end())
            _mMappedId.erase(it);
    }
    
private:
    
    /// @brief Holds one id per Context.
    std::map<uintptr_t, uint> _mMappedId;
};

#endif // GRE_OpenGlIncludes_h
