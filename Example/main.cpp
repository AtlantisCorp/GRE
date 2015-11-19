//
//  main.cpp
//  GResource
//
//  Created by Jacques Tronconi on 28/10/2015.
//  Copyright (c) 2015 Atlanti's Corporation. All rights reserved.
//

#include "ResourceManager.h"
#include "Listener.h"

int main(int argc, const char * argv[]) {
    
    try
    {
        Version version = GetGreVersion();
        std::cout << "[Main] Initialized Gre v." << version.major << "." << version.minor << "." << version.build << std::endl;
        
        ResourceManager::Create();
        ResourceManager::Get().loadPluginsIn("./plugins");
        
        Window myWindow = Window (ResourceManager::Get().loadResourceWith(ResourceManager::Get().getWindowLoaderFactory().getFirst(),
                                                                  Resource::Type::Window, "MyWindow", 0, 0, 1024, 768));
        
        Renderer myRenderer = Renderer (ResourceManager::Get().loadResourceWith(ResourceManager::Get().
                                                getRendererLoaderFactory().get(myWindow.recommendedRenderer() + "Loader"),
                                                Resource::Type::Renderer, "MyRenderer"));
        
        myWindow.associate(myRenderer);
        myWindow.setTitle("My Cool Application");
        
        std::cout << "Pool current size = " << ResourceManager::Get().getResourceUsage() << std::endl;
        
        myWindow.setVerticalSync(false);
        myRenderer.setFramerate(60.0f);
        
        Listener myGenericListener = myWindow.addListener("GenericListener");
        myGenericListener.addAction(EventType::KeyDown, [&] (const Event& e) {
            const KeyDownEvent& kde = e.to<KeyDownEvent>();
            if(kde.key == Key::V) {
                myWindow.setVerticalSync(!myWindow.hasVerticalSync());
                std::cout << "[Main] Changed Vertical Sync." << std::endl;
            }
            std::cout << "[Main] Key Down : " << (int) kde.key << std::endl;
        });
        
        while(!myWindow.isClosed()) {
            
            // Begin the render loop
            myRenderer.beginRender();
            
            // Treat every events.
            myWindow.pollEvent();
            
            // Renders the example scene.
            myRenderer.renderExample();
            
            // Swap buffers of the Window.
            myWindow.swapBuffers();
            
            // Update the Window title to write the fps.
            myWindow.setTitle(std::string("My Cool Application - @") + std::to_string((int)myRenderer.getCurrentFramerate()) + "fps" );
            
            // Tell the Renderer to sleep the current thread according to the FPS.
            myRenderer.endRender();
        }
        
        ResourceManager::Destroy();
        
    } catch (std::exception const& e) {
        std::cout << "Exception : " << e.what() << std::endl;
    }
    
    std::cout << "end." << std::endl;
    return 0;
}
