//
//  ResourceManager.cpp
//  GResource
//
//  Created by Jacques Tronconi on 01/11/2015.
//  Copyright (c) 2015 Atlanti's Corporation. All rights reserved.
//

#include "ResourceManager.h"

#include "BinaryTreeScene.h"

GreBeginNamespace

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
        GreDebugPretty() << "Constructing ResourceManager." << std::endl;
    }
    
    _fileloaders.registers("TextLoader", new TextLoader());
    _fileloaders.registers("PluginLoader", new PluginLoader());
    _meshLoaders.registers("DefaultLoader", new MeshLoader());
    _sceneLoaders.registers("BinaryTreeScene", new BinaryTreeSceneLoader());
    _verbose = false;
}

ResourceManager::~ResourceManager ()
{
    if(_verbose) {
        GreDebugPretty() << "Destroying ResourceManager." << std::endl;
    }
    
    _fileloaders.clear();
    _windowLoaders.clear();
    _rendererLoaders.clear();
    _meshLoaders.clear();
    _imageLoaders.clear();
    _sceneLoaders.clear();
    _shaderLoaders.clear();
    _progManLoaders.clear();
    _fboLoaders.clear();
    
    _resourcesbyname.clear();
    _resourcesbytype[Resource::Type::PureListener].clear();
    _resourcesbytype[Resource::Type::Keyboard].clear();
    _resourcesbytype[Resource::Type::FrameBuff].clear();
    _resourcesbytype[Resource::Type::HdwShader].clear();
    _resourcesbytype[Resource::Type::HwdProgManager].clear();
    _resourcesbytype[Resource::Type::Scene].clear();
    _resourcesbytype[Resource::Type::Image].clear();
    _resourcesbytype[Resource::Type::Texture].clear();
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
        GreDebugPretty() << "Loaded Resource '" << name << "'." << std::endl;
    }
    
    return ResourceUser(_resourcesbyname[name]);
}

ResourceUser ResourceManager::findResourceByName(const std::string &name)
{
    auto it = _resourcesbyname.find(name);
    if(it != _resourcesbyname.end())
        return ResourceUser((*it).second);
    return ResourceUser();
}

