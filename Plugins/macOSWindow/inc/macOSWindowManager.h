//////////////////////////////////////////////////////////////////////
//
//  macOSWindowManager.h
//  This source file is part of Gre
//		(Gang's Resource Engine)
//
//  Copyright (c) 2015 - 2017 Luk2010
//  Created on 09/02/2017.
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

#ifndef macOSWindowManager_h
#define macOSWindowManager_h

#include <Window.h>
#include "macOSPlatform.h"

using namespace Gre ;

class macOSWindowManager : public WindowManager
{
public:
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    macOSWindowManager ( const std::string& name = "macOSWindowManager" ) ;
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual ~macOSWindowManager () noexcept ( false ) ;
    
protected:
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Implementation specific poll events.
    //////////////////////////////////////////////////////////////////////
    virtual void _pollEvents () const ;
    
public:
    
    /// @brief macOSApplicationDelegate object.
    id iDelegate ;
    
    /// @brief Autorelease pool used.
    id iAutorelease ;
};

/// @brief Sends an empty event.
extern void macOSSendEmptyEvent () ;

/// @brief Initialize AppKit.
extern void initializeAppkit ( macOSWindowManager* manager ) ;

/// @brief Terminate Appkit.
extern void terminateAppkit ( macOSWindowManager* manager ) ;

/// @brief Polls events using the shared application.
extern void macOSPollEvents ( macOSWindowManager* manager ) ;

#endif /* macOSWindowManager_h */
