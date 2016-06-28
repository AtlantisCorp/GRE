//
//  OpenGlRenderContext.cpp
//  GRE
//
//  Created by Jacques Tronconi on 01/03/2016.
//
//

#include "OpenGlRenderContext.h"

OpenGlRenderContext::OpenGlRenderContext(const std::string& name, const RenderContextInfo& ctxtInfo, Renderer parent)
: RenderContextPrivate(name, ctxtInfo), _mCtxtId((uintptr_t)this)
{
    _mParentRenderer = parent;
    
    // Depending on ctxtInfo, we will choose the best PixelFormat. But for now, we use best
    // format for each platforms.

#ifdef GrePlatformDarwin
    
    CGLPixelFormatAttribute formatAttribCore4[] =
    {
        kCGLPFADepthSize, (CGLPixelFormatAttribute) 32,
        kCGLPFADoubleBuffer,
        kCGLPFAOpenGLProfile, (CGLPixelFormatAttribute) kCGLOGLPVersion_GL4_Core,
        kCGLPFAAccelerated,
        (CGLPixelFormatAttribute) 0
    };
    
    CGLPixelFormatAttribute formatAttribCore3[] =
    {
        kCGLPFADepthSize, (CGLPixelFormatAttribute) 32,
        kCGLPFADoubleBuffer,
        kCGLPFAOpenGLProfile, (CGLPixelFormatAttribute) kCGLOGLPVersion_3_2_Core,
        kCGLPFAAccelerated,
        (CGLPixelFormatAttribute) 0
    };
    
    CGLPixelFormatAttribute formatAttrib[] =
    {
        kCGLPFADepthSize, (CGLPixelFormatAttribute) 32,
        kCGLPFADoubleBuffer,
        kCGLPFAOpenGLProfile, (CGLPixelFormatAttribute) kCGLOGLPVersion_Legacy,
        kCGLPFAAccelerated,
        (CGLPixelFormatAttribute) 0
    };
    
    CGLPixelFormatAttribute formatAttribSoft[] =
    {
        kCGLPFADepthSize, (CGLPixelFormatAttribute) 32,
        kCGLPFADoubleBuffer,
        (CGLPixelFormatAttribute) 0
    };
    
    CGLPixelFormatObj cGlPixelFormat;
    CGLContextObj cGlContext;
    CGLError cGlErrorCode;
    GLint pixNum;
    bool foundPixelFormat = false;
    
    if(CGLChoosePixelFormat(formatAttribCore4, &cGlPixelFormat, &pixNum) == kCGLNoError)
    {
        GreDebugPretty() << "Context '" << name << "' successfully found pixel format Gl4." << std::endl;
        foundPixelFormat = true;
        
        cGlErrorCode = CGLCreateContext(cGlPixelFormat, NULL, &cGlContext);
        CGLDestroyPixelFormat(cGlPixelFormat);
        
        if(cGlErrorCode != kCGLNoError)
        {
            GreDebugPretty() << "Context '" << name << "' Can't create Context Object. " << std::endl;
            GreDebugPretty() << "Error Code : " << CGLErrorString(cGlErrorCode) << std::endl;
            
            _mIsCreated = false;
        }
        else
        {
            GreDebugPretty() << "Context '" << name << "' Created New Context Object. " << std::endl;
            _mIsCreated = true;
            _mContext = cGlContext;
        }
    }
    
    if(CGLChoosePixelFormat(formatAttribCore3, &cGlPixelFormat, &pixNum) == kCGLNoError && !_mIsCreated)
    {
        GreDebugPretty() << "Context '" << name << "' successfully found pixel format Gl3." << std::endl;
        foundPixelFormat = true;
        
        cGlErrorCode = CGLCreateContext(cGlPixelFormat, NULL, &cGlContext);
        CGLDestroyPixelFormat(cGlPixelFormat);
        
        if(cGlErrorCode != kCGLNoError)
        {
            GreDebugPretty() << "Context '" << name << "' Can't create Context Object. " << std::endl;
            GreDebugPretty() << "Error Code : " << CGLErrorString(cGlErrorCode) << std::endl;
            
            _mIsCreated = false;
        }
        else
        {
            GreDebugPretty() << "Context '" << name << "' Created New Context Object. " << std::endl;
            _mIsCreated = true;
            _mContext = cGlContext;
        }
    }
    
    if(CGLChoosePixelFormat(formatAttrib, &cGlPixelFormat, &pixNum) == kCGLNoError && !_mIsCreated)
    {
        GreDebugPretty() << "Context '" << name << "' successfully found pixel format Gl legacy." << std::endl;
        foundPixelFormat = true;
        
        cGlErrorCode = CGLCreateContext(cGlPixelFormat, NULL, &cGlContext);
        CGLDestroyPixelFormat(cGlPixelFormat);
        
        if(cGlErrorCode != kCGLNoError)
        {
            GreDebugPretty() << "Context '" << name << "' Can't create Context Object. " << std::endl;
            GreDebugPretty() << "Error Code : " << CGLErrorString(cGlErrorCode) << std::endl;
            
            _mIsCreated = false;
        }
        else
        {
            GreDebugPretty() << "Context '" << name << "' Created New Context Object. " << std::endl;
            _mIsCreated = true;
            _mContext = cGlContext;
        }
    }
    
    if(CGLChoosePixelFormat(formatAttribSoft, &cGlPixelFormat, &pixNum) == kCGLNoError && !_mIsCreated)
    {
        GreDebugPretty() << "Context '" << name << "' successfully found pixel format Gl software." << std::endl;
        foundPixelFormat = true;
        
        cGlErrorCode = CGLCreateContext(cGlPixelFormat, NULL, &cGlContext);
        CGLDestroyPixelFormat(cGlPixelFormat);
        
        if(cGlErrorCode != kCGLNoError)
        {
            GreDebugPretty() << "Context '" << name << "' Can't create Context Object. " << std::endl;
            GreDebugPretty() << "Error Code : " << CGLErrorString(cGlErrorCode) << std::endl;
            
            _mIsCreated = false;
        }
        else
        {
            GreDebugPretty() << "Context '" << name << "' Created New Context Object. " << std::endl;
            _mIsCreated = true;
            _mContext = cGlContext;
        }
    }
    
#ifdef GreIsDebugMode
    if(!foundPixelFormat)
    {
        GreDebugPretty() << "Context '" << name << "' did not found any Gl Context suitable. " << std::endl;
    }
#endif
    
#endif
    
    if(_mIsCreated)
    {
#ifdef GrePlatformDarwin
        CGLSetCurrentContext(_mContext);
#endif
        
        _mGl.GreOpenGlInit();
        GreDebugPretty() << "Gl Object Version = '" << _mGl.VersionMajor << "." << _mGl.VersionMinor << "' created." << std::endl;
        
#ifdef GrePlatformDarwin
        CGLSetCurrentContext(NULL);
#endif
    }
}

