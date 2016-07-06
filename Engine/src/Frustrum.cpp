//////////////////////////////////////////////////////////////////////
//
//  Frustrum.cpp
//  This source file is part of Gre
//		(Gang's Resource Engine)
//
//  Copyright (c) 2015 - 2016 Luk2010
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
    
}

Frustrum::Frustrum(const Matrix4& perspective)
: iPerspective(perspective), iView(0)
{
    
}

Frustrum::Frustrum(const Matrix4& perspective, const Matrix4& view)
: iPerspective(perspective), iView(view)
{
    
}

Frustrum::Frustrum(const Frustrum& rhs)
: iPerspective(rhs.getPerspective()), iView(rhs.getView())
{
    
}

Frustrum::~Frustrum()
{
    
}

void Frustrum::setPerspective(const Matrix4 &perspective)
{
    iPerspective = perspective;
}

Matrix4 Frustrum::getPerspective() const
{
    return iPerspective;
}

void Frustrum::setView(const Matrix4 &view)
{
    iView = view;
}

Matrix4 Frustrum::getView() const
{
    return iView;
}

void Frustrum::computePlanes(bool normalize)
{
    // See : http://pastebin.com/1Wsk8LgU
    
    iViewProjection = iView * iPerspective;
    
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

GreEndNamespace

