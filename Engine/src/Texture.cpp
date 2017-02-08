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
#include "HardwareProgram.h"

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

Texture::Texture(const std::string& name)
: Resource(name), iSampler(nullptr)
{
    iType = TextureType::Null;
    iBinded = false;
    iDirty = false;
    iOriginFile = "";
    iSurface = { 0, 0, 0, 0 };
}

Texture::~Texture() noexcept(false)
{
    
}

void Texture::bindWithTextureUnit(int texunit) const
{
    activateTextureUnit(texunit);
    bind();
}

void Texture::bind() const
{
    iBinded = false;
#ifdef GreIsDebugMode
    GreDebugPretty() << "Not implemented." << Gre::gendl;
#endif
}

void Texture::unbind() const
{
    iBinded = false;
#ifdef GreIsDebugMode
    GreDebugPretty() << "Not implemented." << Gre::gendl;
#endif
}

void Texture::activateTextureUnit(int texunit) const
{
#ifdef GreIsDebugMode
    GreDebugPretty() << "Not implemented." << Gre::gendl;
#endif
}

void Texture::setPixelBuffer(const SoftwarePixelBufferUser& pixelbuffer)
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

const SoftwarePixelBufferUser Texture::getPixelBuffer() const
{
    return SoftwarePixelBufferUser(iPixelBuffers.front());
}

SoftwarePixelBufferUser Texture::getPixelBuffer()
{
    return SoftwarePixelBufferUser(iPixelBuffers.front());
}

const std::string& Texture::getOriginalFile() const
{
    return iOriginFile;
}

const Surface& Texture::getSurface() const
{
    return iSurface;
}

Surface& Texture::getSurface()
{
    return iSurface;
}

void Texture::setSurface(const Surface& surface)
{
    iSurface = surface;
    iDirty = true;
}

const TextureType Texture::getType() const
{
    return iType;
}

void Texture::setType(TextureType type)
{
    iType = type;
    iDirty = true;
}

bool Texture::hasHardwareSamplerActivated() const
{
    return iSampler.isInvalid();
}

HardwareSamplerUser Texture::getHardwareSampler()
{
    return iSampler;
}

void Texture::configureProgram(const HardwareProgramHolder &program, int textureunit) const
{
    if ( !program.isInvalid() )
    {
        HardwareProgramVariable iSamplerVar ;
        iSamplerVar.name = std::string("iTexture") + std::to_string(textureunit) ;
        iSamplerVar.type = HdwProgVarType::Integer ;
        iSamplerVar.value.integer = textureunit ;
        program->setVariable(iSamplerVar) ;
    }
}

// ---------------------------------------------------------------------------------------------------

TextureLoader::TextureLoader()
{
    
}

TextureLoader::~TextureLoader()
{
    
}

// ---------------------------------------------------------------------------------------------------

TextureManager::TextureManager()
{
    
}

TextureManager::~TextureManager()
{
    
}

GreEndNamespace
