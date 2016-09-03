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
#include "Renderer.h"

GreBeginNamespace

std::string TextureTypeToString(const TextureType& type)
{
    if ( type == TextureType::OneDimension )
    {
        return "TextureType::1D";
    }
    
    if ( type == TextureType::TwoDimension )
    {
        return "TextureType::2D";
    }
    
    if ( type == TextureType::ThreeDimension )
    {
        return "TextureType::3D";
    }
    
    if ( type == TextureType::CubeMap )
    {
        return "TextureType::Cubemap";
    }
    
    return "TextureType::Null";
}

HdwProgVarType TextureTypeToHdwProgType(const TextureType& type)
{
    if ( type == TextureType::TwoDimension )
    {
        return HdwProgVarType::Sampler2D;
    }
    
    if ( type == TextureType::ThreeDimension )
    {
        return HdwProgVarType::Sampler3D;
    }
    
    return HdwProgVarType::None;
}

// ---------------------------------------------------------------------------------------------------

TexturePrivate::TexturePrivate(const std::string& name)
: Resource(name), iSampler(nullptr)
{
    iType = TextureType::Null;
    iBinded = false;
    iDirty = false;
    iOriginFile = "";
    iSurface = { 0, 0, 0, 0 };
}

TexturePrivate::~TexturePrivate() noexcept(false)
{
    
}

void TexturePrivate::bindWithTextureUnit(int texunit) const
{
    activateTextureUnit(texunit);
    bind();
}

void TexturePrivate::bind() const
{
    iBinded = false;
#ifdef GreIsDebugMode
    GreDebugPretty() << "Not implemented." << std::endl;
#endif
}

void TexturePrivate::unbind() const
{
    iBinded = false;
#ifdef GreIsDebugMode
    GreDebugPretty() << "Not implemented." << std::endl;
#endif
}

