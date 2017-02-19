//////////////////////////////////////////////////////////////////////
//
//  DarwinGlTexture.cpp
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

#include "DarwinGlTexture.h"

namespace DarwinGl
{
    GLenum GlTextureTargetFromTextureType ( const TextureType& type )
    {
        if ( type == TextureType::OneDimension )
            return GL_TEXTURE_1D;
        if ( type == TextureType::TwoDimension )
            return GL_TEXTURE_2D;
        if ( type == TextureType::ThreeDimension )
            return GL_TEXTURE_3D;
        if ( type == TextureType::CubeMap )
            return GL_TEXTURE_CUBE_MAP;
        return GL_INVALID_ENUM;
    }
    
    GLenum GlTextureComponents ( const HardwarePixelFormat& format )
    {
        if ( format == HardwarePixelFormat::RGBAShort )
            return GL_RGBA;
        if ( format == HardwarePixelFormat::RGBAInt )
            return GL_RGBA;
        if ( format == HardwarePixelFormat::RGBAFloat )
            return GL_RGBA;
        return GL_INVALID_ENUM;
    }
    
    GLenum GlTextureFormat ( const HardwarePixelFormat& format )
    {
        if ( format == HardwarePixelFormat::RGBAShort )
            return GL_SHORT;
        if ( format == HardwarePixelFormat::RGBAInt )
            return GL_INT;
        if ( format == HardwarePixelFormat::RGBAFloat )
            return GL_FLOAT;
        return GL_INVALID_ENUM;
    }
    
    DarwinGlTexture::DarwinGlTexture ( const std::string& name )
    : Gre::TexturePrivate(name)
    , iGlTexture(0)
    {
        
    }
    
    DarwinGlTexture::~DarwinGlTexture()
    {
        if ( iGlTexture )
        {
            glDeleteTextures(1, &iGlTexture);
        }
    }
    
    void DarwinGlTexture::bindWithTextureUnit(int texunit) const
    {
        if ( iGlTexture )
        {
            glActiveTexture(texunit);
            glBindTexture(GlTextureTargetFromTextureType(getType()), iGlTexture);
        }
    }
    
    void DarwinGlTexture::bind() const
    {
        if ( iGlTexture )
        {
            glBindTexture(GlTextureTargetFromTextureType(getType()), iGlTexture);
        }
    }
    
    void DarwinGlTexture::unbind() const
    {
        glBindTexture(GlTextureTargetFromTextureType(getType()), 0);
    }
    
    void DarwinGlTexture::activateTextureUnit(int texunit) const
    {
        glActiveTexture(texunit);
    }
    
    void DarwinGlTexture::setPixelBuffer(const Gre::SoftwarePixelBuffer &pixelbuffer)
    {
        TexturePrivate::setPixelBuffer(pixelbuffer);
        iUpdateTextureSurface(GlTextureUpdateType::PixelBuffer);
    }
    
    void DarwinGlTexture::setSurface(const Gre::Surface &surface)
    {
        TexturePrivate::setSurface(surface);
        iUpdateTextureSurface(GlTextureUpdateType::Surface);
    }
    
    void DarwinGlTexture::setType(Gre::TextureType textype)
    {
        TexturePrivate::setType(textype);
        iUpdateTextureSurface(GlTextureUpdateType::Type);
    }
    
    const void* DarwinGlTexture::getProperty(const std::string &name) const
    {
        if ( name == "GlTexture" )
            return &iGlTexture;
        
        return TexturePrivate::getProperty(name);
    }
    
