//////////////////////////////////////////////////////////////////////
//
//  OpenGlRendererLoader.cpp
//  This source file is part of Gre
//		(Gang's Resource Engine)
//
//  Copyright (c) 2015 - 2017 Luk2010
//  Created on 11/02/2017.
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

OpenGlRendererLoader::OpenGlRendererLoader ()
{
    
}

OpenGlRendererLoader::~OpenGlRendererLoader() noexcept ( false )
{
    
}

Gre::ResourceLoader* OpenGlRendererLoader::clone () const
{
    return new OpenGlRendererLoader () ;
}

bool OpenGlRendererLoader::isLoadable ( const std::string& filepath ) const
{
    return false ;
}

bool OpenGlRendererLoader::isCompatible ( const Gre::RendererOptions& options ) const
{
    return true ;
}

Gre::RendererHolder OpenGlRendererLoader::load ( const std::string& name , const Gre::RendererOptions& options ) const
{
    // The OpenGlRenderer class does not need any OpenGl particular initialization. We can
    // just allocate a new instance and install its managers. When using the OpenGlRenderer,
    // the renderer should cache the different functions used in order not to have to retrieve
    // them one by one.
    
    OpenGlRenderer* renderer = new OpenGlRenderer ( name , options ) ;
    if ( !renderer ) {
        GreDebug("[WARN] Can't create OpenGlRenderer '") << name << "'." << Gre::gendl ;
        return Gre::RendererHolder ( nullptr ) ;
    }
    
    if ( !renderer->installManagers() ) {
        GreDebug("[WARN] Can't install OpenGlRenderer '") << name << "'." << Gre::gendl ;
        delete renderer ;
        return Gre::RendererHolder ( nullptr ) ;
    }
    
    GreDebug("[INFO] Created and installed OpenGlRenderer '") << name << "'." << Gre::gendl ;
    return Gre::RendererHolder ( renderer ) ;
}
