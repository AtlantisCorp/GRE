//////////////////////////////////////////////////////////////////////
//
//  Plugin.h
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

#ifndef GResource_Plugin_h
#define GResource_Plugin_h

#include "Platform.h"
#include "Resource.h"
#include "ResourceLoader.h"
#include "ResourceBundle.h"

#if defined(__WINDOWS__)
#   define DYNLIB_HANDLE hInstance
#   define DYNLIB_GETSYM( a, b ) GetProcAddress( a, b )
#   define DYNLIB_LOAD( a ) LoadLibraryEx( a, NULL, LOAD_WITH_ALTERED_SEARCH_PATH )
#   define DYNLIB_UNLOAD( a ) FreeLibrary( a )
#   define DYNLIB_EXTENSION "dll"
#   define DYNLIB_LASTERROR dlerror

struct HINSTANCE__;
typedef struct HINSTANCE__ * hInstance;

/* We provide custom dlerror for Windows(tm).
 * https://mail.python.org/pipermail//pypy-commit/2012-October/066804.html
 */
static const char *dlerror(void)
{
    static char buf[32];
    DWORD dw = GetLastError();
    if (dw == 0)
        return NULL;
    sprintf(buf, "error 0x%x", (unsigned int)dw);
    return buf;
}

#else
#  if defined(__OSX__) || defined (__APPLE__)
#   define DYNLIB_HANDLE void*
#   define DYNLIB_GETSYM( a, b ) dlsym( a, b )
#   define DYNLIB_LOAD( a ) dlopen( a , RTLD_LAZY | RTLD_GLOBAL)
#   define DYNLIB_UNLOAD( a ) !dlclose( a )
#   define DYNLIB_EXTENSION "dylib"
#   define DYNLIB_LASTERROR dlerror
#   include <dlfcn.h>
#
#  else
#
#   define DYNLIB_HANDLE void*
#   define DYNLIB_GETSYM( a, b ) dlsym( a, b )
#   define DYNLIB_LOAD( a ) dlopen( a , RTLD_LAZY | RTLD_GLOBAL)
#   define DYNLIB_UNLOAD( a ) !dlclose( a )
#   define DYNLIB_EXTENSION "so"
#   define DYNLIB_LASTERROR dlerror
#   include <dlfcn.h>
#
#  endif

#endif

GreBeginNamespace

#define GRE_PLUGIN_VERSION 2

//////////////////////////////////////////////////////////////////////
/// @brief Holds information about the loaded plugin.
struct PluginInfo
{
    /// @brief Plugin's complete name.
    std::string name ;

    /// @brief Plugin's description or purpose. This field is optional.
    std::string desc ;

    /// @brief Author(s) of this plugin.
    std::string author ;

    /// @brief Plugin's complete version ( Should be set to GRE_PLUGIN_VERSION ) .
    uint32_t version ;

    /// @brief Plugin's UUID ( example : '33927450-5fff-4cb3-a05e-7f858efad028' ) .
    uuid_t uuid ;

    /// @brief List of plugins' UUID corresponding to the dependencies for this plugin.
    std::vector < Uuid > dependencies ;
};

typedef void  (*PluginStartFunction)   (void);
typedef void  (*PluginStopFunction)    (void);
typedef void* (*PluginGetNameFunction) (void);

/// @brief Should return the PluginInfo hosted by the plugin. Notes this function
/// should not use any engine function.
typedef PluginInfo * (*PluginGetInfoCbk) (void) ;

//////////////////////////////////////////////////////////////////////
/// @brief Handle to a Plugin Object.
//////////////////////////////////////////////////////////////////////
class DLL_PUBLIC Plugin : public Resource
{
public:

    POOLED(Pools::Resource)

    //////////////////////////////////////////////////////////////////////
    /// @brief Initializes the Plugin object with value computed by the
    /// PluginManager ( 'loadFromBundledFile' function ) . Notes the plugin
    /// should have been already started by the PluginManager.
    //////////////////////////////////////////////////////////////////////
    Plugin (const PluginInfo * info ,
            const std::string & path ,
            DYNLIB_HANDLE hdl ,
            const PluginStartFunction & startfunc ,
            const PluginStopFunction & stopfunc );

    //////////////////////////////////////////////////////////////////////
    /// @brief Destroys the Resource. If the plugin was loaded, it will unload
    /// it.
    //////////////////////////////////////////////////////////////////////
    ~Plugin();

    //////////////////////////////////////////////////////////////////////
    /// @brief Start the plugin (call StartPlugin() function).
    //////////////////////////////////////////////////////////////////////
    bool start ();

    //////////////////////////////////////////////////////////////////////
    /// @brief Stop the plugin (call StopPlugin() function).
    //////////////////////////////////////////////////////////////////////
    void stop ();

    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the name of this plugin.
    //////////////////////////////////////////////////////////////////////
    const std::string & getName() const;

