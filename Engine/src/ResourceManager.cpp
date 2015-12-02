//
//  ResourceManager.cpp
//  GResource
//
//  Created by Jacques Tronconi on 01/11/2015.
//  Copyright (c) 2015 Atlanti's Corporation. All rights reserved.
//

#include "ResourceManager.h"

GRE_BEGIN_NAMESPACE

ResourceManager* _manager = nullptr;

ResourceManager::NameGenerator::NameGenerator()
{
    
}

ResourceManager::NameGenerator::~NameGenerator()
{
    
}

std::string ResourceManager::NameGenerator::generateName(const std::string& base)
{
    if(_mUsedNames.find(base) == _mUsedNames.end()) {
        _mUsedNames[base] = 1;
        return base;
    } else {
        _mUsedNames[base]++;
        return base + std::to_string(_mUsedNames[base] - 1);
    }
}

void ResourceManager::Create() {
    _manager = new ResourceManager;
}

void ResourceManager::Destroy() {
    delete _manager;
}
    
ResourceManager& ResourceManager::Get() {
    return *_manager;
}

ResourceManager::ResourceManager()
{
    if(_verbose) {
        std::cout << "[ResourceManager] Constructing ResourceManager." << std::endl;
    }
    
    _fileloaders.registers("TextLoader", new TextLoader());
    _fileloaders.registers("PluginLoader", new PluginLoader());
    _meshLoaders.registers("DefaultLoader", new MeshLoader());
    _verbose = false;
}

ResourceManager::~ResourceManager ()
{
    if(_verbose) {
        std::cout << "[ResourceManager] Destroying ResourceManager." << std::endl;
    }
    
    _fileloaders.clear();
    _windowLoaders.clear();
    _rendererLoaders.clear();
    _meshLoaders.clear();
    
    _resourcesbyname.clear();
    _resourcesbytype[Resource::Type::Text].clear();
    _resourcesbytype[Resource::Type::HwdBuffer].clear();
    _resourcesbytype[Resource::Type::Mesh].clear();
    _resourcesbytype[Resource::Type::Renderer].clear();
    _resourcesbytype[Resource::Type::Window].clear();
    _resourcesbytype[Resource::Type::Plugin].clear();
    _resourcesbytype[Resource::Type::Null].clear();
}

ResourceUser ResourceManager::addResource(Resource::Type type, const std::string& name, std::shared_ptr<Resource> resource)
{
    if(!resource)
        return ResourceUser::Null;
    
    _resourcesbytype[type].push_back(resource);
    _resourcesbyname[name] = resource;
    return ResourceUser(resource);
}

ResourceUser ResourceManager::loadResource(Resource::Type type, const std::string& name) {
    _resourcesbyname[name] = std::shared_ptr<Resource>(new Resource (name));
    
    if(_verbose) {
        std::cout << "[ResourceManager] Loaded Resource '" << name << "'." << std::endl;
    }
    
    return ResourceUser(_resourcesbyname[name]);
}

void ResourceManager::unloadResource(const std::string& name) {
    _resourcesbyname.erase(name);
    
    if(_verbose) {
        std::cout << "[ResourceManager] Unloaded Resource '" << name << "'." << std::endl;
    }
}

void ResourceManager::clear ()
{
    _resourcesbyname.clear();
}

unsigned ResourceManager::getResourceUsage() const
{
    return Pool<Pools::Resource> :: Get().getCurrentSize();
}

FileLoaderFactory& ResourceManager::getFileLoaderFactory()
{
    return _fileloaders;
}

WindowLoaderFactory& ResourceManager::getWindowLoaderFactory()
{
    return _windowLoaders;
}

RendererLoaderFactory& ResourceManager::getRendererLoaderFactory()
{
    return _rendererLoaders;
}

MeshLoaderFactory& ResourceManager::getMeshLoaderFactory()
{
    return _meshLoaders;
}

ResourceManager::NameGenerator& ResourceManager::getNameGenerator()
{
    return _nameGenerator;
}

void ResourceManager::setVerbose(bool flag)
{
    _verbose = flag;
}

int ResourceManager::loadPluginsIn(const std::string &dirname)
{
    int res = 0;
    
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir (dirname.c_str())) != NULL) {
        /* print all the files and directories within directory */
        while ((ent = readdir (dir)) != NULL) {
            std::string d_name(ent->d_name);
            if (d_name == "." || d_name == "..")
                continue;
            
            Plugin newPlugin = std::move(Plugin(loadResourceWith(PluginLoader(), Resource::Type::Plugin, std::string(ent->d_name) + "-plugin", dirname + "/" + ent->d_name)));
            if(newPlugin.lock())
                res++;
        }
        closedir (dir);
    } else {
        /* could not open directory */
        perror ("");
        return 0;
    }
    
    return res;
}

GRE_END_NAMESPACE

