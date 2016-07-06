//////////////////////////////////////////////////////////////////////
//
//  Frustrum.h
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

#ifndef Frustrum_h
#define Frustrum_h

#include "Pools.h"

GreBeginNamespace

//////////////////////////////////////////////////////////////////////
/// @brief Structures representing the 6-planes from the Camera
/// point of view.
///
/// The Camera object should contains and updates one object of this
/// kind. This object can be accessed using Camera::getFrustrum().
///
//////////////////////////////////////////////////////////////////////
class DLL_PUBLIC Frustrum
{
public:
    
    POOLED(Pools::Default)
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    Frustrum();
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    Frustrum(const Matrix4& perspective);
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    Frustrum(const Matrix4& perspective, const Matrix4& view);
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    Frustrum(const Frustrum& rhs);
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual ~Frustrum();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Changes the iPerspective Matrix.
    //////////////////////////////////////////////////////////////////////
    virtual void setPerspective(const Matrix4& perspective);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the iPerspective Matrix.
    //////////////////////////////////////////////////////////////////////
    virtual Matrix4 getPerspective() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Changes the View Matrix.
    //////////////////////////////////////////////////////////////////////
    virtual void setView(const Matrix4& view);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the View Matrix.
    //////////////////////////////////////////////////////////////////////
    virtual Matrix4 getView() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Computes planes from the iPerspective and iView Matrix.
    //////////////////////////////////////////////////////////////////////
    virtual void computePlanes(bool normalize = true);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns one Plane.
    //////////////////////////////////////////////////////////////////////
    virtual Plane getPlane(int i) const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns every Plane (size = 6).
    //////////////////////////////////////////////////////////////////////
    virtual Plane* getPlanes();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns every Plane (size = 6).
    //////////////////////////////////////////////////////////////////////
    virtual const Plane* getPlanes() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns true if the 6 planes have a positive distance with
    /// the point.
    //////////////////////////////////////////////////////////////////////
    virtual bool contains(const Vector3& point) const;
    
protected:
    
    /// @brief The perspective Matrix. ( = Projection matrix )
    Matrix4 iPerspective;
    
    /// @brief The view Matrix.
    Matrix4 iView;
    
    /// @brief The ViewProjection Matrix. (note : this property only change when ::computePlanes() is called)
    Matrix4 iViewProjection;
    
    /// @brief The 6 Planes for the frustrum. (note : this property only change when ::computePlanes() is called)
    Plane iPlanes[6];
};

GreEndNamespace

#endif /* Frustrum_h */
