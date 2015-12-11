//
//  OpenGlTexture.cpp
//  GRE
//
//  Created by Jacques Tronconi on 11/12/2015.
//
//

#include "OpenGlTexture.h"

OpenGlTexture::OpenGlTexture (const std::string& name, const Image& img)
: TexturePrivate(name, img), _mTextureId(0)
{
    if(!img.isEmpty())
    {
        const PixelBatch& pbatch = img.getPixelBatch();
        
        glPixelStorei(GL_UNPACK_ROW_LENGTH, pbatch.width);
        glPixelStorei(GL_UNPACK_ALIGNMENT,  pbatch.pixelsAlignment);
        
        glGenTextures(1, &_mTextureId);
        glBindTexture(GL_TEXTURE_2D, _mTextureId);
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        
        if(pbatch.samplesPerPixel == 3 || pbatch.samplesPerPixel == 4)
        {
            glTexImage2D(GL_TEXTURE_2D, 0,
                         pbatch.samplesPerPixel == 4 ? GL_RGBA8 : GL_RGB8,
                         image.getWidth(),
                         image.getHeight(),
                         0,
                         pbatch.samplesPerPixel == 4 ? GL_RGBA : GL_RGB,
                         GL_UNSIGNED_BYTE,
                         img.getPixelBatch().pixels);
        }
        
        glBindTexture(GL_TEXTURE_2D, 0);
        image.unloadCache();
    }
}

OpenGlTexture::~OpenGlTexture()
{
    if(_mTextureId)
    {
        glDeleteTextures(1, &_mTextureId);
    }
}

void OpenGlTexture::bind() const
{
    if(_mTextureId) {
        glBindTexture(GL_TEXTURE_2D, _mTextureId);
        setBinded(true);
    }
}

void OpenGlTexture::unbind() const
{
    if(_mTextureId) {
        glBindTexture(GL_TEXTURE_2D, 0);
        setBinded(false);
    }
}

bool OpenGlTexture::isInvalid() const
{
    return _mTextureId == 0;
}
