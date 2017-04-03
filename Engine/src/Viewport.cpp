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
}

Viewport::Viewport ( const Viewport & rhs )
{
    rl = rhs.rl ; rt = rhs.rt ;
    rw = rhs.rw ; rh = rhs.rh ;
    
    width = rhs.width ; height = rhs.height ;
    left = rhs.left ;   top = rhs.top ;
}

Viewport::Viewport ( const Surface & rhs ) 
{
    rl = 0.0f ; rt = 0.0f ;
    rw = 1.0f ; rh = 1.0f ;
    
    width = rhs.width ; height = rhs.height ;
    left = rhs.left ;   top = rhs.top ;
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

GreEndNamespace
