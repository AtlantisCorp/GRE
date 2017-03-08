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

std::string TextureTypeToString(const TextureType& type)
{
    if ( type == TextureType::Texture1D ) {
        return "TextureType::Texture1D";
    }
    
    if ( type == TextureType::Texture2D ) {
        return "TextureType::Texture2D";
    }
    
    if ( type == TextureType::Texture3D ) {
        return "TextureType::Texture3D";
    }
    
    if ( type == TextureType::CubeMap ) {
        return "TextureType::Cubemap";
    }
    
    return "TextureType::Null";
}

// ---------------------------------------------------------------------------------------------------

Texture::Texture (const std::string& name , const TextureType & type ,
                  const SoftwarePixelBufferHolderList& buffers , bool keepCache )
: Gre::Resource(name)
, iType ( type ) , iPixelBuffers ( buffers ) , iSurface ( { 0, 0, 0, 0 } ) , iBinded ( false )
{
    iParameters [TextureParameter::DepthStencilMode] = Variant ( (int) TextureDepthStencilMode::Depth ) ;
    iParameters [TextureParameter::MipmapBaseLevel] = Variant ( (int) 0 ) ;
    iParameters [TextureParameter::BorderColor] = Variant ( Color(0.0f, 0.0f, 0.0f, 0.0f) ) ;
    iParameters [TextureParameter::CompareFunc] = Variant ( (int) TextureCompareFunc::Always ) ;
    iParameters [TextureParameter::CompareMode] = Variant ( (int) TextureCompareMode::None ) ;
    iParameters [TextureParameter::LODBias] = Variant ( (float) 0.0f ) ;
    iParameters [TextureParameter::MinFilter] = Variant ( (int) TextureMinFilter::MipmapLinear ) ;
    iParameters [TextureParameter::MagFilter] = Variant ( (int) TextureMagFilter::Linear ) ;
    iParameters [TextureParameter::WrapS] = Variant ( (int) TextureWrap::Repeat ) ;
    iParameters [TextureParameter::WrapT] = Variant ( (int) TextureWrap::Repeat ) ;
    iParameters [TextureParameter::WrapR] = Variant ( (int) TextureWrap::Repeat ) ;
}

Texture::~Texture() noexcept(false)
{
    
}

void Texture::bind() const
{
    GreAutolock ; _bind () ;
}

void Texture::unbind() const
{
    GreAutolock ; _unbind () ;
}

void Texture::setPixelBuffer(const SoftwarePixelBufferUser& pixelbuffer)
{
    GreAutolock ;
    
    if ( !iPixelBuffers.empty() ) {
        (*iPixelBuffers.begin()) = pixelbuffer.lock() ;
    } else {
        iPixelBuffers.add ( pixelbuffer.lock() ) ;
    }
    
    if ( !iPixelBuffers.empty() ) {
        if ( !(*iPixelBuffers.begin()).isInvalid() ) {
            iSurface = (*iPixelBuffers.begin()) -> getSurface () ;
            _setBuffer () ;
        }
    }
}

const SoftwarePixelBufferUser Texture::getPixelBuffer() const
{
    GreAutolock ; return SoftwarePixelBufferUser(iPixelBuffers.front());
}

const Surface& Texture::getSurface() const
{
    GreAutolock ; return iSurface;
}

const TextureType & Texture::getType() const
{
    return iType;
}

void Texture::setParameterValue ( const TextureParameter & param , const Variant & value )
{
    GreAutolock ; iParameters [param] = value ;
}

const Variant & Texture::getParameterValue(const Gre::TextureParameter &param) const
{
    GreAutolock ; return iParameters .at(param) ;
}

// ---------------------------------------------------------------------------------------------------

TextureFileLoader::TextureFileLoader()
{
    
}

TextureFileLoader::~TextureFileLoader() noexcept ( false )
{
    
}

// ---------------------------------------------------------------------------------------------------

TextureInternalCreator::TextureInternalCreator ()
{
    
}

TextureInternalCreator::~TextureInternalCreator()
{
    
}

// ---------------------------------------------------------------------------------------------------

TextureManager::TextureManager( const std::string & name )
: SpecializedResourceManager<Gre::Texture, Gre::TextureFileLoader>(name)
{
    
}

TextureManager::~TextureManager() noexcept ( false )
{
    
}

TextureUser TextureManager::load ( const std::string & name , const TextureType & type, const std::string & filepath )
{
    TextureFileLoader* loader = iFindBestLoader ( filepath ) ;
    
    if ( !loader ) {
        GreDebug("[WARN] No TextureFileLoader to load texture '") << name << "'." << Gre::gendl ;
        return TextureUser ( nullptr ) ;
    }
    
    SoftwarePixelBufferHolder buffer = loader -> load ( filepath ) ;
    
    if ( buffer.isInvalid() ) {
        GreDebug("[WARN] Can't load SoftwarePixelBuffer to load texture '") << name << "'." << Gre::gendl ;
        return TextureUser ( nullptr ) ;
    }
    
#ifdef GreIsDebugMode
    GreDebug("[INFO] Loaded SoftwarePixelBuffer from file '") << filepath << "'." << Gre::gendl ;
#endif
    
    return load ( name , type, buffer ) ;
}

TextureUser TextureManager::load(const std::string &name, const Gre::TextureType &type, const SoftwarePixelBufferHolder &buffer)
{
    if ( !buffer.isInvalid () && iCreator )
    {
        Gre::SoftwarePixelBufferHolderList list ;
        list.add(buffer);
        
        Texture* tex = iCreator -> create ( name , type , list ) ;
        if ( !tex ) {
            GreDebug("[WARN] Can't create Texture '") << name << "'." << Gre::gendl ;
            delete tex ; return Gre::TextureUser ( nullptr ) ;
        }
        
        Gre::TextureHolder holder ( tex ) ;
        iHolders.push_back(holder);
        
#ifdef GreIsDebugMode
        GreDebug("[INFO] Created Texture '") << name << "'." << Gre::gendl ;
#endif
        
        return Gre::TextureUser ( holder ) ;
    }
    
    return Gre::TextureUser ( nullptr ) ;
}

void TextureManager::setInternalCreator(Gre::TextureInternalCreator *creator)
{
    GreAutolock ; iCreator = creator ;
}

GreEndNamespace
