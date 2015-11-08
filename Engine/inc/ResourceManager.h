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

class DLL_PUBLIC ResourceManager
{
private:
    
    std::map<std::string, std::shared_ptr<Resource>> _resourcesbyname;
    FileLoaderFactory                                _fileloaders;
    WindowLoaderFactory                              _windowLoaders;
    RendererLoaderFactory                            _rendererLoaders;
    bool                                             _verbose;
    
public:
    
    POOLED(Pools::Manager)
    
    static void Create();
    static void Destroy();
    
    static ResourceManager& Get();
    
private:
    
    ResourceManager();
    
    ResourceManager(const ResourceManager& rhs) = delete;
    
public:
    
    ~ResourceManager ();
    
    ResourceUser loadResource(Resource::Type type, const std::string& name);
    
    template <class RLoader, class... Args>
    ResourceUser loadResourceWith(RLoader loader, Resource::Type type, const std::string& name, Args&&... args)
    {
        std::shared_ptr<Resource> resptr(loader.load(type, name, std::forward<Args>(args)...));
        if(resptr) {
            _resourcesbyname[name] = resptr;
            std::cout << "[ResourceManager] Loaded Resource " << name << "." << std::endl;
        }
        
        return ResourceUser(_resourcesbyname[name]);
    }
    
    template <class RLoader, class... Args>
    ResourceUser loadResourceWith(RLoader* loader, Resource::Type type, const std::string& name, Args&&... args)
    {
        std::shared_ptr<Resource> resptr(loader->load(type, name, std::forward<Args>(args)...));
        if(resptr) {
            _resourcesbyname[name] = resptr;
            std::cout << "[ResourceManager] Loaded Resource " << name << "." << std::endl;
        }
        
        delete loader;
        return ResourceUser(_resourcesbyname[name]);
    }
    
    void unloadResource(const std::string& name);
    
    void clear ();
    
    unsigned getResourceUsage() const;
    
    FileLoaderFactory& getFileLoaderFactory();
    WindowLoaderFactory& getWindowLoaderFactory();
    RendererLoaderFactory& getRendererLoaderFactory();
    
    void setVerbose(bool flag);
    
    int loadPluginsIn(const std::string& dirname);
};

#endif
