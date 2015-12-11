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

class DLL_PUBLIC TexturePrivate : public Resource
{
public:
    
    POOLED(Pools::Resource)
    
    TexturePrivate();
    TexturePrivate(const std::string& name, const Image& img = Image::Null);
    
    virtual ~TexturePrivate();
    
    virtual bool isInvalid() const;
    virtual void bind() const;
    virtual void unbind() const;
    
    virtual bool isBinded() const;
    
protected:
    
    void setBinded (bool b) const;
    
protected:
    
    Image image; ///< @brief ResourceUser to the Image Object.
    mutable bool _isBinded;///< @brief True if Texture is binded.
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
