//
//  CustomWindow.h
//  GRE
//
//  Created by Jacques Tronconi on 01/04/2016.
//
//

#ifndef GRE_CustomWindow_h
#define GRE_CustomWindow_h

#define OSXFUNCTIONS_OBJC
#include "OSXFunctions.h"

@class OpenGlCustomView;

@interface CustomWindow : NSWindow
{
    @private
    OpenGlCustomView* _nsGlView;
    WindowBufEntry* _nsWindowEntry;
    bool _nsIsClosed;
}

- (OpenGlCustomView*) getGlView;
- (void) setGlView: (OpenGlCustomView*) glView;

- (WindowBufEntry*) getWindowEntry;
- (bool) isClosed;

@end

#endif
