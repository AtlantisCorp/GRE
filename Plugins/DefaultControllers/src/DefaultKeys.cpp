//////////////////////////////////////////////////////////////////////
//
//  DefaultKeys.cpp
//  This source file is part of Gre
//		(Gang's Resource Engine)
//
//  Copyright (c) 2015 - 2017 Luk2010
//  Created on 13/05/2017.
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

#include "DefaultKeys.h"

DefaultKeysController::DefaultKeysController ( const std::string & name ) : Gre::Controller ( name )
{
    iKeyForward = { Key::W , KeyState::Released } ;
    iKeyBackward = { Key::S , KeyState::Released } ;
    iKeyRightward = { Key::D , KeyState::Released } ;
    iKeyLeftward = { Key::A , KeyState::Released } ;
    iKeyUpward = { Key::R , KeyState::Released } ;
    iKeyDownward = { Key::F , KeyState::Released } ;

    iStepRate = 60.0f ;
    iStep = 5.0f ;
}

DefaultKeysController::~DefaultKeysController () noexcept ( false )
{

}

Holder < Controller > DefaultKeysController::clone ( const std::string & name ) const
{
    return Holder < Controller > ( new DefaultKeysController(name) ) ;
}

void DefaultKeysController::onUpdateEvent ( const UpdateEvent & e )
{
    GreAutolock ;

    //////////////////////////////////////////////////////////////////////
    // We must check if the elapsed time is >= to 1/iStepRate.

    float stepseconds = 1.0f / iStepRate ;
    iElapsedTime = iElapsedTime + e.elapsedTime ;

    if ( iElapsedTime.count() < stepseconds )
    return ;

    //////////////////////////////////////////////////////////////////////
    // We get here , so advance the nodes of the given distance. This distance
    // is calculated with the elapsed time.

    float distance = iElapsedTime.count() * iStep ;
    iElapsedTime = Duration(0.0f) ;

    //////////////////////////////////////////////////////////////////////
    // Updates every nodes.

    for ( auto node : iControlled )
    {
        const Vector3 position = node -> getPosition () ;
        const Vector3 target = node -> getTarget () ;

        Vector3 direction = (node -> getDirection () * distance) ;
        Vector3 rightdirection = (node -> getRightDirection () * distance) ;
        Vector3 updirection = (node -> getUpwardDirection () * distance) ;

        if ( glm::length(direction) <= 0.001f )
        continue ;

        if ( iKeyForward.second == KeyState::Pressed )
        {
            node -> setPosition ( position + direction ) ;
            node -> look ( target + direction ) ;
        }

        if ( iKeyBackward.second == KeyState::Pressed )
        {
            node -> setPosition ( position - direction ) ;
            node -> look ( target - direction ) ;
        }

        if ( glm::length(rightdirection) <= 0.001f )
        continue ;

        if ( iKeyRightward.second == KeyState::Pressed )
        {
            node -> setPosition ( position + rightdirection ) ;
            node -> look ( target + rightdirection ) ;
        }

        if ( iKeyLeftward.second == KeyState::Pressed )
        {
            node -> setPosition ( position - rightdirection ) ;
            node -> look ( target - rightdirection ) ;
        }

        if ( glm::length(updirection) <= 0.001f )
        continue ;

        if ( iKeyUpward.second == KeyState::Pressed )
        {
            node -> setPosition ( position + updirection ) ;
            node -> look ( target + updirection ) ;
        }

        if ( iKeyDownward.second == KeyState::Pressed )
        {
            node -> setPosition ( position - updirection ) ;
            node -> look ( target - updirection ) ;
        }
    }
}

void DefaultKeysController::onKeyDownEvent ( const KeyDownEvent & e )
{
    GreAutolock ;

    //////////////////////////////////////////////////////////////////////
    // Checks every keys if is pressed.

    if ( e.iKey == iKeyForward.first )
    iKeyForward.second = KeyState::Pressed ;

    else if ( e.iKey == iKeyBackward.first )
    iKeyBackward.second = KeyState::Pressed ;

    else if ( e.iKey == iKeyLeftward.first )
    iKeyLeftward.second = KeyState::Pressed ;

    else if ( e.iKey == iKeyRightward.first )
    iKeyRightward.second = KeyState::Pressed ;

    else if ( e.iKey == iKeyUpward.first )
    iKeyUpward.second = KeyState::Pressed ;

    else if ( e.iKey == iKeyDownward.first )
    iKeyDownward.second = KeyState::Pressed ;
}

void DefaultKeysController::onKeyUpEvent ( const KeyUpEvent & e )
{
    GreAutolock ;

    //////////////////////////////////////////////////////////////////////
    // Checks every keys if is released.

    if ( e.iKey == iKeyForward.first )
    iKeyForward.second = KeyState::Released ;

    else if ( e.iKey == iKeyBackward.first )
    iKeyBackward.second = KeyState::Released ;

    else if ( e.iKey == iKeyLeftward.first )
    iKeyLeftward.second = KeyState::Released ;

    else if ( e.iKey == iKeyRightward.first )
    iKeyRightward.second = KeyState::Released ;

    else if ( e.iKey == iKeyUpward.first )
    iKeyUpward.second = KeyState::Released ;

    else if ( e.iKey == iKeyDownward.first )
    iKeyDownward.second = KeyState::Released ;
}
