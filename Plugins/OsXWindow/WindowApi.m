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

WindowBufEntry* NsGetWindowBufEntry(CFTypeRef* nsWindow)
{
    if(nsWindow)
    {
        const CustomWindow* nsGlWindow = (__bridge CustomWindow*) *nsWindow;
        return (WindowBufEntry*) ((void*)[nsGlWindow getWindowEntry]);
    }
    
    else
    {
        static WindowBufEntry BadEntry;
        return &BadEntry;
    }
}

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
    if(nsWindow)
    {
        const CustomWindow* nsGlWindow = (__bridge const CustomWindow*) *nsWindow;
        [nsGlWindow setTitle:[NSString stringWithCString:cTitle encoding:NSASCIIStringEncoding]];
    }
}

void NsWindowSwapBuffers(CFTypeRef* nsWindow)
{
    if(nsWindow)
    {
        const CustomWindow* nsGlWindow = (__bridge const CustomWindow*) *nsWindow;
        [[[nsGlWindow getGlView] openGlContext] flushBuffer];
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

bool NsWindowIsVisible(CFTypeRef* nsWindow)
{
    if(nsWindow)
    {
        CustomWindow* nsGlWindow = (__bridge CustomWindow*) *nsWindow;
        return [nsGlWindow isVisible] ;
    }
    
    return false;
}

bool NsWindowIsVertSync(const CFTypeRef* nsWindow)
{
    return true;
}

void NsWindowSetVertSync(CFTypeRef* nsWindow, bool arg)
{
    
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

// Return the best PixelFormat for this computer.
NSOpenGLPixelFormat* GChooseBestOpenGlPixelFormat()
{
    NSOpenGLPixelFormatAttribute formatAttribCore4[] =
    {
        NSOpenGLPFADepthSize, (NSOpenGLPixelFormatAttribute) 32,
        NSOpenGLPFADoubleBuffer,
        NSOpenGLPFAOpenGLProfile, NSOpenGLProfileVersion4_1Core,
        NSOpenGLPFAAccelerated,
        0
    };
    
    NSOpenGLPixelFormatAttribute formatAttribCore3[] =
    {
        NSOpenGLPFADepthSize, (NSOpenGLPixelFormatAttribute) 32,
        NSOpenGLPFADoubleBuffer,
        NSOpenGLPFAOpenGLProfile, NSOpenGLProfileVersion3_2Core,
        NSOpenGLPFAAccelerated,
        0
    };
    
    NSOpenGLPixelFormatAttribute formatAttrib[] =
    {
        NSOpenGLPFADepthSize,(NSOpenGLPixelFormatAttribute)32,
        NSOpenGLPFADoubleBuffer,
        NSOpenGLPFAOpenGLProfile, NSOpenGLProfileVersionLegacy,
        NSOpenGLPFAAccelerated,
        0
    };
    
    NSOpenGLPixelFormatAttribute formatAttribSoft[] =
    {
        NSOpenGLPFADepthSize,(NSOpenGLPixelFormatAttribute)32,
        NSOpenGLPFADoubleBuffer,
        0
    };
    
    NSOpenGLPixelFormat* result = nil;
    
    result = [[NSOpenGLPixelFormat alloc] initWithAttributes: formatAttribCore4];
    
    if(result == nil) {
        printf("[OSXWindow] Can't find OpenGL Core Profile 4.1 . \n");
        result = [[NSOpenGLPixelFormat alloc] initWithAttributes: formatAttribCore3];
        
        if(result == nil) {
            printf("[OSXWindow] Can't find OpenGL Core Profile 3.2 . \n");
            result = [[NSOpenGLPixelFormat alloc] initWithAttributes: formatAttrib];
            
            if(result == nil) {
                printf("[OSXWindow] Can't find OpenGL Legacy Profile. \n");
                result = [[NSOpenGLPixelFormat alloc] initWithAttributes:formatAttribSoft];
                
                if(result != nil) {
                    printf("[OSXWindow] Using not 'hardware-accelerated' renderer. \n");
                } else {
                    printf("[OSXWindow] No Renderer found on this Mac. \n");
                }
                
            } else {
                printf("[OSXWindow] Found OpenGL Legacy Profile. \n");
            }
        } else {
            printf("[OSXWindow] Found OpenGL Core Profile 3.2 . \n");
        }
    } else {
        printf("[OSXWindow] Found OpenGL Core Profile 4.1 . \n");
    }
    
    return result;
}

bool NSIsAppLoaded = false;

void NsLoadPluginApp()
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
    [app finishLaunching];
    
    NSIsAppLoaded = true;
}

void NsCreateWindow(CFTypeRef* cfretreturn, int x0,int y0,int wid,int hei)
{
    NSApplication* app = [NSApplication sharedApplication];
    if(app == nil)
        return;
    
    if( !NSIsAppLoaded )
        NsLoadPluginApp();
    
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
    
    [nsNewWindow display];
    [app updateWindows];
    
    *cfretreturn = CFBridgingRetain(nsNewWindow);
}

void NsPollEventForWindow ( CFTypeRef* window )
{
    if ( window != nil )
    {
        CustomWindow* glwindow = (__bridge CustomWindow*) *window;
        [glwindow update];
    }
}

bool NsPollEvent()
{
    NSApplication* app = [NSApplication sharedApplication];
    if(app == nil) {
        return false;
    }
    
    @try
    {
        NSEvent* event = [app nextEventMatchingMask:NSAnyEventMask untilDate: [NSDate distantPast] inMode: NSDefaultRunLoopMode dequeue:YES];
        
        if( event!=nil )
        {
            if( [event type] == NSRightMouseDown )
            {
                printf("R mouse down event\n");
            }
            
            [app sendEvent:event];
        }
        else
        {
            return false;
        }
        
    }
    @catch (NSException* e)
    {
        printf("Error");
        printf([[e reason] cStringUsingEncoding:NSASCIIStringEncoding]);
    }

    return true;
}

void NsWindowSetRenderContext(CFTypeRef* nsWindow, CGLContextObj ctxt)
{
    if(nsWindow && ctxt)
    {
        CustomWindow* nsCustomWindow = (__bridge CustomWindow*) *nsWindow;
        
        NSRect nsFrame = [[nsCustomWindow contentView] frame];
        NSOpenGLPixelFormat* pixelformat = [[NSOpenGLPixelFormat alloc] initWithCGLPixelFormatObj:CGLGetPixelFormat(ctxt)];
        NSOpenGLContext* context = [[NSOpenGLContext alloc] initWithCGLContextObj:ctxt];
        
        OpenGlCustomView* customview = [[OpenGlCustomView alloc] initWithFrame:nsFrame pixelFormat:pixelformat];
        [customview setOpenGlContext:context];
        
        [nsCustomWindow setGlView:customview];
        [nsCustomWindow update];
    }
}

void NsWindowSetNeedsDisplay(CFTypeRef* window, bool value)
{
    if ( window )
    {
        CustomWindow* nsWindow = (__bridge CustomWindow*) *window;
        [[nsWindow getGlView] setNeedsDisplay:(BOOL)value];
    }
}

void NsWindowDisplay(CFTypeRef* window)
{
    if ( window )
    {
        CustomWindow* nsWindow = (__bridge CustomWindow*) *window;
        
        if ( [nsWindow isOnActiveSpace] && [nsWindow isVisible] )
        {
            [[nsWindow getGlView] display];
        }
    }
}

bool NsWindowPropertyIsVisible ( const CFTypeRef* window )
{
    if ( window )
    {
        const CustomWindow* nsWindow = ( __bridge const CustomWindow* ) *window;
        return [nsWindow isVisible];
    }
    
    return false;
}

bool NsWindowPropertyIsOnActiveSpace ( const CFTypeRef* window )
{
    if ( window )
    {
        const CustomWindow* nsWindow = ( __bridge const CustomWindow* ) *window;
        return [nsWindow isOnActiveSpace];
    }
    
    return false;
}


