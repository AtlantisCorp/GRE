//////////////////////////////////////////////////////////////////////
//
//  OpenGlTextureManager.cpp
//  This source file is part of Gre
//		(Gang's Resource Engine)
//
//  Copyright (c) 2015 - 2017 Luk2010
//  Created on 12/02/2017.
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

#include "OpenGlRenderer.h"

OpenGlTextureManager::OpenGlTextureManager ( const std::string & name , const Gre::Renderer* renderer )
: Gre::TextureManager(name) , iRenderer(renderer)
{
    if ( !iRenderer ) {
        GreDebug("[WARN] Initializing OpenGlTextureManager without renderer has undefined behaviour.") << Gre::gendl ;
    }
}

OpenGlTextureManager::~OpenGlTextureManager() noexcept ( false )
{
    
}

Gre::TextureUser OpenGlTextureManager::load(const std::string &name, const Gre::TextureType & type ,
                                            const Gre::SoftwarePixelBufferHolder &buffer)
{
    if ( !buffer.isInvalid () && iRenderer )
    {
        // Check if the RenderContext is actually binded.
        iRenderer->getRenderContext()->bind() ;
        
        Gre::SoftwarePixelBufferHolderList list ;
        list.add(buffer);
        
        OpenGlTexture* tex = new OpenGlTexture ( name , type , list ) ;
        if ( !tex -> isGlTextureValid() ) {
            GreDebug("[WARN] Can't create OpenGlTexture '") << name << "'." << Gre::gendl ;
            delete tex ; return Gre::TextureUser ( nullptr ) ;
        }
        
        Gre::TextureHolder holder ( tex ) ;
        iHolders.push_back(holder);
        
        return Gre::TextureUser ( holder ) ;
    }
    
    return Gre::TextureUser ( nullptr ) ;
}

