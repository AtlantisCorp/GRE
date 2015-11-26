//
//  main.cpp
//  GResource
//
//  Created by Jacques Tronconi on 28/10/2015.
//  Copyright (c) 2015 Atlanti's Corporation. All rights reserved.
//

#include "ResourceManager.h"
#include "Listener.h"

using namespace Gre;

int main(int argc, const char * argv[]) {
    
    try
    {
        std::cout << "[Main] Initialized Gre v." << localVersion.major << "." << localVersion.minor << "." << localVersion.build << std::endl;
        
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
        myRenderer.setActive(true);
        
        float rtri      = 0.0f;
        float rquad     = 0.0f;
        float trispeed  = 0.5f;
        float quadspeed = 0.25f;
        
        Listener myGenericListener = myWindow.addListener("GenericListener");
        myGenericListener.addAction(EventType::KeyDown, [&] (const Event& e) {
            const KeyDownEvent& kde = e.to<KeyDownEvent>();
            if(kde.key == Key::V) {
                myWindow.setVerticalSync(!myWindow.hasVerticalSync());
                std::cout << "[Main] Changed Vertical Sync." << std::endl;
            }
            if(kde.key == Key::A) {
                myRenderer.setActive(!myRenderer.isActive());
            }
            
            if(kde.key == Key::ArrowUp) {
                trispeed += 0.05f;
            }
            
            if(kde.key == Key::ArrowDown) {
                trispeed -= 0.05f;
            }
            
            std::cout << "[Main] Key Down : " << (int) kde.key << std::endl;
        });
        
        myRenderer.setClearColor({0.0f, 0.0f, 0.0f, 0.0f});
        myRenderer.setImmediateMode(true);
        myRenderer.addImmediateAction([&] () {
            
            myRenderer.translate(-1.5f, 0.0f, -6.0f);
            myRenderer.rotate(rtri, 1.0f, 0.0f, 0.0f);
            myRenderer.drawTriangle(1.0f, Color::Red, Color::Blue, Color::Green);
            myRenderer.rotate(-rtri, 1.0f, 0.0f, 0.0f);
            
            myRenderer.translate(3.0f,0.0f,0.0f);
            myRenderer.rotate(rquad, 0.0f, 1.0f, 0.0f);
            myRenderer.drawQuad(1.0f, Color::Blue, Color::Blue, Color::Blue, Color::Blue);
        });
        
        myRenderer.resetElapsedTime();
        
        while(!myWindow.isClosed()) {
            
            myWindow.beginUpdate();
            {
                myRenderer.render();
            }
            myWindow.endUpdate();
            
            // Update the Window title to write the fps.
            float fps = myRenderer.getCurrentFramerate();
            myWindow.setTitle(std::string("My Cool Application - @") + std::to_string((int)fps) + "fps" );
            
            ElapsedTime etime = myRenderer.getElapsedTime();
            if(etime != ElapsedTime::zero())
            {
                rtri  = trispeed  * (360.0f/1000.0f) * (float) etime.count();
                rquad = quadspeed * (360.0f/1000.0f) * (float) etime.count();
            }
        }
        
        ResourceManager::Destroy();
        
    } catch (std::exception const& e) {
        std::cout << "Exception : " << e.what() << std::endl;
    }
    
    std::cout << "end." << std::endl;
    return 0;
}
