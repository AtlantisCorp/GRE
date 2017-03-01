//
//  Example3.cpp
//  GRE
//
//  Created by Jacques Tronconi on 08/10/2016.
//
//

#include "ResourceManager.h"

using namespace Gre ;

void displayMatrix ( const Matrix4& mat4 )
{
    GreDebug("[") << mat4[0][0] << ", " << mat4[0][1] << ", " << mat4[0][2] << ", " << mat4[0][3] << gendl ;
    GreDebug(" ") << mat4[1][0] << ", " << mat4[1][1] << ", " << mat4[1][2] << ", " << mat4[1][3] << gendl ;
    GreDebug(" ") << mat4[2][0] << ", " << mat4[2][1] << ", " << mat4[2][2] << ", " << mat4[2][3] << gendl ;
    GreDebug(" ") << mat4[3][0] << ", " << mat4[3][1] << ", " << mat4[3][2] << ", " << mat4[3][3] << "]" << gendl ;
}

// This is an example of a basic Key listener. Every times a key is pressed in any Window, this listener
// will notifiate it. In order to make him listen to every active windows, we listen to the global window
// manager listener.
class BasicKeyListener : public EventProceeder
{
public:
    
    BasicKeyListener ( const CameraHolder & holder , const RenderSceneHolder& sceneholder , const WindowHolder& win )
    : EventProceeder () , mycamera(holder) , linearattenuation(0.0f)
    , scene(sceneholder) , angle ( glm::cos(glm::radians(12.5f)) ) , exp(64.0f) , window(win)
    { }
    
    ~BasicKeyListener () noexcept ( false ) { }
    
protected:
    
    void onLeftMousePressEvent ( const LeftMousePressEvent& event ) {
        GreDebug("[INFO] Left Mouse button pressed.") << gendl ;
    }
    
