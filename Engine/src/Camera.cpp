//////////////////////////////////////////////////////////////////////
//
//  Camera.cpp
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

#include "Camera.h"

GreBeginNamespace

Camera::Camera(const std::string& name)
: Gre::Resource(ResourceIdentifier::New() , name), iMaxVerticalAngle(89.0f)
{
    
}

Camera::Camera(const std::string& name, const Vector3& position, const Vector3& to, const Vector3& up)
: Gre::Resource(ResourceIdentifier::New() , name)
, iMaxVerticalAngle(89.0f)
{
    lookAt(position, to, up) ;
}

Camera::~Camera()
{
    
}

const Vector3& Camera::getPosition() const
{
    GreAutolock ; return iPosition ;
}

const Vector3& Camera::getTarget() const
{
    GreAutolock ; return iTarget ;
}

const Vector3& Camera::getUp() const
{
    GreAutolock ; return iUpwardDirection ;
}

void Camera::lookAt(const Vector3 &origin, const Vector3 &point, const Vector3& up)
{
    Matrix4 view = glm::lookAt(origin, point, up) ;
    iFrustrum.setView(view) ;
    iFrustrum.computePlanes() ;
    
    iPosition = origin ;
    iTarget = point ;
    iUpwardDirection = up ;
}

bool Camera::contains(const Vector3 &object) const
{
    return iFrustrum.contains(object);
}

bool Camera::isVisible(const Gre::BoundingBox &bbox) const
{
    return iFrustrum.intersect(bbox) != IntersectionResult::Outside;
}

const Matrix4 & Camera::getProjectionMatrix() const
{
    return iFrustrum.getPerspective() ;
}

const Matrix4 & Camera::getViewMatrix() const
{
    return iFrustrum.getView() ;
}

// ---------------------------------------------------------------------------------------------------

CameraLoader::CameraLoader ()
{
    
}

CameraLoader::~CameraLoader () noexcept ( false )
{
    
}

// ---------------------------------------------------------------------------------------------------

CameraManager::CameraManager ( const std::string & name )
: SpecializedResourceManager<Camera, CameraLoader> ( name )
{
    // Default Camera is centered at the beginning of the scene, looking straith forward.
    
    CameraHolder camera = CameraHolder ( new Camera("Default") ) ;
    camera -> lookAt({2.0f, 2.0f, -2.0f}, {2.0f, 2.0f, 5.0f});
    
    addListener(EventProceederUser(camera));
    iHolders.push_back(camera);
}

CameraManager::~CameraManager () noexcept ( false )
{
    
}

CameraUser CameraManager::load ( const std::string & name , const CameraOptions & options )
{
    auto loadername = options.find("loader") ;
    if ( loadername != options.end() )
    {
        std::string lname = loadername->second.toString() ;
        CameraLoader* loader = iFindBestLoader( lname ) ;
        
        if ( loader )
        {
            CameraHolder newcam = loader->load(name, options) ;
            iHolders.push_back(newcam) ;
            
            return CameraUser ( newcam ) ;
        }
        
        else
        {
            GreDebug("No 'CameraLoader' found to load Camera '") << name << "' (options.loader='" << lname << "')." << Gre::gendl;
        }
    }
    
    else
    {
        GreDebug("No 'loader' options found in 'CameraOptions' object.") << Gre::gendl ;
    }
    
    return CameraUser ( nullptr ) ;
}

GreEndNamespace
