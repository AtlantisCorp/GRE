//////////////////////////////////////////////////////////////////////
//
//  OpenGlTexture.cpp
//  This source file is part of Gre
//		(Gang's Resource Engine)
//
//  Copyright (c) 2015 - 2017 Luk2010
//  Created on 12/02/2017.
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

#include "OpenGlRenderer.h"

using namespace Gre ;

// ---------------------------------------------------------------------------------------------------------
// GRE to OpenGl translations.

GLenum translateGlTexture ( const Gre::TextureType & type )
{
    if ( type == Gre::TextureType::Texture1D )
        return GL_TEXTURE_1D ;
    else if ( type == Gre::TextureType::Texture2D )
        return GL_TEXTURE_2D ;
    else if ( type == Gre::TextureType::Texture3D )
        return GL_TEXTURE_3D ;
    else if ( type == Gre::TextureType::CubeMap )
        return GL_TEXTURE_CUBE_MAP ;
    
    return GL_INVALID_ENUM ;
}

#ifdef GL_DEPTH_STENCIL_TEXTURE_MODE

GLenum translateGlDepthStencilMode ( TextureDepthStencilMode mode )
{
    if ( mode == TextureDepthStencilMode::Depth )
        return GL_DEPTH_COMPONENT ;
    else if ( mode == TextureDepthStencilMode::Stencil )
        return GL_STENCIL_COMPONENT ;
    return GL_INVALID_ENUM ;
}

#endif

#ifdef GL_TEXTURE_COMPARE_FUNC

GLenum translateGlCompareFunc ( TextureCompareFunc func )
{
    if ( func == TextureCompareFunc::LessEqual )
        return GL_LEQUAL ;
    else if ( func == TextureCompareFunc::GreatEqual )
        return GL_GEQUAL ;
    else if ( func == TextureCompareFunc::Less )
        return GL_LESS ;
    else if ( func == TextureCompareFunc::Greater )
        return GL_GREATER ;
    else if ( func == TextureCompareFunc::Equal )
        return GL_EQUAL ;
    else if ( func == TextureCompareFunc::NotEqual )
        return GL_NOTEQUAL ;
    else if ( func == TextureCompareFunc::Always )
        return GL_ALWAYS ;
    else if ( func == TextureCompareFunc::Never )
        return GL_NEVER ;
    return GL_INVALID_ENUM ;
}

#endif

#ifdef GL_TEXTURE_COMPARE_MODE

GLenum translateGlCompareMode ( TextureCompareMode mode )
{
    if ( mode == TextureCompareMode::Ref )
        return GL_COMPARE_REF_TO_TEXTURE ;
    else if ( mode == TextureCompareMode::None )
        return GL_NONE ;
    return GL_INVALID_ENUM ;
}

#endif

#ifdef GL_TEXTURE_MIN_FILTER

GLenum translateGlMinFilter ( TextureMinFilter filter )
{
    if ( filter == TextureMinFilter::Linear )
        return GL_LINEAR ;
    else if ( filter == TextureMinFilter::Nearest )
        return GL_NEAREST ;
    else if ( filter == TextureMinFilter::MipmapLinear )
        return GL_NEAREST_MIPMAP_LINEAR ;
    else if ( filter == TextureMinFilter::MipmapNearest )
        return GL_NEAREST_MIPMAP_NEAREST ;
    return GL_INVALID_ENUM ;
}

#endif

#ifdef GL_TEXTURE_MAG_FILTER

GLenum translateGlMagFilter ( TextureMagFilter filter )
{
    if ( filter == TextureMagFilter::Linear )
        return GL_LINEAR ;
    else if ( filter == TextureMagFilter::Nearest )
        return GL_NEAREST ;
    return GL_INVALID_ENUM ;
}

#endif

#ifdef GL_TEXTURE_WRAP_S

GLenum translateGlWrap ( TextureWrap mode )
{
    if ( mode == TextureWrap::ClampToBorder )
        return GL_CLAMP_TO_BORDER ;
    else if ( mode == TextureWrap::ClampToEdge )
        return GL_CLAMP_TO_EDGE ;
    else if ( mode == TextureWrap::MirroredRepeat )
        return GL_MIRRORED_REPEAT ;
    else if ( mode == TextureWrap::Repeat )
        return GL_REPEAT ;
    
#ifdef GL_MIRROR_CLAMP_TO_EDGE
    else if ( mode == TextureWrap::MirrorClampToEdge )
        return GL_MIRROR_CLAMP_TO_EDGE ;
#endif
    
    return GL_INVALID_ENUM ;
}

