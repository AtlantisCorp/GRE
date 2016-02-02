//
//  Texture.h
//  GRE
//
//  Created by Jacques Tronconi on 26/11/2015.
//
//

#ifndef GRE_Texture_h
#define GRE_Texture_h

#include "Pools.h"
#include "Image.h"

GRE_BEGIN_NAMESPACE

//////////////////////////////////////////////////////////////////////
/// @brief Represents a Texture object.
/// @note The Texture object is completly different from the Image
/// object. The Texture should not stores the Image, this isn't a standard
/// behaviour. The Texture should only create an Hardware Texture object
/// in the Gpu's Memory. Also, you can modify this memory after creation.
//////////////////////////////////////////////////////////////////////
class DLL_PUBLIC TexturePrivate : public Resource
{
public:
    
    /// @brief Resumes the Texture's possible Types.
    enum class Type
    {
        OneDimension,    ///< @brief 1D Texture Type
        TwoDimension,    ///< @brief 2D Texture Type
        ThreeDimension,  ///< @brief 3D Texture Type
        Depth,           ///< @brief Depth Texture.
        Stencil          ///< @brief Stencil Texture.
    };
    
    POOLED(Pools::Resource)
    
    TexturePrivate();
    TexturePrivate(const std::string& name);
    TexturePrivate(const std::string& name, const Image& img);
    
    virtual ~TexturePrivate();
    
    virtual bool isInvalid() const;
    virtual void bind() const;
    virtual void unbind() const;
    
    virtual bool isBinded() const;
    
protected:
    
    void setBinded (bool b) const;
    
protected:
    
    /// @brief Associated Image object to this Texture.
    /// If null, the Texture is probably used to create an Image
    /// from a buffer, or from a FrameBuffer. In this case, every
    /// components of the Texture can be modified.
    /// In the case the Texture object is associated with an Image
    /// object, not every properties can be modified.
    Image image;
    
    mutable bool _isBinded;///< @brief True if Texture is binded.
    
    /// @brief Holds Texture's type.
    Type _mTexType;
    
    /// @brief Holds the number of components.
    int _mComponentsNumber;
    
    /// @brief Holds the MipMap level.
    int _mMipmapLevel;
    
    /// @brief Holds the Width.
    size_t _mWidth;
    
    /// @brief Holds the Height.
    size_t _mHeight;
    
    /// @brief Holds the Storage Type of the data.
    StorageType _mStoreType;
};

class DLL_PUBLIC Texture : public ResourceUser
{
public:
    
    POOLED(Pools::Resource)
    
    Texture();
    Texture(Texture&& rhs);
    Texture(const Texture& rhs);
    explicit Texture(const ResourceUser& rhs);
    
    Texture& operator = (const Texture& rhs);
    
    ~Texture();
    
    bool isInvalid() const;
    
    void bind() const;
    void unbind() const;
    
    bool isBinded() const;
    
    static Texture Null;
    
protected:
    
    std::weak_ptr<TexturePrivate> _mTexture;
};

GRE_END_NAMESPACE

#endif
