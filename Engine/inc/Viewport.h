//////////////////////////////////////////////////////////////////////
//
//  Viewport.h
//  This source file is part of Gre
//		(Gang's Resource Engine)
//
//  Copyright (c) 2015 - 2016 Luk2010
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

#ifndef GRE_Viewport_h
#define GRE_Viewport_h

#include "Pools.h"

GreBeginNamespace

//////////////////////////////////////////////////////////////////////
/// @brief A simple Viewport object.
///
/// The Viewport is defined using ratio. This means that for example,
/// if you set a width ratio of 0.5f, when RenderContext calls
/// onBordersChanged() with the RenderContext new width of 400 px, the
/// width given by getSurface().width will be 400*0.5f = 200.
///
//////////////////////////////////////////////////////////////////////
class DLL_PUBLIC Viewport
{
public:
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    Viewport ( float top = 0.0f, float left = 0.0f, float width = 1.0f, float height = 1.0f ) ;
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual ~Viewport() noexcept ( false ) ;
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    Viewport& operator = (const Viewport& rhs);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Updates the Viewport.
    //////////////////////////////////////////////////////////////////////
    virtual void onBordersChanged(const Surface& parentSurface);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the Surface covered by this Viewport.
    //////////////////////////////////////////////////////////////////////
    const Surface& getSurface() const;
    
protected:
    
    /// @brief Width border ratio.
    float _mBorderWidth;
    
    /// @brief Height border ratio.
    float _mBorderHeight;
    
    /// @brief Top margin ratio.
    float _mBorderTop;
    
    /// @brief Left margin ratio.
    float _mBorderLeft;
    
    /// @brief Surface object updated with Viewport::onBordersChanged().
    Surface _mSurface;
};

GreEndNamespace

#endif
