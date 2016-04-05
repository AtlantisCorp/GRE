//
//  ResourceManager.h
//  GResource
//
//  Created by Jacques Tronconi on 28/10/2015.
//  Copyright (c) 2015 Atlanti's Corporation. All rights reserved.
//

#ifndef GResource_ResourceManager_h
#define GResource_ResourceManager_h

#include "Resource.h"
#include "TextResource.h"
#include "Plugin.h"
#include "Window.h"
#include "Renderer.h"
#include "MeshLoader.h"
#include "Scene.h"
#include "FrameBuffer.h"
#include "LoopBehaviours.h"

GreBeginNamespace

////////////////////////////////////////////////////////////////////////
/// @class ResourceManager
/// @brief Manages every Resource, ResourceFactory.
///
/// The ResourceManager is a Singleton, that can be accessed from any point
/// of any application or program linked with the main library.
///
/// # Application
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
/// # Plugins / Libraries
///
/// You can directly use the ResourceManager, but a call to ResourceManager::Get()
/// is not guaranteed to be successfull, because the main program normally should
/// have called ResourceManager::Create(), but that is not sure. You also
/// can imbricate your calls in a try/catch block.
///
/// @see Resource, ResourceFactory
////////////////////////////////////////////////////////////////////////
class DLL_PUBLIC ResourceManager
{
public:
    
    /// @brief An object that generate names for a resource.
    /// Example : You want to add 10 resources with name "MyName", it will generate
    /// automaticly "MyName", "MyName0", "MyName1", ... "MyName8"
    class NameGenerator
    {
    public:
        
        NameGenerator();
        ~NameGenerator();
        
        /// @return A Name based on current used copies.
        std::string generateName(const std::string& base = "default");
        
    private:
        
        std::map<std::string, unsigned> _mUsedNames;
    };
    
private:
    
    std::map<Resource::Type, std::vector<std::shared_ptr<Resource>>> _resourcesbytype; ///< @brief Stores every resources by type.
    std::map<std::string, std::weak_ptr<Resource>>  _resourcesbyname; ///< @brief A Map storing every resources, by name.
    FileLoaderFactory                               _fileloaders;     ///< @brief Factory to create FileLoaders.
    WindowLoaderFactory                             _windowLoaders;   ///< @brief Factory to create WindowLoaders.
    RendererLoaderFactory                           _rendererLoaders; ///< @brief Factory to create RendererLoaders.
    MeshLoaderFactory                               _meshLoaders;     ///< @brief Factory to create MeshLoaders.
    ImageLoaderFactory                              _imageLoaders;    ///< @brief Creates ImageLoader.
    SceneLoaderFactory                              _sceneLoaders;    ///< @brief Creates SceneLoader objects.
    HardwareShaderLoaderFactory                     _shaderLoaders;   ///< @brief Creates HardwareShaderLoader objects.
    HardwareProgramManagerLoaderFactory             _progManLoaders;  ///< @brief Creates HardwareProgramManagerLoader objects.
    FrameBufferLoaderFactory                        _fboLoaders;      ///< @brief Creates FrameBufferLoader objects.
    NameGenerator                                   _nameGenerator;   ///< @brief Utility class to create Names.
    KeyboardLoader                                  _keyboardLoader;  ///< @brief A simple Keyboard Loader.
    
    bool                                            _verbose;         ///< @brief Set to true to print more informations.
    
    /// @brief Determines when the ResourceManager should stop the Loop.
    CloseBehaviour _mCloseBehaviour;
    
    /// @brief Holds LoopBehaviour's functions.
    LoopBehaviours _mLoopBehaviours;
    
    /// @brief Holds LoopBehaviour's functions for PerWindow loop.
    LoopBehaviours _mPerWindowBehaviours;
    
    /// @brief A property that take effect only when the Loop has started.
    /// When the Loop starts, this property is set to false. When the User
    /// or any other events calls ResourceManager::stop(), this property is
    /// set to true.
    bool _mMustStopLoop;
    
public:
    
    POOLED(Pools::Manager)
    
    /// @brief Creates the ResourceManager singleton object.
    /// @note This function should be called only once per program. Behaviour
    /// is undefined when called more times.
    static void Create();
    
