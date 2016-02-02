//
//  FrameBuffer.h
//  GRE
//
//  Created by Jacques Tronconi on 25/01/2016.
//
//

#ifndef GRE_FrameBuffer_h
#define GRE_FrameBuffer_h

#include "Resource.h"
#include "Texture.h"

GRE_BEGIN_NAMESPACE

//////////////////////////////////////////////////////////////////////
/// @brief A FrameBuffer Object.
/// The Engine can use this object to draw Pass object into it. You can
/// easily take the textures involved, and attach textures to it.
//////////////////////////////////////////////////////////////////////
class DLL_PUBLIC FrameBufferPrivate : public Resource
{
public:
    
    POOLED(Pools::Resource)
    
    /// @brief Attachement possible to this FrameBuffer.
    /// You can attach Texture objects to this FrameBuffer, then attach
    /// the Texture object to a Material to use this Texture in the Engine.
    enum class Attachement
    {
        Color = 0,
//      [...] Normally this value is up to Renderer::getCapacity(Capacity::MaxFrameBufferColorAttachement) .
//      We support up to 32 Color Attachement. Default value for OpenGl is 4.
        Depth = 32,
        Stencil = 33
    };

    FrameBufferPrivate(const std::string& name);
    virtual ~FrameBufferPrivate();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Make the FrameBuffer usable.
    //////////////////////////////////////////////////////////////////////
    virtual void bind() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Make the FrameBuffer unusable.
    //////////////////////////////////////////////////////////////////////
    virtual void unbind() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the default accessible texture for drawing. This
    /// default texture is Attachement::Color .
    //////////////////////////////////////////////////////////////////////
    Texture& getDefaultTextureAccessible();
    const Texture& getDefaultTextureAccessible() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Attach a Texture object to this FrameBuffer.
    /// The Texture object must be unloaded, and not attached with any file.
    /// The Texture object loading is managed by the FrameBuffer.
    //////////////////////////////////////////////////////////////////////
    virtual void attachTexture(Attachement attachement, const Texture& texture);
    
protected:
    
    /// @brief Groups every textures used by a FrameBuffer.
    std::map<Attachement, Texture> _mHdwFboTextures;
    
    /// @brief The Maximum Width the FrameBuffer can reach.
    int _mMaxWidth;
    /// @brief The Maximum Height the FrameBuffer can reach.
    int _mMaxHeight;
    /// @brief The Maximum number of Color attachement.
    int _mMaxColorAttachement;
    /// @brief The Maximum number of Samples.
    int _mMaxSamples;
};

//////////////////////////////////////////////////////////////////////
/// @brief A Resource User to FrameBuffer.
//////////////////////////////////////////////////////////////////////
class DLL_PUBLIC FrameBuffer : public ResourceUser
{
public:
    
    POOLED(Pools::Resource)
    
    FrameBuffer();
    FrameBuffer(const FrameBuffer& rhs);
    FrameBuffer(FrameBuffer&& rhs);
    explicit FrameBuffer(const ResourceUser& rhs);
    FrameBuffer& operator = (const FrameBuffer& rhs);
    bool operator == (const FrameBuffer& rhs) const;
    bool operator != (const FrameBuffer& rhs) const;
    
    ~FrameBuffer();
    
    typedef FrameBufferPrivate::Attachement Attachement;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Make the FrameBuffer usable.
    //////////////////////////////////////////////////////////////////////
    void bind() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Make the FrameBuffer unusable.
    //////////////////////////////////////////////////////////////////////
    void unbind() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the default accessible texture for drawing. This
    /// default texture is Attachement::Color .
    //////////////////////////////////////////////////////////////////////
    Texture& getDefaultTextureAccessible();
    const Texture& getDefaultTextureAccessible() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Attach a Texture object to this FrameBuffer.
    /// The Texture object must be unloade, and not attached with any file.
    /// The Texture object loading is managed by the FrameBuffer.
    //////////////////////////////////////////////////////////////////////
    void attachTexture(Attachement attachement, const Texture& texture);
    
    /// @brief A Null FrameBuffer.
    static FrameBuffer Null;
    
private:
    
    /// @brief Holds the FrameBufferPrivate object.
    std::weak_ptr<FrameBufferPrivate> _mfbo;
};

//////////////////////////////////////////////////////////////////////
/// @brief A Resource User to FrameBuffer.
//////////////////////////////////////////////////////////////////////
class DLL_PUBLIC FrameBufferLoader : public ResourceLoader
{
public:
    
    POOLED(Pools::Loader)
    
    FrameBufferLoader();
    virtual ~FrameBufferLoader();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns true if Resource::Type is ::FrameBuff.
    //////////////////////////////////////////////////////////////////////
    virtual bool isTypeSupported(Resource::Type type) const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Loads a FrameBuffer.
    /// Default implementation returns a null pointer.
    //////////////////////////////////////////////////////////////////////
    virtual Resource* load(Resource::Type type, const std::string& name) const;
};

typedef ResourceLoaderFactory<FrameBufferLoader> FrameBufferLoaderFactory;

GRE_END_NAMESPACE

#endif
