#ifdef GreExample1
//
//  main.cpp
//  GResource
//
//  Created by Jacques Tronconi on 28/10/2015.
//  Copyright (c) 2015 Atlanti's Corporation. All rights reserved.
//

#include "ResourceManager.h"
#include "Listener.h"
#include "Mesh.h"
#include "Keyboard.h"
#include "Camera.h"
#include "RenderContextInfo.h"

using namespace Gre;

Mesh CreateTriangle (Renderer renderer)
{
    static float triangle[] = {
        0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        -1.0f,-1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
        1.0f,-1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f
    };
    
    static unsigned triangleindices[] = { 0, 1, 2 };
    
    HardwareVertexBuffer vbuf = renderer.createVertexBuffer();
    vbuf.add(VertexBatchFromRaw(triangle, 3));
    
    HardwareIndexBuffer ibuf = renderer.createIndexBuffer(PrimitiveType::Triangles, StorageType::UnsignedInt);
    ibuf.setMaterial(Material::Null);
    ibuf.add(IndexedFaceBatchFromRaw(triangleindices, 1, 3));
    HardwareIndexBufferBatch ibufs;
    ibufs.batchs.push_back(ibuf);
    
    return renderer.createMeshFromBuffers("Triangle", vbuf, ibufs);
}

Mesh CreateSquare (Renderer renderer)
{
    static float square[] = {
        -1.0f,  1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        1.0f,  1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        1.0f, -1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        -1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f
    };
    
    static unsigned squarei[] = {
        0, 1, 2,
        2, 3, 0
    };
    
    Material materialWood;
    materialWood.texture = renderer.createTexture("Textures/Wood", "textures/wood.jpg");
    
    HardwareVertexBuffer vbuf = renderer.createVertexBuffer();
    vbuf.add(VertexBatchFromRaw(square, 4));
    
    HardwareIndexBuffer ibuf = renderer.createIndexBuffer(PrimitiveType::Triangles, StorageType::UnsignedInt);
    ibuf.setMaterial(materialWood);
    ibuf.add(IndexedFaceBatchFromRaw(squarei, 2, 3));
    HardwareIndexBufferBatch ibufs;
    ibufs.batchs.push_back(ibuf);
    
    return renderer.createMeshFromBuffers("Quad", vbuf, ibufs);
}

Mesh LoadMesh(const std::string& file, Renderer renderer)
{
    Mesh mymesh = Mesh(ResourceManager::Get().loadResourceWith(ResourceManager::Get().getMeshLoaderFactory().get("ObjMeshLoader"), Resource::Type::Mesh,  "MyMesh", file, renderer));
    return mymesh;
}

