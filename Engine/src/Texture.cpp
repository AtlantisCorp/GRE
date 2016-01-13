//
//  Texture.cpp
//  GRE
//
//  Created by Jacques Tronconi on 03/12/2015.
//
//

#include "Texture.h"

GRE_BEGIN_NAMESPACE

TexturePrivate::TexturePrivate()
: Resource(""), image(Image::Null), _isBinded(false)
{
    
}

TexturePrivate::TexturePrivate(const std::string& name, const Image& img)
: Resource(name), image(img), _isBinded(false)
{
    
}

TexturePrivate::~TexturePrivate()
{
    
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

GRE_END_NAMESPACE
