//////////////////////////////////////////////////////////////////////
//
//  RenderingQuery.cpp
//  This source file is part of Gre
//		(Gang's Resource Engine)
//
//  Copyright (c) 2015 - 2016 Luk2010
//  Created on 14/11/2016.
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

#include "RenderingQuery.h"

GreBeginNamespace

RenderingQuery::RenderingQuery ( )
: iRenderScene(nullptr)
{
    
}

RenderingQuery::~RenderingQuery() noexcept ( false )
{
    
}

void RenderingQuery::setViewport ( const Viewport & v )
{
    GreAutolock ; iViewport = v ;
}

const Viewport & RenderingQuery::getViewport ( ) const
{
    GreAutolock ; return iViewport ;
}

void RenderingQuery::setRenderScene ( const RenderSceneUser & scene )
{
    GreAutolock ; iRenderScene = scene ;
}

const RenderSceneUser & RenderingQuery::getRenderScene ( ) const
{
    GreAutolock ; return iRenderScene ;
}

void RenderingQuery::setRenderPass(const RenderPassUser &pass)
{
    GreAutolock ; iRenderPass = pass ;
}

void RenderingQuery::setCamera(const CameraUser &camera)
{
    GreAutolock ; iCamera = camera ;
}

void RenderingQuery::setHardwareProgram(const HardwareProgramUser &program)
{
    GreAutolock ; iProgram = program ;
}

void RenderingQuery::setElapsedTime ( const Duration& elapsed )
{
    GreAutolock ; iElapsed = elapsed ;
}

void RenderingQuery::setRenderedNodes ( const std::vector < RenderNodeHolder > & nodes )
{
    GreAutolock ; iRenderedNodes = nodes ;
}

const HardwareProgramUser& RenderingQuery::getHardwareProgram() const
{
    GreAutolock ; return iProgram ;
}

const CameraUser& RenderingQuery::getCamera () const
{
    GreAutolock ; return iCamera ;
}

const std::vector < RenderNodeHolder > & RenderingQuery::getRenderedNodes() const
{
    GreAutolock ; return iRenderedNodes ;
}

GreEndNamespace
