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
#include "MacEventQueue.h"

@implementation OpenGlCustomView

- (void) drawRect:(NSRect)dirtyRect
{
    // 'drawRect' is actually responsible for flushing the Buffer.
    // The actual drawing is done by the Renderer object. The Renderer draw every RenderTarget, in a loop,
    // then call 'RenderTarget::draw()' in order to draw the RenderTarget. For example, DarwinWindow::draw()
    // calls 'NSWindow::setNeedsDisplay(true)'.
    
    NSOpenGLContext* context = [self openGLContext];
    
    [context makeCurrentContext];
    CGLLockContext([context CGLContextObj]);
    
    [context flushBuffer];
    CGLUnlockContext([context CGLContextObj]);
}

- (void) setEventQueue:(WindowEventQueue *)queue
{
    iEventQueue = queue;
}

@end