    void DarwinGlTexture::iUpdateTextureSurface( const GlTextureUpdateType& utype )
    {
        if ( utype == GlTextureUpdateType::PixelBuffer )
        {
            SoftwarePixelBuffer softpixu = getPixelBuffer();
            
            if ( softpixu.isInvalid() )
            {
                return;
            }
            
            SoftwarePixelBufferHolder softpix = softpixu.lock();
            
            if ( iGlTexture )
            {
                bind();
                
                // Here we assume the Texture has already been created and the Surface is correct.
                // We just call glTexSubImage depending on the TextureType.
                
                GLenum target = GlTextureTargetFromTextureType(getType());
                GLint level = 0;
                GLint xoffset = 0;                                              // Unsupported
                GLint yoffset = 0;                                              // Unsupported
                GLint zoffset = 0;                                              // Unsupported
                GLsizei width = getSurface().width;
                GLsizei height = getSurface().height;
                GLsizei depth = 0;                                              // Unsupported
                GLenum format = GlTextureComponents(softpix->getPixelFormat());
                GLenum type = GlTextureFormat(softpix->getPixelFormat());
                const GLvoid* pixels = (const GLvoid*) softpix->getData();
                
                TextureType textype = getType();
                
                if ( textype == TextureType::OneDimension )
                {
                    glTexSubImage1D(target, level, xoffset, width, format, type, pixels);
                }
                
                if ( textype == TextureType::TwoDimension )
                {
                    glTexSubImage2D(target, level, xoffset, yoffset, width, height, format, type, pixels);
                }
                
                if ( textype == TextureType::ThreeDimension )
                {
                    glTexSubImage3D(target, level, xoffset, yoffset, zoffset, width, height, depth, format, type, pixels);
                }
                
                if ( textype == TextureType::CubeMap )
                {
                    // Here we have 6 different textures. But we will not do it for now.
                    // UNSUPPORTED
                }
                
            }
            
            else
            {
                // If 'iGlTexture' is invalid , we must create the Texture object.
                // We must also check for TextureType , and Surface .
                
                if ( getType() == TextureType::Null )
                    return;
                Surface surf = getSurface();
                if ( surf.width <= 0 || surf.height <= 0 )
                    return;
                
                glGenTextures(1, &iGlTexture);
                bind();
                
                GLenum target = GlTextureTargetFromTextureType(getType());
                GLint level = 0;
                GLsizei width = getSurface().width;
                GLsizei height = getSurface().height;
                GLsizei depth = 0;                                              // Unsupported
                GLenum format = GlTextureComponents(softpix->getPixelFormat());
                GLenum type = GlTextureFormat(softpix->getPixelFormat());
                const GLvoid* pixels = (const GLvoid*) softpix->getData();
                
                TextureType textype = getType();
                
                if ( textype == TextureType::OneDimension )
                {
                    glTexImage1D(target, level, GL_RGBA, width, 0, format, type, pixels);
                }
                
                if ( textype == TextureType::TwoDimension )
                {
                    glTexImage2D(target, level, GL_RGBA, width, height, 0, format, type, pixels);
                }
                
                if ( textype == TextureType::ThreeDimension )
                {
                    glTexImage3D(target, level, GL_RGBA, width, height, depth, 0, format, type, pixels);
                }
                
                if ( textype == TextureType::CubeMap )
                {
                    // Here we have 6 different textures. But we will not do it for now.
                    // UNSUPPORTED
                }
            }
        }
        
        if ( utype == GlTextureUpdateType::Surface )
        {
            // If we are updating the Surface , but the GlTexture is valid , we should destroy it and
            // make a new one. Normally , you should not use Texture::setSurface if data is already set.
            
            if ( iGlTexture )
            {
                // Destroy the Texture , and launch the Update with SoftwarePixelBuffer .
                glDeleteTextures(1, &iGlTexture);
                iGlTexture = 0;
            }
            
            SoftwarePixelBuffer pixbufu = getPixelBuffer();
            
            if ( pixbufu.isInvalid() )
            {
                // Create a new empty pixel buffer .
                SoftwarePixelBufferHolder pixbuf = SoftwarePixelBufferHolder ( new SoftwarePixelBufferPrivate(getName()+"/0softbuf") );
                pixbuf->setPixelFormat(HardwarePixelFormat::RGBAFloat);
                pixbuf->setData(nullptr, getSurface().width * getSurface().height * sizeof(float));
                setPixelBuffer(pixbuf);
            }
            
            else
            {
                iUpdateTextureSurface(GlTextureUpdateType::PixelBuffer);
            }
        }
        
        if ( utype == GlTextureUpdateType::Type )
        {
            // If TextureType has changed , we should reload the Texture.
            if ( iGlTexture )
            {
                glDeleteTextures(1, &iGlTexture);
                iGlTexture = 0;
            }
            
            iUpdateTextureSurface(GlTextureUpdateType::PixelBuffer);
        }
    }
}
