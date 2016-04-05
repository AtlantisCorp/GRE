//
//  OpenGlFrameBuffer.h
//  GRE
//
//  Created by Jacques Tronconi on 26/01/2016.
//
//

#ifndef GRE_OpenGlFrameBuffer_h
#define GRE_OpenGlFrameBuffer_h

#include "OpenGlIncludes.h"

class OpenGlFrameBuffer : public FrameBufferPrivate, public MultiContextId<GLuint>
{
public:
    
    POOLED(Pools::Resource)
    
    OpenGlFrameBuffer(const std::string& name);
    ~OpenGlFrameBuffer();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Make the FrameBuffer usable.
    //////////////////////////////////////////////////////////////////////
    void bind() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Make the FrameBuffer unusable.
    //////////////////////////////////////////////////////////////////////
    void unbind() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Attach a Texture object to this FrameBuffer.
    /// The Texture object must be unloaded, and not attached with any file.
    /// The Texture object loading is managed by the FrameBuffer.
    //////////////////////////////////////////////////////////////////////
    void attachTexture(Attachement attachement, const Texture& texture);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief This function should construct the Gl Object when it is time, i.e. when
    /// the correct RenderContext object is binded.
    //////////////////////////////////////////////////////////////////////
    void construct();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief This function should destroy the Gl Object when it is time, i.e. when
    /// the correct RenderContext object is binded.
    //////////////////////////////////////////////////////////////////////
    void destroy();
    
private:
    
    /// @brief The FrameBuffer OpenGl Id.
    GLuint _mfboid;
};

// ---------------------------------------------------------------------------------------------------

//////////////////////////////////////////////////////////////////////
/// @brief Loader to create some FrameBuffer blank object.
//////////////////////////////////////////////////////////////////////
class OpenGlFrameBufferLoader : public FrameBufferLoader
{
public:
    
    POOLED(Pools::Loader)
    
    OpenGlFrameBufferLoader();
    ~OpenGlFrameBufferLoader();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Loads a FrameBuffer.
    //////////////////////////////////////////////////////////////////////
    Resource* load(Resource::Type type, const std::string& name) const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns a clone of this object.
    //////////////////////////////////////////////////////////////////////
    ResourceLoader* clone() const;
};

#endif
