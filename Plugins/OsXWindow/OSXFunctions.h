//
//  OSXFunctions.h
//  GRE
//
//  Created by Jacques Tronconi on 10/12/2015.
//
//

#ifndef GRE_OSXFunctions_h
#define GRE_OSXFunctions_h

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>

#define BUILDING_DLL
#include "ResourceManager.h"
#include "WindowResource.h"

using namespace Gre;

typedef const void* CFTypeRef;

extern "C" DLL_PUBLIC void GCreateWindow (int x0,int y0,int wid,int hei);
extern "C" DLL_PUBLIC bool GPollEvent (void);
extern "C" DLL_PUBLIC bool GIsWindowClosed (void);
extern "C" DLL_PUBLIC void GInitRendererContext (void);
extern "C" DLL_PUBLIC void GWSetTitle (const char*);
extern "C" DLL_PUBLIC void GSwapBuffers(void);
extern "C" DLL_PUBLIC void GGetWindowSize (int*, int*);
extern "C" DLL_PUBLIC void GSetVSync (int vsync);
extern "C" DLL_PUBLIC int GHasVSync ();
extern "C" DLL_PUBLIC void GLoad();
extern "C" DLL_PUBLIC void GUnload();

extern "C" DLL_PUBLIC void GImageLoad(CFTypeRef* crefreturn, const char* filename);
extern "C" DLL_PUBLIC void GImageUnload(CFTypeRef* crefreturn);
extern "C" DLL_PUBLIC int  GImageIsEmpty(const CFTypeRef* cref);
extern "C" DLL_PUBLIC size_t GImageGetWidth(const CFTypeRef* cref);
extern "C" DLL_PUBLIC size_t GImageGetHeight(const CFTypeRef* cref);
extern "C" DLL_PUBLIC unsigned char* GImageGetData(const CFTypeRef* cref);
extern "C" DLL_PUBLIC int GImageGetSamples(const CFTypeRef* cref);

extern bool windowExposed;

#endif
