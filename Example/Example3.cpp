//
//  Example3.cpp
//  GRE
//
//  Created by Jacques Tronconi on 08/10/2016.
//
//

#define GreIsExample3

#ifdef GreIsExample3

#include "ResourceManager.h"

using namespace Gre ;

int main ( int argc , char ** argv )
{
    GreDebugPretty() << "GRE Version : "
    << localVersion.major << "." << localVersion.minor << "." << localVersion.build << Gre::gendl;
    
    try
    {
        
        // 'Gre::Application' Basic Example.
        
        // This example should show a basic cube rotating in front of the camera. No controls , only the
        // cross button to close the window. Also , one can easily add some custom functions using Events, but
        // this is not the purpose of this example.
        
        // 1. Load the 'ResourceManager' subsystem .
        ResourceManager::Create () ;
        
        // 2. Loads the plugins in './plugins' directory .
        ResourceManager::Get () .loadPluginsIn( "plugins" ) ;
        
        // 3. Create a 'Gre::Application' object . This Application will be set as a Global Application ,
        // available in the whole program . Only one Application can be set by program .
        ApplicationHolder eApplication = Application::Create( "Example Application" ) ;
        
        if ( eApplication.isInvalid() )
        {
            throw GreExceptionWithText ( "Can't create 'Gre::Application' ." ) ;
        }
        
        eApplication.lock() -> addCloseBehaviour ( ApplicationCloseBehaviour::AllWindowClosed ) ;
        eApplication.lock() -> addCloseBehaviour ( ApplicationCloseBehaviour::TerminateCalled ) ;
        eApplication.lock() -> addCloseBehaviour ( ApplicationCloseBehaviour::EscapeKey ) ;
        
        {
            // 4. Try to create a Renderer.
            
            RendererInfo eRendererInfo ;
            eRendererInfo["Driver.name"] = "OpenGl" ;
            eRendererInfo["Driver.minversion"] = "3.0" ;
            
            RendererUser eRenderer = ResourceManager::Get () .getRendererManager() ->load( "DefaultRenderer" , eRendererInfo ) ;
            
            if ( eRenderer.isInvalid() )
                throw GreExceptionWithText ( "Can't create 'Gre::Renderer' ." ) ;
            
            eRenderer.lock() -> installManagers () ;
            
            if ( !eRenderer.isInstalled() )
                throw GreExceptionWithText ( "Can't install 'Gre::Renderer' ." ) ;
            
            // 5. Try to create a Window.
            
            Surface eWindowSurface ;
            eWindowSurface.top = 100 ;
            eWindowSurface.left = 100 ;
            eWindowSurface.width = 800 ;
            eWindowSurface.height = 600 ;
            
            WindowInfo eWindowInfo ;
            eWindowInfo["Window.title"] = "GRE Example Window" ;
            eWindowInfo["Window.surface"] = eWindowSurface ;
            
            WindowUser eWindow = ResourceManager::Get() .getWindowManager() ->load( "DefaultWindow" , eWindowInfo ) ;
            
            if ( eWindow.isInvalid() )
            {
                throw GreExceptionWithText ( "Can't create 'Gre::Window' ." ) ;
            }
            
            eRenderer.lock() -> registerTarget ( eWindow ) ;
            
            // 6. Try to attach a RenderContext to this Window.
            
            RenderContextInfo eContextInfo ;
            eContextInfo["Context.shared"] = true ;
            
            RenderContextUser eContext = ResourceManager::Get() .getRenderContextManager() ->load( "DefaultContext" , eContextInfo ) ;
            
            if ( eContext.isInvalid() )
            {
                throw GreExceptionWithText ( "Can't create 'Gre::RenderContext' ." ) ;
            }
            
            eContext.lock() -> setClearColor ({ 0.0f , 0.0f , 0.0f , 0.0f });
            eContext.lock() -> setClearDepth ( 1.0f ) ;
            
            eWindow.lock() -> setRenderContext ( eContext ) ;
            
            // 7. Try to create a RenderScene.
            
            RenderSceneUser eScene = ResourceManager::Get() .getRenderSceneManager() ->load( "DefaultRenderScene" ) ;
            
            if ( eScene.isInvalid() )
            {
                throw GreExceptionWithText ( "Can't create 'Gre::RenderScene' ." ) ;
            }
            
            eWindow.lock() -> selectScene ( eScene ) ;
            
            // 8. Try to create a RenderNode to load our cube into.
            
            RenderNodeHolder eNode = eScene.lock() -> createNode ( "CubeNode" ) ;
            
            if ( eNode.isInvalid() )
            {
                throw GreExceptionWithText ( "Can't create 'Gre::RenderNode' ." ) ;
            }
            
            eNode -> setPositioning ( RenderNodePositionning::Relative ) ;
            eNode -> setRelativePosition ({ 1.0f , 1.0f , 1.0f }) ;
            eScene.lock() -> addNode ( eNode ) ;
            
            // 9. Load our cube.
            
            MeshUser eCube = ResourceManager::Get() .getMeshManager() .load( "CubeMesh" , "Resources/obj/Cube.obj" ) ;
            
            if ( eCube.isInvalid() )
            {
                throw GreExceptionWithText ( "Can't create 'Gre::Mesh' ." ) ;
            }
            
            eNodeHolder -> setDrawable ( true ) ;
            eNodeHolder -> setMesh ( eCube ) ;
            
            // 10. Create an update animation for our RenderNode ( here , just rotate the cube ) .
            RenderNodeAnimatorUser eAnimator = ResourceManager::Get() .getAnimatorManager() .load ( "RotationAnimator" , [] (float deltatime , RenderNodeUser node) {
                node.lock() -> rotate ( { 1.0f, 0.0f, 0.0f } , deltatime * 360.0f / 1000.0f ) ;
            }) ;
            
            if ( eAnimator.isInvalid() )
            {
                throw GreExceptionWithText ( "Can't create 'Gre::RenderNodeAnimator' ." ) ;
            }
            
            eAnimator.lock() -> listen ( eNode ) ;
            
            // 11. Create a static Camera.
            CameraUser eCamera = ResourceManager::Get() .getCameraManager() .load ( "StaticCamera" ) ;
            
            if ( eCamera.isInvalid() )
            {
                throw GreExceptionWithText ( "Can't create 'Gre::Camera' ." ) ;
            }
            
            CameraHolder eCameraHolder = eCamera.lock() ;
            
            eCameraHolder -> setPosition ({ -1.0f , -1.0f , -1.0f }) ;
            eCameraHolder -> setTargetPosition ({ 1.0f , 1.0f , 1.0f }) ;
            eScene.lock() -> setCamera ( eCamera ) ;
        }
        
        // 12. Launch the Application loop.
        eApplication.lock() -> loop () ;
        
        // 13. When exiting , release the ResourceManager.
        ResourceManager::Destroy () ;
        
    } catch ( const GreException& e )
    {
        GreDebugPretty() << "Exception launched : " << e.what() << Gre::gendl;
        GreDebugPretty() << "Exiting program." << Gre::gendl;
    }
}


