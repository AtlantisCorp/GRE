//
//  osxwindow.cpp
//  GResource
//
//  Created by Jacques Tronconi on 07/11/2015.
//  Copyright (c) 2015 Atlanti's Corporation. All rights reserved.
//

#include "OSXWindow.h"
#include "OSXImage.h"

WindowBufEntry nsWindowBufs [WINDOW_MAX];

extern "C" DLL_PUBLIC void* GetPluginName (void)
{
    return (void*) "OS X Window System";
}

extern "C" DLL_PUBLIC void StartPlugin (void)
{
    NsLoadPluginApp();
    
    ResourceManager::Get().getWindowLoaderFactory().registers("OsXWindow", new OsXWindowLoader);
    ResourceManager::Get().getImageLoaderFactory().registers("OSXImage", new OSXImageLoader);
}

extern "C" DLL_PUBLIC void StopPlugin (void)
{
    
}