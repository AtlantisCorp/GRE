//
//  Example3.cpp
//  GRE
//
//  Created by Jacques Tronconi on 08/10/2016.
//
//

#define GreIsExample3

#ifdef GreIsExample3

#include "ResourceManager.h"

using namespace Gre ;

int main ( int argc , char ** argv )
{
    GreDebugPretty() << "GRE Version : "
    << localVersion.major << "." << localVersion.minor << "." << localVersion.build << Gre::gendl;
    
    try
    {
        // 'Gre::Application' Basic Example.
        
        // This example should show a basic cube rotating in front of the camera. No controls , only the
        // cross button to close the window. Also , one can easily add some custom functions using Events, but
        // this is not the purpose of this example.
        
        // 1. Load the 'ResourceManager' subsystem .
        ResourceManager::Create () ;
        
        // 2. Loads the plugins in './plugins' directory .
        int count = ResourceManager::Get () .loadPluginsIn( "plugins" ) ;
        GreDebug("[INFO] Loaded '") << count << "' plugins." << Gre::gendl ;
        
        // 3. Create a 'Gre::Application' object . This Application will be set as a Global Application ,
        // available in the whole program . Only one Application can be set by program .
        ApplicationHolder eApplication = Application::Create( "Example Application" ) ;
        
        if ( eApplication.isInvalid() )
        {
            throw GreExceptionWithText ( "Can't create 'Gre::Application' ." ) ;
        }
        
        eApplication.lock() -> addCloseBehaviour ( ApplicationCloseBehaviour::AllWindowClosed ) ;
        eApplication.lock() -> addCloseBehaviour ( ApplicationCloseBehaviour::TerminateCalled ) ;
        eApplication.lock() -> addCloseBehaviour ( ApplicationCloseBehaviour::EscapeKey ) ;
        
        {
            // Gets some managers.
            auto rmanager = ResourceManager::Get().getRendererManager() ;
            auto smanager = ResourceManager::Get().getRenderSceneManager() ;
            auto wmanager = ResourceManager::Get().getWindowManager() ;
            
            // Creates a RenderScene.
            auto scene = smanager -> load ( "Default Scene" , { { "Technique" , "Default" } } ) ;
            if ( scene.isInvalid() ) exit(1) ;
            
            // Creates a Renderer to draw everything.
            auto renderer = rmanager -> load ( "Default Renderer" , {} ) ;
            if ( renderer.isInvalid() ) exit(3) ;
            
            // Creates a Window to render the scene.
            auto window = wmanager -> load ( "Default Window" , { {"Title", "Default Window"} , {"Size" , "1024x768"} } ) ;
            if ( window.isInvalid() ) exit(2) ;
            
            {
                // Assimilate everything.
                auto scenelock = scene.lock() ;
                if ( scenelock.isInvalid() ) exit(4) ;
                
                scenelock -> setRenderer ( renderer ) ;
                scenelock -> setRenderTarget ( window ) ;
                scenelock -> setClearColor ( Color(0.2f, 0.2f, 0.2f) ) ;
            }
            
        }
        
        // Now let the Application loop.
        Application -> loop () ;
        
        // Once here, destroy the ResourceManager.
        ResourceManager::Destroy () ;
        return 0 ;
        
    } catch ( const GreException& e )
    {
        GreDebugPretty() << "Exception launched : " << e.what() << Gre::gendl;
        GreDebugPretty() << "Exiting program." << Gre::gendl;
        return -1 ;
    }
}

#endif