RenderSceneUser mainscene = RenderSceneManager -> create ( "mainscene" ) ;
mainscene -> addMesh ( MeshManager -> createCube ( "cube" , 3.0f ) ) ;
mainscene -> setRenderer ( glrenderer ) ;
mainscene -> setRenderTarget ( window ) ;

RenderTechnique technique = RenderSceneManager -> getDefaultTechnique () ;
technique -> setCamera ( CameraManager -> create ("Default") ) ;
mainscene -> setRenderTechnique ( technique ) ;

Application -> loop ()
{
    if ( !mustExit )
    {
        RenderSceneManager -> drawScenes ()
        {
            RenderScene -> draw ()
            {
                target -> bind () ;
                
                for ( RenderPass : technique )
                {
                    drawPass ( pass )
                    {
                        RenderQuery query ;
                        query -> setRenderScene ( this ) ;
                        query -> setRenderPass ( this ) ;
                        query -> setCamera ( technique -> getCamera() ) ;
                        query -> setRenderedNodes ( technique -> eligibleNodes() ) ;
                        query -> setHardwareProgram ( pass -> hardwareProgram() ) ;
                        query -> setViewport ( technique -> getViewport() ) ;
                        
                        renderer -> drawQuery ( query ) ;
                    }
                }
                
                target -> swapBuffers () ;
                target -> unbind () ;
            }
        }
        
        WindowManager -> pollEvents () ;
    }
}

Application -> paralellLoop ()
{
    if ( !mustExit )
    {
        RenderSceneManager -> updateScenes () ;
    }
}

[...]

#endif
