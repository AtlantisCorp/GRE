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

GreBeginNamespace

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
    
    TexturePrivate(const std::string& name);
    TexturePrivate(const std::string& name, const Image& img);
    
    virtual ~TexturePrivate();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns true if Texture object is loaded in Memory.
    //////////////////////////////////////////////////////////////////////
    bool isLoaded() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief If not loaded yet, change the Texture's type.
    //////////////////////////////////////////////////////////////////////
    void setTextureType(const Type& textureType);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief If not loaded yet, change the component's number.
    //////////////////////////////////////////////////////////////////////
    void setComponentsNumber(int componentsNumber);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief If not loaded yet, change the level of Mipmap this Texture
    /// has effect on.
    //////////////////////////////////////////////////////////////////////
    void setMipmapLevel(int mipmapLevel);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief If not loaded yet, change the Width.
    //////////////////////////////////////////////////////////////////////
    void setWidth(size_t width);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief If not loaded yet, change the Height.
    //////////////////////////////////////////////////////////////////////
    void setHeight(size_t height);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief If not loaded yet, change the Storage Type.
    //////////////////////////////////////////////////////////////////////
    void setStorageType(StorageType storageType);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Loads the Texture in memory, if not yet.
    //////////////////////////////////////////////////////////////////////
    virtual void load();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Sets data in memory. Texture must have already been loaded,
    /// and size of this data must be width*height*components.
    //////////////////////////////////////////////////////////////////////
    virtual void setData(unsigned char* data);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Reset the Texture (creates a new empty Texture).
    //////////////////////////////////////////////////////////////////////
    virtual void reset();
    
    virtual bool isInvalid() const;
    virtual void bind() const;
    virtual void unbind() const;
    
    virtual bool isBinded() const;
    
protected:
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Tells if Texture is loaded.
    //////////////////////////////////////////////////////////////////////
    void setLoaded(bool loaded);
    
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
    
    /// @brief True if loaded in memory.
    bool _mLoaded;
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

GreEndNamespace

#endif
