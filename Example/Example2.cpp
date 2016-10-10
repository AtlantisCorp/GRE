// #define GreExample2
#ifdef GreExample2

//////////////////////////////////////////////////////////////////////
//
// Example2 - Simple Program for Gre 0.0.17
//
// Shows how to initialize an Application using GRE, and using the
// ResourceManager to see if any Window loader is installed.
//
//////////////////////////////////////////////////////////////////////

#include "ResourceManager.h"

using namespace Gre;

int main ( int argc, char* argv[] )
{
    GreDebugPretty() << "Hello, World :-)" << std::endl;
    GreDebugPretty() << "GRE Version : " << localVersion.major << "." << localVersion.minor << "." << localVersion.build << std::endl;
    
    try
    {
        ResourceManager::Create();
        
        GreDebugPretty() << "GRE correctly initialized." << std::endl;
        
        ResourceManager::Get().loadPluginsIn("plugins");
        
        WindowManager& winmanager = ResourceManager::Get().getWindowManager();
        
        if ( winmanager.getWindowLoaderFactory().getLoaders().empty() )
        {
            GreDebugPretty() << "No WindowLoader installed." << std::endl;
        }
        
        else
        {
            Window win = winmanager.load("MyWindow", 10, 10, 800, 600);
            
            if ( win.isInvalid() )
            {
                GreDebugPretty() << "Window is invalid." << std::endl;
            }
            
            else
            {
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
                
                RendererManager& rmanager = ResourceManager::Get().getRendererManager();
                
                if ( rmanager.getRendererLoaderFactory().getLoaders().empty() )
                {
                    GreDebugPretty() << "No RendererLoader installed." << std::endl;
                }
                
                else
                {
                    Renderer renderer = rmanager.load("MyRenderer");
                    
                    if ( renderer.isInvalid() )
                    {
                        GreDebugPretty() << "Renderer is invalid." << std::endl;
                    }
                    
                    else
                    {
                        WindowHolder wholder = win.lock();
                        RendererHolder rholder = renderer.lock();
                        
                        // Set the 'RendererFeature::LoadDefaultProgram' , for test only.
                        rholder->setFeature(RendererFeature::LoadDefaultProgram);
                        
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
                        
                        rholder->registerTarget( RenderTargetHolder(wholder.get()) );
                        rholder->launch();
                    }
                }
                
                ResourceManager::Get().setCloseBehaviour(CloseBehaviour::AllWindowClosed);
                ResourceManager::Get().loop();
            }
        }
        
        ResourceManager::Destroy();
        
        GreDebugPretty() << "GRE correctly destroyed." << std::endl;
    }
    
    catch ( const Gre::GreException& e )
    {
        GreDebugPretty() << "Exception launched : " << e.what() << std::endl;
        GreDebugPretty() << "Exiting program." << std::endl;
    }
    
    return 0;
}

#endif
