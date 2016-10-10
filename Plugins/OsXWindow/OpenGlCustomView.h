//
//  OpenGlCustomView.h
//  GRE
//
//  Created by Jacques Tronconi on 01/04/2016.
//
//

#ifndef GRE_OpenGlCustomView_h
#define GRE_OpenGlCustomView_h

#define OSXFUNCTIONS_OBJC
#include "OSXFunctions.h"

@class NSOpenGLContext, NSOpenGLPixelFormat;

//////////////////////////////////////////////////////////////////////
/// @brief A Custom OpenGl view in order to manage WindowEventQueue.
///
//////////////////////////////////////////////////////////////////////
@interface OpenGlCustomView : NSOpenGLView
{
    @private
    
    /// @brief An Event Queue given by the Window.
    WindowEventQueue* iEventQueue;
}

- (void) setEventQueue: (WindowEventQueue*) queue ;
- (void) drawRect:(NSRect)dirtyRect ;

@end

#endif