    void onKeyDownEvent ( const KeyDownEvent& event ) {
        GreDebug("[INFO] Key pressed : '") << (int) event.iKey << "' Modifiers : " << event.iModifiers << " ." << gendl ;
        
        if (event.iKey == Key::Down ) {
            // Backward
            mycamera -> lookAt(mycamera->getPosition() - Vector3({0.0f,0.0f, 0.4f}), mycamera->getTarget());
        }
        
        if ( event.iKey == Key::Up ) {
            // Upward
            mycamera -> lookAt( mycamera->getPosition() + Vector3({0.0f, 0.0f, 0.4f}), mycamera->getTarget() ) ;
        }
        
        if ( event.iKey == Key::Left ) {
            // left
            mycamera -> lookAt(mycamera->getPosition() + Vector3({0.4f, 0.0f, 0.0f}), mycamera->getTarget() ) ;
        }
        
        if ( event.iKey == Key::Right ) {
            mycamera -> lookAt(mycamera->getPosition() - Vector3({0.4f, 0.0f, 0.0f}), mycamera->getTarget() ) ;
        }
        
        if ( event.iKey == Key::A ) {
            linearattenuation += 0.1f ;
            if ( !scene.isInvalid() ) {
                scene.lock() -> getGlobalLight().setAttenuationLinear ( linearattenuation ) ;
            }
        }
        
        if ( event.iKey == Key::S ) {
            linearattenuation -= 0.1f ;
            if ( !scene.isInvalid() ) {
                scene.lock() -> getGlobalLight().setAttenuationLinear ( linearattenuation ) ;
            }
        }
        
        if ( event.iKey == Key::Z ) {
            angle += glm::radians(1.0f) ;
            if ( !scene.isInvalid() ) {
                scene.lock() -> getGlobalLight().setAngle(angle) ;
            }
        }
        
        if ( event.iKey == Key::X ) {
            angle -= glm::radians(1.0f) ;
            if ( !scene.isInvalid() ) {
                scene.lock() -> getGlobalLight().setAngle(angle) ;
            }
        }
        
        if ( event.iKey == Key::Q ) {
            exp += 0.1f ;
            if ( !scene.isInvalid() ) {
                scene.lock() -> getGlobalLight().setExposition(exp);
            }
        }
        
        if ( event.iKey == Key::W ) {
            exp -= 0.1f ;
            if ( !scene.isInvalid() ) {
                scene.lock() -> getGlobalLight().setExposition(exp);
            }
        }
        
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
    
    CameraHolder mycamera ;
    float linearattenuation ;
    RenderSceneUser scene ;
    float angle ;
    float exp ;
    WindowHolder window ;
};

int main ( int argc , char ** argv )
{
    GreDebugPretty() << "GRE Version : "
    << localVersion.major << "." << localVersion.minor << "." << localVersion.build << Gre::gendl;

    try
    {
        // 'Gre::Application' Basic Example.

        // This example should show a basic black Window, with handling of Key pressed events and left mouse
        // button events.
        
        // 1. Load the 'ResourceManager' subsystem .
        ResourceManager::CreateAndInitialize();
        
        // 2. Loads the plugins in './plugins' directory .
        int count = ResourceManager::Get () .loadPluginsIn( "plugins" ) ;
        GreDebug("[INFO] Loaded ") << count << " plugins." << Gre::gendl ;
        
        // 3. Create a 'Gre::Application' object . This Application will be set as a Global Application ,
        // available in the whole program . Only one Application can be set by program .
        ApplicationHolder eApplication = Application::Create( "Example Application" ) ;
        
        if ( eApplication.isInvalid() )
        {
            throw GreExceptionWithText ( "Can't create 'Gre::Application' ." ) ;
        }
        
        eApplication -> addCloseBehaviour ( ApplicationCloseBehaviour::AllWindowClosed ) ;
        eApplication -> addCloseBehaviour ( ApplicationCloseBehaviour::TerminateCalled ) ;
        eApplication -> addCloseBehaviour ( ApplicationCloseBehaviour::EscapeKey ) ;
		
		GreDebug("[INFO] Application created.") << Gre::gendl ;
        
        // Create the key listener and register it.
        EventProceederHolder keylistener = EventProceederHolder ( nullptr ) ;
        
        {
            // Gets some managers.
            auto rmanager = ResourceManager::Get().getRendererManager() ;
            auto smanager = ResourceManager::Get().getRenderSceneManager() ;
            auto wmanager = ResourceManager::Get().getWindowManager() ;
			
			// Creates a Renderer to draw everything.
            auto renderer = rmanager -> load ( "Default Renderer" , {} ) ;
            if ( renderer.isInvalid() ) exit(3) ;
			
			// Creates a Window to render the scene.
            auto window = wmanager -> load ( "Default Window" , 
											{ {"Title", std::string("Default Window")} , 
											  {"Size" , std::string("1024x768")} 
											} ) ;
            if ( window.isInvalid() ) exit(2) ;
			
            // Creates a RenderScene.
            auto scene = smanager -> load ( "Default Scene" , { { "Technique" , std::string("Default") } } ) ;
            if ( scene.isInvalid() ) exit(1) ;
            
            {
                // Assimilate everything. The renderer needs a RenderContext to load shared data (as buffers,
                // textures, ...). This RenderContext will be the main Window.
                
                renderer.lock() -> setRenderContext ( window.lock() -> getRenderContext () ) ;
                
                auto scenelock = scene.lock() ;
                if ( scenelock.isInvalid() ) exit(4) ;
                
                scenelock -> setRenderer ( renderer ) ;
                scenelock -> setRenderTarget ( window ) ;
                scenelock -> setClearColor ( Color(0.4f, 0.4f, 0.4f) ) ;
                
                // Sets a new light for the scene.
                Light globlight = Light(Vector3( 0.0f, 10.0f, -10.0f ) ,
                                        Color ( 0.4f, 0.4f, 0.4f, 1.0f) ,
                                        Color ( 0.5f, 0.5f, 0.5f, 1.0f) ,
                                        Color ( 0.8f, 0.8f, 0.8f, 1.0f) ,
                                        32.0f) ;
                globlight.setType(LightType::Spot) ;
                globlight.setDirection(Vector3( 2.0f, 2.0f, 5.0f ));
                globlight.setAngle(15.0f);
                scenelock -> setGlobalLight( globlight );
                
                // Create our Root node for the scene.
                RenderNodeHolder root = scenelock -> setRootNode(scenelock->createNode()) ;
                root -> setBoundingBox ( BoundingBox({-1000.0f, -1000.0f, -1000.0f} , {1000.0f, 1000.0f, 1000.0f}) ) ;
                
                // Now we will try to load a Cube, and add it to the RenderScene.
                MeshUser cube = MeshManager::Cube ( 2.0f ) ;
                if ( cube.isInvalid() ) exit(4) ;
                
                RenderNodeHolder cubenode = scenelock -> createNode() ;
                if ( cubenode.isInvalid() ) exit(5) ;
                
                cubenode -> setMesh(cube) ;
                cubenode -> translate({-2.0f, 2.0f, 2.0f}) ;
                scenelock -> addNode(cubenode) ;
                
                // Try to load the teapot model and translate it a bit further.
                MeshUser teapot = ResourceManager::Get().getMeshManager()->load("Teapot", "Models/OBJ/Teapot.obj").at(0) ;
                if ( teapot.isInvalid() ) exit(7) ;
                
                RenderNodeHolder teapotnode = scenelock -> createNode() ;
                if ( teapot.isInvalid() ) exit(8) ;
                
                teapotnode -> setMesh(teapot) ;
                teapotnode -> rotate(glm::pi<float>(), Vector3(0.0f, 1.0f, 0.0f)) ;
                teapotnode -> translate({2.0f, 0.0f, 6.0f}) ;
                scenelock -> addNode(teapotnode) ;
                
                // Create a material for the teapot.
                MaterialHolder material = ResourceManager::Get().getMaterialManager() -> create("teapot") ;
                if ( material.isInvalid() ) exit (9) ;
                
                teapotnode -> setMaterial ( material ) ;
                material -> setAmbient(Color(1.0f, 0.5f, 0.31f, 1.0f)) ;
                material -> setDiffuse(Color(1.0f, 0.5f, 0.31f, 1.0f)) ;
                material -> setSpecular(Color(0.5f, 0.5f, 0.5f, 1.0f)) ;
                material -> setShininess(32.0f) ;
                
                // Create a material for the cube.
                MaterialHolder cubemat = ResourceManager::Get().getMaterialManager() -> create("cubemat") ;
                if ( cubemat.isInvalid() ) exit(10) ;
                
                cubenode -> setMaterial(cubemat) ;
                TextureUser cubetext = ResourceManager::Get().getTextureManager() -> load("cubetext", TextureType::Texture2D, "Textures/Cube.png") ;
                if ( cubetext.isInvalid() ) exit(11) ;
                cubemat -> setTexture(cubetext) ;
                cubemat -> setSpecular(Color(0.0f, 0.0f, 0.0f, 0.0f)) ;
                material -> setShininess(0.0f) ;
                
                TextureUser cubespecimg = ResourceManager::Get().getTextureManager()
                -> load("CubeSpecularMap", TextureType::Texture2D, "Textures/CubeSpecular.png") ;
                if ( cubespecimg.isInvalid() ) exit(12) ;
                cubemat -> setSpecularTexture ( cubespecimg ) ;
                cubemat -> setSpecularTextureEnabled ( true ) ;
                
                keylistener =  EventProceederHolder ( new BasicKeyListener(ResourceManager::Get().getCameraManager()->findFirst("Default").lock() , scenelock , window.lock() ) ) ;
                ResourceManager::Get().getWindowManager()->addGlobalKeyListener(keylistener) ;
            }
        }
    
        // Now let the Application loop.
        eApplication -> run () ;
        
        
    } catch ( const GreException& e )
    {
        GreDebugPretty() << "Exception launched : " << e.what() << Gre::gendl;
        GreDebugPretty() << "Exiting program." << Gre::gendl;
        return -1 ;
    }
    
    // Once here, destroy the ResourceManager.
    ResourceManager::Destroy () ;
    return 0 ;
}