#endif

GLenum translateGlInternalPixelFormat ( InternalPixelFormat format )
{
    if ( format == InternalPixelFormat::Red )
        return GL_RED ;
    else if ( format == InternalPixelFormat::RG )
        return GL_RG ;
    else if ( format == InternalPixelFormat::RGB )
        return GL_RGB ;
    else if ( format == InternalPixelFormat::RGBA )
        return GL_RGBA ;
    
    else if ( format == InternalPixelFormat::RGB4 )
        return GL_RGB4 ;
    else if ( format == InternalPixelFormat::RGB8 )
        return GL_RGB8 ;
    else if ( format == InternalPixelFormat::RGB12 )
        return GL_RGB12 ;
    else if ( format == InternalPixelFormat::RGB16 )
        return GL_RGB16 ;
    else if ( format == InternalPixelFormat::RGB32 )
        return GL_RGB32F ;
    
    else if ( format == InternalPixelFormat::RGBA4 )
        return GL_RGBA4 ;
    else if ( format == InternalPixelFormat::RGBA8 )
        return GL_RGBA8 ;
    else if ( format == InternalPixelFormat::RGBA12 )
        return GL_RGBA12 ;
    else if ( format == InternalPixelFormat::RGBA16 )
        return GL_RGBA16 ;
    else if ( format == InternalPixelFormat::RGBA32 )
        return GL_RGBA32F ;
    
    else if ( format == InternalPixelFormat::DepthComponent )
        return GL_DEPTH_COMPONENT ;
    else if ( format == InternalPixelFormat::DepthStencil )
        return GL_DEPTH_STENCIL ;
    
    else if ( format == InternalPixelFormat::CompressedRed )
        return GL_COMPRESSED_RED ;
    else if ( format == InternalPixelFormat::CompressedRG )
        return GL_COMPRESSED_RG ;
    else if ( format == InternalPixelFormat::CompressedRGB )
        return GL_COMPRESSED_RGB ;
    else if ( format == InternalPixelFormat::CompressedRGBA )
        return GL_COMPRESSED_RGBA ;
    
    return GL_INVALID_ENUM ;
}

GLenum translateGlPixelFormat ( PixelFormat format )
{
    if ( format == PixelFormat::DepthComponent )
        return GL_DEPTH_COMPONENT ;
    else if ( format == PixelFormat::DepthStencil )
        return GL_DEPTH_STENCIL ;
    
    else if ( format == PixelFormat::Red || format == PixelFormat::Luminance )
        return GL_RED ;
    else if ( format == PixelFormat::RG || format == PixelFormat::LuminanceAlpha )
        return GL_RG ;
    else if ( format == PixelFormat::RGB )
        return GL_RGB ;
    else if ( format == PixelFormat::RGBA )
        return GL_RGBA ;
    
    else if ( format == PixelFormat::BGR )
        return GL_BGR ;
    else if ( format == PixelFormat::BGRA )
        return GL_BGRA ;
    
    else if ( format == PixelFormat::StencilIndex )
        return GL_STENCIL_INDEX ;
    
    return GL_INVALID_ENUM ;
}

GLenum translateGlPixelType ( PixelType format )
{
    if ( format == PixelType::Byte )
        return GL_BYTE ;
    else if ( format == PixelType::Short )
        return GL_SHORT ;
    else if ( format == PixelType::Int )
        return GL_INT ;
    else if ( format == PixelType::Float )
        return GL_FLOAT ;
    
    else if ( format == PixelType::UnsignedByte )
        return GL_UNSIGNED_BYTE ;
    else if ( format == PixelType::UnsignedShort )
        return GL_UNSIGNED_SHORT ;
    else if ( format == PixelType::UnsignedInt )
        return GL_UNSIGNED_INT ;
    
    return GL_INVALID_ENUM ;
}

// ---------------------------------------------------------------------------------------------------------
// OpenGlTexture

OpenGlTexture::OpenGlTexture (const std::string & name , const Gre::TextureType & type ,
                              const Gre::SoftwarePixelBufferHolderList& buffers)
: Gre::Texture(name, type, buffers) , iGlTexture(0)
{
    if ( !iPixelBuffers.empty() ) {
        _setBuffer () ;
    }
}

OpenGlTexture::~OpenGlTexture () noexcept ( false )
{
    if ( iGlTexture ) {
        glDeleteTextures(1, &iGlTexture);
        iGlTexture = 0 ;
    }
}

