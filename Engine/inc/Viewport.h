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

GreBeginNamespace ;

//////////////////////////////////////////////////////////////////////
/// @brief A relative Viewport object. Its real size is updated using
/// 'adaptRealValues' and are accessed using the normal surface
/// properties.
///
//////////////////////////////////////////////////////////////////////
class DLL_PUBLIC Viewport : public Surface
{
public:

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    Viewport ( float l = 0.0f , float t = 0.0f , float w = 1.0f , float h = 1.0f ) ;

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    Viewport ( const Viewport & v ) ;

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    Viewport ( const Surface & s ) ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Updates the Surface's real values from the given values and
    /// the relative values.
    //////////////////////////////////////////////////////////////////////
    void adaptRealValues ( const Surface & values ) ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Updates the Surface's values width and height with given
    /// values.
    //////////////////////////////////////////////////////////////////////
    void adaptRealArea ( int width , int height ) ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Updates the Surface's values left and top with given values.
    //////////////////////////////////////////////////////////////////////
    void adaptRealCorner ( int left , int top ) ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Calculates the Area taken by this viewport.
    //////////////////////////////////////////////////////////////////////
    size_t getArea () const ;

protected:

    /// @brief Relative left value.
    float rl ;

    /// @brief Relative top value.
    float rt ;

    /// @brief Relative width value.
    float rw ;

    /// @brief Relative height value.
    float rh ;
};

GreEndNamespace

#endif
