//////////////////////////////////////////////////////////////////////
//
//  DarwinGlRendererMain.cpp
//  This source file is part of Gre
//		(Gang's Resource Engine)
//
//  Copyright (c) 2015 - 2016 Luk2010
//  Created on 04/09/2016.
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

#include "ResourceManager.h"
#include "DarwinGlRenderer.h"

using namespace Gre;

extern "C" void* GetPluginName ( void )
{
    return (void*) "Darwin OS OpenGl Renderer";
}

extern "C" void StartPlugin ( void )
{
    // We have to initialize OpenGl library on this Computer.
    
    if ( DarwinGl::InitializeOpenGl() )
    {
        GreDebugPretty() << (const char*) GetPluginName() << " initialized." << Gre::gendl;
    }
    
    // Register a RendererLoader in the RendererManager.
    
    ResourceManager::Get().getRendererManager().getRendererLoaderFactory().
        registers( "DarwinGlRendererLoader", new DarwinGl::DarwinGlRendererLoader() );
}

extern "C" void StopPlugin ( void )
{
    
}

