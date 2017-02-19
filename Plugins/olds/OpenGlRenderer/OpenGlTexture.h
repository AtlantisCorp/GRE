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
    
    OpenGlTexture(const std::string& name);
    OpenGlTexture(const std::string& name, const Image& img);
    ~OpenGlTexture();
    
    void bind() const;
    void unbind() const;
    bool isInvalid() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Loads the Texture in memory, if not yet.
    //////////////////////////////////////////////////////////////////////
    void load();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Sets data in memory. Texture must have already been loaded,
    /// and size of this data must be width*height*components.
    //////////////////////////////////////////////////////////////////////
    void setData(unsigned char* data);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Reset the Texture (creates a new empty Texture).
    //////////////////////////////////////////////////////////////////////
    void reset();
    
private:
    
    GLuint _mTextureId;
};

#endif