int main(int argc, const char * argv[]) {
    
    try
    {
        GreDebugPretty() << "Initialized Gre v." << localVersion.major << "." << localVersion.minor << "." << localVersion.build << std::endl;
        
        ResourceManager::Create();
        ResourceManager::Get().loadPluginsIn("./plugins");
        
        {
            ResourceManager& rmanager = ResourceManager::Get();
            WindowLoaderFactory& wFactory = rmanager.getWindowLoaderFactory();
            RendererLoaderFactory& rFactory = rmanager.getRendererLoaderFactory();
            
            Window myWindow  = Window (rmanager.loadResourceWith(wFactory.getFirst(), Resource::Type::Window, "Window Context 1", 0, 0, 1024, 768));
            Window myWindow2 = Window (rmanager.loadResourceWith(wFactory.getFirst(), Resource::Type::Window, "Window Context 2", 0, 0, 1024, 768));
            
            Renderer myRenderer = Renderer (rmanager.loadResourceWith(rFactory.get(myWindow.recommendedRenderer() + "Loader"), Resource::Type::Renderer, "MultiContext Renderer"));
            
            // First we create correct Context.
            RenderContextInfo contextInfo1;
            contextInfo1.add("MinVersion", Version({3, 0, 0}));
            contextInfo1.add("HdwAccelerated", true);
            contextInfo1.add("VerticalSync", false);
            RenderContext context1 = myRenderer.createRenderContext("My Context - Window 1", contextInfo1);
            
            RenderContextInfo contextInfo2;
            contextInfo2.add("MaxVersion", Version({2, 1, 0}));
            contextInfo2.add("HdwAccelerated", true);
            contextInfo2.add("VerticalSync", true);
            RenderContext context2 = myRenderer.createRenderContext("My Context - Window 2", contextInfo2);
            
            // We add the two Window as Rendering Targets.
            // A Rendering Target is an object that can be binded in order to draw on it.
            // For example, a Framebuffer will be binded in order to draw on it, but a draw
            // in a Window will first bind the RenderContext, then
            myRenderer.addRenderTarget(myWindow);
            myRenderer.addRenderTarget(myWindow2);
            
            // We add the RenderContext to the Windows.
            myWindow.setRenderContext(context1);
            myWindow2.setRenderContext(context2);
            
            // Now when we call 'Renderer::render()', it will first draw to the first Window, which
            // will bind the Context. Next the second Window will bind the second Context and the Renderer
            // should re-draw the Scene.
            
            //myWindow.associate(myRenderer);
            //myWindow.setTitle("My Cool Application");
            
            GreDebugPretty() << "Pool current size = " << ResourceManager::Get().getResourceUsage() << std::endl;
            
            float rtri      = 0.0f;
            float rquad     = 0.0f;
            float trispeed  = 0.5f;
            float quadspeed = 0.25f;
            
            Mesh triangleMesh = CreateTriangle(myRenderer);
            Mesh squareMesh   = CreateSquare(myRenderer);
            Mesh myCustomMesh = LoadMesh("mesh/lar-08.obj", myRenderer);
            
            Listener myGenericListener = myWindow.addListener("GenericListener");
            myGenericListener.addAction(EventType::KeyDown, [&] (const Event& e) {
                const KeyDownEvent& kde = e.to<KeyDownEvent>();
                
                if(kde.key == Key::T) {
                    triangleMesh.getVertexBuffer().activateColor(!triangleMesh.getVertexBuffer().isColorActivated());
                }
                
                if(kde.key == Key::ArrowUp) {
                    trispeed += 0.05f;
                }
                
                if(kde.key == Key::ArrowDown) {
                    trispeed -= 0.05f;
                }
                
                GreDebugPretty() << "[Main] Key Down : " << (int) kde.key << std::endl;
            });
            
            Keyboard myKeyboard("MyKeyboard");
            myKeyboard.listen(myWindow);
            
            myRenderer.setClearColor({0.0f, 0.0f, 0.0f, 1.0f});
            myRenderer.setImmediateMode(false);
            
            Scene scene = myRenderer.loadSceneByName("BinaryTreeScene", "Scene1");
            Node root = myRenderer.getScene().getRoot();
            
            Camera myCamera = myRenderer.createCamera("MyCamera");
            myCamera.setPosition(Vector3(0.0f, 0.0f, 0.0f));
            myCamera.lookAt(Vector3(0.0f, 0.0f, -10.0f));
            myCamera.listen(myKeyboard);
            Node myCameraNode = root.addChild(scene.createNode(myCamera));
            scene.setActiveCamera(myCameraNode);
            
            Node triangleNode = root.addChild(scene.createNode(triangleMesh));
            triangleNode.translate(Vector3(-1.5f, 0.0f, -10.0f));
            triangleNode.setVisible(true);
            
            Node quadNode = root.addChild(scene.createNode(squareMesh));
            quadNode.translate(Vector3(1.5f, 0.0f, -10.0f));
            quadNode.addAction(EventType::Update, [&] (const Event&) { quadNode.rotate(rquad, Vector3(0.0f, 0.0f, 1.0f)); });
            quadNode.setVisible(true);
            
            myRenderer.resetElapsedTime();
            
            // We select the Scene object for the two Rendering Target.
            // This is the default behaviour as the Renderer's Scene is always the
            // default one but this is shown in case the user wants to use a different Scene.
            myRenderer.selectDefaultScene(myRenderer.getScene());
            
            // Next we tell the Engine that we want to end the App when every Window object has been closed.
            // The default behaviour is 'CloseBehaviour::Manual', which means that you have to manually check
            // every Windows.
            rmanager.setCloseBehaviour(CloseBehaviour::AllWindowClosed);
            
            // The process to draw objects and Scenes in Gang's Rendering Engine.
            //
            // 1. Create a Renderer. This object is able to draw in RenderTarget objects.
            //
            // 2. Create a RenderContext from the Renderer. The RenderContext will link the Renderer
            // to the future RenderTarget's objects.
            //
            // 3. Create some Viewport Objects from the RenderContext. Viewports are part of the RenderContext
            // drawing surface. You can customize them in order to draw just one Scene or just part of the Scene.
            //
            // 4. Create a RenderTarget, as a Window if your system supports it. The RenderTarget is the Hardware Surface
            // where objects will be drawed.
            //
            // 5. Link the RenderTarget with the RenderContext. We highly recommend NOT to share RenderContext between Window
            // objects.
            //
            // 6. Create a Scene and customize it, adding objects, setting some configuration.
            //
            // 7. Select the default Scene for the Renderer.
            //
            // 8. Do whatever you want here (as Keys handling, CloseBehaviour, Custom Behaviours, ...).
            //
            // 9. Run the program using ResourceManager::loop().
            //
            // And that's all.
            
            // The Rendering Process
            //
            // The Renderer draws in Viewports for each RenderContext. Each Viewport should have the
            // possibility to have a different selected Scene, as for RenderTarget. If the Viewport does not
            // have any selected Scene, the RenderTarget's parent Scene will be used. If the RenderTarget does
            // not have any Scene selected, the Renderer's default Scene is used.
            //
            // The RenderTarget holds and interact with a RenderContext. The RenderContext holds Viewports, and
            // is updated using the RenderTarget's events.
            //
            // The Renderer has the Selected Scene for the Current Viewport and Context. Now to render the Scene,
            // it will have to draw it using each Pass defined in the Scene object. For each Pass object, a HardwareProgram
            // is used in order to permit multiple Effects. Each Pass are either rendered in a Framebuffer if it has to be blended after,
            // or the Pass is rendered in the Framebuffer containing already rendered Pass's objects if it does not have to be rendered
            // separatly.
            // The Framebuffers are then blended together in order to form the final image.
            
            // The All-in-one process :
            
            /*
             
             Create Renderer renderer;
             renderer -> Create RenderContext context;
             context -> Create Viewport viewport1;
             context -> Create Viewport viewport2;
             
             Create Window window;
             window -> SetRenderContext context;
             renderer -> AddRenderTarget window;
             
             Create Scene scene;
             scene -> Create Node And Mesh From Files node1, node2, node3, ... ;
             window -> Select Scene scene;
             
             Resource Manager -> Loop ;
             
             */
            
            // Pseudo-code processing the Behaviours.
            /*
             ResourceManager::loop()
             {
             Windows.update();
             {
             foreach(Window : Windows)
             {
             Window.beginupdate() {
             pollEvent();
             context.bind(); { // Bind the Context to draw on it.
             _implementation_specific_binding();
             getRenderer().onCurrentContextChanged(this);
             }
             }
             
             context.getRenderer().render(Window) {
             Window.bindFramebuffer();
             
             // We draw the scene chosen for this RenderTarget.
             selectScene(SceneForRenderTarget[Window]);
             renderScene();
             
             Window.unbindFramebuffer();
             
             // Note : bind/unbindFramebuffer is needed when rendering to Framebuffer
             // object's RenderTarget.
             }
             
             Window.endupdate() {
             loopBehaviour();
             context.flush();  // Swap Buffers.
             context.unbind(); { // Unbind the Context.
             _implementation_specific_unbind();
             getRenderer().onCurrentContextChanged(RenderContext::Null);
             }
             }
             
             perWindowBehaviour();
             }
             }
             
             perloopBehaviour();
             }
             */
            
            // We also tell the Engine to refresh the Window's title for each window, and then we update
            // the Meshes using per-loop function.
            // To add per-object loop function, you have as choices :
            //   - ResourceManager::addLoopBehaviour() : Called every loop, externally from any Window's binding.
            //   - ResourceManager::addPerWindowBehaviour() : Called for every Window, Window is not binded.
            //   - Window::addLoopBehaviour() : Called every Window's loop, Window is binded. This is called after
            // the Renderer has drew the Scene, normally in Window::endUpdate().
            myWindow.addLoopBehaviour([&] () {
                
                // Update the Window title to write the fps.
                float fps = myRenderer.getCurrentFramerate();
                myWindow.setTitle(std::string("Window Context 1 - @") + std::to_string((int)fps) + "fps" );
                
            });
            
            myWindow2.addLoopBehaviour([&] () {
                
                // Update the Window title to write the fps.
                float fps = myRenderer.getCurrentFramerate();
                myWindow2.setTitle(std::string("Window Context 2 - @") + std::to_string((int)fps) + "fps" );
                
            });
            
            rmanager.addLoopBehaviour([&] () {
                
                ElapsedTime etime = myRenderer.getElapsedTime();
                if(etime != ElapsedTime::zero())
                {
                    rtri  = trispeed  * (360.0f/1000.0f) * (float) etime.count();
                    rquad = quadspeed * (360.0f/1000.0f) * (float) etime.count();
                }
                
            });
            
            // Then we add an Action to the Keyboard which is Listening for events.
            // Here we just show how the 'CloseBehaviour::Manual' could be used to make an Application
            // that only close on pressing a key.
            // The 'ResourceManager::stop()' function should close every Window's and then perform a correct
            // cleaning.
            myKeyboard.addAction(EventType::KeyUp, [&] (const Event& e) {
                const KeyUpEvent& kup = e.to<KeyUpEvent>();
                
                if(kup.key == Key::Esc) {
                    rmanager.stop();
                }
                
            });
            
            /*
             while(!myWindow.isClosed()) {
             
             myWindow.beginUpdate();
             {
             myRenderer.render();
             }
             myWindow.endUpdate();
             
             }
             */
            
            // This function currently runs the Engine main loop. It is contitued by :
            //   - Updating the Window's Rendering loop.
            //   - Calls the functions added with ResourceManager::addLoopBehaviour() .
            // Note that you can add per-window update behaviour using function Window::addLoopBehaviour().
            // In this function, the Window will be 'binded' (context is set).
            // You also can access the current Context by using ResourceManager::getCurrentRenderContext();
            rmanager.loop();
            
        }
        
        ResourceManager::Destroy();
        
    } catch (std::exception const& e) {
        GreDebugPretty() << "Exception : " << e.what() << std::endl;
    }
    
    GreDebugPretty() << "end." << std::endl;
    return 0;
}
#endif
