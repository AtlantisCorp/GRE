//////////////////////////////////////////////////////////////////////
//
//  ResourceManager.h
//  This source file is part of Gre
//		(Gang's Resource Engine)
//
//  Copyright (c) 2015 - 2017 Luk2010
//  Created on 28/10/2015.
//
//////////////////////////////////////////////////////////////////////
/*
 -----------------------------------------------------------------------------
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 -----------------------------------------------------------------------------
 */

#ifndef GResource_ResourceManager_h
#define GResource_ResourceManager_h

#include "Resource.h"
#include "ResourceBundle.h"

#include "Plugin.h"
#include "Window.h"
#include "Renderer.h"
#include "RenderScene.h"
#include "FrameBuffer.h"
#include "Keyboard.h"
#include "Material.h"
#include "Application.h"
#include "EventDispatcher.h"
#include "Technique.h"
#include "Controller.h"

GreBeginNamespace

////////////////////////////////////////////////////////////////////////
/// @class ResourceManager
/// @brief Manages every Resource, ResourceFactory.
///
/// The ResourceManager is a Singleton, that can be accessed from any point
/// of any application or program linked with the main library.
///
/// ## Application
///
/// A program should always imbricate operations relating to the ResourceManager
/// in a try/catch block. This is a way to destroy everything correctly when
/// having an exception launched.
///
/// Right in the try block, a user should call ResourceManager::Create()
/// BEFORE using it, in order to initialize properly the Singleton.
///
/// At the end of the try block, the user should always call
/// ResourceManager::Destroy() , in order to properly destroy every Resource
/// objects allocated in the ResourceManager, and thus before the end of
/// the try/catch block.
///
/// ## Plugins / Libraries
///
/// As loading a plugin from a GRE program requires to load the ResourceManager , a call
/// to 'ResourceManager::Get()' in that plugin is guaranteed to success.
///
/// ## Uses to load Resources
///
/// Resources are loaded using Manager's . You can also select the right Loader in the Manager,
/// accessing the Loader Factory. Resources are owned by the Managers. Those Managers are also
/// owned by the 'ResourceManager'.
///
/// ## The ResourceManager loading system
///
/// For example, if you want to load a Mesh object. You can either use
/// MeshManager::createFromFile() or use the ResourceManager. The big
/// difference is that the ResourceManager loading system returns to you
/// an object where you have the owning.
///
/// ```c++
/// MeshHolder mymesh = ResourceManager::Get() .loadResourceWith(ResourceManager::Get().getMeshLoaderFactory()..., args...);
/// Mesh mymeshuser = ResourceManager::Get().getMeshManager().add(mymesh);
/// ```
///
/// The ::loadResourceWith() function can take a variable number of args. Those
/// args are dependent of the Loader you will use. Some Loaders wants plenty of
/// args (for options), some only wants the Resource's name.
///
/// Whenever possible, you should use the Manager's functions to load your
/// Resource's, because they handle the ResourceHolder directly.
///
/// ## ResourceManager's holding
///
/// You can make the ResourceManager holds some Resource for some reasons. You
/// can use add() and remove() function in order to register ResourceHolder objects.
/// Those objects will be destroyed when applying a call to ::Destroy(), or ::clear(),
/// or clearResources().
///
/// @see Resource, ResourceFactory
////////////////////////////////////////////////////////////////////////
class DLL_PUBLIC ResourceManager : public Resource
{
public:

    POOLED ( Pools::Manager )

protected:

    /// @brief Renderer Manager.
    RendererManagerHolder iRendererManager;

    /// @brief Window Manager.
    WindowManagerHolder iWindowManager;

    /// @brief RenderScene's Manager.
    RenderSceneManagerHolder iRenderSceneManager;

    /// @brief Material Manager.
    MaterialManagerHolder iMaterialManager;

    /// @brief Plugin Manager.
    PluginManagerHolder iPluginManager;

    /// @brief Mesh Manager.
    MeshManagerHolder iMeshManager;

    /// @brief Texture Manager.
    TextureManagerHolder iTextureManager ;

