//
//  osxwindow.m
//  osxwindow
//
//  Created by Jacques Tronconi on 07/11/2015.
//  Copyright (c) 2015 Atlanti's Corporation. All rights reserved.
//

#define OSXFUNCTIONS_OBJC
#include "OSXFunctions.h"
#include "OpenGlCustomView.h"
#include "CustomWindow.h"

NSLock* plock = nil ;
NSApplication * GlobalApplication = nil ;

void NsDestroyWindow(CFTypeRef* nsWindow)
{
    if(nsWindow)
    {
        CustomWindow* nsGlWindow = (__bridge CustomWindow*) *nsWindow;
        CFBridgingRelease(*nsWindow);
        [nsGlWindow close];
        [nsGlWindow release];
    }
}

bool NsIsWindowClosed(CFTypeRef* nsWindow)
{
    if(nsWindow)
    {
        const CustomWindow* nsGlWindow = (__bridge const CustomWindow*) *nsWindow;
        return [nsGlWindow isClosed];
    }
    
    return true;
}

void NsSetWindowTitle(CFTypeRef* nsWindow, const char* cTitle)
{
    [plock lock] ;
    
    if(nsWindow)
    {
        const CustomWindow* nsGlWindow = (__bridge const CustomWindow*) *nsWindow;
        [nsGlWindow setTitle:[NSString stringWithCString:cTitle encoding:NSASCIIStringEncoding]];
    }
    
    [plock unlock] ;
}

void NsWindowSwapBuffers(CFTypeRef* nsWindow)
{
    if(nsWindow)
    {
        const CustomWindow* nsGlWindow = (__bridge const CustomWindow*) *nsWindow;
        [[[nsGlWindow getGlView] openGLContext] flushBuffer];
    }
}

void NsGetWindowSize(CFTypeRef* nsWindow, int* retWidth, int* retHeight)
{
    if(nsWindow)
    {
        const CustomWindow* nsGlWindow = (__bridge const CustomWindow*) *nsWindow;
        NSRect nsFrame = [nsGlWindow frame];
        
        *retWidth = nsFrame.size.width;
        *retHeight = nsFrame.size.height;
    }
}

void NsAddMenu (void)
{
    NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
    {
        NSMenu* mainMenu = [[NSMenu alloc] initWithTitle:@"Minimum"];
        
        NSMenuItem* fileMenu = [[NSMenuItem alloc] initWithTitle:@"File" action:NULL keyEquivalent:[NSString string]];
        [mainMenu addItem:fileMenu];
        
        NSMenu* fileSubMenu = [[NSMenu alloc] initWithTitle:@"File"];
        [fileMenu setSubmenu:fileSubMenu];
        
        [NSApp setMainMenu:mainMenu];
    }
    [pool release];
}

bool NSIsAppLoaded = false;

void NsLoadPluginApp()
{
    plock = [[NSLock alloc] init] ;
    
    GlobalApplication = [NSApplication sharedApplication] ;
    
    if( GlobalApplication == nil )
        return;
    
    // loadNibNamed:owner:topLevelObjects was introduced in 10.8 (Mountain Lion).
    // In order to support Lion and Mountain Lion +, we need to see which OS we're
    // on. We do this by testing to see if [NSBundle mainBundle] responds to
    // loadNibNamed:owner:topLevelObjects: ... If so, the app is running on at least
    // Mountain Lion... If not, then the app is running on Lion so we fall back to the
    // the older loadNibNamed:owner: method. If your app does not support Lion, then
    // you can go with strictly the newer one and not deal with the if/else conditional.
    
    if ([[NSBundle mainBundle] respondsToSelector:@selector(loadNibNamed:owner:topLevelObjects:)])
    {
        // We're running on Mountain Lion or higher
        [[NSBundle mainBundle] loadNibNamed:@"MainMenu"
                                      owner:NSApp
                            topLevelObjects:nil];
    }
    else
    {
        // We're running on Lion
        [NSBundle loadNibNamed:@"MainMenu"
                         owner:NSApp];
    }
    
    NsAddMenu();
    
    NSIsAppLoaded = true;
}