    /// @brief Destroys the ResourceManager object.
    /// @note This function should be called only once per program. Behaviour
    /// is undefined when called more times.
    static void Destroy();
    
    /// @brief Returns the ResourceManager singleton object.
    /// @note ResourceManager::Create() must have been called before using
    /// this function.
    static ResourceManager& Get();
    
private:
    
    /// @brief Creates the ResourceManager.
    /// By default, the Constructor registers two FileLoader, the TextLoader
    /// (under name "TextLoader") and the PluginLoader (under name "PluginLoader").
    /// The PluginLoader is the only loader which is necessary to the Engine, to
    /// operate normally.
    ResourceManager();
    
    ResourceManager(const ResourceManager& rhs) = delete;
    
public:
    
    /// @brief Destroys the ResourceManager and, using RAII, all the Resource objects.
    ~ResourceManager ();
    
    /// @brief Adds a new Resource, created by the user.
    /// This resource is allocated by the user, but freed by the ResourceManager.
    ResourceUser addResource(Resource::Type type, const std::string& name, std::shared_ptr<Resource> resource);
    
    /// @brief Loads a Resource giving its type, and its name.
    /// @note This function is only here to give an example function. In fact, it does
    /// nothing but creating an empty Resource with a name. You shouldn't use this function,
    /// unless you want to create empty blank Resource object.
    ResourceUser loadResource(Resource::Type type, const std::string& name);
    
    /// @brief Loads a Resource object using a Loader, the type of the Resource, and the Args used
    /// by the Loader to load the Resource.
    /// @note The Arg awaited by this function are dependant to the Loader given.
    /// @see ResourceLoader
    template <class RLoader, class... Args>
    ResourceUser loadResourceWith(RLoader loader, Resource::Type type, const std::string& name, Args&&... args)
    {
        std::shared_ptr<Resource> resptr(loader.load(type, name, std::forward<Args>(args)...));
        if(resptr) {
            resptr->_type = type;
            _resourcesbytype[type].push_back(resptr);
            _resourcesbyname[name] = resptr;
            GreDebugPretty() << " Loaded Resource " << name << "." << std::endl;
        }
        return std::move(ResourceUser(_resourcesbyname[name]));
    }
    
    /// @brief Loads a Resource object using a Loader pointer, the type of the Resource, and the Args used
    /// by the Loader to load the Resource.
    /// @note The Arg awaited by this function are dependant to the Loader given.
    /// @note Usually, the loader pointer is given using one of the Factory registered in the Engine. The pointer
    /// is ALWAYS destroyed in this function, because the Factory object allocate it. This function assume the
    /// loader is created by either the user or the Factory, and it take responsability of the deleting.
    /// @see ResourceLoader
    template <class RLoader, class... Args>
    ResourceUser loadResourceWith(RLoader* loader, Resource::Type type, const std::string& name, Args&&... args)
    {
        std::shared_ptr<Resource> resptr(loader->load(type, name, std::forward<Args>(args)...));
        if(resptr) {
            resptr->_type = type;
            _resourcesbytype[type].push_back(resptr);
            _resourcesbyname[name] = resptr;
            GreDebugPretty() << "Loaded Resource " << name << "." << std::endl;
        }
        
        delete loader;
        return std::move(ResourceUser(_resourcesbyname[name]));
    }
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the first Resource that has given name.
    //////////////////////////////////////////////////////////////////////
    ResourceUser findResourceByName(const std::string& name);
    
    /// @brief Unload Resource giving its name.
    /// @note This function does not destroy the ResourceUser objects, but
    /// they will be invalidated (ResourceUser::isExpired() should return true).
    void unloadResource(const std::string& name);
    
    /// @brief Unload every Resource objects.
    void clear ();
    
    /// @brief Returns the Memory used by every ResourceObject, using the Pool system.
    unsigned getResourceUsage() const;
    
