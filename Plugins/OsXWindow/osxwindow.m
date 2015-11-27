//
//  osxwindow.m
//  osxwindow
//
//  Created by Jacques Tronconi on 07/11/2015.
//  Copyright (c) 2015 Atlanti's Corporation. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <Cocoa/Cocoa.h>

#define KEYBUF_MAX 256

struct keybuf_t
{
    int key;
    int pressed;
};

extern struct keybuf_t keybuf [KEYBUF_MAX];
extern int      keybuf_sz;

bool windowclosed = true;
bool windowExposed = false;

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
    
    [[NSNotificationCenter defaultCenter]
     addObserver:self
     selector:@selector(windowExposed:)
     name:NSWindowDidExposeNotification
     object:self];
    
    [self setAcceptsMouseMovedEvents:YES];
    //[self isReleasedWhenClosed] = YES;
    
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

- (void) windowExposed: (NSNotification *) notification
{
    windowExposed = true;
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
    if(keybuf_sz < KEYBUF_MAX)
    {
        struct keybuf_t newevent;
        newevent.key = theEvent.keyCode;
        newevent.pressed = 1;
        keybuf[keybuf_sz] = newevent;
        keybuf_sz++;
    }
}

- (void) keyUp:(NSEvent *)theEvent
{
    if(keybuf_sz < KEYBUF_MAX)
    {
        struct keybuf_t newevent;
        newevent.key = theEvent.keyCode;
        newevent.pressed = 0;
        keybuf[keybuf_sz] = newevent;
        keybuf_sz++;
    }
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

void GCloseWindow ()
{
    windowclosed = true;
}

void GAddMenu (void)
{
    NSMenu *mainMenu;
    
    mainMenu=[NSMenu alloc];
    mainMenu = [mainMenu initWithTitle:@"Minimum"];
    
    NSMenuItem *fileMenu;
    fileMenu=[[NSMenuItem alloc] initWithTitle:@"File" action:NULL keyEquivalent:[NSString string]];
    [mainMenu addItem:fileMenu];
    
    NSMenu *fileSubMenu;
    fileSubMenu=[[NSMenu alloc] initWithTitle:@"File"];
    [fileMenu setSubmenu:fileSubMenu];
    
    [NSApp setMainMenu:mainMenu];
}

void GCreateWindow (int x0,int y0,int wid,int hei)
{
    NSApplication* app = [NSApplication sharedApplication];
    if(app == nil)
        return;
    
    // loadNibNamed:owner:topLevelObjects was introduced in 10.8 (Mountain Lion).
    // In order to support Lion and Mountain Lion +, we need to see which OS we're
    // on. We do this by testing to see if [NSBundle mainBundle] responds to
    // loadNibNamed:owner:topLevelObjects: ... If so, the app is running on at least
    // Mountain Lion... If not, then the app is running on Lion so we fall back to the
    // the older loadNibNamed:owner: method. If your app does not support Lion, then
    // you can go with strictly the newer one and not deal with the if/else conditional.
    
    if ([[NSBundle mainBundle] respondsToSelector:@selector(loadNibNamed:owner:topLevelObjects:)]) {
        // We're running on Mountain Lion or higher
        [[NSBundle mainBundle] loadNibNamed:@"MainMenu"
                                      owner:NSApp
                            topLevelObjects:nil];
    } else {
        // We're running on Lion
        [NSBundle loadNibNamed:@"MainMenu"
                         owner:NSApp];
    }
   
    NSRect contRect;
    contRect=NSMakeRect(x0,y0,wid,hei);
    
    unsigned int winStyle = NSTitledWindowMask | NSClosableWindowMask |
                            NSMiniaturizableWindowMask | NSResizableWindowMask;
    
    ysWnd = [gopenglwindow alloc];
    ysWnd = [ysWnd initWithContentRect:contRect
                             styleMask:winStyle
                               backing:NSBackingStoreBuffered
                                 defer:NO];
    
    NSOpenGLPixelFormat *format;
    NSOpenGLPixelFormatAttribute formatAttrib[]=
    {
        NSOpenGLPFADepthSize,(NSOpenGLPixelFormatAttribute)32,
        NSOpenGLPFADoubleBuffer,
        0
    };
    
    format   = [NSOpenGLPixelFormat alloc];
    format   = [format initWithAttributes: formatAttrib];
    contRect = NSMakeRect(0,0,800,600);
    
    ysView   = [YsOpenGLView alloc];
    ysView   = [ysView initWithFrame:contRect
                         pixelFormat:format];
    
    int param = 0;
    CGLSetParameter([[ysView openGLContext] CGLContextObj], kCGLCPSwapInterval, &param);
    
    [ysWnd setContentView:ysView];
    [ysWnd makeFirstResponder:ysView];
    
    [ysWnd makeKeyAndOrderFront:ysWnd];
    [ysWnd makeMainWindow];
    
    [app activateIgnoringOtherApps:YES];
    
    GAddMenu();
    
    [ysWnd setTitle:@"Default Title"];
    [ysWnd display];
    [ysWnd makeKeyWindow];
    [app updateWindows];
    
    windowclosed = false;
    
    [app finishLaunching];
}

bool GPollEvent (void)
{
    NSApplication* app = [NSApplication sharedApplication];
    if(app == nil) {
        return false;
    }
    
    NSEvent *event = [app nextEventMatchingMask:NSAnyEventMask untilDate: [NSDate distantPast] inMode: NSDefaultRunLoopMode dequeue:YES];
    
    if(event == nil)
        return false;
    
    if([event type]==NSRightMouseDown)
    {
        printf("R mouse down event\n");
    }
    if(event!=nil)
    {
        [app sendEvent:event];
        [app updateWindows];
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
    [ysWnd setTitle:[NSString stringWithCString:title encoding:NSASCIIStringEncoding]];
}

void GSwapBuffers(void)
{
    [[ysView openGLContext] flushBuffer];
}

void GGetWindowSize(int* w, int* h)
{
    NSRect rect = [ysView frame];
    *w = rect.size.width;
    *h = rect.size.height;
}

void GSetVSync(int vsync)
{
    CGLSetParameter([[ysView openGLContext] CGLContextObj], kCGLCPSwapInterval, &vsync);
}

int GHasVSync ()
{
    int has;
    return CGLGetParameter([[ysView openGLContext] CGLContextObj], kCGLCPSwapInterval, &has);
    return has;
}

void GLoad()
{
}

void GUnload ()
{
    NSApplication* app = [NSApplication sharedApplication];
    if(app)
        [app terminate:app];
}



