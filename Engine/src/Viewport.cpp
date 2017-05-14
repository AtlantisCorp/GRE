//////////////////////////////////////////////////////////////////////
//
//  Viewport.h
//  This source file is part of Gre
//		(Gang's Resource Engine)
//
//  Copyright (c) 2015 - 2017 Luk2010
//  Created on 09/03/2016.
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

#include "Viewport.h"

GreBeginNamespace

Viewport::Viewport( float l , float t , float w , float h )
{
    rl = l ; rt = t ;
    rw = w ; rh = h ;

    width = height = left = top = 0 ;
    iRegion = { 0 , 0 , 0 , 0 } ;
    iClearDepth = 1.0f ;
    iClearColor = Color ( 0.5f , 0.5f, 0.5f ) ;
    iClearBuffers.set((int)ClearBuffer::Color, true);
    iClearBuffers.set((int)ClearBuffer::Depth, true);
    iClearBuffers.set((int)ClearBuffer::Stencil, true);
}

Viewport::Viewport ( const Viewport & rhs )
{
    rl = rhs.rl ; rt = rhs.rt ;
    rw = rhs.rw ; rh = rhs.rh ;

    width = rhs.width ; height = rhs.height ;
    left = rhs.left ;   top = rhs.top ;

    iRegion = rhs.iRegion ;
    iClearDepth = rhs.iClearDepth ;
    iClearColor = rhs.iClearColor ;
    iClearBuffers = rhs.iClearBuffers ;
}

Viewport::Viewport ( const Surface & rhs )
{
    rl = 0.0f ; rt = 0.0f ;
    rw = 1.0f ; rh = 1.0f ;

    width = rhs.width ; height = rhs.height ;
    left = rhs.left ;   top = rhs.top ;

    iRegion = { 0 , 0 , 0 , 0 } ;
    iClearDepth = 1.0f ;
    iClearColor = Color ( 0.5f , 0.5f, 0.5f ) ;
    iClearBuffers.set((int)ClearBuffer::Color, true);
    iClearBuffers.set((int)ClearBuffer::Depth, true);
    iClearBuffers.set((int)ClearBuffer::Stencil, true);
}

void Viewport::adaptRealValues ( const Surface & rhs )
{
    left   = rhs.left   * rl ;
    top    = rhs.top    * rt ;
    width  = rhs.width  * rw ;
    height = rhs.height * rh ;
}

void Viewport::adaptRealArea ( int _width , int _height )
{
    width  = _width  * rw ;
    height = _height * rh ;
}

void Viewport::adaptRealCorner(int _left, int _top)
{
    left = _left * rl ;
    top  = _top  * rt ;
}

size_t Viewport::getArea() const
{
    return width * height ;
}

void Viewport::setProjection ( const Projection & projection )
{
    iProjection = projection ;
}

const Projection & Viewport::getProjection () const
{
    return iProjection ;
}

bool Viewport::regioned () const
{
    return iRegion.left + iRegion.top + iRegion.width + iRegion.height != 0 ;
}

const Surface Viewport::region () const
{
    return { iRegion.left * left , iRegion.top * top ,
             iRegion.width * width , iRegion.height * height } ;
}

void Viewport::setRegion ( const Surface & r )
{
    iRegion = r ;
}

float Viewport::cleardepth () const
{
    return iClearDepth ;
}

void Viewport::setClearDepth ( float d )
{
    iClearDepth = d ;
}

const Color & Viewport::clearcolor () const
{
    return iClearColor ;
}

void Viewport::setClearColor ( const Color & color )
{
    iClearColor = color ;
}

const ClearBuffers & Viewport::clearbuffers () const
{
    return iClearBuffers ;
}

void Viewport::setClearBuffers ( const ClearBuffers & buffers )
{
    iClearBuffers = buffers ;
}

void Viewport::update ( const Surface & surface )
{
    left   = rl * surface.width ;
    top    = rt * surface.height ;
    width  = rw * surface.width ;
    height = rh * surface.height ;
    iProjection.update ( *this ) ;
}

GreEndNamespace
