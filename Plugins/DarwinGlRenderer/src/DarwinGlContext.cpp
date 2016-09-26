//////////////////////////////////////////////////////////////////////
//
//  DarwinGlContext.cpp
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

#include "DarwinGlContext.h"

namespace DarwinGl
{
    DarwinGlContext::DarwinGlContext(const std::string& name, const Gre::RenderContextInfo& info, const CGLContextObj& cglcontext)
    : Gre::RenderContextPrivate(name, info)
    , iContext(cglcontext)
    , iClearBuffers(0)
    , iClearColor(0.0f , 0.0f , 0.0f , 1.0f)
    {
        if ( !cglcontext )
        {
            GreDebugPretty() << "Initializing 'DarwinGlContext' with null 'cglcontext' has undefined behaviour." << std::endl;
        }
    }
    
    DarwinGlContext::~DarwinGlContext() noexcept ( false )
    {
        if ( iContext )
        {
            GreResourceAutolock ;
            CGLReleaseContext(iContext);
        }
    }
    
    void DarwinGlContext::bind()
    {
        if ( iContext )
        {
            GreResourceAutolock ;
            
            if ( CGLSetCurrentContext(iContext) != kCGLNoError )
            {
                GreDebugPretty() << "Can't bind CGLContext '" << getName() << "'." << std::endl;
                iIsBinded = false;
            }
            
            else
            {
                // Sets clearing state.
                
                glClearColor(iClearColor.getRed(), iClearColor.getGreen(), iClearColor.getBlue(), iClearColor.getAlpha());
                glClear(iClearBuffers);
                
                // Modifiate property 'iIsBinded'.
                
                iIsBinded = true;
            }
        }
    }
    
    void DarwinGlContext::unbind()
    {
        if ( iContext )
        {
            GreResourceAutolock ;
            
            if ( CGLSetCurrentContext(NULL) != kCGLNoError )
            {
                GreDebugPretty() << "Can't unbind CGLContext '" << getName() << "'." << std::endl;
            }
            else
            {
                iIsBinded = false;
            }
        }
    }
    
    void DarwinGlContext::flush()
    {
        if ( iContext )
        {
            GreResourceAutolock ;
            
            if ( CGLFlushDrawable(iContext) != kCGLNoError )
            {
                GreDebugPretty() << "Can't flush CGLContext '" << getName() << "'." << std::endl;
            }
        }
    }
    
    const void* DarwinGlContext::getProperty(const std::string &name) const
    {
        GreResourceAutolock ;
        
        if ( name == "CGLContext" )
            return (const void*) &iContext;
        
        return Gre::RenderContextPrivate::getProperty(name);
    }
    
    void DarwinGlContext::setClearBuffers(const Gre::RenderContextClearBuffers &clearbuffs)
    {
        GLbitfield value = 0;
        
        for ( auto v : clearbuffs )
        {
            if ( v == Gre::RenderContextClearBuffer::ColorBufferBit )
                value |= GL_COLOR_BUFFER_BIT;
            
            if ( v == Gre::RenderContextClearBuffer::DepthBufferBit )
                value |= GL_DEPTH_BUFFER_BIT;
            
            if ( v == Gre::RenderContextClearBuffer::StencilBufferBit )
                value |= GL_STENCIL_BUFFER_BIT;
        }
        
        GreResourceAutolock ;
        iClearBuffers = value;
    }
    
    void DarwinGlContext::setClearColor(const Gre::Color &color)
    {
        GreResourceAutolock ;
        iClearColor = color;
    }
    
    void DarwinGlContext::onUpdateEvent(const Gre::UpdateEvent& e)
    {
        Gre::RenderContextPrivate::onUpdateEvent(e);
        
        // Here we just flush the CGLContext. Flush normally just swap
        // the two buffers.
        
        flush();
    }
}