    /// @brief RenderContext Manager.
    RenderContextManagerHolder iRenderContextManager ;

    /// @brief HardwareProgram Manager.
    HardwareProgramManagerHolder iProgramManager ;

    /// @brief Technique Manager.
    TechniqueManagerHolder iTechniqueManager ;

    /// @brief RenderFramebuffer Manager.
    RenderFramebufferManagerHolder iFramebufferManager ;

    /// @brief Controllers manager.
    ControllerManagerHolder iControllers ;

    /// @brief Application Loaders. As there can be only one Application by process, there is no need to have
    /// an ApplicationManager.
    ApplicationLoaderFactory iApplicationFactory ;

	/// @brief true when the ResourceManager is initialized.
	bool iInitialized ;

    /// @brief Holds every bundled loaded in this Resource Manager. The bundles can
    /// be accessed using appropriates getter/setter.
    std::vector < ResourceBundleHolder > iBundles ;

public:

    /// @brief Holder for ResourceManager.
    typedef Holder<ResourceManager> ResourceManagerHolder ;

    ////////////////////////////////////////////////////////////////////////
    /// @brief Creates a ResourceManager object. If 'setdefault' is true ,
    /// the class singleton will be the created object. Also returns a holder
    /// to the allocated object. Notes this object should be initialized.
    ////////////////////////////////////////////////////////////////////////
    static ResourceManagerHolder Create ( bool setdefault = true ) ;

    ////////////////////////////////////////////////////////////////////////
    /// @brief Clears the global holder managing the resource manager
    /// singleton.
    ////////////////////////////////////////////////////////////////////////
    static void Destroy () ;

    ////////////////////////////////////////////////////////////////////////
    /// @brief Returns the ResourceManager singleton object, or an invalid
    /// holder if no object could be created.
    ////////////////////////////////////////////////////////////////////////
    static ResourceManagerHolder Get();

private:

    /// @brief Default Resource Manager. This object should have been set by the
    /// user using the 'Create()' method.
    static ResourceManagerHolder iManagerHolder ;

private:

    ////////////////////////////////////////////////////////////////////////
    /// @brief Creates the ResourceManager.
    /// By default, the Constructor registers two FileLoader, the TextLoader
    /// (under name "TextLoader") and the PluginLoader (under name "PluginLoader").
    /// The PluginLoader is the only loader which is necessary to the Engine, to
    /// operate normally.
    ////////////////////////////////////////////////////////////////////////
    ResourceManager();

    ////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////
    ResourceManager(const ResourceManager& rhs) = delete;

public:

    ////////////////////////////////////////////////////////////////////////
    /// @brief Destroys the ResourceManager and, all the Resource objects.
    ////////////////////////////////////////////////////////////////////////
    ~ResourceManager() noexcept(false) ;

    ////////////////////////////////////////////////////////////////////////
    /// @brief Initializes every managers.
    ////////////////////////////////////////////////////////////////////////
    void initialize () ;

	////////////////////////////////////////////////////////////////////////
	/// @brief Returns 'iInitialized'.
	////////////////////////////////////////////////////////////////////////
	bool isInitialized () const ;

