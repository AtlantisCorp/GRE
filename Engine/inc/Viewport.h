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

#ifndef GRE_Viewport_h
#define GRE_Viewport_h

#include "Pools.h"
#include "Projection.h"
#include "Color.h"

GreBeginNamespace ;

//////////////////////////////////////////////////////////////////////
/// @brief Represents a relative or static Surface where the renderer
/// will draw objects.
///
/// Depending on the 'iRelative' property, 'Viewport::getSurface()' will
/// return either the relative surface depending on the given one, either
/// a surface constructed with the given values.
///
/// The viewport also hold a Projection object , which values depends on
/// the viewport surface values. The projection is updated when the viewport
/// 'makeSurface()' function is called, because this function is called in
/// the 'RenderPass::renderTechnique()'. This let update the projection relative
/// values for each frame.
///
/// A region's surface can be set to act as a stencil buffer test.
///
//////////////////////////////////////////////////////////////////////
class DLL_PUBLIC Viewport
{
public:

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    Viewport ( float l = 0.0f , float t = 0.0f , float w = 1.0f , float h = 1.0f , bool relative = true ) ;

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    Viewport ( const Viewport & v ) ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Constructs a static viewport given a Surface.
    //////////////////////////////////////////////////////////////////////
    Viewport ( const Surface & s ) ;

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    void setProjection ( const Projection & projection ) ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the projection used with this viewport.
    //////////////////////////////////////////////////////////////////////
    const Projection & getProjection () const ;

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    bool regioned () const ;

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    const Surface region () const ;

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    void setRegion ( const Surface & r ) ;

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    float cleardepth () const ;

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    void setClearDepth ( float v ) ;

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    const Color & clearcolor () const ;

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    void setClearColor ( const Color & c ) ;

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    const ClearBuffers & clearbuffers () const ;

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    void setClearBuffers ( const ClearBuffers & buffers ) ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the surface constructing it depending on the 'iRelative'
    /// flag and the surface's data.
    //////////////////////////////////////////////////////////////////////
    Surface makeSurface ( const Surface & surface = { 0 , 0 , 1 , 1 } ) const ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the last surface made by the viewport after calling
    /// 'makeSurface'.
    //////////////////////////////////////////////////////////////////////
    Surface getSurface () const ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Returns 'iRelative'.
    //////////////////////////////////////////////////////////////////////
    bool isRelative () const ;

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    void setRelative ( bool value ) ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Returns true if width or height is zero. 
    //////////////////////////////////////////////////////////////////////
    bool isZero () const ;

protected:

    /// @brief Relative left value.
    float rl ;

    /// @brief Relative top value.
    float rt ;

    /// @brief Relative width value.
    float rw ;

    /// @brief Relative height value.
    float rh ;

    /// @brief True if data should be interpreted as relatives.
    bool iRelative ;

    /// @brief Surface cached since last call.
    mutable Surface iCachedSurface ;

    /// @brief Holds a projection object. This is done as each framebuffer has
    /// a viewport , it may always be done with a projection object. This object
    /// may be held by the viewport , as copying the viewport will create a new
    /// projection object.
    Projection iProjection ;

    /// @brief A region is a portion of the viewport's area that should be draw
    /// to the framebuffer. Notes if this region has a null area , it will be considered
    /// as empty.
    Surface iRegion ;

    /// @brief The depth clearance value. Default is 0.1f .
    float iClearDepth ;

    /// @brief Color cleareance value. Default is LightGrey .
    Color iClearColor ;

    /// @brief Buffers cleared when binding this viewport to the framebuffer.
    ClearBuffers iClearBuffers ;
};

GreEndNamespace

#endif
