//////////////////////////////////////////////////////////////////////
//
//  Texture.cpp
//  This source file is part of Gre
//		(Gang's Resource Engine)
//
//  Copyright (c) 2015 - 2016 Luk2010
//  Created on 03/12/2015.
//
//////////////////////////////////////////////////////////////////////
/*
 -----------------------------------------------------------------------------
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 -----------------------------------------------------------------------------
 */

#include "Texture.h"

GreBeginNamespace

TexturePrivate::TexturePrivate(const std::string& name)
: Resource(name)
{
    iType = TextureType::Null;
    iBinded = false;
    iDirty = false;
    iOriginFile = "";
    iSurface = { 0, 0, 0, 0 };
}

TexturePrivate::~TexturePrivate()
{
    
}

void TexturePrivate::bind() const
{
    iBinded = false;
}

void TexturePrivate::unbind() const
{
    iBinded = false;
}

void TexturePrivate::setPixelBuffer(const SoftwarePixelBuffer& pixelbuffer)
{
    if( iPixelBuffers.size() >= 1 )
    {
        iPixelBuffers.front() = pixelbuffer.lock();
        iDirty = true;
    }
        
    else
    {
        iPixelBuffers.add(pixelbuffer.lock());
        iDirty = true;
    }
}

const SoftwarePixelBuffer TexturePrivate::getPixelBuffer() const
{
    return SoftwarePixelBuffer(iPixelBuffers.front());
}

SoftwarePixelBuffer TexturePrivate::getPixelBuffer()
{
    return SoftwarePixelBuffer(iPixelBuffers.front());
}

const std::string& TexturePrivate::getOriginalFile() const
{
    return iOriginFile;
}

const Surface& TexturePrivate::getSurface() const
{
    return iSurface;
}

Surface& TexturePrivate::getSurface()
{
    return iSurface;
}

void TexturePrivate::setSurface(const Surface& surface)
{
    iSurface = surface;
    iDirty = true;
}

const TextureType TexturePrivate::getType() const
{
    return iType;
}

void TexturePrivate::setType(TextureType type)
{
    iType = type;
    iDirty = true;
}

// ---------------------------------------------------------------------------------------------------

Texture::Texture(const TexturePrivate* pointer)
: SpecializedResourceUser<TexturePrivate>(pointer)
{
    
}

Texture::Texture(const TextureHolder& holder)
: SpecializedResourceUser<TexturePrivate>(holder)
{
    
}

Texture::Texture(const Texture& user)
: SpecializedResourceUser<TexturePrivate>(user)
{
    
}

Texture::~Texture()
{
    
}

void Texture::bind() const
{
    auto ptr = lock();
    if(ptr)
        ptr->bind();
}

void Texture::unbind() const
{
    auto ptr = lock();
    if(ptr)
        ptr->unbind();
}

void Texture::setPixelBuffer(const SoftwarePixelBuffer& pixelbuffer)
{
    auto ptr = lock();
    if(ptr)
        ptr->setPixelBuffer(pixelbuffer);
}

const SoftwarePixelBuffer Texture::getPixelBuffer() const
{
    auto ptr = lock();
    if(ptr)
        return ptr->getPixelBuffer();
    return SoftwarePixelBuffer::Null;
}

SoftwarePixelBuffer Texture::getPixelBuffer()
{
    auto ptr = lock();
    if(ptr)
        return ptr->getPixelBuffer();
    return SoftwarePixelBuffer::Null;
}

const std::string Texture::getOriginalFile() const
{
    auto ptr = lock();
    if(ptr)
        return ptr->getOriginalFile();
    return std::string();
}

const Surface& Texture::getSurface() const
{
    auto ptr = lock();
    if(ptr)
        return ptr->getSurface();
    return Surface::Null;
}

Surface& Texture::getSurface()
{
    auto ptr = lock();
    if(ptr)
        return ptr->getSurface();
    return Surface::Null;
}

const TextureType Texture::getType() const
{
    auto ptr = lock();
    if(ptr)
        return ptr->getType();
    return TextureType::Null;
}

void Texture::setType(TextureType textype)
{
    auto ptr = lock();
    if(ptr)
        ptr->setType(textype);
}

Texture Texture::Null = Texture(nullptr);

// ---------------------------------------------------------------------------------------------------

TextureLoader::TextureLoader()
{
    
}

TextureLoader::TextureLoader(const TextureLoader& rhs)
{
    
}

TextureLoader::~TextureLoader()
{
    
}

bool TextureLoader::isTypeSupported(Resource::Type type) const
{
    return type == Resource::Type::Texture;
}

ResourceLoader* TextureLoader::clone() const
{
    return new TextureLoader;
}

TextureHolder TextureLoader::load(Resource::Type type, const std::string& name, const std::string& file) const
{
    TextureHolder holder(new TexturePrivate(name));
    return holder;
}

TextureHolder TextureLoader::load(TextureHolder &to, const std::string &file) const
{
    return to;
}

GreEndNamespace
