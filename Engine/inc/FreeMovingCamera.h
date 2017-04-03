//////////////////////////////////////////////////////////////////////
//
//  FreeMovingCamera.h
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

#ifndef FREEMOVINGCAMERA_H
#define FREEMOVINGCAMERA_H

#include "Camera.h"

GreBeginNamespace

//////////////////////////////////////////////////////////////////////
/// @brief The FreeMovingCamera class.
///
/// A Classic Moving around Camera, using arrows keys and mouse to
/// control the view. It has sensibility, and use the update events
/// to update its position and target point.
///
//////////////////////////////////////////////////////////////////////
class DLL_PUBLIC FreeMovingCamera : public Camera
{
public:
    
    POOLED ( Pools::Resource )

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    FreeMovingCamera ( const std::string& name = std::string() ) ;

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual ~FreeMovingCamera () noexcept ( false ) ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Changes 'iSensibility'.
    //////////////////////////////////////////////////////////////////////
    virtual void setSensibility ( float sensibility ) ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Returns 'iSensibility'.
    //////////////////////////////////////////////////////////////////////
    virtual float getSensibility () const ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Changes 'iSensitivity'.
    //////////////////////////////////////////////////////////////////////
    virtual void setSensitivity ( float sensitivity ) ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns 'iSensitivity'.
    //////////////////////////////////////////////////////////////////////
    virtual float getSensitivity () const ;

protected:

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual void onCursorMovedEvent( const CursorMovedEvent& e ) ;

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual void onKeyDownEvent( const KeyDownEvent& e ) ;

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual void onKeyUpEvent( const KeyUpEvent& e ) ;

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual void onUpdateEvent(const UpdateEvent& e ) ;

protected:

    /// @brief Arrow 'up' key state.
    KeyState iArrowUpState ;

    /// @brief Arrow 'down' key state.
    KeyState iArrowDownState ;

    /// @brief Arrow 'left' key state.
    KeyState iArrowLeftState ;

    /// @brief Arrow 'right' key state.
    KeyState iArrowRightState ;

    /// @brief Sensibility (speed) of the Camera.
    float iSensibility ;
    
    /// @brief Sensitivity (motion speed) of the Camera.
    float iSensitivity ;
    
    /// @brief Polar Coordinate phi.
    float iAnglePhi ;
    
    /// @brief Polar Coordinate theta.
    float iAngleTheta ;
};

//////////////////////////////////////////////////////////////////////
/// @brief CameraLoader for FreeMovingCamera.
//////////////////////////////////////////////////////////////////////
class DLL_PUBLIC FreeMovingCameraLoader : public CameraLoader
{
public:
    
    POOLED ( Pools::Loader )
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    FreeMovingCameraLoader () ;
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual ~FreeMovingCameraLoader () noexcept ( false ) ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Loads a Camera and gives it the given CameraOptions.
    //////////////////////////////////////////////////////////////////////
    virtual CameraHolder load ( const std::string & name , const CameraOptions & options ) const ;
    
    ////////////////////////////////////////////////////////////////////////
    /// @brief Returns a clone of this object.
    /// Typically, this function is implemented as 'return new MyLoaderClass();',
    /// but you are free to do whatever you want.
    ////////////////////////////////////////////////////////////////////////
    virtual ResourceLoader* clone() const ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns true if the file given is loadable by this loader.
    //////////////////////////////////////////////////////////////////////
    virtual bool isLoadable( const std::string& filepath ) const ;
};

GreEndNamespace

#endif // FREEMOVINGCAMERA_H
