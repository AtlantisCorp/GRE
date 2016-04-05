#define GreExample2
#ifdef GreExample2

//////////////////////////////////////////////////////////////////////
//
// Example2 - Simple Program for Gre 0.0.15
// Luk2010, 23/03/2016
//
// This example should show you how to basically make a Scene using
// Gre v. 0.0.15.
//
//////////////////////////////////////////////////////////////////////

#include "ResourceManager.h"

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

Mesh LoadObjMesh(const std::string& name, const std::string& file, Renderer& renderer)
{
    return Mesh(ResourceManager::Get().loadResourceWith(ResourceManager::Get().getMeshLoaderFactory().get("ObjMeshLoader"), Resource::Type::Mesh, name, file, renderer));
}

Keyboard CreateKeyboard(const std::string& name, Window& emitter)
{
    Keyboard kbd = ResourceManager::Get().createKeyboard(name);
    kbd.listen(emitter);
    return kbd;
}

Scene CreateScene(const std::string& name, Renderer renderer)
{
    return renderer.loadSceneByName("BinaryTreeScene", name);
}

int main(int argc, char** argv)
{
    try
    {
        GreDebugPretty() << "Initialized Gre v." << localVersion.major << "." << localVersion.minor << "." << localVersion.build << std::endl;
        
        ResourceManager::Create();
        ResourceManager::Get().loadPluginsIn("./plugins");
        
        {
            ResourceManager& rmanager = ResourceManager::Get();
            WindowLoaderFactory& wFactory = rmanager.getWindowLoaderFactory();
            RendererLoaderFactory& rFactory = rmanager.getRendererLoaderFactory();
            
            std::string chosenRenderer = ResourceManager::Helper::ChooseRenderer(rFactory);
            GreDebugPretty() << "Chosen Renderer : " << chosenRenderer << std::endl;
            
            Renderer loadedRenderer = ResourceManager::Helper::LoadRenderer("MultiContext Renderer", chosenRenderer, rFactory);
            if(loadedRenderer.expired())
            {
                throw Gre::GreExceptionWithText("Renderer could not be loaded.");
            }
            
            loadedRenderer.launchDrawingThread();
            
            std::string chosenWindowLoader = ResourceManager::Helper::ChooseWindowLoader(wFactory);
            GreDebugPretty() << "Chosen Window Loader : " << chosenWindowLoader << std::endl;
            
            Window loadedWindow = ResourceManager::Helper::LoadWindow("Window Example 1", chosenWindowLoader, wFactory, { 0, 0, 1024, 768 });
            if(loadedWindow.expired())
            {
                throw Gre::GreExceptionWithText("Window could not be loaded.");
            }
        
            loadedRenderer.addRenderTarget(loadedWindow);
            if(loadedWindow.getRenderContext().expired())
            {
                throw Gre::GreExceptionWithText("Renderer did not create the RenderContext correctly.");
            }
            
            GreDebugPretty() << loadedWindow.getRenderContext().toString() << std::endl;
            
            rmanager.setCloseBehaviour(CloseBehaviour::AllWindowClosed);
            loadedWindow.addLoopBehaviour([&] () {
                
                // Update the Window title to write the fps.
                float fps = loadedRenderer.getCurrentFramerate();
                loadedWindow.setTitle(std::string("Window Example 1 - @") + std::to_string((int)fps) + "fps" );
                
            });
            
            Keyboard keyboard = CreateKeyboard("MyKeyboard", loadedWindow);
            keyboard.addAction(EventType::KeyDown, [&] (const Event& e){
                auto keyEvent = e.to<KeyDownEvent>();
                GreDebugPretty() << "Key is pressed ! (" << (int) keyEvent.key << ")" << std::endl;
            });
            
            keyboard.addAction(EventType::KeyUp, [&] (const Event& e){
                auto keyEvent = e.to<KeyUpEvent>();
                GreDebugPretty() << "Key is unpressed ! (" << (int) keyEvent.key << ")" << std::endl;
            });
            
            Mesh triangleMesh = CreateTriangle(loadedRenderer);
            Mesh squareMesh   = CreateSquare(loadedRenderer);
            Mesh myCustomMesh = LoadObjMesh("Mesh/Lar-08", "mesh/lar-08.obj", loadedRenderer);
            
            Scene scene = CreateScene("MySuperScene", loadedRenderer);
            loadedRenderer.selectDefaultScene(scene);
            Node root = loadedRenderer.getDefaultScene().getRoot();
            
            Camera myCamera = loadedRenderer.createCamera("MyCamera");
            myCamera.setPosition(Vector3(0.0f, 0.0f, 0.0f));
            myCamera.lookAt(Vector3(0.0f, 0.0f, -10.0f));
            myCamera.listen(keyboard);
            Node myCameraNode = root.addChild(scene.createNode(myCamera));
            scene.setActiveCamera(myCameraNode);
            
            Node triangleNode = root.addChild(scene.createNode(triangleMesh));
            triangleNode.translate(Vector3(-1.5f, 0.0f, -10.0f));
            triangleNode.setVisible(true);
            
            Node quadNode = root.addChild(scene.createNode(squareMesh));
            quadNode.translate(Vector3(1.5f, 0.0f, -10.0f));
            quadNode.setVisible(true);
            
            // The ResourceManager's loop is responsible for updating the Window's objects.
            // The Window's Update Loop is here, while the Renderer's drawing loop is launched
            // using Renderer::launchDrawingThread().
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
