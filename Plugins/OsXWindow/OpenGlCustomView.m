//
//  OpenGlCustomView.m
//  GRE
//
//  Created by Jacques Tronconi on 01/04/2016.
//
//

#include "OpenGlCustomView.h"

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
    if(self != nil)
    {
        _pixelFormat = [format retain];
        [[NSNotificationCenter defaultCenter] addObserver:self
                                                 selector:@selector(_surfaceNeedsUpdate:)
                                                     name:NSViewGlobalFrameDidChangeNotification
                                                   object:self];
    }
    
    return self;
}

- (void) clearGLContext
{
    if(_openGlContext != nil)
    {
        [_openGlContext clearDrawable];
        [_openGlContext release];
        _openGlContext = nil;
    }
}

- (void) update
{
    if(_openGlContext != nil)
    {
        [_openGlContext update];
    }
}

- (void) _surfaceNeedsUpdate:(NSNotification *)notification
{
    [self update];
}

- (void) setPixelFormat:(NSOpenGLPixelFormat *)pixelFormat
{
    if(_pixelFormat != nil)
    {
        [_pixelFormat release];
        _pixelFormat = nil;
    }
    
    _pixelFormat = [pixelFormat retain];
}

- (NSOpenGLPixelFormat*) pixelFormat
{
    return _pixelFormat;
}

- (void) setCustomOpenGLContext:(CGLContextObj)cglContext
{
    if(_openGlContext != nil)
    {
        [self clearGLContext];
    }
    
    _openGlContext = [[NSOpenGLContext alloc] initWithCGLContextObj:cglContext];
}

- (void) setOpenGLContext:(NSOpenGLContext *)context
{
    if(_openGlContext != nil)
    {
        [self clearGLContext];
    }
    
    _openGlContext = [context retain];
}

- (NSOpenGLContext*) openGLContext
{
    return _openGlContext;
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
