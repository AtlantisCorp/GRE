//
//  Example3.cpp
//  GRE
//
//  Created by Jacques Tronconi on 08/10/2016.
//
//

#include "ResourceManager.h"
//#include "ShadowMapping.h"

//#include <unistd.h>

using namespace Gre ;

// This is an example of a basic Key listener. Every times a key is pressed in any Window, this listener
// will notifiate it. In order to make him listen to every active windows, we listen to the global window
// manager listener.
class BasicKeyListener : public EventProceeder
{
public:

    BasicKeyListener ( const RenderSceneHolder& sceneholder , const WindowHolder& win )
    : EventProceeder () , linearattenuation(0.0f)
    , scene(sceneholder) , angle ( glm::radians(12.5f) ) , exp(64.0f) , window(win)
    { }

    ~BasicKeyListener () noexcept ( false ) { }

protected:

    void onLeftMousePressEvent ( const LeftMousePressEvent& event ) {
        GreDebug("[INFO] Left Mouse button pressed.") << gendl ;
    }

    void onKeyDownEvent ( const KeyDownEvent& event ) {
        GreDebug("[INFO] Key pressed : '") << (int) event.iKey << "' Modifiers : " << event.iModifiers << " ." << gendl ;

        if ( event.iKey == Key::P ) {
            window -> setCursorCentered(true) ;
        }

        if ( event.iKey == Key::O ) {
            window -> setCursorCentered(false) ;
        }

        if ( event.iKey == Key::I ) {
            window -> toggleCursor(true) ;
        }

        if ( event.iKey == Key::U ) {
            window -> toggleCursor(false) ;
        }
    }

    float linearattenuation ;
    RenderSceneUser scene ;
    float angle ;
    float exp ;
    WindowHolder window ;
};

class MyApplicationExample
{

public:

    MyApplicationExample ( )
    : iApplication(nullptr) , iListener(nullptr)
    {

    }

    ~MyApplicationExample ()
    {

    }

    static void DisplayMatrix ( const Matrix4& mat4 ) ;

    void init ( int argc , char ** argv ) ;

    void createScene () ;

    void run () ;

protected:

    ApplicationHolder iApplication ;
    EventProceederHolder iListener ;

    Gre::ResourceManagerHolder rmanager ;
};

void MyApplicationExample::DisplayMatrix(const Matrix4 &mat4)
{
    GreDebug("[") << mat4[0][0] << ", " << mat4[0][1] << ", " << mat4[0][2] << ", " << mat4[0][3] << gendl ;
    GreDebug(" ") << mat4[1][0] << ", " << mat4[1][1] << ", " << mat4[1][2] << ", " << mat4[1][3] << gendl ;
    GreDebug(" ") << mat4[2][0] << ", " << mat4[2][1] << ", " << mat4[2][2] << ", " << mat4[2][3] << gendl ;
    GreDebug(" ") << mat4[3][0] << ", " << mat4[3][1] << ", " << mat4[3][2] << ", " << mat4[3][3] << "]" << gendl ;
}

void MyApplicationExample::init ( int argc , char** argv )
{
    // Creating a GRE application requires three basic steps.
    // Creating the ResourceManager, loading plugins and creating an Application object.

	// Creating the ResourceManager only allocate a new object. This one must be initialized using
	// the 'ResourceManager::initialize()' function. The 'initialize()' function only loads every
	// managers present in the ResourceManager object. Notes the 'Create()' function also set the
	// resource manager 's singleton.

    rmanager = Gre::ResourceManager::Create () ;
	rmanager -> initialize () ;

	// Then , we can set some informations like the default plugin directory and the SDK
	// directory. Those will be included when loading some resources. A ResourceBundle is an object
	// responsible for holding some pathes to look for each resource's type. In this example , the
    // SDK files are located directly in our directory.

	Gre::ResourceBundleHolder rbundle = rmanager -> addBundle ( "DefaultSdk" ) ;
	rbundle -> addDirectory ( Gre::ResourceType::Plugin ,  "Plugins" ) ;
	rbundle -> addDirectory ( Gre::ResourceType::Program , "Programs" ) ;
	rbundle -> addDirectory ( Gre::ResourceType::Texture , "Textures" ) ;
	rbundle -> addDirectory ( Gre::ResourceType::Mesh ,    "Models" ) ;
	rbundle -> addDirectory ( Gre::ResourceType::Effect ,  "Effects" ) ;

	// Now , try to load every plugins in bundles. Those plugins should be loaded only if the
	// plugins dependencies are respected. Each plugin should be identified using a unique UUID
	// identifier. This identifier is used to look for a plugin.

	Gre::PluginManagerHolder pm = rmanager -> getPluginManager () ;
	if ( pm -> loadFromBundle(rbundle) == 0 ) {
		GreDebug ( "[ERRO] No plugins found in bundle '") << rbundle -> getName () << "'." << Gre::gendl ;
		exit ( -1 ) ;
	}

	// Now , create our application object. This application should be initialized after allocation.

	Gre::ApplicationHolder app = rmanager -> createApplication ( "Example Application" ) ;
	if ( app.isInvalid() ) {
		GreDebug ( "[ERRO] Can't create Application 'Example Application'.") << Gre::gendl ;
		exit ( -2 ) ;
    }

    app -> addCloseBehaviour(ApplicationCloseBehaviour::AllWindowClosed) ;
    app -> addCloseBehaviour(ApplicationCloseBehaviour::EscapeKey) ;
    app -> addCloseBehaviour(ApplicationCloseBehaviour::TerminateCalled) ;

    app -> initialize ( argc , argv ) ;
    iApplication = app ;
}

