//
//  mypluginmain.cpp
//  GResource
//
//  Created by Jacques Tronconi on 05/11/2015.
//  Copyright (c) 2015 Atlanti's Corporation. All rights reserved.
//

#define BUILDING_DLL
#include "ResourceManager.h"

extern "C" DLL_PUBLIC void StartPlugin (void)
{
    
}

extern "C" DLL_PUBLIC void StopPlugin (void)
{
    
}

extern "C" DLL_PUBLIC void* GetPluginName (void)
{
    return (void*) "MyPluginExample";
}


