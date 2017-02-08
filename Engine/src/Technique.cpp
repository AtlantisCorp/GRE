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
    
}

Technique::~Technique() noexcept ( false )
{
    
}

const std::vector < RenderPassHolder > & Technique::getPasses() const
{
    GreAutolock ; return iPasses ;
}

const CameraHolder & Technique::getCamera() const
{
    GreAutolock ; return iCamera ;
}

const Viewport & Technique::getViewport () const
{
    GreAutolock ; return iViewport ;
}

bool Technique::isExclusive () const
{
    GreAutolock ; return iExclusive ;
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

GreEndNamespace
