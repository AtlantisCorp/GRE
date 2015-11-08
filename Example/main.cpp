//
//  main.cpp
//  GResource
//
//  Created by Jacques Tronconi on 28/10/2015.
//  Copyright (c) 2015 Atlanti's Corporation. All rights reserved.
//

#include <unistd.h>
#include "ResourceManager.h"

int main(int argc, const char * argv[]) {
    // insert code here...
    std::cout << "Hello, World!\n";
    
    try
    {
        ResourceManager::Create();
        ResourceManager::Get().loadPluginsIn("./plugins");
        
        Window myWindow = Window(ResourceManager::Get().loadResourceWith(ResourceManager::Get().getWindowLoaderFactory().getFirst(),
                                                                  Resource::Type::Window, "MyWindow", 0, 0, 1024, 768));
        
        if(myWindow.lock())
            std::cout << "BOM" << std::endl;
        
        Plugin rendererPlugin = Plugin(ResourceManager::Get().loadResourceWith(PluginLoader(), Resource::Type::Plugin, "RendererPlugin", "libopenglrenderer.dylib"));
        
        
        Renderer myRenderer = Renderer (ResourceManager::Get().loadResourceWith(ResourceManager::Get().
                                                getRendererLoaderFactory().get(myWindow.recommendedRenderer() + "Loader"),
                                                Resource::Type::Renderer, "MyRenderer"));
        
        myWindow.associate(myRenderer);
        
        std::cout << "Pool current size = " << ResourceManager::Get().getResourceUsage() << std::endl;
        
        while(!myWindow.isClosed()) {
            myWindow.pollEvent();
            myRenderer.render();
            usleep(10000);
        }
        
        ResourceManager::Destroy();
        
    } catch (std::exception const& e) {
        std::cout << "Exception : " << e.what() << std::endl;
    }
    
    std::cout << "end." << std::endl;
    return 0;
}
