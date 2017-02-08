//////////////////////////////////////////////////////////////////////
//
//  DarwinApplication.cpp
//  This source file is part of Gre
//		(Gang's Resource Engine)
//
//  Copyright (c) 2015 - 2016 Luk2010
//  Created on 08/10/2016.
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

#include "DarwinApplication.h"

DarwinApplication::DarwinApplication ( const std::string& name , const std::string& author , const std::string& description )
: Gre::ApplicationPrivate(name, author, description)
{
    // Here , we do not need to do anything. In fact , we can just let the plugin get the shared application object. We
    // do not want to have more than one Application , so even storing the NSApplication object is not necessary.
    NsLoadPluginApp() ;
}

DarwinApplication::~DarwinApplication() noexcept ( false )
{
    // If the Application is here , it means the NSApplication object is going to terminate. So do nothing, again.
}

void DarwinApplication::run()
{
    // What we have to do here , is to call the 'NSApplication::run()' function , but also 'Application::run()' function in
    // order to let Gre::Window object use 'iPollEvent' . To achieve this , we launch a new thread to call
    // 'Application::run()' and then launch 'NSGlobalApplicationRun()' .
    
    iRunloop = std::thread ( [] (ApplicationPrivate* app) {
        
        app->ApplicationPrivate::run();
        
    } , this);
    
    // Only call the NSApplication run function.
    NSGlobalApplicationRun () ;
    
    iRunloop.join() ;
}

void DarwinApplication::terminate()
{
    // Only call NSApplication::terminate .
    NSGlobalApplicationTerminate () ;
    
    // Also call Application::terminate() in order to let the main loop terminate.
    ApplicationPrivate::terminate();
}

// ---------------------------------------------------------------------------------------

DarwinApplicationLoader::DarwinApplicationLoader ( )
{
    
}

DarwinApplicationLoader::~DarwinApplicationLoader ( ) noexcept ( false )
{
    
}

ApplicationHolder DarwinApplicationLoader::load(const std::string &name, const std::string &author, const std::string &desc) const
{
    return ApplicationHolder ( (ApplicationPrivate*) new DarwinApplication(name, author, desc) ) ;
}

bool DarwinApplicationLoader::isLoadable(const std::string &filepath) const
{
    return false ;
}

ResourceLoader* DarwinApplicationLoader::clone() const
{
    return new DarwinApplicationLoader () ;
}
