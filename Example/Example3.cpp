//
//  Example3.cpp
//  GRE
//
//  Created by Jacques Tronconi on 08/10/2016.
//
//

#include "ResourceManager.h"

using namespace Gre ;

class MyApplicationExample
{

public:

    MyApplicationExample ( )
    : iApplication(nullptr)
    {

    }

    ~MyApplicationExample ()
    {
        iApplication.clear();
        rmanager.clear();
        window.clear () ;
        renderpass.clear () ;
        renderpass2.clear () ;

        Gre::ResourceManager::Destroy();
    }

    void init ( int argc , char ** argv ) ;

    bool createScene () ;

    void run () ;

    //////////////////////////////////////////////////////////////////////
    // Loads the Scene that will be rendered on the screen. This function
    // first loads textures and meshes. Then , it will create a Scene using
    // the 'scenes.loaders.default' loader. It will integrate a light node ,
    // a cube node and a camera node.
    void loadScene ( void ) ;

protected:

    ApplicationHolder iApplication ;
    ResourceManagerHolder rmanager ;
    WindowHolder window ;
    RenderPassHolder renderpass ;
    RenderPassHolder renderpass2 ;
};

void MyApplicationExample::init ( int argc , char** argv )
{
    // Creating a GRE application requires three basic steps.
    // Creating the ResourceManager, loading plugins and creating an Application object.

	// Creating the ResourceManager only allocate a new object. This one must be initialized using
	// the 'ResourceManager::initialize()' function. The 'initialize()' function only loads every
	// managers present in the ResourceManager object. Notes the 'Create()' function also set the
	// resource manager 's singleton.

    rmanager = Gre::ResourceManager::CreateDefaultAndLoadPlugins () ;

	// Now , create our application object. This application should be initialized after allocation.

	iApplication = rmanager -> loadApplicationAndInitializeBehaviour (
            "::first" , "GRE Example Application" ,
            "Luk2010" , "Example Program to show GRE main features." ,
            argc , argv
    ) ;

    if ( iApplication.isInvalid() )
	{
		GreDebug( "[WARN] Application object couldn't be loaded." ) << Gre::gendl ;
		exit ( -244 ) ;
	}
}

bool MyApplicationExample::createScene()
{
    // Render something in GRE depends on three factors :
    //  - Creating one or more Windows.
    //  - Creating one or more Renderers , and link them with the Window's render context.
    //  - Creating one or more RenderScene .
    //
    // When those three types of objects are created , we can create one or more RenderPass
    // to connect everything. For example , we will in this example create a simple RenderPass
    // that use a BlinnPhong Lighting program to draw the scene , an FPS - looking Camera ,
    // a basic Scene that holds some objects , and one light to populate the program.
    //

    auto manager  = ResourceManager::Get() ;
    auto rmanager = ResourceManager::Get()->getRendererManager() ;
    auto smanager = ResourceManager::Get()->getRenderSceneManager() ;
    auto wmanager = ResourceManager::Get()->getWindowManager() ;
    auto tmanager = ResourceManager::Get()->getTechniqueManager() ;

    ResourceLoaderOptions WinOptions ;
    WinOptions ["Window.Title"] = std::string ( "Default Window" ) ;
    WinOptions ["Window.Size"]  = std::string ( "1024x768" ) ;

    WindowStyles styles = {
        WindowStyle::Resizable , WindowStyle::Miniaturizable ,
        WindowStyle::Closable , WindowStyle::Titled
    };

    WinOptions ["Window.Styles"] = styles ;

    WindowContextAttributes attr ({

        WCAAccelerated ,
        WCADoubleBuffer ,

        WCADepthSize , (WindowContextAttribute) 24 ,

        WCAMultiSample ,
        WCASampleBuffers , (WindowContextAttribute) 1 ,
        WCASamples , (WindowContextAttribute) 4 ,

        WCAMajorVersion , (WindowContextAttribute) 3 ,
        WCAMinorVersion , (WindowContextAttribute) 2

    }) ;

    WinOptions ["Window.ContextAttributes"] = attr ;

    window = wmanager -> load ( "Default Window" , WinOptions ) ;
    if ( window.isInvalid() ) return false ;

    //////////////////////////////////////////////////////////////////////
    // Creates a Renderer. As the first loader will be used , this one should
    // be compatible with the Window's API used.

    ResourceLoaderOptions RendOptions ;

    auto renderer = rmanager -> load ( "Default Renderer" , RendOptions ) ;
    if ( renderer.isInvalid() ) exit( -4 ) ;

    renderer -> setRenderContext ( window -> getRenderContext () ) ;
    renderer -> installManagers() ;

    //////////////////////////////////////////////////////////////////////
    // Tries to load definitions using the bundles added.
    // As the definition working stage is an asynchronous processing , here
    // as it is just an example we wait for the definition parser to finish
    // its job.

    GreDebug( "[INFO] Trying to load definitions." ) << gendl ;

    auto bundles = manager -> getBundles() ;
    auto result  = manager -> loadDefinitions( bundles , std::chrono::hours(1) );
    if ( !result.empty() )
    {
        GreDebug( "[ERRO] Printing Definitions Errors : " ) << result << gendl ;
        exit( -4 );
    }

    manager -> getDefinitionParser() -> reset() ;
    GreDebug( "[INFO] Loading Definitions from bundle finished." ) << gendl ;

    {
        auto pipeline = renderer -> getPipeline () ;
        pipeline -> setPass( 1 , "example.pass:1" , "learnopengl.shadowmapping.phase1.technique" ) ;
        pipeline -> setPass( 2 , "example.pass:2" , "learnopengl.shadowmapping.phase2.technique" ) ;

        renderpass2 = pipeline -> getPass ( 1 ) ;
        renderpass  = pipeline -> getPass ( 2 ) ;

        renderpass -> addNamedParameter("shadows", HdwProgVarType::Bool1, (int) 1) ;
    }

    return true ;
}

