//
//  Texture.cpp
//  GRE
//
//  Created by Jacques Tronconi on 03/12/2015.
//
//

#include "Texture.h"

GreBeginNamespace

TexturePrivate::TexturePrivate(const std::string& name)
: Resource(name), image(Image::Null), _isBinded(false)
{
    _mTexType = Type::TwoDimension;
    _mComponentsNumber = 0;
    _mMipmapLevel = 0;
    _mWidth = 0;
    _mHeight = 0;
    _mStoreType = StorageType::UnsignedByte;
    _mLoaded = false;
}

TexturePrivate::TexturePrivate(const std::string& name, const Image& img)
: Resource(name), image(img), _isBinded(false)
{
    
}

TexturePrivate::~TexturePrivate()
{
    
}

void TexturePrivate::setComponentsNumber(int componentsNumber)
{
    _mComponentsNumber = componentsNumber;
}

void TexturePrivate::setHeight(size_t height)
{
    _mHeight = height;
}

void TexturePrivate::setLoaded(bool loaded)
{
    _mLoaded = loaded;
}

void TexturePrivate::setMipmapLevel(int mipmapLevel)
{
    _mMipmapLevel = mipmapLevel;
}

void TexturePrivate::setStorageType(Gre::StorageType storageType)
{
    _mStoreType = storageType;
}

void TexturePrivate::setTextureType(const Gre::TexturePrivate::Type &textureType)
{
    _mTexType = textureType;
}

void TexturePrivate::setWidth(size_t width)
{
    _mWidth = width;
}

bool TexturePrivate::isLoaded() const
{
    return _mLoaded;
}

bool TexturePrivate::isInvalid() const
{
    GreDebugFunctionNotImplemented();
    return true;
}

void TexturePrivate::bind() const
{
    GreDebugFunctionNotImplemented();
}

void TexturePrivate::unbind() const
{
    GreDebugFunctionNotImplemented();
}

bool TexturePrivate::isBinded() const
{
    return _isBinded;
}

void TexturePrivate::setBinded(bool b) const
{
    _isBinded = b;
}

void TexturePrivate::load()
{
    GreDebugFunctionNotImplemented();
}

void TexturePrivate::setData(unsigned char *data)
{
    GreDebugFunctionNotImplemented();
}

void TexturePrivate::reset()
{
    _mTexType = Type::TwoDimension;
    _mComponentsNumber = 0;
    _mMipmapLevel = 0;
    _mWidth = 0;
    _mHeight = 0;
    _mStoreType = StorageType::UnsignedByte;
    _mLoaded = false;
    image = Image::Null;
}

// ---------------------------------------------------------------------------------------------------

Texture::Texture()
{
    
}

Texture::Texture(Texture&& rhs)
: ResourceUser(rhs), _mTexture(std::move(rhs._mTexture))
{
    
}

Texture::Texture(const Texture& rhs)
: ResourceUser(rhs), _mTexture(rhs._mTexture)
{
    
}

Texture::Texture(const ResourceUser& rhs)
: ResourceUser(rhs), _mTexture(std::dynamic_pointer_cast<TexturePrivate>(rhs.lock()))
{
    
}

Texture::~Texture()
{
    
}

Texture& Texture::operator=(const Texture & rhs)
{
    _mTexture = rhs._mTexture;
    return *this;
}

bool Texture::isInvalid() const
{
    auto ptr = _mTexture.lock();
    if(ptr)
        return ptr->isInvalid();
    return true;
}

bool Texture::isBinded() const
{
    auto ptr = _mTexture.lock();
    if(ptr)
        return ptr->isBinded();
    return false;
}

void Texture::bind() const
{
    auto ptr = _mTexture.lock();
    if(ptr)
        ptr->bind();
}

void Texture::unbind() const
{
    auto ptr = _mTexture.lock();
    if(ptr)
        ptr->unbind();
}

Texture Texture::Null = Texture();

GreEndNamespace