OpenGlRenderContext::~OpenGlRenderContext()
{
    if(_mIsCreated)
    {
#ifdef GrePlatformDarwin
        CGLSetCurrentContext(NULL);
        CGLDestroyContext(_mContext);
#endif
    }
}

void OpenGlRenderContext::bind()
{
    if(_mIsCreated)
    {
#ifdef GrePlatformDarwin
        CGLSetCurrentContext(_mContext);
#endif
        
        getRenderer().onCurrentContextChanged(this);
    }
}

void OpenGlRenderContext::unbind()
{
    if(_mIsCreated)
    {
#ifdef GrePlatformDarwin
        CGLSetCurrentContext(NULL);
#endif
        
        getRenderer().onCurrentContextChanged(nullptr);
    }
}

void OpenGlRenderContext::flush()
{
    if(_mIsCreated)
    {
#ifdef GrePlatformDarwin
        CGLFlushDrawable(_mContext);
#endif
    }
}

Gl& OpenGlRenderContext::getGl()
{
    return _mGl;
}

const uintptr_t& OpenGlRenderContext::getId() const
{
    return _mCtxtId;
}

const void* OpenGlRenderContext::getCustomData(const std::string &dataname) const
{
#ifdef GrePlatformDarwin
    if(dataname == "CGLContext")
    {
        return &_mContext;
    }
#endif
    
    return nullptr;
}