    ////////////////////////////////////////////////////////////////////////
    /// @brief Unloads the Resource.
    ////////////////////////////////////////////////////////////////////////
    virtual void unload () ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Returns 'iPath'.
    //////////////////////////////////////////////////////////////////////
    virtual const std::string & getPath () const ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Returns 'iInfo.uuid'.
    //////////////////////////////////////////////////////////////////////
    virtual const uuid_t & getUUID () const ;

private:

    PluginStartFunction _mStart;
    PluginStopFunction  _mStop;
    DYNLIB_HANDLE       _mHandle;
    bool                _mIsStarted;

    /// @brief Should corresponds to the value computed by the plugin manager
    /// when creating the plugin.
    PluginInfo iInfo ;

    /// @brief Path this plugin refers to (the physical file this plugin is) .
    std::string iPath ;
};

/// @brief Holder for PluginPrivate.
typedef Holder<Plugin> PluginHolder;

/// @brief SpecializedResourceHolderList for PluginPrivate.
typedef SpecializedResourceHolderList<Plugin> PluginHolderList;

//////////////////////////////////////////////////////////////////////
/// @brief Plugin Manager.
//////////////////////////////////////////////////////////////////////
class DLL_PUBLIC PluginManager : public Resource
{
public:

    POOLED(Pools::Manager)

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    PluginManager ( const std::string & name = "DefaultPluginManager" );

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual ~PluginManager() noexcept ( false ) ;

public:

    ////////////////////////////////////////////////////////////////////////
    /// @brief Calls 'loadFromBundle' for every bundles.
    ////////////////////////////////////////////////////////////////////////
    virtual uint32_t loadFromBundles ( const std::vector < ResourceBundleHolder > & bundles ) ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Loads plugins found in each directory of the type 'Plugin'
    /// in the given bundle. Returns the number of plugin loaded.
    //////////////////////////////////////////////////////////////////////
    virtual uint32_t loadFromBundle ( const ResourceBundleHolder & bundle ) ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Loads a given plugin with given list of plugins paths all in
    /// the currently loaded bundle ( using 'loadFromBundle' function ) .
    //////////////////////////////////////////////////////////////////////
    virtual PluginHolder loadFromBundledFile (const std::string & path ,
                                              const std::vector<std::string> & bundlepaths ) ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Loads a Plugin from its UUID. Loads every plugins untill the
    /// correct UUID is found. If the UUID is found upon already loaded plugins ,
    /// simply returns it.
    //////////////////////////////////////////////////////////////////////
    virtual PluginHolder loadFromBundledUUID (const uuid_t & uuid ,
                                              const std::vector<std::string> & bundlepaths ) ;

public:

    //////////////////////////////////////////////////////////////////////
    /// @brief Finds a loaded Plugin with given path.
    //////////////////////////////////////////////////////////////////////
    virtual PluginHolder findFromPath (const std::string & path) ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Finds a loaded Plugin from its UUID.
    //////////////////////////////////////////////////////////////////////
    virtual PluginHolder findFromUUID (const uuid_t & uuid) ;

public:

    //////////////////////////////////////////////////////////////////////
    /// @brief Returns Plugin with given 'name'.
    //////////////////////////////////////////////////////////////////////
    virtual PluginHolder get ( const std::string& name );

    //////////////////////////////////////////////////////////////////////
    /// @brief Returns Plugin with given 'name'.
    //////////////////////////////////////////////////////////////////////
    virtual const PluginHolder get ( const std::string& name ) const;

public:

    //////////////////////////////////////////////////////////////////////
    /// @brief Removes a Plugin.
    //////////////////////////////////////////////////////////////////////
    virtual void remove ( const std::string& name );

    //////////////////////////////////////////////////////////////////////
    /// @brief Removes every Plugin.
    //////////////////////////////////////////////////////////////////////
    virtual void clearPlugins();

    //////////////////////////////////////////////////////////////////////
    /// @brief Clears every Plugins and the Factory.
    //////////////////////////////////////////////////////////////////////
    virtual void clear();

    ////////////////////////////////////////////////////////////////////////
    /// @brief Unloads the Resource.
    ////////////////////////////////////////////////////////////////////////
    virtual void unload () ;

    ////////////////////////////////////////////////////////////////////////
    /// @brief Calls the stop function for every plugins but do not unload
    /// the dynamic libraries.
    ////////////////////////////////////////////////////////////////////////
    virtual void callStops () ;

protected:

    /// @brief Installed Plugins.
    PluginHolderList iPlugins;
};

/// @brief Holder for PluginManager.
typedef Holder < PluginManager > PluginManagerHolder ;

GreEndNamespace

#endif
