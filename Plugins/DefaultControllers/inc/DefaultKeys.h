//////////////////////////////////////////////////////////////////////
//
//  DefaultKeys.h
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

#ifndef GRE_PLUGIN_DEFAULTKEYS_H
#define GRE_PLUGIN_DEFAULTKEYS_H

#include <Controller.h>
using namespace Gre ;

//////////////////////////////////////////////////////////////////////
/// @brief Moves the nodes with given keys.
//////////////////////////////////////////////////////////////////////
class DefaultKeysController : public Controller
{
public:

    POOLED ( Pools::Controller )

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    DefaultKeysController ( const std::string & name ) ;

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual ~DefaultKeysController () noexcept ( false ) ;

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual Holder < Controller > clone ( const std::string & name ) const ;

protected:

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual void onUpdateEvent ( const UpdateEvent & e ) ;

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual void onKeyDownEvent ( const KeyDownEvent & e ) ;

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual void onKeyUpEvent ( const KeyUpEvent & e ) ;

protected:

    /// @brief Forward key status.
    std::pair < Key , KeyState > iKeyForward ;

    /// @brief Backward key status.
    std::pair < Key , KeyState > iKeyBackward ;

    /// @brief Rightward key status.
    std::pair < Key , KeyState > iKeyRightward ;

    /// @brief Leftward key status.
    std::pair < Key , KeyState > iKeyLeftward ;

    /// @brief Upward key status.
    std::pair < Key , KeyState > iKeyUpward ;

    /// @brief Downward key status.
    std::pair < Key , KeyState > iKeyDownward ;

    /// @brief Step rate in times per second. Default is 60.0f.
    float iStepRate ;

    /// @brief Step distance in direction times per second. Default is 5.0f. As the
    /// node's directions are normally normalized, iStep times the direction should
    /// make its advance of a lenght of iStep.
    float iStep ;

    /// @brief Elapsed time since last update.
    Duration iElapsedTime ;
};

GRE_MAKE_HOLDER( DefaultKeysController ) ;

#endif
