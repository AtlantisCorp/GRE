//
//  OpenGlTexture.cpp
//  GRE
//
//  Created by Jacques Tronconi on 11/12/2015.
//
//

#include "OpenGlTexture.h"
#include "OpenGlRenderContext.h"

typedef GreExceptionWithText OpenGlGenTexture;

OpenGlTexture::OpenGlTexture(const std::string& name)
: TexturePrivate(name)
{
    _mTextureId = 0;
    glGlobalContext->getGl().GenTextures(1, &_mTextureId);
    
#ifdef GreIsDebugMode
    if(!_mTextureId)
    {
        GreDebugPretty() << "Can't create texture '" << name << "'." << Gre::gendl;
        throw OpenGlGenTexture(name + " (glGenTextures)");
    }
#endif
}

OpenGlTexture::OpenGlTexture (const std::string& name, const Image& img)
: TexturePrivate(name, img), _mTextureId(0)
{
    if(!img.isEmpty())
    {
        const PixelBatch& pbatch = img.getPixelBatch();
        
        glGlobalContext->getGl().PixelStorei(GL_UNPACK_ROW_LENGTH, pbatch.width);
        glGlobalContext->getGl().PixelStorei(GL_UNPACK_ALIGNMENT,  pbatch.pixelsAlignment);
        
        glGlobalContext->getGl().GenTextures(1, &_mTextureId);
        glGlobalContext->getGl().BindTexture(GL_TEXTURE_2D, _mTextureId);
        
        glGlobalContext->getGl().TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glGlobalContext->getGl().TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        
        if(pbatch.samplesPerPixel == 3 || pbatch.samplesPerPixel == 4)
        {
            glGlobalContext->getGl().TexImage2D(GL_TEXTURE_2D, 0,
                         pbatch.samplesPerPixel == 4 ? GL_RGBA8 : GL_RGB8,
                         image.getWidth(),
                         image.getHeight(),
                         0,
                         pbatch.samplesPerPixel == 4 ? GL_RGBA : GL_RGB,
                         GL_UNSIGNED_BYTE,
                         img.getPixelBatch().pixels);
        }
        
        // Filling properties
        _mTexType = Type::TwoDimension;
        _mMipmapLevel = 0;
        _mStoreType = StorageType::UnsignedByte;
        _mWidth = image.getWidth();
        _mHeight = image.getHeight();
        _mComponentsNumber = pbatch.samplesPerPixel;
        
        glGlobalContext->getGl().BindTexture(GL_TEXTURE_2D, 0);
        image.unloadCache();
        setLoaded(true);
    }
}

OpenGlTexture::~OpenGlTexture()
{
    if(_mTextureId)
    {
        glGlobalContext->getGl().DeleteTextures(1, &_mTextureId);
    }
}

void OpenGlTexture::bind() const
{
    if(_mTextureId) {
        glGlobalContext->getGl().BindTexture(GL_TEXTURE_2D, _mTextureId);
        setBinded(true);
    }
}

void OpenGlTexture::unbind() const
{
    if(_mTextureId) {
        glGlobalContext->getGl().BindTexture(GL_TEXTURE_2D, 0);
        setBinded(false);
    }
}

bool OpenGlTexture::isInvalid() const
{
    return _mTextureId == 0;
}

void OpenGlTexture::load()
{
    if(isLoaded()) return;
    
    glGlobalContext->getGl().BindTexture(glGlobalContext->getGl().TextureTypeToGlType(_mTexType), _mTextureId);
    glGlobalContext->getGl().PixelStorei(GL_UNPACK_ROW_LENGTH, (GLint) _mWidth);
    glGlobalContext->getGl().PixelStorei(GL_UNPACK_ALIGNMENT, (GLint) 1);
    glGlobalContext->getGl().TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glGlobalContext->getGl().TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    if(_mTexType == Type::TwoDimension)
    {
        if(_mComponentsNumber == 3)
        {
            glGlobalContext->getGl().TexImage2D(GL_TEXTURE_2D, _mMipmapLevel, GL_RGB8, (GLsizei) _mWidth, (GLsizei) _mHeight, 0, GL_RGB, OpenGlUtils::StorageTypeToGl(_mStoreType), 0);
        }
        else if(_mComponentsNumber == 4)
        {
            glGlobalContext->getGl().TexImage2D(GL_TEXTURE_2D, _mMipmapLevel, GL_RGBA8, (GLsizei) _mWidth, (GLsizei) _mHeight, 0, GL_RGBA, OpenGlUtils::StorageTypeToGl(_mStoreType), 0);
        }
    }
    
    else if(_mTexType == Type::Depth)
    {
        glGlobalContext->getGl().TexImage2D(GL_TEXTURE_2D, _mMipmapLevel, GL_DEPTH_COMPONENT, (GLsizei) _mWidth, (GLsizei) _mHeight, 0, GL_DEPTH_COMPONENT, OpenGlUtils::StorageTypeToGl(_mStoreType), 0);
    }
    
    setLoaded(true);
}

void OpenGlTexture::setData(unsigned char *data)
{
    if(!isLoaded()) return;
    
    glGlobalContext->getGl().BindTexture(glGlobalContext->getGl().TextureTypeToGlType(_mTexType), _mTextureId);
    
    if(_mTexType == Type::TwoDimension)
    {
        if(_mComponentsNumber == 3)
        {
            glGlobalContext->getGl().TexSubImage2D(GL_TEXTURE_2D, _mMipmapLevel, 0, 0, (GLsizei) _mWidth, (GLsizei) _mHeight, GL_RGB, OpenGlUtils::StorageTypeToGl(_mStoreType), data);
        }
        
        if(_mComponentsNumber == 4)
        {
            glGlobalContext->getGl().TexSubImage2D(GL_TEXTURE_2D, _mMipmapLevel, 0, 0, (GLsizei) _mWidth, (GLsizei) _mHeight, GL_RGBA, OpenGlUtils::StorageTypeToGl(_mStoreType), data);
        }
    }
    
    if(_mTexType == Type::Depth)
    {
        glGlobalContext->getGl().TexSubImage2D(GL_TEXTURE_2D, _mMipmapLevel, 0, 0, (GLsizei) _mWidth, (GLsizei) _mHeight, GL_DEPTH_COMPONENT, OpenGlUtils::StorageTypeToGl(_mStoreType), data);
    }
}

void OpenGlTexture::reset()
{
    if(_mTextureId)
    {
        glGlobalContext->getGl().DeleteTextures(1, &_mTextureId);
        TexturePrivate::reset();
        glGlobalContext->getGl().GenTextures(1, &_mTextureId);
    }
}


