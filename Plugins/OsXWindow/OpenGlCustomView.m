//
//  OpenGlCustomView.m
//  GRE
//
//  Created by Jacques Tronconi on 01/04/2016.
//
//

#include "OpenGlCustomView.h"
#include <OpenGL/OpenGL.h>
#include <OpenGL/gl3.h>

@implementation OpenGlCustomView

+ (NSOpenGLPixelFormat*) defaultPixelFormat
{
    NSOpenGLPixelFormatAttribute formatAttribCore3[] =
    {
        0
    };
    
    return [[NSOpenGLPixelFormat alloc] initWithAttributes: formatAttribCore3];
}

- (id) initWithFrame:(NSRect)frameRect pixelFormat:(NSOpenGLPixelFormat *)format
{
    self = [super initWithFrame:frameRect];
    
    if( self != nil )
    {
        iPixelFormat = format;
        iOpenGlContext = nil;
        
        [[NSNotificationCenter defaultCenter]
         addObserver:self
         selector:@selector(_surfaceNeedsUpdate:)
         name:NSViewGlobalFrameDidChangeNotification
         object:self];
    }
    
    return self;
}

- (void) setOpenGlContext:(NSOpenGLContext *)context
{
    if ( iOpenGlContext != nil )
    {
        [self clearGlContext];
    }
    
    iOpenGlContext = context;
}

- (NSOpenGLContext*) openGlContext
{
    return iOpenGlContext;
}

- (void) clearGlContext
{
    if( iOpenGlContext != nil )
    {
        [iOpenGlContext release];
        iOpenGlContext = nil;
    }
}

- (void) setPixelFormat:(NSOpenGLPixelFormat *)pixelFormat
{
    if( iPixelFormat != nil )
    {
        [iPixelFormat release];
        iPixelFormat = nil;
    }
    
    iPixelFormat = pixelFormat;
}

- (NSOpenGLPixelFormat*) pixelFormat
{
    return iPixelFormat;
}

- (void) update
{
    if ( iOpenGlContext != nil )
    {
        [iOpenGlContext update];
    }
}

- (void) _surfaceNeedsUpdate:(NSNotification *)notification
{
    [self update];
}

- (void) lockFocus
{
    NSOpenGLContext* context = [self openGlContext];
    
    [super lockFocus];
    if ( [context view] != self ) {
        [context setView:self];
    }
    
    [context makeCurrentContext];
    
    // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // The 'lockFocus' method is responsible for setting the OpenGl clear states.
}

- (void) drawRect:(NSRect)dirtyRect
{
    // 'drawRect' is actually responsible for flushing the Buffer.
    // The actual drawing is done by the Renderer object. The Renderer draw every RenderTarget, in a loop,
    // then call 'RenderTarget::draw()' in order to draw the RenderTarget. For example, DarwinWindow::draw()
    // calls 'NSWindow::setNeedsDisplay(true)'.
    
    NSOpenGLContext* context = [self openGlContext];
    
    [context makeCurrentContext];
    [context flushBuffer];
}

- (void) viewDidMoveToWindow
{
    [super viewDidMoveToWindow];
/*
    if ( [self window] == nil ) {
        [[self openGlContext] clearDrawable];
    }
*/
}

-(NSMenu *)menuForEvent: (NSEvent *)theEvent
{
    return [NSView defaultMenu];
}

- (void) flagsChanged: (NSEvent *)theEvent
{
    unsigned int flags;
    flags=[theEvent modifierFlags];
}

@end
