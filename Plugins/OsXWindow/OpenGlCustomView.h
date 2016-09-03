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
/// @brief A Custom OpenGl view in order to manage NSOpenGlContext from
/// CGLContext.
///
/// CGLContext are created using the OpenGl Renderer. Those context are
/// linked to the Window which should set the context using the View's methods.
///
//////////////////////////////////////////////////////////////////////
@interface OpenGlCustomView : NSView
{
    @private
    
    /// @brief NSOpenGLContext normal context.
    NSOpenGLContext* _openGlContext;
    
    /// @brief NSOpenGLPixelFormat describe the used PixelFormat.
    NSOpenGLPixelFormat* _pixelFormat;
}

+ (NSOpenGLPixelFormat*) defaultPixelFormat;
- (id) initWithFrame:(NSRect)frameRect pixelFormat:(NSOpenGLPixelFormat*)format;
- (void) _surfaceNeedsUpdate:(NSNotification*)notification;

- (void) clearGLContext;
- (void) update;

- (void) setPixelFormat:(NSOpenGLPixelFormat*)pixelFormat;
- (NSOpenGLPixelFormat*) pixelFormat;

- (void) setCustomOpenGLContext:(CGLContextObj)cglContext;
- (void) setOpenGLContext:(NSOpenGLContext*)context;
- (NSOpenGLContext*)openGLContext;

@end

#endif
