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

Plugin::Plugin (const std::string& name, const std::string& file)
: Resource(ResourceIdentifier::New() , name)
, _file (file)
{
    GreAutolock ;
    
    // We check the extension of the file.
    std::size_t found = _file.find(std::string(".") + DYNLIB_EXTENSION);
    if(found != std::string::npos) {
    }
    else {
        GreDebugPretty() << name << " : Bad extension." << Gre::gendl;
        throw BadPluginExtension(_file + " [Contructor]");
    }
    
    DYNLIB_HANDLE hdl = DYNLIB_LOAD(_file.c_str());
    if(!hdl) {
        GreDebugPretty() << "{" << name << "} Could not load file." << Gre::gendl;
        throw BadPluginFile(_file + " [Constructor]");
    }
    
    PluginGetNameFunction mFunc   = nullptr;
    PluginStartFunction   mFunc2  = nullptr;
    PluginStopFunction    mFunc3  = nullptr;
    
    * (void**) &mFunc = DYNLIB_GETSYM(hdl, "GetPluginName");
    if(!mFunc) {
        GreDebugPretty() << name << " : " << dlerror() << Gre::gendl;
        DYNLIB_UNLOAD(hdl);
        throw BadPluginConception(_file + " [GetPluginName]");
    }
    
    _mName = (const char*) mFunc ();
    
    * (void**) &mFunc2 = DYNLIB_GETSYM(hdl, "StartPlugin");
    * (void**) &mFunc3 = DYNLIB_GETSYM(hdl, "StopPlugin");
    
    if(!mFunc2 || !mFunc3) {
        GreDebugPretty() << name << " : " << dlerror() << Gre::gendl;
        DYNLIB_UNLOAD(hdl);
        throw BadPluginConception(_file + " [StartOrStopPlugin]");
    }
    
    _mStart  = mFunc2;
    _mStop   = mFunc3;
    _mHandle = hdl;
    _mIsStarted = false;
}

Plugin::~Plugin()
{
    unload();
}

bool Plugin::start ()
{
    GreAutolock ;
    
    if(_mStart && !_mIsStarted) {
        _mStart ();
#ifdef DEBUG
        GreDebugPretty() << _mName << " : Started." << Gre::gendl;
#endif
        _mIsStarted = true;
        return true;
    }
    
    return false;
}

void Plugin::stop ()
{
    GreAutolock ;
    
    if(_mStop && _mHandle && _mIsStarted) {
        _mStop();
        _mIsStarted = false;
#ifdef DEBUG
        GreDebugPretty() << _mName << " : Stopped." << Gre::gendl;
#endif
    }
}

const std::string& Plugin::getName() const
{
    GreAutolock ;
    return _mName;
}

