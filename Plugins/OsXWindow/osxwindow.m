//
//  osxwindow.m
//  osxwindow
//
//  Created by Jacques Tronconi on 07/11/2015.
//  Copyright (c) 2015 Atlanti's Corporation. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <Cocoa/Cocoa.h>

bool windowclosed = true;

@interface gappdelegate : NSObject
{
}
@end

@implementation gappdelegate

- (BOOL) applicationShouldTerminateAfterLastWindowClosed:(NSApplication *)theApplication
{
    return YES;
}

@end

@interface gopenglwindow : NSWindow
{

}
@end

@implementation gopenglwindow

- (id) initWithContentRect: (NSRect)rect styleMask:(NSUInteger)wndStyle backing:(NSBackingStoreType)bufferingType defer:(BOOL)deferFlg
{
    self = [super initWithContentRect:rect styleMask:wndStyle backing:bufferingType defer:deferFlg];
    
    [[NSNotificationCenter defaultCenter]
     addObserver:self
     selector:@selector(windowDidResize:)
     name:NSWindowDidResizeNotification
     object:self];
    
    [[NSNotificationCenter defaultCenter]
     addObserver:self
     selector:@selector(windowWillClose:)
     name:NSWindowWillCloseNotification
     object:self];
    
    [self setAcceptsMouseMovedEvents:YES];
    
    printf("%s\n",__FUNCTION__);
    return self;
}

- (void) windowDidResize: (NSNotification *)notification
{
}

- (void) windowWillClose: (NSNotification *)notification
{
    windowclosed = true;
}

@end

@interface YsOpenGLView : NSOpenGLView
{
}
- (void) drawRect: (NSRect) bounds;
@end

@implementation YsOpenGLView
-(void) drawRect: (NSRect) bounds
{
    printf("%s\n",__FUNCTION__);
    
}

-(void) prepareOpenGL
{
    printf("%s\n",__FUNCTION__);
}

-(NSMenu *)menuForEvent: (NSEvent *)theEvent
{
    printf("%s\n",__FUNCTION__);
    return [NSView defaultMenu];
}

- (void) flagsChanged: (NSEvent *)theEvent
{
    unsigned int flags;
    flags=[theEvent modifierFlags];
}

- (void) keyDown:(NSEvent *)theEvent
{
    
}

- (void) keyUp:(NSEvent *)theEvent
{
    
}

- (void) mouseMoved:(NSEvent *)theEvent
{
    
}

- (void) mouseDragged:(NSEvent *)theEvent
{
    [self mouseMoved:theEvent];
}

- (void) rightMouseDragged:(NSEvent *)theEvent
{
    [self mouseMoved:theEvent];
}

- (void) otherMouseDragged:(NSEvent *)theEvent
{
    [self mouseMoved:theEvent];
}

- (void) mouseDown:(NSEvent *)theEvent
{
    
}

- (void) mouseUp:(NSEvent *)theEvent
{
    
}

- (void) rightMouseDown:(NSEvent *)theEvent
{
    
}

- (void) rightMouseUp:(NSEvent *)theEvent
{
    
}

- (void) otherMouseDown:(NSEvent *)theEvent
{
    
}

- (void) otherMouseUp:(NSEvent *)theEvent
{
    
}

@end


gopenglwindow* ysWnd = nil;
YsOpenGLView* ysView = nil;
NSRect contRect;

void GAddMenu (void)
{
//    NSAutoreleasePool *pool=[[NSAutoreleasePool alloc] init];
    
    NSMenu *mainMenu;
    
    mainMenu=[NSMenu alloc];
    mainMenu = [mainMenu initWithTitle:@"Minimum"];
    
    NSMenuItem *fileMenu;
    fileMenu=[[NSMenuItem alloc] initWithTitle:@"File" action:NULL keyEquivalent:[NSString string]];
    [mainMenu addItem:fileMenu];
    
    NSMenu *fileSubMenu;
    fileSubMenu=[[NSMenu alloc] initWithTitle:@"File"];
    [fileMenu setSubmenu:fileSubMenu];
    
    NSMenuItem *fileMenu_Quit;
    fileMenu_Quit=[[NSMenuItem alloc] initWithTitle:@"Quit"  action:@selector(terminate:) keyEquivalent:@"q"];
    [fileMenu_Quit setTarget:NSApp];
    [fileSubMenu addItem:fileMenu_Quit];
    
    [NSApp setMainMenu:mainMenu];
    
 //   [pool release];
}

void GCreateWindow (int x0,int y0,int wid,int hei)
{
    [NSApplication sharedApplication];
    [NSBundle loadNibNamed:@"MainMenu" owner:NSApp];
    
    gappdelegate *delegate;
    delegate=[gappdelegate alloc];
    delegate = [delegate init];
    [NSApp setDelegate: delegate];
    
    [NSApp finishLaunching];
    
    NSRect contRect;
    contRect=NSMakeRect(x0,y0,wid,hei);
    
    unsigned int winStyle=
    NSTitledWindowMask|
    NSClosableWindowMask|
    NSMiniaturizableWindowMask|
    NSResizableWindowMask;
    
    ysWnd=[gopenglwindow alloc];
    ysWnd = [ysWnd
     initWithContentRect:contRect
     styleMask:winStyle
     backing:NSBackingStoreBuffered 
     defer:NO];
    
    NSOpenGLPixelFormat *format;
    NSOpenGLPixelFormatAttribute formatAttrib[]=
    {
        NSOpenGLPFAWindow,
        NSOpenGLPFADepthSize,(NSOpenGLPixelFormatAttribute)32,
        NSOpenGLPFADoubleBuffer,
        0
    };
    
    format=[NSOpenGLPixelFormat alloc];
    [format initWithAttributes: formatAttrib];
    
    ysView=[YsOpenGLView alloc];
    contRect=NSMakeRect(0,0,800,600);
    [ysView
     initWithFrame:contRect
     pixelFormat:format];
    
    [ysWnd setContentView:ysView];
    [ysWnd makeFirstResponder:ysView];
    
    [ysWnd makeKeyAndOrderFront:nil];
    [ysWnd makeMainWindow];
    
    [NSApp activateIgnoringOtherApps:YES];
    
    GAddMenu();
    
    [ysWnd setTitle:@"Default Title"];
    [ysWnd display];
    [NSApp updateWindows];
    
    windowclosed = false;
}

bool GPollEvent (void)
{
    NSEvent *event;
    event=[NSApp
           nextEventMatchingMask:NSAnyEventMask
           untilDate: [NSDate distantPast]
           inMode: NSDefaultRunLoopMode
           dequeue:YES];
    if([event type]==NSRightMouseDown)
    {
        printf("R mouse down event\n");
    }
    if(event!=nil)
    {
        [NSApp sendEvent:event];
        [NSApp updateWindows];
    }
    else
    {
        return false;
    }
    
    return true;
}

bool GIsWindowClosed (void)
{ return windowclosed; }

void GInitRendererContext (void)
{
    
}

void GWSetTitle(const char* title)
{
    [ysWnd setTitle:[NSString stringWithUTF8String:title] ];
}

void GSwapBuffers(void)
{
    [[ysView openGLContext] flushBuffer];
}

void GGetWindowSize(int* w, int* h)
{
    NSRect rect;
    rect = [ysView frame];
    *w = rect.size.width;
    *h = rect.size.height;
}




