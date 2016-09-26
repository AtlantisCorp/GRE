//////////////////////////////////////////////////////////////////////
//
//  DarwinGlRenderer.h
//  This source file is part of Gre
//		(Gang's Resource Engine)
//
//  Copyright (c) 2015 - 2016 Luk2010
//  Created on 04/09/2016.
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

#ifndef DarwinGlRenderer_h
#define DarwinGlRenderer_h

#include "DarwinGlHeader.h"
#include "Renderer.h"

namespace DarwinGl
{
    //////////////////////////////////////////////////////////////////////
    /// @brief OpenGl3 Renderer for Mac Os.
    ///
    /// OpenGl Context creation : Every OpenGl Context is shared with a
    /// Global Context for multithread purpose. ( see 'iGlobalContext' ).
    /// The Global Context is created using the default CGLPixelFormat .
    //////////////////////////////////////////////////////////////////////
    class DarwinGlRenderer : public Gre::RendererPrivate
    {
    public:
        
        //////////////////////////////////////////////////////////////////////
        //////////////////////////////////////////////////////////////////////
        DarwinGlRenderer ( const std::string& name );
        
        //////////////////////////////////////////////////////////////////////
        //////////////////////////////////////////////////////////////////////
        virtual ~DarwinGlRenderer() noexcept ( false );
        
        //////////////////////////////////////////////////////////////////////
        /// @brief Calls the API - dependent 'drawVertexBuffer' stuff.
        //////////////////////////////////////////////////////////////////////
        void drawVertexBufferPrivate(const Gre::HardwareVertexBuffer& vbuf, const Gre::HardwareProgramHolder& program);
        
        //////////////////////////////////////////////////////////////////////
        /// @brief Calls the API - dependent 'drawIndexBuffer' stuff.
        //////////////////////////////////////////////////////////////////////
        void drawIndexBufferPrivate(const Gre::HardwareIndexBuffer& ibuf, const Gre::HardwareVertexBuffer& vbuf, const Gre::HardwareProgramHolder& program);
        
        //////////////////////////////////////////////////////////////////////
        /// @brief Sets the buffer's base color when clearing it.
        /// @sa glSetClearColor()
        //////////////////////////////////////////////////////////////////////
        void setClearColor(const Gre::Color& color);
        
        //////////////////////////////////////////////////////////////////////
        /// @brief Sets the buffer's base depth when clearing it.
        /// @sa glSetClearDepth()
        //////////////////////////////////////////////////////////////////////
        void setClearDepth(float depth);
        
        //////////////////////////////////////////////////////////////////////
        /// @brief Creates a RenderContext using given Info.
        /// This function also creates a Global OpenGl Context, if this is the
        /// first time this function is called. Also, every OpenGl Context are
        /// shared with the Global Context.
        //////////////////////////////////////////////////////////////////////
        Gre::RenderContextHolder createRenderContext ( const std::string& name , const Gre::RenderContextInfo& info );
        
    protected:
        
        //////////////////////////////////////////////////////////////////////
        /// @brief Internal method to create a Vertex buffer.
        //////////////////////////////////////////////////////////////////////
        Gre::HardwareVertexBufferHolder iCreateVertexBuffer ( const std::string& name , size_t sz ) const;
        
        //////////////////////////////////////////////////////////////////////
        /// @brief Creates a Texture Object with given name, initialize it
        /// using API calls. ( like glGenTextures )
        //////////////////////////////////////////////////////////////////////
        Gre::TextureHolder iCreateTexturePrivate ( const std::string& name ) const;
        
        //////////////////////////////////////////////////////////////////////
        /// @brief Creates a RenderContext object.
        //////////////////////////////////////////////////////////////////////
        Gre::RenderContextHolder iCreateRenderContext ( const std::string& name , const Gre::RenderContextInfo& info ) const;
        
    private:
        
        /// @brief Global OpenGl context. This object is created after the first call
        /// to 'iCreateRenderContext'. The 'iGlobalContext' is used to create Resource
        /// like Texture, as they are shared with every other OpenGl Context. This make
        /// multithreaded Resource creation able, as one thread is allowed by OpenGl
        /// Context.
        CGLContextObj iGlobalContext;
        
        /// @brief Default Pixel Format.
        CGLPixelFormatObj iDefaultPixelFormat;
    };
    
    //////////////////////////////////////////////////////////////////////
    /// @brief RendererLoader for DarwinGlRenderer.
    //////////////////////////////////////////////////////////////////////
    class DarwinGlRendererLoader : public Gre::RendererLoader
    {
    public:
        
        //////////////////////////////////////////////////////////////////////
        //////////////////////////////////////////////////////////////////////
        DarwinGlRendererLoader();
        
        //////////////////////////////////////////////////////////////////////
        //////////////////////////////////////////////////////////////////////
        ~DarwinGlRendererLoader();
        
        //////////////////////////////////////////////////////////////////////
        /// @brief Returns 'true' if the given RenderingApiDescriptor is compatible
        /// with the Renderer loaded by this object.
        //////////////////////////////////////////////////////////////////////
        bool isCompatible ( const Gre::RenderingApiDescriptor& apidescriptor ) const;
        
        //////////////////////////////////////////////////////////////////////
        /// @brief Creates a new Renderer Object.
        //////////////////////////////////////////////////////////////////////
        Gre::RendererHolder load ( const std::string& name ) const;
        
        ////////////////////////////////////////////////////////////////////////
        /// @brief Returns a clone of this object.
        ////////////////////////////////////////////////////////////////////////
        Gre::ResourceLoader* clone() const;
        
        //////////////////////////////////////////////////////////////////////
        /// @brief Returns always false.
        //////////////////////////////////////////////////////////////////////
        bool isLoadable( const std::string& filepath ) const;
    };
}

#endif /* DarwinGlRenderer_h */
