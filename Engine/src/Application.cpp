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

Application::AllWindowClosedListener::AllWindowClosedListener ( Application* app )
: EventProceeder ( ) , appuser(app)
{
    
}

Application::AllWindowClosedListener::~AllWindowClosedListener ( ) noexcept ( false )
{
    
}

void Application::AllWindowClosedListener::onLastWindowClosed(const Gre::LastWindowClosedEvent &e)
{
    if ( appuser )
    {
        appuser -> iTerminatePrivate ( ApplicationCloseBehaviour::AllWindowClosed ) ;
    }
}

// ---------------------------------------------------------------------------------------------------

Application::EscapeKeyListener::EscapeKeyListener ( Application * appli )
: EventProceeder ( ) , app ( appli )
{
    
}

Application::EscapeKeyListener::~EscapeKeyListener() noexcept ( false )
{
    
}

void Application::EscapeKeyListener::onKeyDownEvent(const Gre::KeyDownEvent &e)
{
    if ( e.iKey == Key::Escape && app )
    {
        app ->iTerminatePrivate ( ApplicationCloseBehaviour::EscapeKey ) ;
    }
}

// ---------------------------------------------------------------------------------------------------

void Application::WorkerThreadMain ( const Application * app , SpecializedCountedObjectUser<EventProceeder> proceeder )
{
    TimePoint t = Time::now() ;
    
    while ( !app->shouldTerminate() )
    {
        EventHolder uevent = EventHolder ( new UpdateEvent( (const EventProceeder*) app , Time::now() - t ) ) ;
        if ( !proceeder.isInvalid() )
            proceeder.lock() -> onEvent(uevent) ;
        
        t = Time::now() ;
    }
}

Application::Application ( const std::string& name , const std::string& author , const std::string& description )
: Gre::Resource(ResourceIdentifier::New(), name)
, iAuthor(author), iDescription(description), iShouldTerminate(false)
, iWindowManager(nullptr), iRenderSceneManager(nullptr)
{
    
}

Application::~Application() noexcept ( false )
{
    {
        GreAutolock ;
        
        if ( !iShouldTerminate )
            iShouldTerminate = true ;
    }
        
    for ( std::thread & t : iWorkerThreads )
        t.join();
}

void Application::run()
{
    GreAutolock ;
    
    if ( iRunAlreadyCalled )
    {
#ifdef GreIsDebugMode
        GreDebugPretty() << "'Application::run' has already been called elsewhere." << Gre::gendl ;
#endif
        throw GreExceptionWithText ( "'Application::run' has already been called elsewhere." ) ;
    }
    
    iRunAlreadyCalled = true ;
    
    // The Hardware Thread ( or 'Main Thread' ) is actually updating the Window Manager ( which updates every
    // Window ) , and the RendererManager ( which renders every registered RenderTarget ) .
    
    // The Renderer is responsible for updating the MeshManager , the RenderContextManager , the TextureManager
    // and the HardwareProgramManager .
    
    if ( !ResourceManager::Get() .getWindowManager() .isInvalid() )
    {
        // If the CloseBehaviour 'AllWindowClosed' is set , we must see when the Window Manager is empty. For this
        // purpose , we listen to him .
        
        iAllWindowClosedListener = AllWindowClosedListenerHolder ( new AllWindowClosedListener ( this ) ) ;
        ResourceManager::Get() .getWindowManager() -> addListener ( EventProceederUser ( iAllWindowClosedListener ) ) ;
    }
    
    if ( !ResourceManager::Get() .getWindowManager() .isInvalid() )
    {
        // If the CloseBehaviour 'EscapeKey' is set , we must rely on the Keyboard Manager to send us key events.
        
        iEscapeListener = EscapeKeyListenerHolder ( new EscapeKeyListener ( this ) ) ;
        ResourceManager::Get() .getWindowManager() -> addGlobalKeyListener ( EventProceederUser( iEscapeListener ) ) ;
    }
    
    // The Logicals Threads are threads spawned for every 'logical' managers ( or 'Workers Threads' ) . They
    // are spawned for the RenderSceneManager ( which updates every Node ) , the AnimationManager , the
    // MaterialManager , the PluginManager , the CameraManager . They are updated in parallel with the main
    // thread.
    
    addWorkerThread ( EventProceederUser (ResourceManager::Get() .getRenderSceneManager()) ) ;
    addWorkerThread ( EventProceederUser (ResourceManager::Get() .getAnimatorManager()) ) ;
    addWorkerThread ( EventProceederUser (ResourceManager::Get() .getMaterialManager()) ) ;
    addWorkerThread ( EventProceederUser (ResourceManager::Get() .getCameraManager()) ) ;
    
    // Those managers have a Worker Thread but at least , normally they don't need one .
    
    addWorkerThread( EventProceederUser (ResourceManager::Get() .getRenderContextManager()) ) ;
    addWorkerThread( EventProceederUser (ResourceManager::Get() .getPluginManager()) ) ;
    
    addWorkerThread ( EventProceederUser (ResourceManager::Get() .getWindowManager()) ) ;
    addWorkerThread ( EventProceederUser (ResourceManager::Get() .getRendererManager()) ) ;
    
    // Adds the WindowManager and the RenderSceneManager to the Application.
    
    iWindowManager = ResourceManager::Get().getWindowManager() ;
    iRenderSceneManager = ResourceManager::Get().getRenderSceneManager() ;
    
    iMainStart = Time::now() ;
    iMainThreadLoop () ;
}

