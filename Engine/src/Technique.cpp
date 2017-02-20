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
: Gre::Resource(name)
{
    iActivated = true ;
}

Technique::~Technique() noexcept ( false )
{
    
}

const std::vector < RenderPassHolder > & Technique::getPasses() const
{
    GreAutolock ; return iPasses ;
}

void Technique::addPass ( const RenderPassHolder& pass )
{
    GreAutolock ; iPasses.push_back ( pass ) ;
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

bool Technique::hasSubtechniques () const
{
    GreAutolock ; return iSubtechniques.size() > 0 ;
}

const std::vector < TechniqueHolder > & Technique::getSubtechniques () const
{
    GreAutolock ; return iSubtechniques ;
}

bool Technique::isActivated () const
{
    return iActivated ;
}

void Technique::onUpdateEvent(const Gre::UpdateEvent &e)
{
    
}

void Technique::onWindowSizedEvent(const Gre::WindowSizedEvent &e)
{
    GreAutolock ; iViewport.onBordersChanged({0, 0, e.Width, e.Height});
}

GreEndNamespace
