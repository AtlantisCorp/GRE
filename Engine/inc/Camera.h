//////////////////////////////////////////////////////////////////////
//
//  Camera.h
//  This source file is part of Gre
//		(Gang's Resource Engine)
//
//  Copyright (c) 2015 - 2016 Luk2010
//  Created on 07/05/2016.
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

#ifndef GRE_Camera_h
#define GRE_Camera_h

#include "Variant.h"
#include "Resource.h"
#include "Frustrum.h"
#include "BoundingBox.h"

#include "ResourceLoader.h"
#include "SpecializedResourceManager.h"

GreBeginNamespace

/// @brief Defines some options the user can set when creating a new Camera object.
typedef VariantDictionnary CameraOptions ;

//////////////////////////////////////////////////////////////////////
/// @brief Base Class to every Camera.
///
/// You can define your own Camera behaviour by subclassing the event's
/// routines. Dont' forget to update the iFrustrum property when changing
/// positions, look' position or any other properties.
///
//////////////////////////////////////////////////////////////////////
class DLL_PUBLIC Camera : public Resource
{
public:
    
    POOLED(Pools::Resource)
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    Camera(const std::string& name);
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    Camera(const std::string& name, const Vector3& position, const Vector3& to);
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    Camera(const std::string& name, const Vector3& position, const Radian& angle, const Vector3& direction);
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual ~Camera();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Changes the Camera's absolute position.
    //////////////////////////////////////////////////////////////////////
    virtual void setPosition(const Vector3& position);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the Camera's absolute position.
    //////////////////////////////////////////////////////////////////////
    virtual Vector3 getPosition() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Compute a View Matrix to make this Camera look at the given
    /// point.
    //////////////////////////////////////////////////////////////////////
    virtual void lookAt(const Vector3& position);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Compute a View Matrix to make this Camera look at the given
    /// direction.
    //////////////////////////////////////////////////////////////////////
    virtual void lookAt(const Radian& angle, const Vector3& direction);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Sets up the Frustrum from the Orientation Matrix cached.
    //////////////////////////////////////////////////////////////////////
    virtual void lookAtCache();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns true if an object is inside the Frustrum from this
    /// Camera.
    //////////////////////////////////////////////////////////////////////
    virtual bool contains (const Vector3& object) const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns true if given BoundingBox is visible by this Camera.
    //////////////////////////////////////////////////////////////////////
    virtual bool isVisible ( const BoundingBox& bbox ) const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the Projection Matrix held by the 'iFrustrum'.
    //////////////////////////////////////////////////////////////////////
    virtual const Matrix4 & getProjectionMatrix () const ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the View Matrix held by 'iFrustrum'.
    //////////////////////////////////////////////////////////////////////
    virtual const Matrix4 & getViewMatrix () const ;
    
protected:
    
    /// @brief Absolute position of the Camera.
    Vector3 iPosition;
    
    /// @brief The Frustrum object.
    Frustrum iFrustrum;
    
    /// @brief Max Vertical Angle the Camera can reach. (Default is 89.0f)
    float iMaxVerticalAngle;
    
    /// @brief Defines cached data.
    struct Cache
    {
        Vector3 iLookDirection;
        Matrix4 iOrientation;
    };
    
    /// @brief Holds Cached data.
    Cache iCache;
};

/// @brief SpecializedCountedObjectHolder for Camera.
typedef SpecializedCountedObjectHolder<Camera> CameraHolder;

/// @brief SpecializedResourceHolderList for Camera.
typedef SpecializedResourceHolderList<Camera> CameraHolderList;

/// @brief SpecializedCountedObjectUser for Camera.
typedef SpecializedCountedObjectUser<Camera> CameraUser;

//////////////////////////////////////////////////////////////////////
/// @brief Loads a Camera object.
//////////////////////////////////////////////////////////////////////
class CameraLoader : public ResourceLoader
{
public:
    
    POOLED ( Pools::Loader )
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    CameraLoader () ;
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual ~CameraLoader () noexcept ( false ) ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Loads a Camera and gives it the given CameraOptions.
    //////////////////////////////////////////////////////////////////////
    virtual CameraHolder load ( const std::string & name , const CameraOptions & options ) const = 0 ;
};

//////////////////////////////////////////////////////////////////////
/// @brief Manages Camera objects.
//////////////////////////////////////////////////////////////////////
class CameraManager : public SpecializedResourceManager < Camera , CameraLoader >
{
public:
    
    POOLED ( Pools::Manager )
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    CameraManager ( const std::string & name = "DefaultCameraManager" ) ;
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual ~CameraManager () noexcept ( false ) ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Creates a new Camera using the given CameraOptions.
    //////////////////////////////////////////////////////////////////////
    virtual CameraUser load ( const std::string & name , const CameraOptions & options ) ;
};

/// @brief SpecializedCountedObjectHolder for CameraManager.
typedef SpecializedCountedObjectHolder < CameraManager > CameraManagerHolder ;

GreEndNamespace

#endif
