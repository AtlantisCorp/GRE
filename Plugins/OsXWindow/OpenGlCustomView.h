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
    NSOpenGLContext* iOpenGlContext;
    
    /// @brief NSOpenGLPixelFormat describe the used PixelFormat.
    NSOpenGLPixelFormat* iPixelFormat;
}

/// @brief Allocates and initialize the NSOpenGLPixelFormat object.
+ (NSOpenGLPixelFormat*) defaultPixelFormat;

/// @brief Retains the pixel format and sets up the notification NSViewGlobalFrameDidChangeNotification.
- (id) initWithFrame:(NSRect)frameRect pixelFormat:(NSOpenGLPixelFormat*)format;

/// @brief Changes the 'iOpenGlContext' property.
- (void) setOpenGlContext: (NSOpenGLContext*) context;
/// @brief Returns the 'iOpenGlContext' property.
- (NSOpenGLContext*)openGlContext;

/// @brief Clears the OpenGl Context.
- (void) clearGlContext;

/// @brief Changes the 'iPixelFormat' property.
- (void) setPixelFormat: (NSOpenGLPixelFormat*) pixelformat;
/// @brief Returns the 'iPixelFormat' property.
- (NSOpenGLPixelFormat*) pixelFormat;

/// @brief Calls the update method of the NSOpenGLContext class.
- (void) update;
- (void) _surfaceNeedsUpdate: (NSNotification*) notification;

/// @brief Makes sure that the view is locked prior to drawing and that the context is the current one.
- (void) lockFocus;
- (void) drawRect:(NSRect)dirtyRect;

/// @brief Detaches the context from the drawable object when the custom view is moved from the window.
- (void) viewDidMoveToWindow;

@end

#endif
