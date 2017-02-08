//
//  OpenGlIncludes.cpp
//  GRE
//
//  Created by Jacques Tronconi on 11/12/2015.
//
//

#include "OpenGlIncludes.h"
#include "OpenGlRenderContext.h"

typedef GreExceptionWithText GreOpenGlVersionInitException;
typedef GreExceptionWithText GreOpenGl3ExtensionInitException;

std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}


std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, elems);
    return elems;
}

GLenum OpenGlUtils::PrimitiveTypeToGl (PrimitiveType ptype)
{
    switch (ptype)
    {
        case PrimitiveType::Points:
            return GL_POINTS;
        case PrimitiveType::Lines:
            return GL_LINES;
        case PrimitiveType::LinesAdjacency:
            return GL_LINES_ADJACENCY_EXT;
        case PrimitiveType::LinesLoop:
            return GL_LINE_LOOP;
        case PrimitiveType::LinesStrip:
            return GL_LINE_STRIP;
        case PrimitiveType::LinesStripAdjacency:
            return GL_LINE_STRIP_ADJACENCY_EXT;
        case PrimitiveType::Patches:
            return GL_POINTS;
        case PrimitiveType::Triangles:
            return GL_TRIANGLES;
        case PrimitiveType::TrianglesAdjacency:
            return GL_TRIANGLES_ADJACENCY_EXT;
        case PrimitiveType::TrianglesFan:
            return GL_TRIANGLE_FAN;
        case PrimitiveType::TrianglesStrip:
            return GL_TRIANGLE_STRIP;
        case PrimitiveType::TrianglesStripAdjacency:
            return GL_TRIANGLE_STRIP_ADJACENCY_EXT;
        default:
            return GL_POINTS;
    }
}

GLenum OpenGlUtils::StorageTypeToGl (StorageType stype)
{
    switch (stype)
    {
        case StorageType::UnsignedByte:
            return GL_UNSIGNED_BYTE;
        case StorageType::UnsignedShort:
            return GL_UNSIGNED_SHORT;
        case StorageType::UnsignedInt:
            return GL_UNSIGNED_INT;
        default:
            return GL_UNSIGNED_INT;
    }
}

#ifdef GrePlatformDarwin

#include <AvailabilityMacros.h>

#ifdef MAC_OS_X_VERSION_10_3
#include <dlfcn.h>

void* MyNSGLGetProcAddress(const char* name)
{
    static void* image = NULL;
    if (NULL == image)
        image = dlopen("/System/Library/Frameworks/OpenGL.framework/Versions/Current/OpenGL", RTLD_LAZY);
    return (image ? dlsym(image, (const char *)name) : NULL);
}

#else
#include <mach-o/dyld.h>

void * MyNSGLGetProcAddress (const char *name)
{
    NSSymbol symbol;
    char *symbolName;
    symbolName = (char*) malloc (strlen (name) + 2); // 1
    strcpy(symbolName + 1, name); // 2
    symbolName[0] = '_'; // 3
    symbol = NULL;
    if (NSIsSymbolNameDefined (symbolName)) // 4
        symbol = NSLookupAndBindSymbol (symbolName);
    free (symbolName); // 5
    return symbol ? NSAddressOfSymbol (symbol) : NULL; // 6
}

#endif // MAC_OS_X_VERSION_10_3

#endif // GrePlatformDarwin

// ---------------------------------------------------------------------------------------------------

GLenum Gl::TextureTypeToGlType(const TexturePrivate::Type& textureType)
{
    if(textureType == TexturePrivate::Type::OneDimension)
        return GL_TEXTURE_1D;
    else if(textureType == TexturePrivate::Type::Depth)
        return GL_TEXTURE_2D;
    else if(textureType == TexturePrivate::Type::Stencil)
        return GL_TEXTURE_2D;
    else if(textureType == TexturePrivate::Type::TwoDimension)
        return GL_TEXTURE_2D;
    else if(textureType == TexturePrivate::Type::ThreeDimension)
        return GL_TEXTURE_3D;
    return GL_TEXTURE_2D;
}