void MyApplicationExample::run()
{
    iApplication -> run() ;
}

void MyApplicationExample::loadScene ( void )
{
    //////////////////////////////////////////////////////////////////////
    // Checks managers.

    auto textures = ResourceManager::Get () -> getTextureManager () ;
    if ( textures.isInvalid() ) exit ( -25 ) ;

    auto meshes = ResourceManager::Get () -> getMeshManager () ;
    if ( meshes.isInvalid() ) exit ( -26 ) ;

    auto scenes = ResourceManager::Get () -> getRenderSceneManager () ;
    if ( scenes.isInvalid() ) exit ( -27 ) ;

    auto materials = ResourceManager::Get () -> getMaterialManager () ;
    if ( materials.isInvalid() ) exit ( -28 ) ;

    auto framebuffers = ResourceManager::Get () -> getFramebufferManager () ;
    if ( framebuffers.isInvalid() ) exit ( -38 ) ;

    auto controllers = ResourceManager::Get () -> getControllerManager () ;
    if ( controllers.isInvalid() ) return ;

    ResourceLoaderOptions defops ;

    //////////////////////////////////////////////////////////////////////
    // Creates textures and meshes.

    auto diffuse = textures -> loadBundledFile ( "textures.cube.diff" , "cube.diff.png" , TextureType::Texture2D , defops ) ;
    if ( diffuse.isInvalid() ) exit ( -29 ) ;

    auto specular = textures -> loadBundledFile ( "textures.cube.spec" , "cube.spec.png" , TextureType::Texture2D , defops ) ;
    if ( specular.isInvalid() ) exit ( -30 ) ;

    auto cube = meshes -> loadBundledFile ( "cube.obj" , defops ) ;
    if ( cube.isInvalid() ) exit ( -31 ) ;

    auto teapot = meshes -> loadBundledFile ( "teapot.obj" , defops ) ;
    if ( teapot.isInvalid() ) exit ( -40 ) ;

    //////////////////////////////////////////////////////////////////////
    // Creates the cube default material. This material will be applied to
    // any node that change its mesh to the cube. The second material will
    // be applied to the light node to emit light.

    auto material = materials -> loadBlank ( "materials.cube" ) ;
    if ( material.isInvalid() ) exit ( -32 ) ;

    material -> setUseTextures ( true ) ;
    material -> setDiffuseTexture ( diffuse ) ;
    material -> setSpecularTexture ( specular ) ;
    material -> setShininess ( 32.0f ) ;
    cube -> setDefaultMaterial ( material ) ;

    auto emissive = materials -> loadBlank ( "materials.light" ) ;
    if ( emissive.isInvalid() ) exit ( -33 ) ;

    emissive -> setUseTextures ( true ) ;
    emissive -> setAmbient ({ 0.5f , 0.5f , 0.5f }) ;
    emissive -> setDiffuse ({ 0.8f , 0.8f , 0.8f }) ;
    emissive -> setSpecular ({ 1.0f , 1.0f , 1.0f }) ;

    auto shadowtex = textures -> findFirstHolder ( "learnopengl.shadowmapping.depthtexture" ) ;
    if ( shadowtex.isInvalid() ) exit ( -24 ) ;

    emissive -> setTexture ( TechniqueParam::LightTexture0 , shadowtex ) ;
    emissive -> setEmissive ( true ) ;

    //////////////////////////////////////////////////////////////////////
    // Creates a new scene. The used root node will have a size of 1024 by
    // 1024 by 1024 .

    ResourceLoaderOptions sceneops ;
    sceneops["scene.loader"] = std::string ( "scenes.loaders.default" ) ;
    sceneops["scene.root.size"] = Vector3 ( 1024.0f , 1024.0f , 1024.0f ) ;

    auto scene = scenes -> load ( "scene" , sceneops ) ;
    if ( scene.isInvalid() ) exit ( -34 ) ;

    renderpass  -> setScene ( scene ) ;
    renderpass2 -> setScene ( scene ) ;

    //////////////////////////////////////////////////////////////////////
    // Creates the cube node.

    auto cubenode = scene -> create ( "scene.cube" ) ;
    if ( cubenode.isInvalid() ) exit ( -35 ) ;

    cubenode -> translate ( 0.0f , 0.0f , -1.0f ) ;
    cubenode -> scale ( 0.4f , 0.4f , 0.4f ) ;
    cubenode -> setMesh ( teapot ) ;
    cubenode -> setMaterial ( material ) ;

    scene -> add ( cubenode ) ;

    auto cubenode2 = scene -> create ( "scene.cube2" ) ;
    cubenode2 -> translate( 0.0f , -2.0f , -3.0f ) ;
    cubenode2 -> scale ( 10.0f , 0.3f , 10.0f ) ;
    cubenode2 -> setMesh( cube ) ;
    cubenode2 -> setMaterial( material ) ;

    scene -> add ( cubenode2 ) ;

    //////////////////////////////////////////////////////////////////////
    // Creates the light node. In order to achieve our shadowing technique ,
    // we must link the emissive material texture 0 to the framebuffer depth
    // texture. We can recuperate the texture thanks to its name and the
    // texture manager.

    auto lightnode = scene -> create ( "scene.light" ) ;
    if ( lightnode.isInvalid() ) exit ( -36 ) ;

    lightnode -> translate ( 0.0f , 3.0f , -1.0f ) ;
    lightnode -> look ( 0.0f , -1.0f , -1.0f ) ;
    lightnode -> setEmissiveMaterial ( emissive ) ;
    lightnode -> activeViewMatrix ( true ) ;

    scene -> add ( lightnode ) ;
    renderpass2 -> setCamera ( lightnode ) ;

    //////////////////////////////////////////////////////////////////////
    // Creates the camera. The camera has position , direction , but also
    // a projection matrix that will serve the scene. The ratio used for
    // the projection will be set automatically by the controller. If we
    // set a custom ratio , viewport will also be adapted to this ratio.

    auto cameranode = scene -> create ( "scene.camera" ) ;
    if ( cameranode.isInvalid() ) exit ( -37 ) ;

    cameranode -> translate ( 0.0f , 2.0f , 2.0f ) ;
    cameranode -> look ( 0.0f , 0.0f , -1.0f ) ;
    cameranode -> activeViewMatrix ( true ) ;

    scene -> add ( cameranode ) ;
    renderpass -> setCamera ( cameranode ) ;

    //////////////////////////////////////////////////////////////////////
    // Creates a controller to control the camera node when keys are pressed.
    // The 'gre.controllers.defaultkeys' controller is a default controller
    // where you can set keys to control the node. The controller updates the
    // node on update event , launched by the controller manager.

    auto controller = controllers -> create ( "scene.camera.controller" , "gre.controllers.defaultkeys" ) ;
    if ( controller.isInvalid() ) return ;

    controller -> control ( cameranode ) ;
    controller -> listen ( window , { EventType::KeyDown , EventType::KeyUp } ) ;

/*
    tech1 -> getViewport () -> setProjection ( ... ) ;
    tech2 -> getViewport () -> setProjection ( ... ) ;

    node -> look ( ... ) ;

    on render : renderpass -> tech1 -> projection () * cameranode -> getView () * node -> getModel () .
                renderpass -> tech2 -> projection () * cameranode -> getView () * node -> getModel () .
*/
}

int main ( int argc , char ** argv )
{
    GreDebugPretty() << "GRE Version : "
    << localVersion.major << "." << localVersion.minor << "." << localVersion.build << Gre::gendl;

    try
    {

        // Creates an object that we immediatly initialize.
        MyApplicationExample app ;
        app.init( argc , argv ) ;

        // Creates the RenderPass , RenderScene , ...
        if ( !app.createScene() )
        return -2 ;

        app.loadScene () ;

        // Runs the application.
        app.run() ;

    } catch ( const GreException& e )
    {
        GreDebugPretty() << "Exception launched : " << e.what() << Gre::gendl;
        GreDebugPretty() << "Exiting program." << Gre::gendl;
        return -1 ;
    }

    return 0 ;
}
