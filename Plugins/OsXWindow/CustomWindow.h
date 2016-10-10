//////////////////////////////////////////////////////////////////////
//
//  CustomWindow.h
//  This source file is part of Gre
//		(Gang's Resource Engine)
//
//  Copyright (c) 2015 - 2016 Luk2010
//  Created on 01/04/2016.
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

#ifndef GRE_CustomWindow_h
#define GRE_CustomWindow_h

#define OSXFUNCTIONS_OBJC
#import "OSXFunctions.h"
#import "OpenGlCustomView.h"

//////////////////////////////////////////////////////////////////////
/// @brief A Custom NSWindow object that deals with the WindowBufEntry
/// buffer and a customisable OpenGlCustomView.
//////////////////////////////////////////////////////////////////////
@interface CustomWindow : NSWindow
{
    @private
    
    /// @brief The WindowEventQueue used by DarwinWindow to get events.
    WindowEventQueue* iEventQueue ;
    
    /// @brief The OpenGlCustomView. This View can change its OpenGlContext whenever we want.
    OpenGlCustomView* _nsGlView;
    
    /// @brief The WindowBufEntry buffer. This Buffer has a fixed size.
    WindowBufEntry* _nsWindowEntry;
    
    /// @brief True if this Window is closed.
    bool _nsIsClosed;
    
    /// @brief True if Window was not visible.
    bool iIsInvisible ;
}

- (OpenGlCustomView*) getGlView;
- (void) setGlView: (OpenGlCustomView*) glView;

- (WindowBufEntry*) getWindowEntry;
- (bool) isClosed;

- (void) setWindowEventQueue: (WindowEventQueue*) queue ;
- (void) setTitle:(NSString *)title ;

- (void) windowExposed:(NSNotification*) notification ;

@end

#endif
