//////////////////////////////////////////////////////////////////////
//
//  ShadowMapping.cpp
//  This source file is part of Gre
//		(Gang's Resource Engine)
//
//  Copyright (c) 2015 - 2017 Luk2010
//  Created on 09/03/2017.
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

#include "ShadowMapping.h"
#include "ResourceManager.h"

GreBeginNamespace

ShadowMapping::ShadowMapping ( const std::string & name )
: Gre::Technique(name)
{
    iLightingMode = TechniqueLightingMode::PerLight ;
    //iClearBuffers.reset() ;
    //iClearBuffers.set((int)ClearBuffer::Depth) ;
}

ShadowMapping::~ShadowMapping() noexcept ( false )
{

}

bool ShadowMapping::_initialize()
{
    bool result = true ;
/*
    if ( iFramebuffer.isInvalid() )
    {
        iFramebuffer = ResourceManager::Get()->getFramebufferManager()->load(getName() + "/Framebuffer", ResourceLoaderOptions()) ;
        if ( iFramebuffer.isInvalid() ) result = false ;
    }

    if ( iProgram.isInvalid() )
    {
        iProgram = ResourceManager::Get()->getHardwareProgramManager()->createHardwareProgramFromFiles
        (getName() + "/Program",
         "Programs/GLSL/ShadowMapping.vert",
         "Programs/GLSL/ShadowMapping.frag").lock();

        if ( iProgram.isInvalid() ) result = false ;
    }
    */
    //iViewport = Viewport::FromFixedSurface ({ 0, 0, 1024, 1024 }) ;
    return result ;
}

bool ShadowMapping::onPerLightRendering(const Gre::LightHolder &light)
{
    // We must use the light shadow texture to render the depth values.
/*
    if ( !iFramebuffer.isInvalid() )
    {
        TextureHolder shadowtex = light->getShadowTexture () ;
        if ( !shadowtex.isInvalid() ) {
            iFramebuffer -> setAttachement(RenderFramebufferAttachement::Depth, shadowtex) ;
            //iCamera = light->getShadowCamera() ;
            return true ;
        }
    }
*/
    return false ;
}

GreEndNamespace