void MyApplicationExample::createScene()
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

    auto rmanager = ResourceManager::Get()->getRendererManager() ;
    auto smanager = ResourceManager::Get()->getRenderSceneManager() ;
    auto wmanager = ResourceManager::Get()->getWindowManager() ;
    auto tmanager = ResourceManager::Get()->getTechniqueManager() ;
    auto cmanager = ResourceManager::Get()->getCameraManager() ;

    ResourceLoaderOptions WinOptions ;
    WinOptions ["Title"] = std::string ( "Default Window" ) ;
    WinOptions ["Size"]  = std::string ( "1024x768" ) ;

    auto window = wmanager -> load ( "Default Window" , WinOptions ) ;
    if ( window.isInvalid() ) exit( -3 ) ;

    //////////////////////////////////////////////////////////////////////
    // Creates a Renderer. As the first loader will be used , this one should
    // be compatible with the Window's API used.

    ResourceLoaderOptions RendOptions ;

    auto renderer = rmanager -> load ( "Default Renderer" , RendOptions ) .lock() ;
    if ( renderer.isInvalid() ) exit( -4 ) ;

    renderer -> setRenderContext ( window.lock() -> getRenderContext () ) ;
    renderer -> installManagers() ;

    //////////////////////////////////////////////////////////////////////
    // Tries to load techniques using the bundles added.

    int tec = tmanager -> loadFromBundles ( ResourceManager::Get() -> getBundles() ) ;
    GreDebug ( "[INFO] " ) << tec << " techniques loaded." << Gre::gendl ;

    //////////////////////////////////////////////////////////////////////
    // Loads a RenderScene.

    ResourceLoaderOptions SceneOptions ;

    auto scene = smanager -> load ( "Default Scene" , SceneOptions ) .lock() ;
    if ( scene.isInvalid() ) exit( -5 ) ;

    {
        // Rendering in GRE in the viewport at the left. We need to create a 'RenderPass' which is an object
        // that hols everything for the renderer to render the 'pass'. Notes that a pass can be a multi-framebuffer
        // or a multi-technique drawing (the scene may be rendered more than once) , as the technique may have some
        // pre or post render techniques.

        auto renderpass = renderer -> addPass ( "RenderPass1" ) ;
        renderpass -> setRenderTarget ( window.lock() ) ;
        renderpass -> setViewport ({ 0.0f , 0.0f , 0.5f , 1.0f });
        renderpass -> setClearColor ( Color(0.4f, 0.8f, 0.4f) ) ;
        renderpass -> setScene ( scene ) ;
        renderpass -> setClearViewport ( true ) ;

        //////////////////////////////////////////////////////////////////////
        // Tries to get the 'BlinnPhongTechnique' technique.

        auto blinnphongtech = tmanager -> get ( "BlinnPhongTechnique" ) ;
        if ( blinnphongtech.isInvalid() ) exit( -6 ) ;

        renderpass -> setTechnique ( blinnphongtech ) ;

        //////////////////////////////////////////////////////////////////////
        // Tries to create an FPS-like Camera.

        ResourceLoaderOptions options ;
        options["Loader"] = std::string("FreeMovingCameraLoader") ;
        options["Position"] = Vector3 ( 0.0f, 0.0f, 0.0f ) ;
        options["Direction"] = Vector3 ( 0.0f, 0.0f, 1.0f ) ;

        auto camera = cmanager -> load ( "Camera1" , options ) .lock() ;
        camera -> setController ( window ) ;
        scene -> addCamera ( camera ) ;
        renderpass -> setCamera ( camera ) ;

        //////////////////////////////////////////////////////////////////////
        // Adds a second pass where we draw from another camera , but with a different viewport. We simply copy the
        // created pass and change the viewport to begin at 50% left.

        auto renderpass2 = renderer -> copyPass ( renderpass ) ;
        renderpass2 -> setViewport ({ 0.5f , 0.0f , 1.0f , 1.0f });
        renderpass2 -> setClearColor ({ 0.4f , 0.4f , 0.8f }) ;
    }

    {
        auto mmanager = ResourceManager::Get()->getMeshManager() ;
        auto textures = ResourceManager::Get() -> getTextureManager() ;
        
        // Create our Root node for the scene.
        RenderNodeHolder root = scene -> setRootNode(scene->createNode()) ;
        root -> setBoundingBox ( BoundingBox({-1000.0f, -1000.0f, -1000.0f} , {1000.0f, 1000.0f, 1000.0f}) ) ;

        //////////////////////////////////////////////////////////////////////
        // Adds a directionnal light to the scene.

        LightRenderNodeHolder lightnode = scene -> createLightNode ( "Directionnal Light" ) ;
        if ( lightnode.isInvalid() ) exit ( -7 ) ;

        lightnode -> loadLightCamera () ;
        lightnode -> loadShadowTexture ( 1024 , 1024 ) ;

        LightHolder & light = lightnode -> getLight () ;
        light -> setAmbient ({ 0.6f , 0.3f , 0.0f }) ;
        light -> setDiffuse ({ 1.0f , 0.5f , 0.0f }) ;
        light -> setSpecular ({ 0.0f , 1.0f , 0.0f }) ;
        light -> setPosition ({ -10.0f , 10.0f , 10.0f }) ;
        light -> setDirection ({ 1.0f , -1.0f , -1.0f }) ;
        scene -> addLightNode ( lightnode ) ;

        //////////////////////////////////////////////////////////////////////
        // Loads 'Cube.obj' and creates a node to render the 'Cube' mesh.

        ResourceLoaderOptions ops ;
        int meshes = mmanager -> loadBundledFile ( "Cube.obj" , ops ) ;
        if ( !meshes ) exit ( -8 ) ;
        
        MeshHolder cubemesh = mmanager -> get ( "Cube" ) ;
        if ( cubemesh.isInvalid() ) exit ( -9 ) ;

        RenderNodeHolder cubenode = scene -> createNode ( "Cube Mesh Node" ) ;
        if ( cubenode.isInvalid() ) exit ( -10 ) ;

        cubenode -> setMesh ( cubemesh ) ;
        scene -> addNode ( cubenode ) ;

        //////////////////////////////////////////////////////////////////////
        // Loads the textures 'Cube.png' for diffuse texture and 'CubeSpecular.png'
        // for specular texture.

        TextureHolder cubetex = textures -> loadBundledFile ( "Cube.tex" , "Cube.png" ,
                                                              TextureType::Texture2D , ops ) ;
        if ( cubetex.isInvalid() ) exit ( -11 ) ;
        TextureHolder cubespectex = textures -> loadBundledFile ( "Cube.spectex" , "CubeSpecular.png" ,
                                                                  TextureType::Texture2D , ops ) ;
        if ( cubespectex.isInvalid() ) exit ( -12 ) ;

        cubemesh -> getDefaultMaterial () -> setDiffuseTexture ( cubetex ) ;
        cubemesh -> getDefaultMaterial () -> setSpecularTexture ( cubespectex ) ;

        //////////////////////////////////////////////////////////////////////
        // Configure the node's position and scale.

        cubenode -> setPosition ({ 0.0f , 2.0f , -4.0f }) ;
        cubenode -> setScale ( 4.0f ) ;
    }
}

void MyApplicationExample::run()
{
    iApplication -> run() ;
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
        app.createScene() ;

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