bool OpenGlTexture::isGlTextureValid() const
{
    GreAutolock ; return iGlTexture != 0 ;
}

void OpenGlTexture::_setParameters ( GLenum target ) const
{
    GreAutolock ;
    
#ifdef GL_DEPTH_STENCIL_TEXTURE_MODE
    glTexParameteri(target, GL_DEPTH_STENCIL_TEXTURE_MODE,
                    translateGlDepthStencilMode(getParameterValue(TextureParameter::DepthStencilMode).toInteger()));
#endif
    
#ifdef GL_TEXTURE_BASE_LEVEL
    glTexParameteri(target, GL_TEXTURE_BASE_LEVEL,
                    getParameterValue(TextureParameter::MipmapBaseLevel).toInteger());
#endif
    
#ifdef GL_TEXTURE_BORDER_COLOR
    glTexParameterfv(target, GL_TEXTURE_BORDER_COLOR,
                     &getParameterValue(TextureParameter::BorderColor).toColor().toFloat4()[0]);
#endif
    
#ifdef GL_TEXTURE_COMPARE_FUNC
    glTexParameteri(target, GL_TEXTURE_COMPARE_FUNC,
                    translateGlCompareFunc((TextureCompareFunc)getParameterValue(TextureParameter::CompareFunc).toInteger()));
#endif
    
#ifdef GL_TEXTURE_COMPARE_MODE
    glTexParameteri(target, GL_TEXTURE_COMPARE_MODE,
                    translateGlCompareMode((TextureCompareMode)getParameterValue(TextureParameter::CompareMode).toInteger()));
#endif
    
#ifdef GL_TEXTURE_LOD_BIAS
    glTexParameterf(target, GL_TEXTURE_LOD_BIAS,
                    getParameterValue(TextureParameter::LODBias).toFloat());
#endif
    
#ifdef GL_TEXTURE_MIN_FILTER
    glTexParameteri(target, GL_TEXTURE_MIN_FILTER,
                    translateGlMinFilter((TextureMinFilter)getParameterValue(TextureParameter::MinFilter).toInteger()));
#endif
    
#ifdef GL_TEXTURE_MAG_FILTER
    glTexParameteri(target, GL_TEXTURE_MAG_FILTER,
                    translateGlMagFilter((TextureMagFilter)getParameterValue(TextureParameter::MagFilter).toInteger()));
#endif
    
#ifdef GL_TEXTURE_WRAP_S
    glTexParameteri(target, GL_TEXTURE_WRAP_S,
                    translateGlWrap((TextureWrap)getParameterValue(TextureParameter::WrapS).toInteger()));
#endif
    
#ifdef GL_TEXTURE_WRAP_T
    glTexParameteri(target, GL_TEXTURE_WRAP_T,
                    translateGlWrap((TextureWrap)getParameterValue(TextureParameter::WrapT).toInteger()));
#endif
    
#ifdef GL_TEXTURE_WRAP_R
    glTexParameteri(target, GL_TEXTURE_WRAP_R,
                    translateGlWrap((TextureWrap)getParameterValue(TextureParameter::WrapR).toInteger()));
#endif
}

void OpenGlTexture::_applySwizzling ( GLenum target , const Gre::SoftwarePixelBufferHolder& buffer ) const
{
    if ( !buffer.isInvalid() )
    {
        if ( buffer -> getPixelFormat() == Gre::PixelFormat::Luminance )
        {
            GLint swizzleMask [] = { GL_RED , GL_RED , GL_RED , GL_ONE } ;
            glTexParameteriv(target, GL_TEXTURE_SWIZZLE_RGBA, swizzleMask);
        }
        
        else if ( buffer -> getPixelFormat() == Gre::PixelFormat::LuminanceAlpha )
        {
            GLint swizzleMask [] = { GL_RED , GL_RED , GL_RED , GL_GREEN } ;
            glTexParameteriv(target, GL_TEXTURE_SWIZZLE_RGBA, swizzleMask);
        }
    }
}

void OpenGlTexture::_bind() const
{
    GreAutolock ; glBindTexture(translateGlTexture(getType()), iGlTexture);
    iBinded = true ;
}

void OpenGlTexture::_unbind() const
{
    GreAutolock ; glBindTexture(translateGlTexture(getType()), 0);
    iBinded = false ;
}

