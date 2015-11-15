//
//  Plugin.cpp
//  GResource
//
//  Created by Jacques Tronconi on 01/11/2015.
//  Copyright (c) 2015 Atlanti's Corporation. All rights reserved.
//

#include "Plugin.h"

Plugin::Plugin(Plugin&& movref)
: ResourceUser(movref)
{
    
}

Plugin::Plugin (const Plugin& plugin)
: ResourceUser(plugin.lock())
{
    
}

Plugin::Plugin (const ResourceUser& ruser)
: ResourceUser(ruser.lock())
{
    
}

Plugin::~Plugin ()
{
    
}

const std::string Plugin::getName() const
{
    return getData<Plugin::Data>().name;
}


PluginResource::PluginResource (const std::string& name, const std::string& file) :
Resource(name),
_file (file)
{
    
}

bool PluginResource::start ()
{
    std::size_t found = _file.find(std::string(".") + DYNLIB_EXTENSION);
    if(found != std::string::npos)
    {
        std::cout << "Found at pos " << found << ", " << DYNLIB_EXTENSION << std::endl;
    }
    else {
        return false;
    }
    
    DYNLIB_HANDLE hdl = DYNLIB_LOAD(_file.c_str());
    if(!hdl) return false;
    
    PluginGetNameFunction mFunc = nullptr;
    PluginStartFunction mFunc2  = nullptr;
    PluginStopFunction  mFunc3  = nullptr;
    
    * (void**) &mFunc = DYNLIB_GETSYM(hdl, "GetPluginName");
    if(!mFunc) {
        std::cout << dlerror() << std::endl;
        DYNLIB_UNLOAD(hdl);
        throw BadPluginConception(_file + " [GetPluginName]");
    }
    
    _data.name = (const char*) mFunc ();
    
    * (void**) &mFunc2 = DYNLIB_GETSYM(hdl, "StartPlugin");
    * (void**) &mFunc3 = DYNLIB_GETSYM(hdl, "StopPlugin");
    
    if(!mFunc2 || !mFunc3) {
        DYNLIB_UNLOAD(hdl);
        throw BadPluginConception(_file);
    }
    
    _data.start  = mFunc2;
    _data.stop   = mFunc3;
    _data.handle = hdl;
    
    _data.start ();
    
#ifdef DEBUG
    std::cout << "[Plugin:" << _data.name << "] Started." << std::endl;
#endif
    
    return true;
}

void PluginResource::stop ()
{
    if(_data.stop && _data.handle) {
        _data.stop();
        DYNLIB_UNLOAD(_data.handle);
    }
    
#ifdef DEBUG
    std::cout << "[Plugin:" << _data.name << "] Stopped." << std::endl;
#endif
    
    _data.name.clear();
    _data.start = nullptr;
    _data.stop  = nullptr;
    _data.handle = 0;
}

const void* PluginResource::_getData() const
{
    return &_data;
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