void TexturePrivate::activateTextureUnit(int texunit) const
{
#ifdef GreIsDebugMode
    GreDebugPretty() << "Not implemented." << std::endl;
#endif
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

bool TexturePrivate::hasHardwareSamplerActivated() const
{
    return iSampler.isInvalid();
}

HardwareSampler TexturePrivate::getHardwareSampler()
{
    return iSampler;
}

// ---------------------------------------------------------------------------------------------------

Texture::Texture(const TexturePrivate* pointer)
: ResourceUser(pointer)
, SpecializedResourceUser<TexturePrivate>(pointer)
{
    
}

Texture::Texture(const TextureHolder& holder)
: Gre::ResourceUser(holder)
, SpecializedResourceUser<TexturePrivate>(holder)
{
    
}

Texture::Texture(const Texture& user)
: Gre::ResourceUser(user)
, SpecializedResourceUser<TexturePrivate>(user)
{
    
}

Texture::~Texture()
{
    
}

void Texture::bindWithTextureUnit(int texunit) const
{
    auto ptr = lock();
    if ( ptr )
        ptr->bindWithTextureUnit(texunit);
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

void Texture::activateTextureUnit(int texunit) const
{
    auto ptr = lock();
    if ( ptr )
        ptr->activateTextureUnit(texunit);
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

void Texture::setSurface(const Gre::Surface &surface)
{
    auto ptr = lock();
    if ( ptr )
        ptr->setSurface(surface);
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

bool Texture::hasHardwareSamplerActivated() const
{
    auto ptr = lock();
    if ( ptr )
        return ptr->hasHardwareSamplerActivated();
    return false;
}

HardwareSampler Texture::getHardwareSampler()
{
    auto ptr = lock();
    if ( ptr )
        return ptr->getHardwareSampler();
    return HardwareSampler::Null;
}

Texture Texture::Null = Texture(nullptr);

// ---------------------------------------------------------------------------------------------------

TextureLoader::TextureLoader()
{
    
}

TextureLoader::~TextureLoader()
{
    
}

// ---------------------------------------------------------------------------------------------------

TextureManager::TextureManager()
: iRenderer(nullptr)
{
    
}

TextureManager::~TextureManager()
{
    
}

void TextureManager::setRenderer(Gre::RendererPrivate *renderer)
{
    if ( iRenderer )
    {
        clearTextures();
    }
    
    iRenderer = renderer;
}

Texture TextureManager::load(const std::string &name, const std::string &filepath)
{
    RendererHolder rholder ( iRenderer );
    
    if ( !rholder.isInvalid() )
    {
        if ( !name.empty() )
        {
            // Check if this name is not already used.
            
            if ( isLoaded(name) )
            {
                return load(name + '*', filepath);
            }
            
            if ( !filepath.empty() )
            {
                // Iterates through the Loader to find one able to load this file.
                
                auto loadermap = iLoaders.getLoaders();
                
                for ( auto it = loadermap.begin(); it != loadermap.end(); it++ )
                {
                    if ( it->second->isLoadable(filepath) )
                    {
                        // File is loadable by this loader, so use it.
                        
                        TextureHolder holder = rholder->createTexture(name);
                        
                        if ( holder.isInvalid() )
                        {
#ifdef GreIsDebugMode
                            GreDebugPretty() << "Resource '" << name << "' could not be created by Renderer." << std::endl;
#endif
                            return Texture ( nullptr );
                        }
                        
                        holder = it->second->load(holder, filepath);
                        
                        if ( holder.isInvalid() )
                        {
#ifdef GreIsDebugMode
                            GreDebugPretty() << "Resource '" << name << "' could not be loaded." << std::endl;
#endif
                            return Texture ( nullptr );
                        }
                        
                        iTextures.add(holder);
                        
#ifdef GreIsDebugMode
                        GreDebugPretty() << "Resource '" << name << "' registered." << std::endl;
#endif
                        
                        return Texture ( holder );
                    }
                }
                
#ifdef GreIsDebugMode
                GreDebugPretty() << "No loader found for Resource '" << name << "'." << std::endl;
#endif
                return Texture ( nullptr );
            }
            
            else
            {
#ifdef GreIsDebugMode
                GreDebugPretty() << "'filepath' is empty." << std::endl;
#endif
                return Texture ( nullptr );
            }
        }
        
        else
        {
#ifdef GreIsDebugMode
            GreDebugPretty() << "'name' parameter is empty." << std::endl;
#endif
            return Texture ( nullptr );
        }
    }
    
    else
    {
#ifdef GreIsDebugMode
        GreDebugPretty() << "'iRenderer' property is invalid." << std::endl;
#endif
        return Texture ( nullptr );
    }
}

Texture TextureManager::load(TextureHolder &holder)
{
    if ( !holder.isInvalid() )
    {
        if ( isLoaded(holder->getName()) )
        {
#ifdef GreIsDebugMode
            GreDebugPretty() << "Resource Texture '" << holder->getName() << "' has an already registered name." << std::endl;
#endif
            return Texture ( nullptr );
        }
        
        iTextures.add(holder);
        
#ifdef GreIsDebugMode
        GreDebugPretty() << "Resource Texture '" << holder->getName() << "' registered." << std::endl;
#endif
        
        return Texture ( holder );
    }
    
    else
    {
#ifdef GreIsDebugMode
        GreDebugPretty() << "'holder' parameter is invalid." << std::endl;
#endif
        return Texture ( nullptr );
    }
}

bool TextureManager::isLoaded(const std::string &name) const
{
    if ( !name.empty() )
    {
        for ( auto holder : iTextures )
        {
            if ( !holder.isInvalid() )
            {
                if ( holder->getName() == name )
                {
                    return true;
                }
            }
        }
        
#ifdef GreIsDebugMode
        GreDebugPretty() << "Resource Texture '" << name << "' not found." << std::endl;
#endif
        
        return false;
    }
    
    else
    {
#ifdef GreIsDebugMode
        GreDebugPretty() << "'name' parameter is invalid." << std::endl;
#endif
        return false;
    }
}

Texture TextureManager::get(const std::string &name, const std::string &filepath)
{
    if ( !name.empty() )
    {
        for ( auto holder : iTextures )
        {
            if ( !holder.isInvalid() )
            {
                if ( holder->getName() == name )
                {
                    return Texture ( holder );
                }
            }
        }
        
        // If we reach this point, we try to load the 'filepath' file to given 'name' Texture.
        
        return load ( name , filepath );
    }
    
    else
    {
#ifdef GreIsDebugMode
        GreDebugPretty() << "'name' parameter is empty." << std::endl;
#endif
        return Texture ( nullptr );
    }
}

const Texture TextureManager::get(const std::string &name) const
{
    if ( !name.empty() )
    {
        for ( auto holder : iTextures )
        {
            if ( !holder.isInvalid() )
            {
                if ( holder->getName() == name )
                {
                    return Texture ( holder );
                }
            }
        }
        
#ifdef GreIsDebugMode
        GreDebugPretty() << "Resource Texture '" << name << "' not found." << std::endl;
#endif
        
        return Texture ( nullptr );
    }
    
    else
    {
#ifdef GreIsDebugMode
        GreDebugPretty() << "'name' parameter is empty." << std::endl;
#endif
        return Texture ( nullptr );
    }
}

void TextureManager::unload(const std::string &name)
{
    if ( !name.empty() )
    {
        for ( auto it = iTextures.begin(); it != iTextures.end(); it++ )
        {
            auto holder = (*it);
            
            if ( !holder.isInvalid() )
            {
                if ( holder->getName() == name )
                {
                    iTextures.erase(it);
#ifdef GreIsDebugMode
                    GreDebugPretty() << "Resource Texture '" << name << "' unloaded." << std::endl;
#endif
                    return;
                }
            }
        }
        
#ifdef GreIsDebugMode
        GreDebugPretty() << "Resource Texture '" << name << "' not found." << std::endl;
#endif
    }
}

void TextureManager::clearTextures()
{
    iTextures.clear();
}

TextureLoaderFactory& TextureManager::getLoaderFactory()
{
    return iLoaders;
}

const TextureLoaderFactory& TextureManager::getLoaderFactory() const
{
    return iLoaders;
}

void TextureManager::clear()
{
    iTextures.clear();
    iLoaders.clear();
}

GreEndNamespace
