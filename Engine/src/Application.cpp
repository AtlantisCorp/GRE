//////////////////////////////////////////////////////////////////////
//
//  Application.cpp
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

#include "Application.h"
#include "ResourceManager.h"

GreBeginNamespace

ApplicationPrivate::ApplicationPrivate ( const std::string& name , const std::string& author , const std::string& description )
: Gre::Resource(name)
, iAuthor(author)
, iDescription(description)
, iShouldTerminate(false)
{
    
}

ApplicationPrivate::~ApplicationPrivate() noexcept ( false )
{
    
}

void ApplicationPrivate::run()
{
    // Should send events to WindowManager and RendererManager . Those will updates the Windows objects and
    // the Renderer's objects. The RenderSceneManager should act the same way , but in another thread .
    
    // Adds the Managers as Listeners for this Application .
    
    addListener( ResourceUser ( & ResourceManager::Get().getWindowManager() ) ) ;
    addListener( ResourceUser ( & ResourceManager::Get().getRenderSceneManager() ) ) ;
    addListener( ResourceUser ( & ResourceManager::Get().getRendererManager() ) ) ;
    
    // Add every Renderer's as listeners .
    
    UpdateTime t = UpdateChrono::now() ;
    
    while ( !iShouldTerminate )
    {
        UpdateEvent* updateevent = new UpdateEvent ( this , UpdateChrono::now() - t ) ;
        
        sendEvent( updateevent );
        
        t = UpdateChrono::now() ;
    }
}

void ApplicationPrivate::terminate()
{
    iShouldTerminate = true ;
}

// ---------------------------------------------------------------------------------------------------

Application::Application ( const ApplicationPrivate* pointer )
: Gre::ResourceUser(pointer)
, SpecializedResourceUser<Gre::ApplicationPrivate>(pointer)
{
    
}

Application::Application ( const ApplicationHolder& holder )
: Gre::ResourceUser(holder)
, SpecializedResourceUser<Gre::ApplicationPrivate>(holder)
{
    
}

Application::Application ( const Application& user )
: Gre::ResourceUser(user)
, SpecializedResourceUser<Gre::ApplicationPrivate>(user)
{
    
}

Application::~Application() noexcept ( false )
{
    
}

void Application::run()
{
    auto ptr = lock();
    if ( ptr )
        ptr->run();
}

void Application::terminate()
{
    auto ptr = lock();
    if ( ptr )
        ptr->terminate();
}

ApplicationHolder Application::Create(const std::string &name , const std::string& author , const std::string& description )
{
    ApplicationLoaderFactory& f = ResourceManager::Get().getApplicationFactory() ;
    ApplicationLoader* l = f.getFirst() ;
    if ( l )
    {
        return l->load(name, author, description);
    }
    
    return ApplicationHolder ( nullptr ) ;
}

Application Application::Null = Application ( nullptr ) ;

// ---------------------------------------------------------------------------------------------------

ApplicationLoader::ApplicationLoader ()
{
    
}

ApplicationLoader::~ApplicationLoader () noexcept ( false ) 
{
    
}

GreEndNamespace


