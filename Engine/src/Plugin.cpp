//////////////////////////////////////////////////////////////////////
//
//  Plugin.cpp
//  This source file is part of Gre
//		(Gang's Resource Engine)
//
//  Copyright (c) 2015 - 2016 Luk2010
//  Created on 01/11/2015.
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

#include "Plugin.h"

GreBeginNamespace

PluginPrivate::PluginPrivate (const std::string& name, const std::string& file)
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

PluginPrivate::~PluginPrivate()
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

bool PluginPrivate::start ()
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

void PluginPrivate::stop ()
{
    if(_mStop && _mHandle && _mIsStarted) {
        _mStop();
        _mIsStarted = false;
#ifdef DEBUG
        GreDebugPretty() << _mName << " : Stopped." << std::endl;
#endif
    }
}

const void* PluginPrivate::_getData() const
{
    return &_mHandle;
}

const std::string& PluginPrivate::getName() const
{
    return _mName;
}

// ---------------------------------------------------------------------------------------------------

Plugin::Plugin(const PluginPrivate* pointer)
: SpecializedResourceUser<Gre::PluginPrivate>(pointer)
{
    
}

Plugin::Plugin(const PluginHolder& holder)
: SpecializedResourceUser<Gre::PluginPrivate>(holder)
{
    
}

Plugin::Plugin(const Plugin& user)
: SpecializedResourceUser<Gre::PluginPrivate>(user)
{
    
}

Plugin::~Plugin ()
{
    
}

std::string _tmp;

const std::string& Plugin::getName() const
{
    auto ptr = lock();
    if(ptr)
        return ptr->getName();
    return _tmp;
}

bool Plugin::start()
{
    auto ptr = lock();
    if(ptr)
        return ptr->start();
    return false;
}

void Plugin::stop()
{
    auto ptr = lock();
    if(ptr)
        ptr->stop();
}

// ---------------------------------------------------------------------------------------------------

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
    
    PluginPrivate* plugres = new PluginPrivate(name, file);
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