    /// @brief Returns the FileLoader Factory.
    FileLoaderFactory& getFileLoaderFactory();
    /// @brief Returns the WindowLoader Factory.
    WindowLoaderFactory& getWindowLoaderFactory();
    /// @brief Returns the RendererLoader Factory.
    RendererLoaderFactory& getRendererLoaderFactory();
    /// @brief Returns the MeshLoader Factory
    MeshLoaderFactory& getMeshLoaderFactory();
    /// @brief Returns the ImageLoader Factory.
    ImageLoaderFactory& getImageLoaderFactory();
    /// @brief Returns the SceneLoader Factory.
    SceneLoaderFactory& getSceneLoaderFactory();
    /// @brief Returns the NameGenerator object.
    NameGenerator& getNameGenerator();
    /// @brief Returns the HardwareShaderLoader Factory.
    HardwareShaderLoaderFactory& getHardwareShaderLoaderFactory();
    /// @brief Returns the HardwareProgramManagerLoader Factory.
    HardwareProgramManagerLoaderFactory& getHardwareProgramManagerLoaderFactory();
    /// @brief Returns the FrameBufferLoader Factory.
    FrameBufferLoaderFactory& getFrameBufferLoaderFactory();
    /// @brief Returns the Keyboard Loader.
    KeyboardLoader& getKeyboardLoader();
    
    /// @brief Set to true if you want verbose mode.
    void setVerbose(bool flag);
    
    /// @brief Loads every plugins in given directory name.
    ///
    /// Plugins loaded are every files finishing with '.so' for Linux,
    /// '.dylib' for OsX, or '.dll' for Windows' users.
    ///
    /// @note The directory must be different from the Working Directory.
    int loadPluginsIn(const std::string& dirname);
    
    ////////////////////////////////////////////////////////////////////////
    /// @brief Set the CloseBehaviour.
    ////////////////////////////////////////////////////////////////////////
    void setCloseBehaviour(const CloseBehaviour& behaviour);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Adds a loop behaviour function.
    //////////////////////////////////////////////////////////////////////
    void addLoopBehaviour(LoopBehaviour behaviour);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Erases every Loop Behaviours.
    //////////////////////////////////////////////////////////////////////
    void clearLoopBehaviour();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Stops the current loop processing.
    /// This is done by simply setting the property _mMustStopLoop to true.
    //////////////////////////////////////////////////////////////////////
    void stop();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Updates the Window objects. 
    //////////////////////////////////////////////////////////////////////
    void loop();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Creates a Resource that should be used only for Event handling/
    /// sending.
    //////////////////////////////////////////////////////////////////////
    ResourceUser createPureListener(const std::string& name);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Creates a simple Keyboard Resource.
    //////////////////////////////////////////////////////////////////////
    Keyboard createKeyboard(const std::string& name);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Unites some Utilities function to help the user do some
    /// basics things.
    //////////////////////////////////////////////////////////////////////
    class DLL_PUBLIC Helper
    {
    public:
        
        //////////////////////////////////////////////////////////////////////
        /// @brief Let the final user choose a RendererLoader using Console
        /// prompting.
        ///
        /// It returns the name of the Chosen Renderer.
        //////////////////////////////////////////////////////////////////////
        static std::string ChooseRenderer(RendererLoaderFactory& rFactory);
        
        //////////////////////////////////////////////////////////////////////
        /// @brief Load a Renderer object using Renderer Name, Loader Name
        /// and the RendererLoaderFactory.
        //////////////////////////////////////////////////////////////////////
        static Renderer LoadRenderer(const std::string& rname, const std::string& lname, RendererLoaderFactory& rFactory);
        
        //////////////////////////////////////////////////////////////////////
        /// @brief Let the final user choose a WindowLoader using Console
        /// prompting.
        ///
        /// It returns the name of the Chosen Window.
        //////////////////////////////////////////////////////////////////////
        static std::string ChooseWindowLoader(WindowLoaderFactory& wFactory);
        
        //////////////////////////////////////////////////////////////////////
        /// @brief Load a Window Object using its Name, Loader Name, Window
        /// Loader Factory, and a Surface that corresponds to the future size
        /// of the Window.
        //////////////////////////////////////////////////////////////////////
        static Window LoadWindow(const std::string& wname, const std::string& lname, WindowLoaderFactory& wFactory, const Surface& param);
    };
};

GreEndNamespace
#endif
