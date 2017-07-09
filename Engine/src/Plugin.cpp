//////////////////////////////////////////////////////////////////////
//
//  Plugin.cpp
//  This source file is part of Gre
//		(Gang's Resource Engine)
//
//  Copyright (c) 2015 - 2017 Luk2010
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
#include "Platform.h"

GreBeginNamespace

Plugin::Plugin (const PluginInfo * info , const std::string & path ,
                DYNLIB_HANDLE hdl , const PluginStartFunction & startfunc , const PluginStopFunction & stopfunc )
: Gre::Resource()
{
    setName(info->name) ;
    _mStart = startfunc ;
    _mStop = stopfunc ;
    _mIsStarted = true ;
    _mHandle = hdl ;

    iInfo.name = info->name ;
    iInfo.author = info->author ;
    iInfo.version = info->version ;
    uuid_copy(iInfo.uuid, info->uuid) ;

    iPath = path ;
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
    GreAutolock ; return iInfo.name ;
}

void Plugin::unload()
{
    GreAutolock ;

    if(_mStop && _mStart && _mHandle)
    {
        stop();
        DYNLIB_UNLOAD(_mHandle);

        _mStart  = nullptr;
        _mStop   = nullptr;
        _mHandle = 0;
    }
}

const std::string & Plugin::getPath() const
{
    GreAutolock ; return iPath ;
}

const uuid_t & Plugin::getUUID() const
{
    GreAutolock ; return iInfo.uuid ;
}

// ---------------------------------------------------------------------------------------------------

PluginManager::PluginManager(const std::string & name)
: Resource ( ResourceIdentifier::New() , name )
{

}

PluginManager::~PluginManager() noexcept ( false )
{

}

uint32_t PluginManager::loadFromBundles ( const ResourceBundleHolderList & bundles )
{
    GreAutolock ;

    uint32_t num = 0 ;

    for ( auto bundle : bundles )
    num = num + loadFromBundle ( bundle ) ;

    return num ;
}

uint32_t PluginManager::loadFromBundle(const ResourceBundleHolder &bundle)
{
    GreAutolock ;

    if ( bundle.isInvalid() ) {
        GreDebug ( "[WARN] Bundle given is invalid.") << gendl ;
        return 0 ;
    }

    // We must get every directories in the bundle. Then , try to get a list of plugins
    // in all the directories. Then , we try to load them all. This last pass should also
    // resolve dependencies between plugins.

    // Dependency is resolved with the following algorithm : See a plugin dependencies. If it has,
    // look for the dependency uuid in each plugins. If not loaded , try to load other plugins using
    // the bundle directories only if they match the given UUID.

    const std::vector<std::string> & directories = bundle -> getDirectories ( ResourceType::Plugin ) ;
    if ( directories.empty() ) {
        GreDebug ( "[WARN] No directory for 'Plugin' in bundle '") << bundle->getName() << "'." << gendl ;
        return 0 ;
    }

    std::vector < std::string > pluginpaths ;

    for ( const auto & dir : directories )
    {
        std::vector < std::string > paths = Platform::FindFilesWithExtension ( dir , DYNLIB_EXTENSION ) ;

        if ( paths.empty() ) {
            GreDebug ( "[INFO] Directory '" ) << dir << "' doesn't exist or doesn't contain any plugin." << gendl ;
            continue ;
        }

        else {
            GreDebug ( "[INFO] Found " ) << paths.size() << " plugins in directory '" << dir << "'." << gendl ;
            pluginpaths.insert(pluginpaths.begin(), paths.begin(), paths.end());
        }
    }

    if ( pluginpaths.empty() ) {
        GreDebug ( "[WARN] Bundle '") << bundle->getName() << "' empty." << gendl ;
        return 0 ;
    }

    uint32_t ret = 0 ;

    for ( const auto & path : pluginpaths )
    {
        PluginHolder plugin = loadFromBundledFile ( path , pluginpaths ) ;
        if ( !plugin.isInvalid() ) {
            GreDebug("[INFO] Loaded Plugin's path '") << path << "'." << gendl ;
            ret ++ ;
        }

        else {
            GreDebug("[INFO] Can't load Plugin's path '") << path << "'." << gendl ;
        }
    }

    return ret ;
}

