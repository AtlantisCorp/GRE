//
//  OSXFunctions.h
//  GRE
//
//  Created by Jacques Tronconi on 10/12/2015.
//
//

#ifndef GRE_OSXFunctions_h
#define GRE_OSXFunctions_h

#ifndef OSXFUNCTIONS_OBJC
#   include <stdio.h>
#   include <stdlib.h>
#   include <stddef.h>
#   include <string.h>
#
#   define BUILDING_DLL
#   include "ResourceManager.h"
#   include "WindowResource.h"
#endif // OSXFUNCTIONS_OBJC

#ifdef OSXFUNCTIONS_OBJC
#   import <Foundation/Foundation.h>
#   import <Cocoa/Cocoa.h>
#endif

#include <OpenGL/OpenGL.h>

#ifndef OSXFUNCTIONS_OBJC
using namespace Gre;
typedef const void* CFTypeRef;
#endif

// Maximum number of Windows.
#define WINDOW_MAX 16

// Maximum number of events handled at the same time.
#define KEYBUF_MAX 256

/// @brief A simple structure that represent a Key event.
struct keybuf_t
{
    int key;
    int pressed;
};

// ----------------------------------------------------------------------------------------------------------------------------------

// WindowBufEntry API 1

// ----------------------------------------------------------------------------------------------------------------------------------

/// @brief A C-style structure which contains every buffered properties
/// that might have changed in a Window object.
/// Those properties are given by the function NsGetWindowBufEntry(CFTypeRef).
typedef struct window_t_id
{
    /// @brief The Keyboard buffer we use.
    struct keybuf_t keybufs [KEYBUF_MAX];
    /// @brief The Current keybuf size.
    int keybuf_sz;
    
    /// @brief Sets this to true if the Window has closed.
    bool closed;
    /// @brief Property set to true when window is exposed.
    bool exposed;
    
    /// @brief This property is true if Size has changed.
    bool sizeChanged;
    /// @brief Holds the Width if changed.
    int newWidth;
    /// @brief Holds the Height if changed.
    int newHeight;
    /// @brief Holds the left border if changed.
    int newX;
    /// @brief Holds the top border if changed.
    int newY;
    
#ifdef OSXFUNCTIONS_OBJC
    /// @brief The Window we refer to.
    id window;
#endif
    
} WindowBufEntry;

extern WindowBufEntry nsWindowBufs [WINDOW_MAX];

#ifdef OSXFUNCTIONS_OBJC

/// @brief Init every Window Entries in order to be able
/// to call anyone of them.
void InitWindowEntry();

/// @brief Find a WindowEntry depending on given object's id.
WindowBufEntry* FindWindowEntry(id nsWindow);

/// @brief Find the next available window entry, or return NULL.
WindowBufEntry* FindNextAvailableWindowEntry();

#endif

#ifndef OSXFUNCTIONS_OBJC

// ----------------------------------------------------------------------------------------------------------------------------------

// OsXWindow API 2

// ----------------------------------------------------------------------------------------------------------------------------------

/// @brief The base function that must be called in StartPlugin.
extern "C" DLL_PUBLIC void NsLoadPluginApp();

/// @brief Treats Window events.
extern "C" DLL_PUBLIC bool NsPollEvent();

/// @brief Creates a new window and returns it in given CFTypeRef pointer.
extern "C" DLL_PUBLIC void NsCreateWindow(CFTypeRef* crefreturn, int x0,int y0,int wid,int hei);

/// @brief Returns the current WindowBufEntry used by this Window.
extern "C" DLL_PUBLIC WindowBufEntry* NsGetWindowBufEntry(CFTypeRef* nsWindow);

/// @brief Destroys the Window and make it desappear.
extern "C" DLL_PUBLIC void NsDestroyWindow(CFTypeRef* nsWindow);

/// @brief Returns true if Window is closed.
extern "C" DLL_PUBLIC bool NsIsWindowClosed(CFTypeRef* nsWindow);

/// @brief Sets a new title for the Window.
extern "C" DLL_PUBLIC void NsSetWindowTitle(CFTypeRef* nsWindow, const char* cTitle);

/// @brief Swap the Buffers.
extern "C" DLL_PUBLIC void NsWindowSwapBuffers(CFTypeRef* nsWindow);

/// @brief Returns the Width and the Height of the Window.
extern "C" DLL_PUBLIC void NsGetWindowSize(CFTypeRef* nsWindow, int* retWidth, int* retHeight);

/// @brief Tells if the Window is visible.
extern "C" DLL_PUBLIC bool NsWindowIsVisible(CFTypeRef* nsWindow);

/// @brief Tells if Window is Vertically synchronized (experimental)
extern "C" DLL_PUBLIC bool NsWindowIsVertSync(const CFTypeRef* nsWindow);

/// @brief Sets the Window V'sync option. (experimental)
extern "C" DLL_PUBLIC void NsWindowSetVertSync(CFTypeRef* nsWindow, bool arg);

/// @brief Changes the RenderContext from the Window.
extern "C" DLL_PUBLIC void NsWindowSetRenderContext(CFTypeRef* nsWindow, CGLContextObj ctxt);

// ----------------------------------------------------------------------------------------------------------------------------------

// OSXImage API 1

// ----------------------------------------------------------------------------------------------------------------------------------

extern "C" DLL_PUBLIC void GImageLoad(CFTypeRef* crefreturn, const char* filename);
extern "C" DLL_PUBLIC void GImageUnload(CFTypeRef* crefreturn);
extern "C" DLL_PUBLIC int  GImageIsEmpty(const CFTypeRef* cref);
extern "C" DLL_PUBLIC size_t GImageGetWidth(const CFTypeRef* cref);
extern "C" DLL_PUBLIC size_t GImageGetHeight(const CFTypeRef* cref);
extern "C" DLL_PUBLIC unsigned char* GImageGetData(const CFTypeRef* cref);
extern "C" DLL_PUBLIC int GImageGetSamples(const CFTypeRef* cref);

extern bool windowExposed;

#endif // OSXFUNCTIONS_OBJC

#endif
