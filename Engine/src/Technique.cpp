//////////////////////////////////////////////////////////////////////
//
//  Technique.cpp
//  This source file is part of Gre
//		(Gang's Resource Engine)
//
//  Copyright (c) 2015 - 2017 Luk2010
//  Created on 08/02/2017.
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

#include "Technique.h"

GreBeginNamespace

Technique::Technique ( const std::string & name )
: Gre::Resource(name), iProgram ( nullptr )
{
    iActivated = true ;
    iLightingMode = TechniqueLightingMode::AllLights ;
}

Technique::~Technique() noexcept ( false )
{
    
}

const HardwareProgramHolder & Technique::getHardwareProgram () const
{
    GreAutolock ; return iProgram ;
}

void Technique::setHardwareProgram ( const HardwareProgramHolder& program )
{
    GreAutolock ; iProgram = program ;
}

const CameraHolder & Technique::getCamera() const
{
    GreAutolock ; return iCamera ;
}

void Technique::setCamera ( const CameraUser& camera )
{
    GreAutolock ; iCamera = camera.lock() ;
}

const Viewport & Technique::getViewport () const
{
    GreAutolock ; return iViewport ;
}

void Technique::setViewport ( const Viewport& viewport )
{
    GreAutolock ; iViewport = viewport ;
}

bool Technique::isExclusive () const
{
    GreAutolock ; return iExclusive ;
}

void Technique::setExclusive ( bool value )
{
    GreAutolock ; iExclusive = value ;
}

const std::vector < RenderNodeHolder > & Technique::getNodes() const
{
    GreAutolock ; return iRenderedNodes ;
}

bool Technique::hasPreTechniques () const
{
    GreAutolock ; return iPreTechniques.size() > 0 ;
}

const std::vector < TechniqueHolder > & Technique::getPreTechniques () const
{
    GreAutolock ; return iPreTechniques ;
}

bool Technique::hasPostTechniques () const
{
    GreAutolock ; return iPostTechniques.size() > 0 ;
}

const std::vector < TechniqueHolder > & Technique::getPostTechniques () const
{
    GreAutolock ; return iPostTechniques ;
}

bool Technique::isActivated () const
{
    GreAutolock ; return iActivated ;
}

TechniqueLightingMode Technique::getLightingMode () const
{
    GreAutolock ; return iLightingMode ;
}

void Technique::setLightingMode(const Gre::TechniqueLightingMode &lightingmode)
{
    GreAutolock ; iLightingMode = lightingmode ;
}

const RenderFramebufferHolder & Technique::getFramebuffer() const
{
    GreAutolock ; return iFramebuffer ;
}

void Technique::setFramebuffer(const RenderFramebufferHolder &framebuffer)
{
    GreAutolock ; iFramebuffer = framebuffer ;
}

void Technique::onPerLightRendering ( const Light & light )
{
    
}

void Technique::addPreTechnique(const TechniqueHolder &tech)
{
    GreAutolock ; iPreTechniques.push_back(tech) ;
}

void Technique::onUpdateEvent(const Gre::UpdateEvent &e)
{
    
}

void Technique::onWindowSizedEvent(const Gre::WindowSizedEvent &e)
{
    GreAutolock ; iViewport.onBordersChanged({0, 0, e.Width, e.Height});
}

// ---------------------------------------------------------------------------------------------------

TechniqueManager::TechniqueManager ( const std::string & name )
: ResourceManagerBase<Gre::Technique>(name)
{
    
}

TechniqueManager::~TechniqueManager() noexcept ( false )
{
    
}

TechniqueHolder TechniqueManager::load(const std::string &name, const TechniqueHolder& techniqueholder)
{
    GreAutolock ;
    
    if ( !techniqueholder.isInvalid() )
    {
        {
            TechniqueHolder tech = findFirstHolder(name) ;
            if ( !tech.isInvalid() )
            {
#ifdef GreIsDebugMode
                GreDebug("[WARN] Technique '") << name << "' is already registered. Overwriting it." << Gre::gendl ;
#endif
                tech = techniqueholder ;
                return tech ;
            }
        }
        
#ifdef GreIsDebugMode
        GreDebug("[INFO] Added Technique '") << name << "'." << Gre::gendl ;
#endif
        iHolders.add(techniqueholder) ;
        return techniqueholder ;
    }
    
    else
    {
        TechniqueHolder tech = findFirstHolder(name) ;
        
#ifdef GreIsDebugMode
        if ( tech.isInvalid() ) {
            GreDebug("[WARN] Technique '") << name << "' not found." << Gre::gendl ;
        }
#endif
        
        return tech ;
    }
}

GreEndNamespace
