#define GreExample2
#ifdef GreExample2

//////////////////////////////////////////////////////////////////////
//
// Example2 - Simple Program for Gre 0.0.16
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
                        RendererHolder rholder = renderer.lock();
                    }
                }
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