PluginHolder PluginManager::loadFromBundledFile ( const std::string & path , const std::vector<std::string> & bundlepaths )
{
    GreAutolock ;

    // First , try to see if the given plugin has already been loaded. To do this , we look for a plugin with the same
    // path as given one. If so , we just return it and do nothing more.

    {
        PluginHolder tmp = findFromPath ( path ) ;
        if ( !tmp.isInvalid() ) return tmp ;
    }

    // First load the plugin dynamic library . Then , we should take the PluginInfo structure to check version , uuid and
    // then , dependencies . If there are , look for them . Finally , load the plugin .

    DYNLIB_HANDLE hdl = DYNLIB_LOAD(path.c_str()) ;

    if ( !hdl ) {
        GreDebug("[WARN] Can't load '") << path << "'." << gendl ;
        return PluginHolder ( nullptr ) ;
    }

    PluginGetInfoCbk plgGetInfo = nullptr ;
    * (void**) & plgGetInfo = DYNLIB_GETSYM(hdl, "GetPluginInfo") ;

    if ( !plgGetInfo ) {
        GreDebug("[WARN] Can't find 'GetPluginInfo' function in plugin '") << path << "'." << gendl ;
        DYNLIB_UNLOAD(hdl) ;
        return PluginHolder ( nullptr ) ;
    }

    PluginInfo * info = plgGetInfo () ;
    if ( !info ) {
        GreDebug ( "[WARN] Can't get PluginInfo structure from '") << path << "'." << gendl ;
        DYNLIB_UNLOAD(hdl) ;
        return PluginHolder ( nullptr ) ;
    }

    if ( info->version != GRE_PLUGIN_VERSION ) {
        GreDebug( "[WARN] Invalid Version in plugin '") << path << "'." << gendl ;
        DYNLIB_UNLOAD(hdl) ;
        return PluginHolder ( nullptr ) ;
    }

    if ( uuid_is_null(info->uuid) ) {
        GreDebug( "[WARN] Invalid UUID in plugin '") << path << "'." << gendl ;
        DYNLIB_UNLOAD(hdl) ;
        return PluginHolder ( nullptr ) ;
    }

    if ( !info->dependencies.empty() )
    {
        for ( const Uuid & dep : info->dependencies )
        {
            PluginHolder tmp = loadFromBundledUUID(dep.uuid, bundlepaths) ;

            if ( tmp.isInvalid() )
            {
                GreDebug("[WARN] Dependency for Plugin '") << path << "' could not be achieved." << gendl ;
                DYNLIB_UNLOAD(hdl) ;
                return PluginHolder ( nullptr ) ;
            }
        }
    }

    // Start the plugin , registers every functions and returns the object.

    PluginStartFunction startfunc = (PluginStartFunction) DYNLIB_GETSYM(hdl, "StartPlugin") ;
    PluginStopFunction stopfunc = (PluginStopFunction) DYNLIB_GETSYM(hdl, "StopPlugin") ;

    if ( !startfunc || !stopfunc )
    {
        GreDebug("[WARN] 'StartPlugin' or 'StopPlugin' not found in plugin '") << path << "'." << gendl ;
        DYNLIB_UNLOAD(hdl) ;
        return PluginHolder ( nullptr ) ;
    }

    startfunc () ;

    PluginHolder ret ( new Plugin ( info , path , hdl , startfunc , stopfunc ) ) ;

    if ( ret.isInvalid() )
    {
        GreDebug("[WARN] Can't create holder for plugin '") << path << "'." << gendl ;
        DYNLIB_UNLOAD(hdl) ;
        return PluginHolder ( nullptr ) ;
    }

    GreDebug("[INFO] Loaded Plugin '") << ret -> getName() << "'." << gendl ;

    iPlugins.add(ret);
    return ret ;
}

