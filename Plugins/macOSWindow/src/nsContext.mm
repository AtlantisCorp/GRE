//////////////////////////////////////////////////////////////////////
//
//  nsContext.mm
//  This source file is part of Gre
//		(Gang's Resource Engine)
//
//  Copyright (c) 2015 - 2017 Luk2010
//  Created on 10/02/2017.
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

#include "nsContext.h"

// ---------------------------------------------------------------------------------------------------------
// nsContext C++

nsContext::nsContext ( const std::string & name , const RenderContextInfo& info , id obj , id pix )
: Gre::RenderContext(name, info) , nsglContext(obj) , nsglPixelFormat(pix)
{

}

nsContext::~nsContext() noexcept ( false )
{
    [nsglPixelFormat release] ;
    nsglPixelFormat = nil ;

    [nsglContext release] ;
    nsglContext = nil ;
}

void nsContext::bind() const
{
    [nsglContext makeCurrentContext];
    iIsBinded = true ;
}

void nsContext::unbind() const
{
    [NSOpenGLContext clearCurrentContext];
    iIsBinded = false ;
}

void nsContext::flush() const
{
    [nsglContext flushBuffer];
}

Surface nsContext::getSurface() const
{
    NSView* view = [nsglContext view] ;

    if ( view == nil )
    return { 0 , 0 , 0 , 0 } ;

    NSRect frame = [view frame] ;
    return { (int) frame.origin.x , (int) frame.origin.y , (int) frame.size.width , (int) frame.size.height } ;
}
