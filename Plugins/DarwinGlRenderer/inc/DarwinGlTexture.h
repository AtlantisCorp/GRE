//////////////////////////////////////////////////////////////////////
//
//  DarwinGlTexture.h
//  This source file is part of Gre
//		(Gang's Resource Engine)
//
//  Copyright (c) 2015 - 2016 Luk2010
//  Created on 05/10/2016.
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

#ifndef DarwinGlTexture_h
#define DarwinGlTexture_h

#include "DarwinGlHeader.h"
#include "Texture.h"

namespace DarwinGl
{
    enum class GlTextureUpdateType
    {
        PixelBuffer ,
        Surface ,
        Type 
    };
    
    //////////////////////////////////////////////////////////////////////
    /// @brief OpenGl Texture object.
    ///
    /// # Manipulating Texture objects for OpenGl .
    ///
    /// Texture objects are NOT created at construction . You must at least
    /// set a SoftwarePixelBuffer , or set a Surface ( which will create
    /// a zero-d filled buffer ) , and a Texture Type .
    ///
    /// If the Texture has TextureType::Null , or if the Texture has no Buffer ,
    /// it will not be bindable and/or constructed into OpenGl structure.
    ///
    //////////////////////////////////////////////////////////////////////
    class DarwinGlTexture : public Gre::TexturePrivate
    {
    public:
        
        //////////////////////////////////////////////////////////////////////
        //////////////////////////////////////////////////////////////////////
        DarwinGlTexture ( const std::string& name ) ;
        
        //////////////////////////////////////////////////////////////////////
        //////////////////////////////////////////////////////////////////////
        virtual ~DarwinGlTexture () ;
        
        //////////////////////////////////////////////////////////////////////
        /// @brief Bind the Texture using given Texture Unit.
        //////////////////////////////////////////////////////////////////////
        virtual void bindWithTextureUnit ( int texunit ) const;
        
        //////////////////////////////////////////////////////////////////////
        /// @brief Bind the Texture using activated Texture Unit.
        //////////////////////////////////////////////////////////////////////
        virtual void bind() const;
        
        //////////////////////////////////////////////////////////////////////
        /// @brief Unbind the Texture from activated Texture Unit.
        //////////////////////////////////////////////////////////////////////
        virtual void unbind() const;
        
        //////////////////////////////////////////////////////////////////////
        /// @brief Activate given Texture Unit.
        //////////////////////////////////////////////////////////////////////
        virtual void activateTextureUnit ( int texunit ) const;
        
        //////////////////////////////////////////////////////////////////////
        /// @brief Change the first PixelBuffer used by this Texture.
        //////////////////////////////////////////////////////////////////////
        virtual void setPixelBuffer(const SoftwarePixelBuffer& pixelbuffer);
        
        //////////////////////////////////////////////////////////////////////
        /// @brief Sets the Surface representing this Texture.
        //////////////////////////////////////////////////////////////////////
        virtual void setSurface(const Surface& surface);
        
        //////////////////////////////////////////////////////////////////////
        /// @brief Changes the Texture's type.
        //////////////////////////////////////////////////////////////////////
        virtual void setType(TextureType textype);
        
        //////////////////////////////////////////////////////////////////////
        /// @brief Returns a custom property defined by given name.
        //////////////////////////////////////////////////////////////////////
        const void* getProperty ( const std::string& name ) const;
        
    protected:
        
        //////////////////////////////////////////////////////////////////////
        /// @brief Internally updates the Texture Surface .
        //////////////////////////////////////////////////////////////////////
        virtual void iUpdateTextureSurface ( const GlTextureUpdateType& utype );
        
    protected:
        
        /// @brief OpenGl Texture id .
        GLuint iGlTexture ;
    };
}

#endif /* DarwinGlTexture_h */
