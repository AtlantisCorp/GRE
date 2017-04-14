//////////////////////////////////////////////////////////////////////
//
//  Frustrum.cpp
//  This source file is part of Gre
//		(Gang's Resource Engine)
//
//  Copyright (c) 2015 - 2017 Luk2010
//  Created on 05/07/2016.
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

#include "Frustrum.h"

GreBeginNamespace

Frustrum::Frustrum()
: iPerspective(0), iView(0)
{
    iFovy   = 45.0f ;
    iAspect = 4.0f/3.0f ;
    iNear   = 0.1f ;
    iFar    = 100.0f ;
    
    float max = std::numeric_limits<float>::max() ;
    iOrthoSurface.bottom = - max ;
    iOrthoSurface.top    =   max ;
    iOrthoSurface.left   = - max ;
    iOrthoSurface.right  =   max ;
    
    iProjectionType = ProjectionType::Perspective ;
    iView = glm::mat4 () ;
    
    computePlanes() ;
}

Frustrum::Frustrum ( const OrthogonalVolume& orthosurface , float near , float far )
{
    iFovy = iAspect = 0.0f ;
    iNear = near ;
    iFar  = far ;
    iOrthoSurface = orthosurface ;
    iProjectionType = ProjectionType::Orthogonal ;
    iView = glm::mat4 () ;
    computePlanes() ;
}

Frustrum::Frustrum ( float fovy , float aspect , float near , float far )
{
    iFovy = fovy ;
    iAspect = aspect ;
    iNear = near ;
    iFar = far ;
    iOrthoSurface = { 0.0f , 0.0f , 0.0f , 0.0f } ;
    iProjectionType = ProjectionType::Perspective ;
    iView = glm::mat4 () ;
    computePlanes() ;
}

Frustrum::Frustrum(const Frustrum& rhs)
: iPerspective(0) , iView(0)
{
    iFovy = rhs.iFovy ;
    iAspect = rhs.iAspect ;
    iNear = rhs.iNear ;
    iFar = rhs.iFar ;
    iOrthoSurface = rhs.iOrthoSurface ;
    iProjectionType = rhs.iProjectionType ;
    iView = rhs.iView ;
    computePlanes() ;
}

Frustrum::~Frustrum()
{
    
}

void Frustrum::setPerspective(const Matrix4 &perspective)
{
    iPerspective = perspective;
}

const Matrix4 & Frustrum::getPerspective() const
{
    return iPerspective;
}

void Frustrum::setView(const Matrix4 &view)
{
    iView = view;
}

const Matrix4 & Frustrum::getView() const
{
    return iView;
}

void Frustrum::computePlanes(bool normalize)
{
    // Compute also the projection matrix.
    
    if ( iProjectionType == ProjectionType::Orthogonal ) {
        iPerspective = glm::ortho (iOrthoSurface.bottom , iOrthoSurface.top ,
                                   iOrthoSurface.left , iOrthoSurface.right ,
                                   iNear , iFar ) ;
    } else if ( iProjectionType == ProjectionType::Perspective ) {
        iPerspective = glm::perspective(iFovy, iAspect, iNear, iFar) ;
    }
    
    // See : http://pastebin.com/1Wsk8LgU
    
    iViewProjection = iPerspective * iView;
    
    // create non-normalized clipping planes
    iPlanes[0] = Plane( iViewProjection[3] + iViewProjection[0] );       // left
    iPlanes[1] = Plane( iViewProjection[3] - iViewProjection[0] );       // right
    iPlanes[2] = Plane( iViewProjection[3] - iViewProjection[1] );       // top
    iPlanes[3] = Plane( iViewProjection[3] + iViewProjection[1] );       // bottom
    iPlanes[4] = Plane( iViewProjection[3] + iViewProjection[2] );       // near
    iPlanes[5] = Plane( iViewProjection[3] - iViewProjection[2] );       // far
    
    // normalize the plane equations, if requested
    if (normalize)
    {
        glm::normalize(iPlanes[0]);
        glm::normalize(iPlanes[1]);
        glm::normalize(iPlanes[2]);
        glm::normalize(iPlanes[3]);
        glm::normalize(iPlanes[4]);
        glm::normalize(iPlanes[5]);
    }
}

Plane Frustrum::getPlane(int i) const
{
    return iPlanes[i];
}

Plane* Frustrum::getPlanes()
{
    return iPlanes;
}