void Application::addWorkerThread ( SpecializedCountedObjectUser<EventProceeder> eventproceeder )
{
    GreAutolock ; iWorkerThreads.push_back ( std::thread(Application::WorkerThreadMain , this , eventproceeder) ) ;
}

void Application::addMainThread ( EventProceederHolder holder )
{
    GreAutolock ; iMainProceeders.push_back(holder) ;
}

void Application::terminate()
{
    GreAutolock ; iTerminatePrivate ( ApplicationCloseBehaviour::TerminateCalled ) ;
}

void Application::addCloseBehaviour(const Gre::ApplicationCloseBehaviour &behaviour)
{
    GreAutolock ; iCloseBehaviours.set ( (size_t) behaviour ) ;
}

bool Application::shouldTerminate () const
{
    GreAutolock ; return iShouldTerminate ;
}

void Application::iMainThreadLoop()
{
    while ( !iShouldTerminate )
    {
        Duration delta = Time::now() - iMainStart ;
        EventHolder elapsed = EventHolder ( new UpdateEvent ( this , delta ) ) ;
        iMainStart = Time::now() ;
        
        // We must achieve a normal update loop. This consiste in drawing everything we need
        // and then, polling for events using the window system.
        
        iRenderSceneManager -> drawScenes (elapsed) ;
        iWindowManager -> pollEvents (delta) ;
    }
}

void Application::iTerminatePrivate(Gre::ApplicationCloseBehaviour why)
{
    GreAutolock ;
    
    if ( iCloseBehaviours.test( (size_t) why ) )
    {
        iShouldTerminate = true ;
    }
}

ApplicationHolder Application::Create(const std::string &name , const std::string& author , const std::string& description )
{
    ApplicationLoaderFactory & f = ResourceManager::Get().getApplicationFactory() ;
    ApplicationLoader* l = f.getFirst() ;
    
    if ( l ) {
        ApplicationHolder app = l->load(name, author, description) ;
        if ( !app.isInvalid() ) iSharedApplication = app ;
        return app ;
    }
    
    return ApplicationHolder ( nullptr ) ;
}

ApplicationHolder Application::GetShared ()
{
    return iSharedApplication ;
}

void Application::Destroy()
{
    if ( !iSharedApplication.isInvalid() ) {
        iSharedApplication.clear() ;
    }
}

ApplicationHolder Application::iSharedApplication = ApplicationHolder ( nullptr ) ;

// ---------------------------------------------------------------------------------------------------

ApplicationLoader::ApplicationLoader ()
{
    
}

ApplicationLoader::~ApplicationLoader () noexcept ( false ) 
{
    
}

GreEndNamespace