void ResourceManager::unloadResource(const std::string& name) {
    std::weak_ptr<Resource>& untypedResource = _resourcesbyname[name];
    if(!untypedResource.expired())
    {
        std::vector<std::shared_ptr<Resource> >& typedResources = _resourcesbytype[untypedResource.lock()->getType()];
        auto it = std::find(typedResources.begin(), typedResources.end(), untypedResource.lock());
        if(it != typedResources.end())
        {
            typedResources.erase(it);
        }
    }
    
    _resourcesbyname.erase(name);
    
    if(_verbose) {
        GreDebugPretty() << "Unloaded Resource '" << name << "'." << std::endl;
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

ImageLoaderFactory& ResourceManager::getImageLoaderFactory()
{
    return _imageLoaders;
}

SceneLoaderFactory& ResourceManager::getSceneLoaderFactory()
{
    return _sceneLoaders;
}

ResourceManager::NameGenerator& ResourceManager::getNameGenerator()
{
    return _nameGenerator;
}

HardwareShaderLoaderFactory& ResourceManager::getHardwareShaderLoaderFactory()
{
    return _shaderLoaders;
}

HardwareProgramManagerLoaderFactory& ResourceManager::getHardwareProgramManagerLoaderFactory()
{
    return _progManLoaders;
}

FrameBufferLoaderFactory& ResourceManager::getFrameBufferLoaderFactory()
{
    return _fboLoaders;
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

void ResourceManager::setCloseBehaviour(const CloseBehaviour &behaviour)
{
    _mCloseBehaviour = behaviour;
}

void ResourceManager::addLoopBehaviour(LoopBehaviour behaviour)
{
    _mLoopBehaviours.add(behaviour);
}

void ResourceManager::clearLoopBehaviour()
{
    _mLoopBehaviours.clear();
}

void ResourceManager::stop()
{
    _mMustStopLoop = true;
}

void ResourceManager::loop()
{
    _mMustStopLoop = false;
    
    while( !_mMustStopLoop )
    {
        std::vector<std::shared_ptr<Resource> >& windowsResources = _resourcesbytype[Resource::Type::Window];
        if(windowsResources.empty() && _mCloseBehaviour == CloseBehaviour::AllWindowClosed)
        {
            _mMustStopLoop = true;
        }
        
        else
        {
            if( !windowsResources.empty() )
            {
                // We must loop through every Window objects to update them.
                for(auto windowSharedResource : windowsResources)
                {
                    Window windowUser((std::weak_ptr<Resource>(windowSharedResource)));
                    if(!windowUser.expired())
                    {
                        windowUser.pollEvent();
                        windowUser.update();
                        
                        if(windowUser.hasBeenClosed())
                        {
                            // Window has been closed, we can destroy it.
                            unloadResource(windowUser.getName());
                        }
                        
                        /* This should be done by the renderer. 
                         The Renderer should draw every Window object as RenderTarget.
                         
                        // We begin updating the Window.
                        // This permits pollEvent, and Context binding.
                        // The Context should notifiate the Renderer that the Current Context
                        // has changed.
                        windowUser.beginUpdate();
                        
                        // Then we use the Window's Context to render the Scene defined for
                        // the renderer. Renderer object renders the Window as a normal RenderTarget.
#ifndef GreIsDebugMode
                        windowUser.getRenderContext().getRenderer().render(windowUser);
#else
                        RenderContext ctxt = windowUser.getRenderContext();
                        Renderer renderer = ctxt.getRenderer();
                        renderer.render(windowUser);
#endif
                        
                        // Finally we end the Window's update.
                        windowUser.endUpdate();
                         
                         */
                        
                        _mPerWindowBehaviours.call();
                    }
                    
                    
                }
            }
            
            _mLoopBehaviours.call();
        }
    }
}

ResourceUser ResourceManager::createPureListener(const std::string& name)
{
    std::shared_ptr<Resource> listener = std::make_shared<Resource>(name);
    listener->_type = Resource::Type::PureListener;
    _resourcesbytype[Resource::Type::PureListener].push_back(listener);
    _resourcesbyname[name] = listener;
    return ResourceUser(listener);
}

KeyboardLoader& ResourceManager::getKeyboardLoader()
{
    return _keyboardLoader;
}

Keyboard ResourceManager::createKeyboard(const std::string &name)
{
    return Keyboard(loadResourceWith(getKeyboardLoader(), Resource::Type::Keyboard, name));
}

// ---------------------------------------------------------------------------------------------------

std::string ResourceManager::Helper::ChooseRenderer(RendererLoaderFactory& rFactory)
{
    // First of all, you should create the Renderer which will render your Scene.
    StringList renderers = rFactory.getLoadersName();
    GreDebugPretty() << "Please choose a Renderer in the List : " << std::endl;
    GreDebugPretty() << Gre::DebugListNumeroted(renderers) << std::endl;
    GreDebugPretty() << ":> ";
    
    int rNum = Gre::DebugGetNumber();
    return renderers[rNum-1];
}

Renderer ResourceManager::Helper::LoadRenderer(const std::string& rname, const std::string& lname, RendererLoaderFactory& rFactory)
{
    RendererLoader* rLoader = rFactory.get(lname);
    if(rLoader)
    {
        return Renderer(ResourceManager::Get().loadResourceWith(rLoader, Resource::Type::Renderer, rname));
    }
    
    else
    {
        return Renderer::Null;
    }
}

std::string ResourceManager::Helper::ChooseWindowLoader(WindowLoaderFactory& wFactory)
{
    StringList windowloaders = wFactory.getLoadersName();
    GreDebugPretty() << "Please choose a Window Loader in the List : " << std::endl;
    GreDebugPretty() << Gre::DebugListNumeroted(windowloaders) << std::endl;
    GreDebugPretty() << ":> ";
    
    int wNum = Gre::DebugGetNumber();
    return windowloaders[wNum-1];
}

Window ResourceManager::Helper::LoadWindow(const std::string& wname, const std::string& lname, WindowLoaderFactory& wFactory, const Surface& param)
{
    WindowLoader* wLoader = wFactory.get(lname);
    if(wLoader)
    {
        return Window(ResourceManager::Get().loadResourceWith(wLoader, Resource::Type::Window, wname, param.left, param.top, param.width, param.height));
    }
    
    else
    {
        return Window::Null;
    }
}

GreEndNamespace