void NsCreateWindow(CFTypeRef* cfretreturn, int x0,int y0,int wid,int hei)
{
    NSApplication* app = GlobalApplication;
    
    if(app == nil)
        return;
    
    CustomWindow* nsNewWindow = nil;
    
    // We create the NSOpenGLWindow object with given x, y, width and height.
    // [TODO] Make Window's style customizable.
    
    NSRect contRect;
    contRect=NSMakeRect(x0,y0,wid,hei);
    
    unsigned int winStyle = NSTitledWindowMask | NSClosableWindowMask |
    NSMiniaturizableWindowMask | NSResizableWindowMask;
    
    nsNewWindow = [CustomWindow alloc];
    nsNewWindow = [nsNewWindow initWithContentRect:contRect styleMask:winStyle backing:NSBackingStoreBuffered defer:NO];
    
    // [nsNewWindow makeKeyAndOrderFront:nsNewWindow];
    // [nsNewWindow makeMainWindow];
    
    [app activateIgnoringOtherApps:YES];
    
    [nsNewWindow setTitle:@"Default Title"];
    [nsNewWindow makeKeyWindow];
    [nsNewWindow orderFront: nsNewWindow];
    [nsNewWindow makeMainWindow];
    
  //  [nsNewWindow display];
    [app updateWindows];
    
    *cfretreturn = CFBridgingRetain(nsNewWindow);
}

void NsWindowSetRenderContext(CFTypeRef* nsWindow, CGLContextObj ctxt)
{
    [plock lock] ;
    
    if(nsWindow && ctxt)
    {
        CustomWindow* nsCustomWindow = (__bridge CustomWindow*) *nsWindow;
        
        NSRect nsFrame = [[nsCustomWindow contentView] frame];
        NSOpenGLPixelFormat* pixelformat = [[NSOpenGLPixelFormat alloc] initWithCGLPixelFormatObj:CGLGetPixelFormat(ctxt)];
        NSOpenGLContext* context = [[NSOpenGLContext alloc] initWithCGLContextObj:ctxt];
        
        OpenGlCustomView* glview = [[OpenGlCustomView alloc] initWithFrame:nsFrame];
        [glview setOpenGLContext:context];
        [nsCustomWindow setGlView:glview];
    }
    
    [plock unlock] ;
}

bool NsWindowPropertyIsVisible ( const CFTypeRef* window )
{
    const CustomWindow * nsWindow = NULL ;
    bool ret = false ;
    
    [plock lock];
    
    if ( window )
    {
        nsWindow = ( __bridge const CustomWindow* ) *window;
        ret = [nsWindow isVisible];
    }
    
    [plock unlock] ;
    
    return ret ;
}

bool NsWindowPropertyIsOnActiveSpace ( const CFTypeRef* window )
{
    const CustomWindow * nsWindow = NULL ;
    bool ret = false ;
    
    [plock lock];
    
    if ( window )
    {
        nsWindow = ( __bridge const CustomWindow* ) *window;
        ret = true ;
    }
    
    [plock unlock] ;
    
    return ret ;
}
 
void NsWindowSetEventQueue ( CFTypeRef* window , WindowEventQueue* queue )
{
    if ( window && queue )
    {
        CustomWindow* nsWindow = ( __bridge CustomWindow* ) *window ;
        [nsWindow setWindowEventQueue:queue];
    }
}

void NSGlobalApplicationRun ( )
{
    NSApplication* app = [GlobalApplication retain];
    [app performSelectorOnMainThread:@selector(run) withObject:app waitUntilDone:YES];
}

void NSGlobalApplicationTerminate ( )
{
    NSApplication* app = GlobalApplication;
    [app terminate:nil];
}