void OpenGlTexture::_setBuffer () const
{
    GreAutolock ;
    
    if ( getType() == Gre::TextureType::Texture1D && iPixelBuffers.size() > 1 )
    {
        glGenTextures( 1, & iGlTexture ) ;
        
        if ( !iGlTexture ) {
            GreDebug("[WARN] Can't use glGenTextures.") << Gre::gendl ;
            return ;
        }
        
        glBindTexture(GL_TEXTURE_1D, iGlTexture);
        const Gre::SoftwarePixelBufferHolder & buffer = *iPixelBuffers.begin() ;
        
        _setParameters ( GL_TEXTURE_1D ) ;
        _applySwizzling( GL_TEXTURE_1D, buffer ) ;
        
        glTexImage1D(GL_TEXTURE_1D, 0,
                     translateGlInternalPixelFormat(buffer->getInternalPixelFormat()),
                     buffer->getSurface().width, 0,
                     translateGlPixelFormat(buffer->getPixelFormat()),
                     translateGlPixelType(buffer->getPixelType()),
                     buffer->getData());
        
        glGenerateMipmap(GL_TEXTURE_1D);
        glBindTexture(GL_TEXTURE_1D, 0);
    }
    
    else if ( getType() == Gre::TextureType::Texture2D && iPixelBuffers.size() > 0 )
    {
        glGenTextures( 1, & iGlTexture ) ;
        
        if ( !iGlTexture ) {
            GreDebug("[WARN] Can't use glGenTextures.") << Gre::gendl ;
            return ;
        }
        
        glBindTexture(GL_TEXTURE_2D, iGlTexture);
        const Gre::SoftwarePixelBufferHolder & buffer = *iPixelBuffers.begin() ;
        
        _setParameters ( GL_TEXTURE_2D ) ;
        _applySwizzling( GL_TEXTURE_2D, buffer ) ;
        
        glTexImage2D(GL_TEXTURE_2D, 0,
                     translateGlInternalPixelFormat(buffer->getInternalPixelFormat()),
                     buffer->getSurface().width, buffer->getSurface().height, 0,
                     translateGlPixelFormat(buffer->getPixelFormat()),
                     translateGlPixelType(buffer->getPixelType()),
                     buffer->getData());
        
        glGenerateMipmap(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    
    else if ( getType() == Gre::TextureType::Texture3D && iPixelBuffers.size() > 0 )
    {
        glGenTextures( 1, & iGlTexture ) ;
        
        if ( !iGlTexture ) {
            GreDebug("[WARN] Can't use glGenTextures.") << Gre::gendl ;
            return ;
        }
        
        glBindTexture(GL_TEXTURE_3D, iGlTexture);
        const Gre::SoftwarePixelBufferHolder & buffer = *iPixelBuffers.begin() ;
        
        _setParameters ( GL_TEXTURE_3D ) ;
        _applySwizzling( GL_TEXTURE_3D, buffer ) ;
        
        glTexImage3D(GL_TEXTURE_3D, 0,
                     translateGlInternalPixelFormat(buffer->getInternalPixelFormat()),
                     buffer->getSurface().width, buffer->getSurface().height, buffer->getDepth(), 0,
                     translateGlPixelFormat(buffer->getPixelFormat()),
                     translateGlPixelType(buffer->getPixelType()),
                     buffer->getData());
        
        glGenerateMipmap(GL_TEXTURE_3D);
        glBindTexture(GL_TEXTURE_3D, 0);
    }
    
    else if ( getType() == Gre::TextureType::CubeMap && iPixelBuffers.size() == 6 )
    {
        glGenTextures(1, & iGlTexture) ;
        
        if ( !iGlTexture ) {
            GreDebug("[WARN] Can't use glGenTextures.") << Gre::gendl ;
            return ;
        }
        
        glBindTexture(GL_TEXTURE_CUBE_MAP, iGlTexture);
        _setParameters ( GL_TEXTURE_CUBE_MAP ) ;
        
        int layer = 0 ;
        for ( const Gre::SoftwarePixelBufferHolder & it : iPixelBuffers )
        {
            const Gre::SoftwarePixelBufferHolder & buffer = it ;
            _applySwizzling( GL_TEXTURE_CUBE_MAP_POSITIVE_X + layer , buffer ) ;
            
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + layer , 0,
                         translateGlInternalPixelFormat(buffer->getInternalPixelFormat()),
                         buffer->getSurface().width, buffer->getSurface().height, 0,
                         translateGlPixelFormat(buffer->getPixelFormat()),
                         translateGlPixelType(buffer->getPixelType()),
                         buffer->getData());
            layer ++ ;
        }
        
        glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
        glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    }
}

GLuint OpenGlTexture::getGlTexture() const
{
    GreAutolock ; return iGlTexture ;
}


