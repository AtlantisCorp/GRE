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

GRE_BEGIN_NAMESPACE

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
private:
    
    std::map<std::string, std::shared_ptr<Resource>> _resourcesbyname; ///< @brief A Map storing every resources, by name.
    FileLoaderFactory                                _fileloaders;     ///< @brief Factory to create FileLoaders.
    WindowLoaderFactory                              _windowLoaders;   ///< @brief Factory to create WindowLoaders.
    RendererLoaderFactory                            _rendererLoaders; ///< @brief Factory to create RendererLoaders.
    bool                                             _verbose;         ///< @brief Set to true to print more informations.
    
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
            _resourcesbyname[name] = resptr;
            std::cout << "[ResourceManager] Loaded Resource " << name << "." << std::endl;
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
            _resourcesbyname[name] = resptr;
            std::cout << "[ResourceManager] Loaded Resource " << name << "." << std::endl;
        }
        
        delete loader;
        return std::move(ResourceUser(_resourcesbyname[name]));
    }
    
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
    
    /// @brief Set to true if you want verbose mode.
    void setVerbose(bool flag);
    
    /// @brief Loads every plugins in given directory name.
    ///
    /// Plugins loaded are every files finishing with '.so' for Linux,
    /// '.dylib' for OsX, or '.dll' for Windows' users.
    ///
    /// @note The directory must be different from the Working Directory.
    int loadPluginsIn(const std::string& dirname);
};

GRE_END_NAMESPACE
#endif
