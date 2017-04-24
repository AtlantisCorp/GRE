//////////////////////////////////////////////////////////////////////
//
//  FreeMovingCamera.cpp
//  This source file is part of Gre
//		(Gang's Resource Engine)
//
//  Copyright (c) 2015 - 2017 Luk2010
//  Created on 05/03/2017.
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

#include "FreeMovingCamera.h"

GreBeginNamespace

// ---------------------------------------------------------------------------------------------------
// Gre::FreeMovingCamera

FreeMovingCamera::FreeMovingCamera(const std::string &name)
    : Camera(name)
{
    iControllerFilters.push_back ( EventType::KeyUp ) ;
    iControllerFilters.push_back ( EventType::KeyDown ) ;
    iControllerFilters.push_back ( EventType::CursorMoved ) ;

    iArrowUpState = KeyState::Released ;
    iArrowDownState = KeyState::Released ;
    iArrowLeftState = KeyState::Released ;
    iArrowRightState = KeyState::Released ;
    
    iSensibility = 3.0f ;
    iSensitivity = 0.2f ;
}

FreeMovingCamera::~FreeMovingCamera () noexcept ( false )
{

}

void FreeMovingCamera::setSensibility(float sensibility)
{
    GreAutolock ; iSensibility = sensibility ;
}

float FreeMovingCamera::getSensibility() const
{
    GreAutolock ; return iSensibility ;
}

void FreeMovingCamera::setSensitivity(float sensitivity)
{
    GreAutolock ; iSensitivity = sensitivity ;
}

float FreeMovingCamera::getSensitivity() const
{
    GreAutolock ; return iSensitivity ;
}

void FreeMovingCamera::onCursorMovedEvent(const Gre::CursorMovedEvent &e)
{
    GreAutolock ;

    iAngleTheta += e.DeltaX * iSensitivity ;
    iAnglePhi -= e.DeltaY * iSensitivity ;

    if ( iAnglePhi > 89.0f )
        iAnglePhi = 89.0f ;
    if ( iAnglePhi < -89.0f )
        iAnglePhi = -89.0f ;

    Vector3 forward ;
    forward.x = cosf(glm::radians(iAnglePhi)) * cosf(glm::radians(iAngleTheta)) ;
    forward.z = cosf(glm::radians(iAnglePhi)) * sinf(glm::radians(iAngleTheta)) ;
    forward.y = sinf(glm::radians(iAnglePhi)) ;
    setTarget ( iPosition + forward ) ;
}

void FreeMovingCamera::onKeyDownEvent(const KeyDownEvent &e)
{
    GreAutolock ;

    if ( e.iKey == Key::Up )
        iArrowUpState = KeyState::Pressed ;
    else if ( e.iKey == Key::Down )
        iArrowDownState = KeyState::Pressed ;
    else if ( e.iKey == Key::Left )
        iArrowLeftState = KeyState::Pressed ;
    else if ( e.iKey == Key::Right )
        iArrowRightState = KeyState::Pressed ;
}

void FreeMovingCamera::onKeyUpEvent(const KeyUpEvent &e)
{
    GreAutolock ;

    if ( e.iKey == Key::Up )
        iArrowUpState = KeyState::Released ;
    else if ( e.iKey == Key::Down )
        iArrowDownState = KeyState::Released ;
    else if ( e.iKey == Key::Left )
        iArrowLeftState = KeyState::Released ;
    else if ( e.iKey == Key::Right )
        iArrowRightState = KeyState::Released ;
}

void FreeMovingCamera::onUpdateEvent(const UpdateEvent &e)
{
    GreAutolock ;

    if ( iArrowUpState == KeyState::Pressed )
    {
        Vector3 direction = iTarget - iPosition ;
        Vector3 step = direction * iSensibility * e.elapsedTime.count() ;

        setPosition ( iPosition + step ) ;
        setTarget ( iTarget + step ) ;
    }

    if ( iArrowDownState == KeyState::Pressed )
    {
        Vector3 direction = iTarget - iPosition ;
        Vector3 step = direction * iSensibility * e.elapsedTime.count() ;

        setPosition ( iPosition - step ) ;
        setTarget ( iTarget - step ) ;
    }

    if ( iArrowLeftState == KeyState::Pressed )
    {
        Vector3 direction = iTarget - iPosition ;
        Vector3 directionRight = glm::cross(direction, iUpwardDirection) ;
        Vector3 stepRight = directionRight * iSensibility * e.elapsedTime.count() ;

        setPosition ( iPosition - stepRight ) ;
        setTarget ( iTarget - stepRight ) ;
    }

    if ( iArrowRightState == KeyState::Pressed )
    {
        Vector3 direction = iTarget - iPosition ;
        Vector3 directionRight = glm::cross(direction, iUpwardDirection) ;
        Vector3 stepRight = directionRight * iSensibility * e.elapsedTime.count() ;

        setPosition ( iPosition + stepRight ) ;
        setTarget ( iTarget + stepRight ) ;
    }

    Camera::onUpdateEvent(e);
}

// ---------------------------------------------------------------------------------------------------
// Gre::FreeMovingCameraLoader

FreeMovingCameraLoader::FreeMovingCameraLoader ()
{

}

FreeMovingCameraLoader::~FreeMovingCameraLoader() noexcept ( false )
{

}

ResourceLoader* FreeMovingCameraLoader::clone() const
{
    return new FreeMovingCameraLoader () ;
}

bool FreeMovingCameraLoader::isLoadable(const std::string &filepath) const
{
    return false ;
}

CameraHolder FreeMovingCameraLoader::load(const std::string &name, const CameraOptions &options) const
{
    FreeMovingCamera* ret = new FreeMovingCamera(name) ;
    if ( !ret ) return CameraHolder ( nullptr ) ;

    auto it = options.find ( "Sensibility" ) ;
    if ( it != options.end() )
        ret -> setSensibility(it->second.to<float>()) ;

    it = options.find ( "Sensitivity" ) ;
    if ( it != options.end() )
        ret -> setSensitivity(it->second.to<float>()) ;

    return CameraHolder (ret) ;
}

GreEndNamespace