Gl::Gl()
{
    VersionMajor = -1;
    VersionMinor = -1;
    Vendor = "Unknown";
    Renderer = "Unknown";
    Initialized = false;
    
    GetIntegerv = nullptr;
    GetString = nullptr;
    GetStringi = nullptr;
    ClearColor = nullptr;
    ClearDepth = nullptr;
    Viewport = nullptr;
    Clear = nullptr;
    Enable = nullptr;
    Disable = nullptr;
    Flush = nullptr;
    DepthFunc = nullptr;
    
    GenFramebuffers = nullptr;
    DeleteFramebuffers = nullptr;
    BindFramebuffer = nullptr;
    FramebufferTexture = nullptr;
    
    GenVertexArrays = nullptr;
    DeleteVertexArrays = nullptr;
    BindVertexArray = nullptr;
    
    VertexAttribPointer = nullptr;
    EnableVertexAttribArray = nullptr;
    DisableVertexAttribArray = nullptr;
    
    ActiveTexture = nullptr;
    
    Uniform4fv = nullptr;
    CreateShader = nullptr;
    ShaderSource = nullptr;
    CompileShader = nullptr;
    UniformMatrix4fv = nullptr;
    GetShaderiv = nullptr;
    GetProgramInfoLog = nullptr;
    DeleteShader = nullptr;
    CreateProgram = nullptr;
    LinkProgram = nullptr;
    AttachShader = nullptr;
    GetProgramiv = nullptr;
    DetachShader = nullptr;
    DeleteProgram = nullptr;
    UseProgram = nullptr;
    GetUniformLocation = nullptr;
    GetAttribLocation = nullptr;
    GenTextures = nullptr;
    PixelStorei = nullptr;
    BindTexture = nullptr;
    TexParameteri = nullptr;
    TexImage2D = nullptr;
    DeleteTextures = nullptr;
    GenBuffers = nullptr;
    BindBuffer = nullptr;
    DeleteBuffers = nullptr;
    BufferData = nullptr;
    BufferSubData = nullptr;
    TexSubImage2D = nullptr;
}

Gl::~Gl()
{
    
}

int Gl::FindFirstOccurence(const std::string& in, char what)
{
    if(in.empty())
        return -1;
    
    int i = 0;
    while(in[i] != '\0' && in[i] != what)
        ++i;
    
    if(in[i] == '\0' || in[i] == what)
        return i;
    return -1;
}

void Gl::GreOpenGlVersionInit()
{
    char* GL_version  = (char*) GetString(GL_VERSION);
    int vpoint = FindFirstOccurence(std::string(GL_version), '.');
    if(vpoint >= 0)
    {
        VersionMajor = GL_version[vpoint-1] - '0';
        VersionMinor = GL_version[vpoint+1] - '0';
        
#ifdef GreIsDebugMode
        GreDebugPretty() << "OpenGl Version found : " << VersionMajor << "." << VersionMinor << Gre::gendl;
#endif
    }
    
    Vendor = std::string ((char*) GetString(GL_VENDOR));
    Renderer = std::string ((char*) GetString(GL_RENDERER));
    
#ifdef GreIsDebugMode
    GreDebugPretty() << "Vendor : '" << Vendor << "'." << Gre::gendl;
    GreDebugPretty() << "Renderer : '" << Renderer << "'." << Gre::gendl;
#endif
}

void Gl::GreOpenGl3ExtensionInit()
{
    GLint extnum = 0;
    GetIntegerv(GL_NUM_EXTENSIONS, &extnum);
    
    for(GLuint cext = 0; cext < extnum; ++cext)
    {
        Extensions.push_back(std::string((char*) GetStringi(GL_EXTENSIONS, cext)));
    }
    
#ifdef GreIsDebugMode
    GreDebugPretty() << "Extensions Found : " << Extensions.size() << Gre::gendl;
#endif
}

