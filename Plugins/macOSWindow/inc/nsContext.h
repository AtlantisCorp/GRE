//////////////////////////////////////////////////////////////////////
//
//  nsContext.h
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

#ifndef nsContext_h
#define nsContext_h

#include "macOSPlatform.h"
#include <RenderContext.h>

using namespace Gre ;

//////////////////////////////////////////////////////////////////////
/// @brief macOS NSOpenGLContext C++ wrapper.
//////////////////////////////////////////////////////////////////////
class nsContext : public RenderContext
{
public:

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    nsContext ( const std::string& name , const RenderContextInfo& info , id obj , id pix ) ;

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual ~nsContext () noexcept ( false ) ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Bind the RenderContext.
    //////////////////////////////////////////////////////////////////////
    virtual void bind() const;

    //////////////////////////////////////////////////////////////////////
    /// @brief Unbind this Object (make it unusable).
    //////////////////////////////////////////////////////////////////////
    virtual void unbind() const;

    //////////////////////////////////////////////////////////////////////
    /// @brief Send every operations remaining to Hardware, and generally
    /// swap buffers.
    //////////////////////////////////////////////////////////////////////
    virtual void flush() const;

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual Surface getSurface () const ;

    /// @brief
    id nsglContext ;

    /// @brief
    id nsglPixelFormat ;
};

#endif /* nsContext_h */
