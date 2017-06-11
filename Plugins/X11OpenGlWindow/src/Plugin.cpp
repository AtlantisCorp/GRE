//////////////////////////////////////////////////////////////////////
//
//  Plugin.cpp
//  This source file is part of Gre
//		(Gang's Resource Engine)
//
//  Copyright (c) 2015 - 2017 Luk2010
//  Created on 17/04/2017.
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

#include <X11OpenGlWindow.h>
#include <X11Application.h>

Gre::PluginInfo info ;

extern "C" Gre::PluginInfo* GetPluginInfo( void )
{
	info.name    = "X11OpenGlWindow" ;
 	info.author  = "Luk2010" ;
 	info.version = GRE_PLUGIN_VERSION ;
 	uuid_parse("45b70521-9657-4349-82a4-83b4998f559b" , info.uuid);

 	return &info ;
}

extern "C" void StartPlugin( void )
{
    if ( X11Application::X11Initialize() )
    GreDebug ( "[INFO] X11 Initialized." ) << Gre::gendl ;

	auto windows = Gre::ResourceManager::Get()->getWindowManager() ;

    if ( windows.isInvalid() )
    {
        GreDebug ( "[WARN] No WindowManager installed." ) << Gre::gendl ;
        return ;
    }

    windows->getFactory().registers( "windows.loaders.x11opengl" , new X11OpenGlWindowLoader() ) ;

    auto & appfactory = Gre::ResourceManager::Get()->getApplicationFactory() ;
    appfactory.registers( "apps.loaders.x11" , new X11ApplicationLoader() ) ;
}

extern "C" void StopPlugin( void )
{
	auto windows = Gre::ResourceManager::Get()->getWindowManager() ;

    if ( windows.isInvalid() )
    {
        GreDebug ( "[WARN] No WindowManager installed." ) << Gre::gendl ;
        return ;
    }

    windows->getFactory().unregister( "windows.loaders.x11opengl" ) ;

    auto appfactory = Gre::ResourceManager::Get()->getApplicationFactory() ;
    appfactory.unregister( "apps.loaders.x11" ) ;
}