    ////////////////////////////////////////////////////////////////////////
    /// @brief Unloads the Resource.
    ////////////////////////////////////////////////////////////////////////
    void unload () ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Changes 'iApplicationFactory'.
    //////////////////////////////////////////////////////////////////////
    void setApplicationFactory ( const ApplicationLoaderFactory & appfactory ) ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Return 'iApplicationFactory'.
    //////////////////////////////////////////////////////////////////////
    ApplicationLoaderFactory & getApplicationFactory () ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Changes 'iRendererManager'.
    //////////////////////////////////////////////////////////////////////
    void setRendererManager ( const RendererManagerHolder & rendmanager ) ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Returns 'iRendererManager'.
    //////////////////////////////////////////////////////////////////////
    RendererManagerHolder getRendererManager () ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Changes 'iWindowManager'.
    //////////////////////////////////////////////////////////////////////
    void setWindowManager ( const WindowManagerHolder & winmanager ) ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Returns 'iWindowManager'.
    //////////////////////////////////////////////////////////////////////
    WindowManagerHolder getWindowManager () ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Changes 'iRenderSceneManager'.
    //////////////////////////////////////////////////////////////////////
    void setRenderSceneManager ( const RenderSceneManagerHolder & rsceneholder ) ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Returns 'iRenderSceneManager'.
    //////////////////////////////////////////////////////////////////////
    RenderSceneManagerHolder getRenderSceneManager () ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Changes 'iMeshManager'.
    //////////////////////////////////////////////////////////////////////
    void setMeshManager ( const MeshManagerHolder & mmholder ) ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Returns 'iMeshManager'.
    //////////////////////////////////////////////////////////////////////
    MeshManagerHolder getMeshManager () ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Changes 'iTextureManager'.
    //////////////////////////////////////////////////////////////////////
    void setTextureManager ( const TextureManagerHolder & manager ) ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Returns 'iTextureManager'.
    //////////////////////////////////////////////////////////////////////
    TextureManagerHolder getTextureManager () ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Changes 'iRenderContextManager'.
    //////////////////////////////////////////////////////////////////////
    void setRenderContextManager ( const RenderContextManagerHolder & manager ) ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Returns 'iRenderContextManager'.
    //////////////////////////////////////////////////////////////////////
    RenderContextManagerHolder getRenderContextManager () ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Changes 'iProgramManager'.
    //////////////////////////////////////////////////////////////////////
    void setHardwareProgramManager ( const HardwareProgramManagerHolder & manager ) ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Returns 'iProgramManager'.
    //////////////////////////////////////////////////////////////////////
    HardwareProgramManagerHolder getHardwareProgramManager () ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Changes 'iMaterialManager'.
    //////////////////////////////////////////////////////////////////////
    void setMaterialManager ( const MaterialManagerHolder& manager ) ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Returns 'iMaterialManager'.
    //////////////////////////////////////////////////////////////////////
    MaterialManagerHolder getMaterialManager () ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Changes 'iPluginManager'.
    //////////////////////////////////////////////////////////////////////
    void setPluginManager ( const PluginManagerHolder& manager ) ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Returns 'iPluginManager'.
    //////////////////////////////////////////////////////////////////////
    PluginManagerHolder getPluginManager () ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Changes 'iTechniqueManager'.
    //////////////////////////////////////////////////////////////////////
    void setTechniqueManager ( const TechniqueManagerHolder& manager ) ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Returns 'iPluginManager'.
    //////////////////////////////////////////////////////////////////////
    TechniqueManagerHolder getTechniqueManager () ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Changes 'iFramebufferManager'.
    //////////////////////////////////////////////////////////////////////
    void setFramebufferManager ( const RenderFramebufferManagerHolder& manager ) ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Returns 'iFramebufferManager'.
    //////////////////////////////////////////////////////////////////////
    RenderFramebufferManagerHolder getFramebufferManager () ;

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    void setControllerManager ( const ControllerManagerHolder & holder ) ;

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    ControllerManagerHolder getControllerManager () ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Creates a new bundle with given name.
    //////////////////////////////////////////////////////////////////////
    ResourceBundleHolder addBundle ( const std::string & name ) ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Creates an Application object using the first loader registered
    /// in the ApplicationFactory. Notes that there should be only one loader
    /// registered in this factory.
    //////////////////////////////////////////////////////////////////////
    ApplicationHolder createApplication (const std::string & name ,
                                         const std::string & author = std::string () ,
                                         const std::string & description = std::string () );

    //////////////////////////////////////////////////////////////////////
    /// @brief Tries to find a file among the loaded bundles. Returns the
    /// filepath if found , or an empty string.
    //////////////////////////////////////////////////////////////////////
    std::string findBundledFile (const ResourceType & type ,
                                 const std::string & filename) const ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Returns every bundles.
    //////////////////////////////////////////////////////////////////////
    const std::vector < ResourceBundleHolder > & getBundles () const ;
};

/// @brief Holder for ResourceManager.
typedef Holder<ResourceManager> ResourceManagerHolder ;

GreEndNamespace
#endif
