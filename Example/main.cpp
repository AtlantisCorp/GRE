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
            Window myWindow = Window (ResourceManager::Get().loadResourceWith(ResourceManager::Get().getWindowLoaderFactory().getFirst(),
                                                                              Resource::Type::Window, "MyWindow", 0, 0, 1024, 768));
            
            Renderer myRenderer = Renderer (ResourceManager::Get().loadResourceWith(ResourceManager::Get().
                                                                                    getRendererLoaderFactory().get(myWindow.recommendedRenderer() + "Loader"),
                                                                                    Resource::Type::Renderer, "MyRenderer"));
            
            myWindow.associate(myRenderer);
            myWindow.setTitle("My Cool Application");
            
            GreDebugPretty() << "Pool current size = " << ResourceManager::Get().getResourceUsage() << std::endl;
            
            myWindow.setVerticalSync(false);
            myRenderer.setFramerate(60.0f);
            myRenderer.setActive(true);
            
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
            
            Camera myCamera = myRenderer.createCamera("MyCamera");
            myCamera.setPosition(Vector3(0.0f, 0.0f, 0.0f));
            myCamera.lookAt(Vector3(0.0f, 0.0f, -10.0f));
            myCamera.listen(myKeyboard);
            
            myRenderer.setClearColor({0.0f, 0.0f, 0.0f, 1.0f});
            myRenderer.setImmediateMode(false);
            
            Scene scene = myRenderer.loadSceneByName("BinaryTreeScene", "Scene1");
            Node root = myRenderer.getScene().getRoot();
            
            Node triangleNode = root.addChild(scene.createNode(triangleMesh));
            triangleNode.translate(Vector3(-1.5f, 0.0f, -10.0f));
            triangleNode.setVisible(true);
            
            Node quadNode = root.addChild(scene.createNode(squareMesh));
            quadNode.translate(Vector3(1.5f, 0.0f, -10.0f));
            quadNode.addAction(EventType::Update, [&] (const Event&) { quadNode.rotate(rquad, Vector3(0.0f, 0.0f, 1.0f)); });
            quadNode.setVisible(true);
            
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
        }
        
        ResourceManager::Destroy();
        
    } catch (std::exception const& e) {
        GreDebugPretty() << "Exception : " << e.what() << std::endl;
    }
    
    GreDebugPretty() << "end." << std::endl;
    return 0;
}
