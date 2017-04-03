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

OpenGlTextureCreator::OpenGlTextureCreator ( const Gre::Renderer* renderer )
: iRenderer(renderer)
{
    if ( !iRenderer ) {
        GreDebug("[WARN] Initializing OpenGlTextureManager without renderer has undefined behaviour.") << Gre::gendl ;
    }
}

OpenGlTextureCreator::~OpenGlTextureCreator()
{
    
}

Gre::Texture* OpenGlTextureCreator::load(const std::string & name ,
                                           const Gre::SoftwarePixelBufferHolderList & buffers ,
                                           const Gre::TextureType & type ,
                                           const Gre::ResourceLoaderOptions & ops ) const
{
    if ( iRenderer )
    {
        // Check if the RenderContext is actually binded.
        iRenderer->getRenderContext()->bind() ;
        
        OpenGlTexture* tex = new OpenGlTexture ( name , type , buffers ) ;
        if ( !tex -> isGlTextureValid() ) {
            GreDebug("[WARN] Can't create OpenGlTexture '") << name << "'." << Gre::gendl ;
            delete tex ; return nullptr ;
        }
        
        return tex ;
    }
    
    return nullptr ;
}

