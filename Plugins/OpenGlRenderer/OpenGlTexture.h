//
//  OpenGlTexture.h
//  GRE
//
//  Created by Jacques Tronconi on 11/12/2015.
//
//

#ifndef GRE_OpenGlTexture_h
#define GRE_OpenGlTexture_h

#include "OpenGlIncludes.h"

class OpenGlTexture : public TexturePrivate
{
public:
    
    OpenGlTexture (const std::string& name, const Image& img);
    ~OpenGlTexture();
    
    void bind() const;
    void unbind() const;
    bool isInvalid() const;
    
private:
    
    GLuint _mTextureId;
};

#endif
