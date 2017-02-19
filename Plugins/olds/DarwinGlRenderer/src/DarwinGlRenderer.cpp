//////////////////////////////////////////////////////////////////////
//
//  DarwinGlRenderer.cpp
//  This source file is part of Gre
//		(Gang's Resource Engine)
//
//  Copyright (c) 2015 - 2016 Luk2010
//  Created on 05/09/2016.
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

#include "DarwinGlRenderer.h"
#include "DarwinGlContext.h"
#include "DarwinGlTexture.h"
#include "DarwinGlHardwareProgramManager.h"

namespace DarwinGl
{
    DarwinGlRenderer::DarwinGlRenderer(const std::string& name)
    : Gre::RendererPrivate(name)
    , iGlobalContext()
    , iDefaultPixelFormat()
    {
        // Here we find the best Default pixel format.
        
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
        
        GLint npix = 0;
        bool success = false;
        
        // Test for OpenGl 4 profile.
        
        if ( CGLChoosePixelFormat(formatAttribCore4, &iDefaultPixelFormat, &npix) == kCGLNoError )
        {
            GreDebugPretty() << "OpenGl 4 PixelFormat is supported." << Gre::gendl;
            
            if ( !iGlobalContext )
            {
                if ( CGLCreateContext(iDefaultPixelFormat, nullptr, &iGlobalContext) == kCGLNoError )
                {
                    GreDebugPretty() << "Global Context successfully created." << Gre::gendl;
                    success = true;
                }
            }
        }
        
        // Test for OpenGl 3 profile.
        
        if ( CGLChoosePixelFormat(formatAttribCore3, &iDefaultPixelFormat, &npix) == kCGLNoError && !success )
        {
            GreDebugPretty() << "OpenGl 3 PixelFormat is supported (min 3.2 core)." << Gre::gendl;
            
            if ( !iGlobalContext )
            {
                if ( CGLCreateContext(iDefaultPixelFormat, nullptr, &iGlobalContext) == kCGLNoError )
                {
                    GreDebugPretty() << "Global Context successfully created." << Gre::gendl;
                    success = true;
                }
            }
        }
        
        if ( !success )
        {
            iDefaultPixelFormat = nullptr;
            iGlobalContext = nullptr;
            
            GreDebugPretty() << "This Renderer needs at least OpenGl 3.2 Core profile." << Gre::gendl;
            throw Gre::GreExceptionWithText("No OpenGl 3.2 detected.");
        }
        
        else
        {
            // Create the HardwareProgramManager and bind the GlobalContext.
            CGLSetCurrentContext(iGlobalContext);
            iProgramManager = HardwareProgramManagerHolder ( new DarwinGlHardwareProgramManager("GlProgramManager") );
        }
    }
    
    DarwinGlRenderer::~DarwinGlRenderer() noexcept ( false )
    {
        
    }
    
    void DarwinGlRenderer::drawVertexBufferPrivate(const Gre::HardwareVertexBuffer &vbuf, const Gre::HardwareProgramHolder &program)
    {
        
    }
    
    void DarwinGlRenderer::drawIndexBufferPrivate(const Gre::HardwareIndexBuffer &ibuf, const Gre::HardwareVertexBuffer &vbuf, const Gre::HardwareProgramHolder &program)
    {
        
    }
    
    void DarwinGlRenderer::setClearColor(const Gre::Color &color)
    {
        glClearColor(color.getRed(), color.getGreen(), color.getBlue(), color.getAlpha());
    }
    
    void DarwinGlRenderer::setClearDepth(float depth)
    {
        glClearDepth(depth);
    }
    
    Gre::RenderContextHolder DarwinGlRenderer::createRenderContext(const std::string &name, const Gre::RenderContextInfo &info)
    {
        if ( iDefaultPixelFormat )
        {
            return RendererPrivate::createRenderContext(name, info);
        }
        
        else
        {
            GreDebugPretty() << "'iDefaultPixelFormat' wasn't created." << Gre::gendl;
            return Gre::RenderContextHolder ( nullptr );
        }
    }
    
    Gre::HardwareVertexBufferHolder DarwinGlRenderer::iCreateVertexBuffer(const std::string &name, size_t sz) const
    {
        return Gre::HardwareVertexBufferHolder ( nullptr );
    }
    
    Gre::TextureHolder DarwinGlRenderer::iCreateTexturePrivate(const std::string &name) const
    {
        return Gre::TextureHolder ( new DarwinGlTexture(name) );
    }
    
    Gre::RenderContextHolder DarwinGlRenderer::iCreateRenderContext(const std::string &name, const Gre::RenderContextInfo &info) const
    {
        if ( iGlobalContext )
        {
            if ( !name.empty() )
            {
                // For now, we don't take advantage of the data in 'info' .
                CGLContextObj ctxt = nullptr;
                
                if ( CGLCreateContext(iDefaultPixelFormat, iGlobalContext, &ctxt) == kCGLNoError )
                {
                    GreDebugPretty() << "OpenGl Context '" << name << "' created." << Gre::gendl;
                    
                    Gre::RenderContextHolder holder = Gre::RenderContextHolder ( new DarwinGlContext(name, info, ctxt) );
                    
                    if ( holder.isInvalid() )
                    {
                        GreDebugPretty() << "Can't create 'Gre::RenderContextHolder' Resource." << Gre::gendl;
                        
                        if ( CGLDestroyContext(ctxt) != kCGLNoError )
                        {
                            GreDebugPretty() << "Can't destroy OpenGl Context." << Gre::gendl;
                        }
                        
                        return Gre::RenderContextHolder ( nullptr );
                    }
                    
                    else
                    {
                        return holder;
                    }
                }
                
                else
                {
                    GreDebugPretty() << "OpenGl Context '" << name << "' couldn't be created." << Gre::gendl;
                    return Gre::RenderContextHolder ( nullptr );
                }
            }
            
            else
            {
                GreDebugPretty() << "'name' is invalid." << Gre::gendl;
                return Gre::RenderContextHolder ( nullptr );
            }
        }
        
        else
        {
            GreDebugPretty() << "'iGlobalContext' has not been initialized." << Gre::gendl;
            return Gre::RenderContextHolder ( nullptr );
        }
    }
    
    // ---------------------------------------------------------------------------------------------------
    
    DarwinGlRendererLoader::DarwinGlRendererLoader()
    {
        
    }
    
    DarwinGlRendererLoader::~DarwinGlRendererLoader()
    {
        
    }
    
    bool DarwinGlRendererLoader::isCompatible(const Gre::RenderingApiDescriptor &apidescriptor) const
    {
        if ( apidescriptor.getApiName() == "OpenGl" )
        {
            return true;
        }
        
        else
        {
            return false;
        }
    }
    
    Gre::RendererHolder DarwinGlRendererLoader::load(const std::string &name) const
    {
        return Gre::RendererHolder ( new DarwinGlRenderer(name) );
    }
    
    Gre::ResourceLoader* DarwinGlRendererLoader::clone() const
    {
        return new DarwinGlRendererLoader();
    }
    
    bool DarwinGlRendererLoader::isLoadable(const std::string &filepath) const
    {
        return false;
    }
    
}