bool Gl::IsExtensionSupported(const std::string& extension)
{
    return std::find(Extensions.begin(), Extensions.end(), extension) != Extensions.end();
}

void Gl::GreOpenGlInit()
{
    if(Initialized)
        return;
    
    // Those are required functions...
    
    GetIntegerv = (PFNGLGETINTEGERVPROC) GlGetProcAddress("glGetIntegerv");
    GetString = (PFNGLGETSTRINGPROC) GlGetProcAddress("glGetString");
    GetStringi = (PFNGLGETSTRINGIPROC) GlGetProcAddress("glGetStringi");
    ClearColor = (PFNGLCLEARCOLORPROC) GlGetProcAddress("glClearColor");
    ClearDepth = (PFNGLCLEARDEPTHPROC) GlGetProcAddress("glClearDepth");
    Viewport = (PFNGLVIEWPORTPROC) GlGetProcAddress("glViewport");
    Clear = (PFNGLCLEARPROC) GlGetProcAddress("glClear");
    Enable = (PFNGLENABLEPROC) GlGetProcAddress("glEnable");
    Disable = (PFNGLDISABLEPROC) GlGetProcAddress("glDisable");
    Flush = (PFNGLFLUSHPROC) GlGetProcAddress("glFlush");
    DepthFunc = (PFNGLDEPTHFUNCPROC) GlGetProcAddress("glDepthFunc");
    ActiveTexture = (PFNGLACTIVETEXTUREPROC) GlGetProcAddress("glActiveTexture");
    
    if(GetString)
    {
        GreOpenGlVersionInit();
    }
    
#ifdef GreIsDebugMode
    else
    {
        GreDebugPretty() << "No glGlobalContext->getGl().GetString function found to initialize Gl Version." << Gre::gendl;
        throw GreOpenGlVersionInitException ("glGlobalContext->getGl().GetString not found.");
    }
#endif
    
    if(GetStringi && GetIntegerv)
    {
        GreOpenGl3ExtensionInit();
    }
    
#ifdef GreIsDebugMode
    else
    {
        GreDebugPretty() << "No glGlobalContext->getGl().GetStringi function found... Please update your drivers. glGetStringi is supported from OpenGl "
        << "3.0 version." << Gre::gendl;
        throw GreOpenGl3ExtensionInitException ("glGlobalContext->getGl().GetStringi not found.");
    }
#endif
    
    if(VersionMajor >= 3 || IsExtensionSupported("GL_ARB_framebuffer_object"))
    {
        GenFramebuffers = (PFNGLGENFRAMEBUFFERSPROC) GlGetProcAddress("glGenFramebuffers");
        DeleteFramebuffers = (PFNGLDELETEFRAMEBUFFERSPROC) GlGetProcAddress("glDeleteFramebuffers");
        BindFramebuffer = (PFNGLBINDFRAMEBUFFERPROC) GlGetProcAddress("glBindFramebuffer");
        FramebufferTexture = (PFNGLFRAMEBUFFERTEXTUREPROC) GlGetProcAddress("glFramebufferTexture");
    }
    else if(IsExtensionSupported("GL_EXT_framebuffer_object"))
    {
        GenFramebuffers = (PFNGLGENFRAMEBUFFERSPROC) GlGetProcAddress("glGenFramebuffersEXT");
        DeleteFramebuffers = (PFNGLDELETEFRAMEBUFFERSPROC) GlGetProcAddress("glDeleteFramebuffersEXT");
        BindFramebuffer = (PFNGLBINDFRAMEBUFFERPROC) GlGetProcAddress("glBindFramebufferEXT");
        FramebufferTexture = (PFNGLFRAMEBUFFERTEXTUREPROC) GlGetProcAddress("glFramebufferTextureEXT");
    }
    else
    {
        GenFramebuffers = nullptr;
        DeleteFramebuffers = nullptr;
        BindFramebuffer = nullptr;
        FramebufferTexture = nullptr;
        
#ifdef GreIsDebugMode
        GreDebugPretty() << "Extension 'GL_ARB_framebuffer_object' is not supported but may be required." << Gre::gendl;
#endif
    }
    
    if(VersionMajor >= 3 || IsExtensionSupported("GL_ARB_vertex_array_object"))
    {
        GenVertexArrays = (PFNGLGENVERTEXARRAYSPROC) GlGetProcAddress("glGenVertexArrays");
        DeleteVertexArrays = (PFNGLDELETEVERTEXARRAYSPROC) GlGetProcAddress("glDeleteVertexArrays");
        BindVertexArray = (PFNGLBINDVERTEXARRAYPROC) GlGetProcAddress("glBindVertexArray");
    }
    
    else if(IsExtensionSupported("GL_APPLE_vertex_array_object"))
    {
        GenVertexArrays = (PFNGLGENVERTEXARRAYSPROC) GlGetProcAddress("glGenVertexArraysAPPLE");
        DeleteVertexArrays = (PFNGLDELETEVERTEXARRAYSPROC) GlGetProcAddress("glDeleteVertexArraysAPPLE");
        BindVertexArray = (PFNGLBINDVERTEXARRAYPROC) GlGetProcAddress("glBindVertexArrayAPPLE");
    }
    
    else
    {
        GenVertexArrays = nullptr;
        DeleteVertexArrays = nullptr;
        BindVertexArray = nullptr;
        
#ifdef GreIsDebugMode
        GreDebugPretty() << "Extension 'GL_ARB_vertex_array_object' is not supported but may be required." << Gre::gendl;
#endif
    }
    
    if(VersionMajor >= 2)
    {
        VertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERPROC) GlGetProcAddress("glVertexAttribPointer");
        EnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYPROC) GlGetProcAddress("glEnableVertexAttribArray");
        DisableVertexAttribArray = (PFNGLDISABLEVERTEXATTRIBARRAYPROC) GlGetProcAddress("glDisableVertexAttribArray");
    }
    else if(IsExtensionSupported("GL_ARB_vertex_program"))
    {
        VertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERPROC) GlGetProcAddress("glVertexAttribPointerARB");
        EnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYARBPROC) GlGetProcAddress("glEnableVertexAttribArrayARB");
        DisableVertexAttribArray = (PFNGLDISABLEVERTEXATTRIBARRAYARBPROC) GlGetProcAddress("glDisableVertexAttribArrayARB");
    }
    else if(IsExtensionSupported("GL_NV_vertex_program"))
    {
        VertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERPROC) GlGetProcAddress("glVertexAttribPointerNV");
        EnableVertexAttribArray = nullptr;
        DisableVertexAttribArray = nullptr;
    }
    else
    {
        VertexAttribPointer = nullptr;
        EnableVertexAttribArray = nullptr;
        DisableVertexAttribArray = nullptr;
        
#ifdef GreIsDebugMode
        GreDebugPretty() << "Extension 'GL_ARB_vertex_program' is not supported but may be required." << Gre::gendl;
#endif
    }
    
    // VersionMajor >= 2
    
    Uniform4fv = (PFNGLUNIFORM4FVPROC) GlGetProcAddress("glUniform4fv");
    if(!Uniform4fv) Uniform4fv = (PFNGLUNIFORM4FVPROC) GlGetProcAddress("glUniform4fvARB");
    
    CreateShader = (PFNGLCREATESHADERPROC) GlGetProcAddress("glCreateShader");
    if(!CreateShader) CreateShader = (PFNGLCREATESHADERPROC) GlGetProcAddress("glCreateShaderObjectARB");
    
    ShaderSource = (PFNGLSHADERSOURCEPROC) GlGetProcAddress("glShaderSource");
    if(!ShaderSource) ShaderSource = (PFNGLSHADERSOURCEPROC) GlGetProcAddress("glShaderSourceARB");
    
    CompileShader = (PFNGLCOMPILESHADERPROC) GlGetProcAddress("glCompileShader");
    if(!CompileShader) CompileShader = (PFNGLCOMPILESHADERPROC) GlGetProcAddress("glCompileShaderARB");
    
    UniformMatrix4fv = (PFNGLUNIFORMMATRIX4FVPROC) GlGetProcAddress("glUniformMatrix4fv");
    if(!UniformMatrix4fv) UniformMatrix4fv = (PFNGLUNIFORMMATRIX4FVPROC) GlGetProcAddress("glUniformMatrix4fvARB");
    
    GetShaderiv = (PFNGLGETSHADERIVPROC) GlGetProcAddress("glGetShaderiv");
    if(!GetShaderiv) GetShaderiv = (PFNGLGETSHADERIVPROC) GlGetProcAddress("glGetObjectParameterivARB");
    if(!GetShaderiv) GetShaderiv = (PFNGLGETSHADERIVPROC) GlGetProcAddress("glGetObjectParameterivAPPLE");
    
    GetProgramInfoLog = (PFNGLGETPROGRAMINFOLOGPROC) GlGetProcAddress("glGetProgramInfoLog");
    if(!GetProgramInfoLog) GetProgramInfoLog = (PFNGLGETPROGRAMINFOLOGPROC) GlGetProcAddress("glGetInfoLogARB");
    
    DeleteShader = (PFNGLDELETESHADERPROC) GlGetProcAddress("glDeleteShader");
    if(!DeleteShader) DeleteShader = (PFNGLDELETESHADERPROC) GlGetProcAddress("glDeleObjectARB");
    
    CreateProgram = (PFNGLCREATEPROGRAMPROC) GlGetProcAddress("glCreateProgram");
    if(!CreateProgram) CreateProgram = (PFNGLCREATEPROGRAMPROC) GlGetProcAddress("glCreateProgramObjectARB");
    
    AttachShader = (PFNGLATTACHSHADERPROC) GlGetProcAddress("glAttachShader");
    if(!AttachShader) AttachShader = (PFNGLATTACHSHADERPROC) GlGetProcAddress("glAttachObjectARB");
    
    LinkProgram = (PFNGLLINKPROGRAMPROC) GlGetProcAddress("glLinkProgram");
    if(!LinkProgram) LinkProgram = (PFNGLLINKPROGRAMPROC) GlGetProcAddress("glLinkProgramARB");
    
    GetProgramiv = (PFNGLGETPROGRAMIVPROC) GlGetProcAddress("glGetProgramiv");
    if(!GetProgramiv) GetProgramiv = (PFNGLGETPROGRAMIVPROC) GlGetProcAddress("glGetProgramivARB");
    if(!GetProgramiv) GetProgramiv = (PFNGLGETPROGRAMIVPROC) GlGetProcAddress("glGetProgramivNV");
    if(!GetProgramiv) GetProgramiv = (PFNGLGETPROGRAMIVPROC) GlGetProcAddress("glGetObjectParameterivARB");
    if(!GetProgramiv) GetProgramiv = (PFNGLGETPROGRAMIVPROC) GlGetProcAddress("glGetObjectParameterivAPPLE");
    
    DetachShader = (PFNGLDETACHSHADERPROC) GlGetProcAddress("glDetachShader");
    if(!DetachShader) DetachShader = (PFNGLDETACHSHADERPROC) GlGetProcAddress("glDetachObjectARB");
    
    DeleteProgram = (PFNGLDELETEPROGRAMPROC) GlGetProcAddress("glDeleteProgram");
    if(!DeleteProgram) DeleteProgram = (PFNGLDELETEPROGRAMPROC) GlGetProcAddress("glDeleteObjectARB");
    
    UseProgram = (PFNGLUSEPROGRAMPROC) GlGetProcAddress("glUseProgram");
    if(!UseProgram) UseProgram = (PFNGLUSEPROGRAMPROC) GlGetProcAddress("glUseProgramObjectARB");
    
    GetUniformLocation = (PFNGLGETUNIFORMLOCATIONPROC) GlGetProcAddress("glGetUniformLocation");
    if(!GetUniformLocation) GetUniformLocation = (PFNGLGETUNIFORMLOCATIONPROC) GlGetProcAddress("glGetUniformLocationARB");
    
    GetAttribLocation = (PFNGLGETATTRIBLOCATIONPROC) GlGetProcAddress("glGetAttribLocation");
    if(!GetAttribLocation) GetAttribLocation = (PFNGLGETATTRIBLOCATIONPROC) GlGetProcAddress("glGetAttribLocationARB");
    
    GenTextures = (PFNGLGENTEXTURESPROC) GlGetProcAddress("glGenTextures");
    if(!GenTextures) GenTextures = (PFNGLGENTEXTURESEXTPROC) GlGetProcAddress("glGenTexturesEXT");
    
    PixelStorei = (PFNGLPIXELSTOREIPROC) GlGetProcAddress("glPixelStorei");
    
    BindTexture = (PFNGLBINDTEXTUREPROC) GlGetProcAddress("glBindTexture");
    if(!BindTexture) BindTexture = (PFNGLBINDTEXTUREPROC) GlGetProcAddress("glBindTextureEXT");
    
    TexParameteri = (PFNGLTEXPARAMETERIPROC) GlGetProcAddress("glTexParameteri");
    TexImage2D = (PFNGLTEXIMAGE2DPROC) GlGetProcAddress("glTexImage2D");
    
    DeleteTextures = (PFNGLDELETETEXTURESPROC) GlGetProcAddress("glDeleteTextures");
    if(!DeleteTextures) DeleteTextures = (PFNGLDELETETEXTURESPROC) GlGetProcAddress("glDeleteTexturesEXT");
    
    GenBuffers = (PFNGLGENBUFFERSPROC) GlGetProcAddress("glGenBuffers");
    if(!GenBuffers) GenBuffers = (PFNGLGENBUFFERSPROC) GlGetProcAddress("glGenBuffersARB");
    
    BindBuffer = (PFNGLBINDBUFFERPROC) GlGetProcAddress("glBindBuffer");
    if(!BindBuffer) BindBuffer = (PFNGLBINDBUFFERPROC) GlGetProcAddress("glBindBufferARB");
    
    DeleteBuffers = (PFNGLDELETEBUFFERSPROC) GlGetProcAddress("glDeleteBuffers");
    if(!DeleteBuffers) DeleteBuffers = (PFNGLDELETEBUFFERSPROC) GlGetProcAddress("glDeleteBuffersARB");
    
    BufferData = (PFNGLBUFFERDATAPROC) GlGetProcAddress("glBufferData");
    if(!BufferData) BufferData = (PFNGLBUFFERDATAPROC) GlGetProcAddress("glBufferDataARB");
    
    BufferSubData = (PFNGLBUFFERSUBDATAPROC) GlGetProcAddress("glBufferSubData");
    if(!BufferSubData) BufferSubData = (PFNGLBUFFERSUBDATAPROC) GlGetProcAddress("glBufferSubDataARB");
    
    TexSubImage2D = (PFNGLTEXSUBIMAGE2DPROC) GlGetProcAddress("glTexSubImage2D");
    if(!TexSubImage2D) TexSubImage2D = (PFNGLTEXSUBIMAGE2DPROC) GlGetProcAddress("glTexSubImage2DEXT");
    
    Initialized = true;
}

std::shared_ptr<OpenGlRenderContext> glGlobalContext = nullptr;




