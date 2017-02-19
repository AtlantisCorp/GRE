//////////////////////////////////////////////////////////////////////
//
//  GlXApplication.cpp
//  This source file is part of Gre
//		(Gang's Resource Engine)
//
//  Copyright (c) 2015 - 2016 Luk2010
//  Created on 17/10/2016.
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

#include "GlXApplication.h"

GlXApplication * app = nullptr ;

GlXApplication::GlXApplication ( const std::string& name , const std::string& author, const std::string& description )
: Gre::ApplicationPrivate(name , author , description)
{
    GreResourceAutolock ;
    
    iDisplay = XOpenDisplay(NULL);
    
    if ( !iDisplay )
    {
        GreDebugPretty() << "'XOpenDisplay(NULL)' failed." << Gre::gendl ;
        throw Gre::GreConstructorException ( "GlXApplication" , "'XOpenDisplay()' failed." ) ;
    }
    
    app = this;
}

GlXApplication::~GlXApplication() noexcept ( false )
{
    GreResourceAutolock;
    
    if ( !iDisplay )
    {
        GreDebugPretty() << "'iDisplay' is not initialized." << Gre::gendl ;
        throw Gre::GreExceptionWithText ( "'iDisplay' not initialized but 'GlXApplication' is." ) ;
    }
    
    XCloseDisplay(iDisplay);
    app = nullptr ;
}

void GlXApplication::run()
{
    // Here we can just use the 'Application::run()' basic function , as when updating GlX objects they will
    // see with XNextEvent if they have an Event pending for them .
    
    ApplicationPrivate::run() ;
}

void GlXApplication::terminate()
{
    ApplicationPrivate::terminate();
}

Display * GlXApplication::getXDisplay()
{
    GreResourceAutolock ;
    return iDisplay ;
}

GlXApplication * GlXApplication::GetApplication()
{
    return app ;
}

// ---------------------------------------------------------------------------------------

GlXApplicationLoader::GlXApplicationLoader ()
{
    
}

GlXApplicationLoader::~GlXApplicationLoader() noexcept ( false )
{
    
}

Gre::ApplicationHolder GlXApplicationLoader::load(const std::string &name, const std::string &author, const std::string &desc) const
{
    return Gre::ApplicationHolder ( new GlXApplication(name, author, desc) ) ;
}

Gre::ResourceLoader* GlXApplicationLoader::clone() const
{
    return new GlXApplicationLoader () ;
}

bool GlXApplicationLoader::isLoadable(const std::string &filepath) const
{
    return false ;
}
