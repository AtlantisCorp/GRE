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

int main ( int argc , char** argv )
{
    GreDebugPretty() << "GRE Version : "
    << localVersion.major << "." << localVersion.minor << "." << localVersion.build << std::endl;
    
    try
    {
        // In this example , we show the use of the Application Object.
        //
        // The goal for this Object , is to let platform-dependent organization do its job in this object. For
        // example , on macOS , this object should be linked with a NSApplication object. It should take care
        // about running the NSApplication loop in the main thread , and run ResourceManager::update() in a
        // different thread.
        //
        // This will make that the Window's objects will be drawed only when a WindowMustRedrawEvent is sent to
        // the RenderContext. The RenderContext will so havethe 'iMustRedraw' flag to true and the Renderer will
        // draw the RenderTarget according to this value.
        //
        // Logical datas are updated in the ResourceManager::update loop. SceneManager basically are updated here.
        // The Window objects do not need to be updated by the ResourceManager.
        
        ResourceManager::Create() ;
        
        // Loads every plugins in given directory.
        
        ResourceManager::Get().loadPluginsIn("plugins");
        
        // Create an Application will let the Engine organize the differente phase.
        // In fact , 'Application::Create()' only use the 'ApplicationLoaderFactory' to load an Application. As
        // there should not be more than one Application , there is no ApplicationManager available.
        
        ApplicationHolder myapp = Application::Create ( "Example 3" , "Luk2010" , "A Cool Example to show 'Gre::Application'." );
        
        if ( myapp.isInvalid() )
        {
            GreDebugPretty() << "Can't create 'Gre::Application' ." << std::endl;
            throw GreExceptionWithText("Can't create 'Gre::Application' .") ;
        }
        
        // Get Window Manager.
        
        WindowManager& winmanager = ResourceManager::Get().getWindowManager();
        
        if ( winmanager.getWindowLoaderFactory().getLoaders().empty() )
        {
            GreDebugPretty() << "No WindowLoader installed." << std::endl;
            throw GreExceptionWithText("No Window Loader.") ;
        }
        
        // Create a Window.
        
        Window win = winmanager.load("MyWindow", 10, 10, 800, 600);
        
        if ( win.isInvalid() )
        {
            GreDebugPretty() << "Window 'win' can't be loaded." << std::endl;
            throw GreExceptionWithText("Window not loaded.") ;
        }
        
        // Try to create a Keyboard listener and to set some actions.
        
        bool typing = false;
        std::string title;
        // float color = 0;
        KeyboardHolder kbd = KeyboardHolder ( new KeyboardPrivate("MyKBD") );
        
        if ( kbd.isInvalid() )
        {
            GreDebugPretty() << "Can't initialize Keyboard." << std::endl;
        }
        else
        {
            win.addListener( Keyboard(kbd) );
            kbd->addAction(EventType::KeyUp, [&] (const Event& e) {
                const KeyUpEvent& kue = e.to<KeyUpEvent>();
                
                if ( kue.iKey == Key::Escape && !typing ) {
                    typing = true;
                    title.clear();
                    GreDebugPretty() << "Please begin typing new title." << std::endl;
                }
                
                else if ( kue.iKey == Key::Escape && typing ) {
                    typing = false;
                    GreDebugPretty() << "New title : " << title << std::endl;
                    win.setTitle(title);
                }
                
                else if ( typing ) {
                    title.push_back( KeyToChar(kue.iKey) );
                }
                
            });
        }
        
        // Get the RendererManager and creates the Renderer.
        
        RendererManager& rmanager = ResourceManager::Get().getRendererManager();
        
        if ( rmanager.getRendererLoaderFactory().getLoaders().empty() )
        {
            GreDebugPretty() << "No RendererLoader installed." << std::endl;
            throw GreExceptionWithText("No Renderer Loader.") ;
        }
        
        Renderer renderer = rmanager.load("MyRenderer");
        
        if ( renderer.isInvalid() )
        {
            GreDebugPretty() << "Renderer is invalid." << std::endl;
        }
        
        // Just lock the holders.
        
        WindowHolder wholder = win.lock();
        RendererHolder rholder = renderer.lock();
        
        // Set the 'RendererFeature::LoadDefaultProgram' , for test only.
        rholder->setFeature(RendererFeature::LoadDefaultProgram);
        
        // Create a RenderContext with default info.
        
        RenderContextInfo ctxtinfo;
        RenderContextHolder rctxt = rholder->createRenderContext("myctxt", ctxtinfo);
        
        rctxt->setClearColor( Color(1.0f, 0.4f, 0.4f, 1.0f) );
        RenderContextClearBuffers bufs;
        bufs.push_back ( RenderContextClearBuffer::ColorBufferBit );
        bufs.push_back ( RenderContextClearBuffer::DepthBufferBit );
        rctxt->setClearBuffers( bufs );
        
        if ( rctxt.isInvalid() )
        {
            GreDebugPretty() << "RenderContext is invalid." << std::endl;
        }
        
        else
        {
            wholder->setRenderContext( RenderContext(rctxt) );
        }
        
        // We try to load a Wavefront OBJ model.
        
        Mesh mycube = ResourceManager::Get().getMeshManager().load("Cube1", "Resources/obj/Cube.obj");
        
        if ( mycube.isInvalid() )
        {
            GreDebugPretty() << "'mycube' is invalid." << std::endl;
        }
        
        // Try to load a RenderScene .
        
        RenderScene scene = ResourceManager::Get().getRenderSceneManager().load ( "CubeScene" );
        
        if ( scene.isInvalid() )
        {
            GreDebugPretty() << "'scene' is invalid." << std::endl;
        }
        
        else
        {
            RenderSceneHolder sceneholder = scene.lock();
            
            RenderNodeHolder node = sceneholder->createNodeWithName( "CubeNode" );
            node->translate( Vector3(0.0f, 0.0f, 1.0f) );
            node->setMesh(mycube);
            sceneholder->addNode(node);
            
            wholder->selectScene(scene);
        }
        
        // Add the RenderTarget to the renderer.
        
        rholder->registerTarget( RenderTargetHolder(wholder.get()) );
        
        // And now we can run the Application object.
        
        myapp->run();
        
    } catch ( const GreException& e )
    {
        GreDebugPretty() << "Exception launched : " << e.what() << std::endl;
        GreDebugPretty() << "Exiting program." << std::endl;
    }
}




#endif