const Plane* Frustrum::getPlanes() const
{
    return iPlanes;
}

bool Frustrum::contains(const Vector3 &pt) const
{
    for (int i = 0; i < 6; i++)
    {
        // See https://msdn.microsoft.com/en-us/library/windows/desktop/bb205387(v=vs.85).aspx for formula.
        
        const Plane& p = iPlanes[i];
        float value = p.x * pt.x + p.y * pt.y + p.z * pt.z + p.w * 1.0f;
        
        if(value < 0.0f)
        {
            return false;
        }
    }
    
    return true;
}

IntersectionResult Frustrum::intersect(const Gre::BoundingBox &bbox) const
{
    //////////////////////////////////////////////////////////////////////
    // Bounding Box Frustrum Culling algorithm.
    // http://www.lighthouse3d.com/tutorials/view-frustum-culling/geometric-approach-testing-boxes-ii/
    
    return getBoundingBox().intersect(bbox);
}

const ProjectionType & Frustrum::getProjectionType() const
{
    return iProjectionType ;
}

void Frustrum::setProjectionType(const Gre::ProjectionType &type)
{
    if ( type != iProjectionType )
    {
        iProjectionType = type ;
        computePlanes() ;
    }
}

float Frustrum::getNear() const
{
    return iNear ;
}

void Frustrum::setNear(float near)
{
    iNear = near ; computePlanes() ;
}

float Frustrum::getFar() const
{
    return iFar ;
}

void Frustrum::setFar(float far)
{
    iFar = far ; computePlanes() ;
}

float Frustrum::getAspectRatio() const
{
    return iAspect ;
}

void Frustrum::setAspectRatio(float v)
{
    iAspect = v ;
}

IntersectionResult Frustrum::intersect ( float radius , const Vector3 & center ) const
{
    float fDistance;
    
    //////////////////////////////////////////////////////////////////////
    // Calculate our distances to each of the planes
    
    for(int i = 0; i < 6; ++i)
    {
        //////////////////////////////////////////////////////////////////////
        // Find the distance to this plane
        
        fDistance = glm::dot( Vector3(iPlanes[i]) , center ) + iPlanes[i].w ;
        
        //////////////////////////////////////////////////////////////////////
        // If this distance is < -sphere.radius, we are outside.
        
        if( fDistance < -radius )
        return IntersectionResult::Outside ;
        
        //////////////////////////////////////////////////////////////////////
        // Else if the distance is between +- radius, then we intersect
        
        if( (float)fabs(fDistance) < radius )
        return IntersectionResult::Between ;
    }
    
    return IntersectionResult::Inside ;
}

IntersectionResult Frustrum::intersect(const Gre::Frustrum &rhs) const
{
    //////////////////////////////////////////////////////////////////////
    // Calculates the bounding box and intersect it with the given frustrum.
    
    return intersect ( rhs.getBoundingBox() ) ;
}

BoundingBox Frustrum::getBoundingBox() const
{
    //////////////////////////////////////////////////////////////////////
    // Calculates the bounding box using the method described here :
    // https://www.gamedev.net/topic/321595-how-to-get-the-frustum-bounding-box/
    
    Matrix4 invmat = glm::inverse( getPerspective() * getView() ) ;
    
    Vector3 p1 = Vector3 ( Vector4( -1 , -1 , -1 , 1 ) * invmat ) ;
    Vector3 p2 = Vector3 ( Vector4( 1 , -1 , -1 , 1 ) * invmat ) ;
    Vector3 p3 = Vector3 ( Vector4( 1 , 1 , -1 , 1 ) * invmat ) ;
    Vector3 p4 = Vector3 ( Vector4( -1 , 1 , -1 , 1 ) * invmat ) ;
    
    Vector3 p5 = Vector3 ( Vector4( -1 , -1 , 1 , 1 ) * invmat ) ;
    Vector3 p6 = Vector3 ( Vector4( 1 , -1 , 1 , 1 ) * invmat ) ;
    Vector3 p7 = Vector3 ( Vector4( 1 , 1 , 1 , 1 ) * invmat ) ;
    Vector3 p8 = Vector3 ( Vector4( -1 , 1 , 1 , 1 ) * invmat ) ;
    
    BoundingBox result ( p1 , p2 ) ;
    result.add(p3) ; result.add(p4) ;
    result.add(p5) ; result.add(p6) ;
    result.add(p7) ; result.add(p8) ;
    
    return result ;
}

GreEndNamespace

