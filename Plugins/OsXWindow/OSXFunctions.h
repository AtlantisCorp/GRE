//////////////////////////////////////////////////////////////////////
//
//  OSXFunctions.h
//  This source file is part of Gre
//		(Gang's Resource Engine)
//
//  Copyright (c) 2015 - 2016 Luk2010
//  Created on 10/12/2015.
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

#ifndef GRE_OSXFunctions_h
#define GRE_OSXFunctions_h

#ifndef OSXFUNCTIONS_OBJC

#   include <stdio.h>
#   include <stdlib.h>
#   include <stddef.h>
#   include <string.h>
#
#   define BUILDING_DLL
#   include "ResourceManager.h"

using namespace Gre;

#endif // OSXFUNCTIONS_OBJC

#ifdef OSXFUNCTIONS_OBJC
#   import <Foundation/Foundation.h>
#   import <Cocoa/Cocoa.h>
#endif

#include <OpenGL/OpenGL.h>

#include "MacEventQueue.h"

#ifndef OSXFUNCTIONS_OBJC

/// @brief The base function that must be called in StartPlugin.
extern "C" DLL_PUBLIC void NsLoadPluginApp();

/// @brief Creates a new window and returns it in given CFTypeRef pointer.
extern "C" DLL_PUBLIC void NsCreateWindow(CFTypeRef* crefreturn, int x0,int y0,int wid,int hei);

/// @brief Destroys the Window and make it desappear.
extern "C" DLL_PUBLIC void NsDestroyWindow(CFTypeRef* nsWindow);

/// @brief Returns true if Window is closed.
extern "C" DLL_PUBLIC bool NsIsWindowClosed(CFTypeRef* nsWindow);

/// @brief Sets a new title for the Window.
extern "C" DLL_PUBLIC void NsSetWindowTitle(CFTypeRef* nsWindow, const char* cTitle);

/// @brief Swap the Buffers.
extern "C" DLL_PUBLIC void NsWindowSwapBuffers(CFTypeRef* nsWindow);

/// @brief Returns the Width and the Height of the Window.
extern "C" DLL_PUBLIC void NsGetWindowSize(CFTypeRef* nsWindow, int* retWidth, int* retHeight);

/// @brief Changes the RenderContext from the Window.
extern "C" DLL_PUBLIC void NsWindowSetRenderContext(CFTypeRef* nsWindow, CGLContextObj ctxt);

/// @brief Calls [window isVisible] .
extern "C" DLL_PUBLIC bool NsWindowPropertyIsVisible ( const CFTypeRef* window );

/// @brief Calls [window isOnActiveSpace] .
extern "C" DLL_PUBLIC bool NsWindowPropertyIsOnActiveSpace ( const CFTypeRef* window );

/// @brief Sets the EventQueue that should be used by the Window to send event.
extern "C" DLL_PUBLIC void NsWindowSetEventQueue ( CFTypeRef* window , WindowEventQueue* queue ) ;

/// @brief Run the shared application.
extern "C" DLL_PUBLIC void NSGlobalApplicationRun ( ) ;

/// @brief Terminate the shared application.
extern "C" DLL_PUBLIC void NSGlobalApplicationTerminate ( ) ;

// ---------------------------------------------------------------------------------------

// OSXImage API 1

// ----------------------------------------------------------------------------------------------------------

/*
extern "C" DLL_PUBLIC void GImageLoad(CFTypeRef* crefreturn, const char* filename);
extern "C" DLL_PUBLIC void GImageUnload(CFTypeRef* crefreturn);
extern "C" DLL_PUBLIC int  GImageIsEmpty(const CFTypeRef* cref);
extern "C" DLL_PUBLIC size_t GImageGetWidth(const CFTypeRef* cref);
extern "C" DLL_PUBLIC size_t GImageGetHeight(const CFTypeRef* cref);
extern "C" DLL_PUBLIC unsigned char* GImageGetData(const CFTypeRef* cref);
extern "C" DLL_PUBLIC int GImageGetSamples(const CFTypeRef* cref);
*/

extern bool windowExposed;

#endif // OSXFUNCTIONS_OBJC

#endif