PluginHolder PluginManager::loadFromBundledUUID(const uuid_t &uuid, const std::vector<std::string> &bundlepaths)
{
    GreAutolock ;

    // First , try to see if the given plugin has already been loaded. To do this , we look for a plugin with the same
    // uuid as given one. If so , we just return it and do nothing more.

    {
        PluginHolder tmp = findFromUUID ( uuid ) ;
        if ( !tmp.isInvalid() ) return tmp ;
    }

    // Loads every plugins in the 'bundlepaths' and see for their uuid in plugin's info. Notes that if we
    // try to get a handle for an already loaded library , 'dlclose' and 'FreeLibrary' both use a reference
    // counter to close the handle only once it has been called as many times as 'dlopen' or 'LoadLibrary'.

    for ( const std::string & path : bundlepaths )
    {
        DYNLIB_HANDLE hdl = DYNLIB_LOAD(path.c_str()) ;

        if ( !hdl ) {
            GreDebug("[WARN] Can't load '") << path << "'." << gendl ;
            continue ;
        }

        PluginGetInfoCbk plgGetInfo = nullptr ;
        * (void**) & plgGetInfo = DYNLIB_GETSYM(hdl, "GetPluginInfo") ;

        if ( !plgGetInfo ) {
            GreDebug("[WARN] Can't find 'GetPluginInfo' function in plugin '") << path << "'." << gendl ;
            DYNLIB_UNLOAD(hdl) ;
            continue ;
        }

        PluginInfo * info = plgGetInfo () ;
        if ( !info ) {
            GreDebug ( "[WARN] Can't get PluginInfo structure from '") << path << "'." << gendl ;
            DYNLIB_UNLOAD(hdl) ;
            continue ;
        }

        if ( info->version != GRE_PLUGIN_VERSION ) {
            GreDebug( "[WARN] Invalid Version in plugin '") << path << "'." << gendl ;
            DYNLIB_UNLOAD(hdl) ;
            continue ;
        }

        if ( uuid_is_null(info->uuid) ) {
            GreDebug( "[WARN] Invalid UUID in plugin '") << path << "'." << gendl ;
            DYNLIB_UNLOAD(hdl) ;
            continue ;
        }

        if ( uuid_compare(info->uuid, uuid) != 0 )
        {
            DYNLIB_UNLOAD(hdl) ;
            continue ;
        }

        // If we are here , this significates the library used here is the correct dependency. We should
        // load it as a normal library.

        DYNLIB_UNLOAD(hdl) ;

        PluginHolder ret = loadFromBundledFile(path, bundlepaths) ;

        if ( ret.isInvalid() )
        {
            GreDebug("[WARN] Can't load '") << path << "'." << gendl ;
            return PluginHolder ( nullptr ) ;
        }

        return ret ;
    }

    return PluginHolder ( nullptr ) ;
}

PluginHolder PluginManager::findFromPath(const std::string &path)
{
    GreAutolock ;

    for ( auto plug : iPlugins )
    {
        if ( !plug.isInvalid() )
            if ( plug->getPath() == path )
                return plug ;
    }

    return PluginHolder ( nullptr ) ;
}

PluginHolder PluginManager::findFromUUID(const uuid_t &uuid)
{
    GreAutolock ;

    for ( auto plug : iPlugins )
    {
        if ( !plug.isInvalid() )
            if ( uuid_compare(uuid, plug->getUUID()) == 0 )
                return plug ;
    }

    return PluginHolder ( nullptr ) ;
}

PluginHolder PluginManager::get(const std::string &name)
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
                    return PluginHolder ( holder );
                }
            }
        }

        return PluginHolder ( nullptr );
    }

    else
    {
#ifdef GreIsDebugMode
        GreDebugPretty() << "'name' is empty." << Gre::gendl;
#endif
        return PluginHolder ( nullptr );
    }
}

const PluginHolder PluginManager::get(const std::string &name) const
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
                    return PluginHolder ( holder );
                }
            }
        }

        return PluginHolder ( nullptr );
    }

    else
    {
#ifdef GreIsDebugMode
        GreDebugPretty() << "'name' is empty." << Gre::gendl;
#endif
        return PluginHolder ( nullptr );
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

void PluginManager::clear()
{
    GreAutolock ;
    clearPlugins();
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
