//////////////////////////////////////////////////////////////////////
//
//  Projection.h
//  This source file is part of Gre
//		(Gang's Resource Engine)
//
//  Copyright (c) 2015 - 2017 Luk2010
//  Created on 03/05/2017.
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

#ifndef GRE_PROJECTION_H
#define GRE_PROJECTION_H

#include "Pools.h"

GreBeginNamespace

//////////////////////////////////////////////////////////////////////
/// @brief Defines a Projection Matrix from given parameters.
///
/// If the projection is orthographic , you should use the '::Ortho()'
/// function to cerate your matrix. If the projection is a perspective ,
/// you can use '::Perspective()' to create it.
///
/// Notes for orthographic projection, the 'left' , 'right' , 'top' and
/// 'bottom' parameters acts the same way the ratio.
///
//////////////////////////////////////////////////////////////////////
class DLL_PUBLIC Projection
{
public:

    //////////////////////////////////////////////////////////////////////
    /// @brief Creates an orthographic projection matrix.
    /// If parameters 'near' and 'far' are zero , the orthographic projection
    /// will be usable only to display 2D positions.
    //////////////////////////////////////////////////////////////////////
    static Projection Ortho (float left , float right ,
                             float top , float bottom ,
                             float near = 0.1f , float far = 100.0f );

    //////////////////////////////////////////////////////////////////////
    /// @brief Creates a perspective projection matrix.
    //////////////////////////////////////////////////////////////////////
    static Projection Perspective (float fov , float ratio ,
                                   float near = 0.1f , float far = 100.0f ) ;

public:

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    Projection () ;

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    Projection (float left , float right , float top , float bottom , float near , float far ) ;

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    Projection (float fov , float ratio , float near , float far) ;

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    Projection ( const Projection & rhs ) ;

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    ~Projection () ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Sets orthographic parameters. Notes the 'iOrthographic'
    /// parameter is also set to true. Those are all relatives parameters.
    //////////////////////////////////////////////////////////////////////
    void ortho (float left , float right ,
                float top , float bottom ,
                float near = 0.0f , float far = 0.0f ) ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Sets perspective parameters. Notes 'iOrthographic' is set to
    /// false , and the 'iDirty' flag is set to true. Those are all relatives
    /// parameters.
    //////////////////////////////////////////////////////////////////////
    void perspective (float fov , float ratio ,
                      float near , float far ) ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the parameters for the orthographic projection.
    //////////////////////////////////////////////////////////////////////
    const Vector4 & getOrthographicParameters () const ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the parameters for the perspective projection.
    //////////////////////////////////////////////////////////////////////
    const Vector2 & getPerspectiveParameters () const ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Returns near and far.
    //////////////////////////////////////////////////////////////////////
    const Vector2 & getNearFar () const ;

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    void setNearFar ( float near , float far ) ;

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    bool isOrthographic () const ;

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    bool isPerspective () const ;

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    bool isDirty () const ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the projection matrix.
    //////////////////////////////////////////////////////////////////////
    const Matrix4 & get () const ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Clears and reset the projection matrix.
    //////////////////////////////////////////////////////////////////////
    void clear () ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Updates the parameters of the projection matrix only if
    /// relatives.
    //////////////////////////////////////////////////////////////////////
    void update ( const Surface & surface ) ;

protected:

    /// @brief Relatives Orthographic parameters. (left, right, top, bottom)
    Vector4 iOrtho ;

    /// @brief Perspective parameters. (fov, relative ratio)
    Vector2 iPerspective ;

    /// @brief Near/Far parameters.
    Vector2 iNearfar ;

    /// @brief Boolean true if the projection matrix calcul should be done with
    /// orthographic parameters.
    bool iOrthographic ;

    /// @brief Flag true if the projection parameters have changed. This case , the
    /// projection matrix will be recalculated even if the parameters for 'get()' are
    /// the same.
    mutable bool iDirty ;

    /// @brief Temporary matrix used to cache the projection calculed. That way ,
    /// calculating the projection matrix with the same parameters will be done
    /// more quickly. Note this is useful for a perspective projection matrix , because
    /// the ratio is viewport dependent. For an orthographic matrix , the viewport can
    /// also be used if 'iRelative' is true.
    mutable Matrix4 iProjectionMatrix ;

    /// @brief Temporary surface used to compare to updating surface.
    mutable Surface iLastSurface ;
};

GreEndNamespace

#endif // GRE_PROJECTION_H