void Plugin::unload()
{
    GreAutolock ;
    
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

// ---------------------------------------------------------------------------------------------------

PluginLoader::PluginLoader ()
{
    
}

PluginLoader::~PluginLoader() noexcept(false)
{
    
}

bool PluginLoader::isLoadable(const std::string &filepath) const
{
    // Check extension validity.
    
    size_t ext = filepath.find_last_of( std::string(".") );
    std::string extension = filepath.substr( ext + 1 , filepath.size() );
    
    if ( extension != DYNLIB_EXTENSION )
    {
#ifdef GreIsDebugMode
        GreDebugPretty() << "'" << extension << "' is not a viable extension for Dynamic Library file. ('" << DYNLIB_EXTENSION << "' is correct.)" << Gre::gendl;
#endif
        return false;
    }
    
    else
    {
        // Try to open the file in binary mode.
        
        std::ifstream tmpstream ( filepath , std::ios::binary );
        
        if ( tmpstream )
        {
            return true;
        }
        
        else
        {
#ifdef GreIsDebugMode
            GreDebugPretty() << "File '" << filepath << "' cannot be opened." << Gre::gendl;
#endif
            return false;
        }
    }
}

PluginHolder PluginLoader::load(const std::string &name, const std::string &file) const
{
    if ( !isLoadable(file) )
    {
#ifdef GreIsDebugMode
        GreDebugPretty() << "File '" << file << "' is not loadable by this Loader." << Gre::gendl;
#endif
        return PluginHolder ( nullptr );
    }
    
    else
    {
        try
        {
            Plugin* newPlugin = new Plugin ( name , file );
            
            if ( !newPlugin )
            {
#ifdef GreIsDebugMode 
                GreDebugPretty() << "Plugin '" << name << "' could not be created." << Gre::gendl;
#endif
                return PluginHolder ( nullptr );
            }
            
            else
            {
                return PluginHolder ( newPlugin );
            }
        }
        catch ( const std::exception& e )
        {
#ifdef GreIsDebugMode 
            GreDebugPretty() << "Plugin '" << name << "' could not be created." << Gre::gendl;
            GreDebugPretty() << "Exception : " << e.what() << "." << Gre::gendl;
#endif
            return PluginHolder ( nullptr );
        }
    }
}

ResourceLoader* PluginLoader::clone() const
{
    return new PluginLoader();
}

// ---------------------------------------------------------------------------------------------------

PluginManager::PluginManager(const std::string & name)
: Resource ( ResourceIdentifier::New() , name )
{
    // Register basic PluginLoader.
    iFactory.registers("DefaultPluginLoader", new PluginLoader());
}

PluginManager::~PluginManager() noexcept ( false )
{
    
}

PluginUser PluginManager::load(const PluginHolder &holder)
{
    GreAutolock ;
    
    if ( !holder.isInvalid() )
    {
        PluginUser tmp = get(holder->getName());
        
        if ( tmp.isInvalid() )
        {
#ifdef GreIsDebugMode 
            GreDebugPretty() << "Plugin Resource '" << holder->getName() << "' already installed." << Gre::gendl;
#endif
            return PluginUser ( nullptr );
        }
        
        iPlugins.add(holder);
        return PluginUser ( holder );
    }
    
    else
    {
#ifdef GreIsDebugMode
        GreDebugPretty() << "'holder' is invalid." << Gre::gendl;
#endif
        return PluginUser ( nullptr );
    }
}

PluginUser PluginManager::load(const std::string &name, const std::string &filepath)
{
    GreAutolock ;
    
    if ( !name.empty() )
    {
        PluginUser tmp = get(name);
        
        if ( !tmp.isInvalid() )
        {
#ifdef GreIsDebugMode
            GreDebugPretty() << "Plugin Resource '" << name << "' already installed." << Gre::gendl;
#endif
            return PluginUser ( nullptr );
        }
        
        if ( !filepath.empty() )
        {
            for ( auto it = iFactory.getLoaders().begin(); it != iFactory.getLoaders().end(); it++ )
            {
                auto loader = it->second;
                
                if ( loader )
                {
                    if ( loader->isLoadable(filepath) )
                    {
                        PluginHolder plugin = loader->load(name, filepath);
                        
                        if ( !plugin.isInvalid() )
                        {
                            iPlugins.add(plugin);
                            return PluginUser ( plugin );
                        }
                        
                        else
                        {
#ifdef GreIsDebugMode
                            GreDebugPretty() << "Plugin Resource '" << name << "' can't be loaded by loader '" << it->first << "'." << Gre::gendl;
#endif
                            return PluginUser ( nullptr );
                        }
                    }
                    
                    else
                    {
#ifdef GreIsDebugMode 
                        GreDebugPretty() << "Plugin Loader '" << it->first << "' can't load Plugin Resource '" << name << "'." << Gre::gendl;
#endif
                    }
                }
            }
            
#ifdef GreIsDebugMode
            GreDebugPretty() << "Plugin Resource '" << name << "' don't have any loadable Plugin Loader." << Gre::gendl;
#endif
            return PluginUser ( nullptr );
        }
        
#ifdef GreIsDebugMode
        GreDebugPretty() << "'filepath' is empty." << Gre::gendl;
#endif
        return PluginUser ( nullptr );
    }
    
    else
    {
#ifdef GreIsDebugMode
        GreDebugPretty() << "'name' is empty." << Gre::gendl;
#endif
        return PluginUser ( nullptr );
    }
}

PluginUser PluginManager::get(const std::string &name)
{
    GreAutolock ;
    
    if ( !name.empty() )
    {
        for ( PluginHolder& holder : iPlugins )
        {
            if ( !holder.isInvalid() )
            {
                if ( holder->getName() == name )
                {
                    return PluginUser ( holder );
                }
            }
        }
        
        return PluginUser ( nullptr );
    }
    
    else
    {
#ifdef GreIsDebugMode
        GreDebugPretty() << "'name' is empty." << Gre::gendl;
#endif
        return PluginUser ( nullptr );
    }
}

const PluginUser PluginManager::get(const std::string &name) const
{
    GreAutolock ;
    
    if ( !name.empty() )
    {
        for ( const PluginHolder& holder : iPlugins )
        {
            if ( !holder.isInvalid() )
            {
                if ( holder->getName() == name )
                {
                    return PluginUser ( holder );
                }
            }
        }
		
        return PluginUser ( nullptr );
    }
    
    else
    {
#ifdef GreIsDebugMode
        GreDebugPretty() << "'name' is empty." << Gre::gendl;
#endif
        return PluginUser ( nullptr );
    }
}

void PluginManager::remove(const std::string &name)
{
    GreAutolock ;
    
    if ( !name.empty() )
    {
        for ( auto it = iPlugins.begin(); it != iPlugins.end(); it++ )
        {
            PluginHolder& holder = *it;
            
            if ( !holder.isInvalid() )
            {
                if ( holder->getName() == name )
                {
                    iPlugins.erase(it);
                }
            }
        }
        
#ifdef GreIsDebugMode
        GreDebugPretty() << "Plugin Resource '" << name << "' not found." << Gre::gendl;
#endif
    }
    
    else
    {
#ifdef GreIsDebugMode
        GreDebugPretty() << "'name' is empty." << Gre::gendl;
#endif
    }
}

void PluginManager::clearPlugins()
{
    GreAutolock ;
    iPlugins.clear();
}

PluginLoaderFactory& PluginManager::getPluginLoaderFactory()
{
    GreAutolock ;
    return iFactory;
}

const PluginLoaderFactory& PluginManager::getPluginLoaderFactory() const
{
    GreAutolock ;
    return iFactory;
}

void PluginManager::clear()
{
    GreAutolock ;
    clearPlugins();
    iFactory.clear();
}

void PluginManager::unload()
{
    clear();
}

void PluginManager::callStops()
{
    for ( PluginHolder & plugin : iPlugins )
    {
        plugin -> stop () ;
    }
}

GreEndNamespace
