//
//  Plugin.cpp
//  GResource
//
//  Created by Jacques Tronconi on 01/11/2015.
//  Copyright (c) 2015 Atlanti's Corporation. All rights reserved.
//

#include "Plugin.h"

GreBeginNamespace

PluginResource::PluginResource (const std::string& name, const std::string& file)
: Resource(name), _file (file)
{
    // We check the extension of the file.
    std::size_t found = _file.find(std::string(".") + DYNLIB_EXTENSION);
    if(found != std::string::npos) {
    }
    else {
        GreDebugPretty() << name << " : Bad extension." << std::endl;
        throw BadPluginExtension(_file + " [Contructor]");
    }
    
    DYNLIB_HANDLE hdl = DYNLIB_LOAD(_file.c_str());
    if(!hdl) {
        GreDebugPretty() << "{" << name << "} Could not load file." << std::endl;
        throw BadPluginFile(_file + " [Constructor]");
    }
    
    PluginGetNameFunction mFunc   = nullptr;
    PluginStartFunction   mFunc2  = nullptr;
    PluginStopFunction    mFunc3  = nullptr;
    
    * (void**) &mFunc = DYNLIB_GETSYM(hdl, "GetPluginName");
    if(!mFunc) {
        GreDebugPretty() << name << " : " << dlerror() << std::endl;
        DYNLIB_UNLOAD(hdl);
        throw BadPluginConception(_file + " [GetPluginName]");
    }
    
    _mName = (const char*) mFunc ();
    
    * (void**) &mFunc2 = DYNLIB_GETSYM(hdl, "StartPlugin");
    * (void**) &mFunc3 = DYNLIB_GETSYM(hdl, "StopPlugin");
    
    if(!mFunc2 || !mFunc3) {
        GreDebugPretty() << name << " : " << dlerror() << std::endl;
        DYNLIB_UNLOAD(hdl);
        throw BadPluginConception(_file + " [StartOrStopPlugin]");
    }
    
    _mStart  = mFunc2;
    _mStop   = mFunc3;
    _mHandle = hdl;
    _mIsStarted = false;
}

PluginResource::~PluginResource()
{
    if(_mStop && _mStart && _mHandle)
    {
        stop();
        DYNLIB_UNLOAD(_mHandle);
        _mName.clear();
        _mStart  = nullptr;
        _mStop   = nullptr;
        _mHandle = 0;
    }
}

bool PluginResource::start ()
{
    
    if(_mStart && !_mIsStarted) {
        _mStart ();
#ifdef DEBUG
        GreDebugPretty() << _mName << " : Started." << std::endl;
#endif
        _mIsStarted = true;
        return true;
    }
    
    return false;
}

void PluginResource::stop ()
{
    if(_mStop && _mHandle && _mIsStarted) {
        _mStop();
        _mIsStarted = false;
#ifdef DEBUG
        GreDebugPretty() << _mName << " : Stopped." << std::endl;
#endif
    }
}

const void* PluginResource::_getData() const
{
    return &_mHandle;
}

const std::string& PluginResource::getName() const
{
    return _mName;
}


Plugin::Plugin(Plugin&& movref)
: ResourceUser(movref), _mPlugin(std::move(movref._mPlugin))
{
    
}

Plugin::Plugin (const Plugin& plugin)
: ResourceUser(plugin.lock()), _mPlugin(plugin._mPlugin)
{
    
}

Plugin::Plugin (const ResourceUser& ruser)
: ResourceUser(ruser.lock()), _mPlugin(std::dynamic_pointer_cast<PluginResource>(ruser.lock()))
{
    
}

Plugin::~Plugin ()
{
    
}

std::string _tmp;

const std::string& Plugin::getName() const
{
    auto ptr = _mPlugin.lock();
    if(ptr)
        return ptr->getName();
    return _tmp;
}

bool Plugin::start()
{
    auto ptr = _mPlugin.lock();
    if(ptr)
        return ptr->start();
    return false;
}

void Plugin::stop()
{
    auto ptr = _mPlugin.lock();
    if(ptr)
        ptr->stop();
}

PluginLoader::PluginLoader ()
{
    
}

bool PluginLoader::isTypeSupported(Resource::Type type) const
{
    return type == Resource::Type::Plugin;
}

Resource* PluginLoader::load(Resource::Type type, const std::string& name, const std::string& file) const
{
    if(!isTypeSupported(type))
        return nullptr;
    
    PluginResource* plugres = new PluginResource(name, file);
    if(!plugres) return nullptr;
    
    if(!plugres->start()) {
        delete plugres;
        return nullptr;
    }
    
    return (Resource*) plugres;
}

ResourceLoader* PluginLoader::clone() const
{
    return new PluginLoader();
}

GreEndNamespace
