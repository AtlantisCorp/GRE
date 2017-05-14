//////////////////////////////////////////////////////////////////////
//
//  main.cpp
//  This source file is part of Gre
//		(Gang's Resource Engine)
//
//  Copyright (c) 2015 - 2017 Luk2010
//  Created on 13/05/2017.
//
//////////////////////////////////////////////////////////////////////
/*
 -----------------------------------------------------------------------------
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 -----------------------------------------------------------------------------
 */

#include <ResourceManager.h>
#include "DefaultKeys.h"

Gre::PluginInfo info ;

extern "C" Gre::PluginInfo * GetPluginInfo ( void )
{
    info.name = "DefaultControllersPlugin" ;
    info.desc = "Defines some usually used controllers." ;
    info.author = "Luk2010" ;
    info.version = GRE_PLUGIN_VERSION ;

    uuid_parse ( "fab63fb6-c10c-463d-b414-8aee6f3868f4" , info.uuid ) ;
    return &info ;
}

extern "C" void StartPlugin ( void )
{
    auto controllers = ResourceManager::Get() -> getControllerManager () ;

    if ( controllers.isInvalid() )
    return ;

    controllers -> create ("gre.controllers.defaultkeys" ,
                           ControllerHolder(new DefaultKeysController("gre.controllers.defaultkeys")) ) ;
}

extern "C" void StopPlugin ( void )
{
    auto controllers = ResourceManager::Get() -> getControllerManager () ;

    if ( controllers.isInvalid() )
    return ;

    controllers -> remove ( "gre.controllers.defaultkeys" ) ;
}
