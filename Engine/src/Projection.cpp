//////////////////////////////////////////////////////////////////////
//
//  Projection.cpp
//  This source file is part of Gre
//		(Gang's Resource Engine)
//
//  Copyright (c) 2015 - 2017 Luk2010
//  Created on 04/05/2017.
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

#include "Projection.h"

GreBeginNamespace

// -----------------------------------------------------------------------------

Projection Projection::Ortho ( float left , float right , float top , float bottom , float near , float far )
{
    return Projection ( left , right , top , bottom , near , far ) ;
}

Projection Projection::Perspective ( float fov , float ratio , float near , float far )
{
    return Projection ( fov , ratio , near , far ) ;
}

// -----------------------------------------------------------------------------

Projection::Projection ()
{
    clear () ;
}

Projection::Projection ( float left , float right , float top , float bottom , float near , float far )
{
    ortho ( left , right , top , bottom , near , far ) ;
}

Projection::Projection ( float fov , float ratio , float near , float far )
{
    perspective ( fov , ratio , near , far ) ;
}

Projection::Projection ( const Projection & rhs )
{
    iOrtho = rhs.iOrtho ;
    iPerspective = rhs.iPerspective ;
    iNearfar = rhs.iNearfar ;
    iOrthographic = rhs.iOrthographic ;
    iDirty = rhs.iDirty ;
    iProjectionMatrix = rhs.iProjectionMatrix ;
    iLastSurface = rhs.iLastSurface ;
}

Projection::~Projection ()
{

}

void Projection::ortho ( float left , float right , float top , float bottom , float near , float far )
{
    clear () ;

    //////////////////////////////////////////////////////////////////////
    // Sets orthographic parameters.

    iOrtho = { left , right , top , bottom } ;
    iNearfar = { near , far } ;
    iOrthographic = true ;
    iDirty = true ;

    update ({ 1 , 1 , 1 , 1}) ;
}

void Projection::perspective ( float fov , float ratio , float near , float far )
{
    clear () ;

    //////////////////////////////////////////////////////////////////////
    // Sets perspective parameters.

    iPerspective = { fov , ratio } ;
    iNearfar = { near , far } ;
    iOrthographic = false ;
    iDirty = true ;

    update ({ 1 , 1 , 1 , 1}) ;
}

const Vector4 & Projection::getOrthographicParameters () const
{
    return iOrtho ;
}

const Vector2 & Projection::getPerspectiveParameters () const
{
    return iPerspective ;
}

const Vector2 & Projection::getNearFar () const
{
    return iNearfar ;
}

void Projection::setNearFar ( float near , float far )
{
    iNearfar = { near , far } ;
    iDirty = true ;
}

bool Projection::isOrthographic () const
{
    return iOrthographic ;
}

bool Projection::isPerspective () const
{
    return !iOrthographic ;
}

bool Projection::isDirty () const
{
    return iDirty ;
}

const Matrix4 & Projection::get () const
{
    return iProjectionMatrix ;
}

void Projection::clear ()
{
    iOrtho = { -10.0f , 10.0f , -10.0f , 10.0f } ;
    iPerspective = Vector2 ( glm::radians(70.0f) , 1.0f) ;
    iNearfar = Vector2(0.1f , 1000.0f) ;
    iOrthographic = false ;
    iDirty = false ;
    iProjectionMatrix = Matrix4 ( 1.0f ) ;
    iLastSurface = { 0 , 0 , 0 , 0 } ;
}

void Projection::update ( const Surface & surface )
{
    if ( surface == iLastSurface && !iDirty )
    return ;

    if ( iOrthographic )
    {
        iProjectionMatrix = glm::ortho ( - surface.width  * iOrtho.x ,   surface.width  * iOrtho.y ,
                                           surface.height * iOrtho.z , - surface.height * iOrtho.w ,
                                           iNearfar.x , iNearfar.y ) ;
    }

    else
    {
        iProjectionMatrix = glm::perspective (iPerspective.x ,
                                              iPerspective.y * ((float) surface.width / (float) surface.height) ,
                                              iNearfar.x , iNearfar.y ) ;
    }

    iDirty = false ;
    iLastSurface = surface ;
}

GreEndNamespace
