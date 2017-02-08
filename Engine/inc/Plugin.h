//////////////////////////////////////////////////////////////////////
//
//  Plugin.h
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

#ifndef GResource_Plugin_h
#define GResource_Plugin_h

#include "Resource.h"
#include "ResourceLoader.h"

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

typedef void  (*PluginStartFunction)   (void);
typedef void  (*PluginStopFunction)    (void);
typedef void* (*PluginGetNameFunction) (void);

//////////////////////////////////////////////////////////////////////
/// @brief Handle to a Plugin Object.
//////////////////////////////////////////////////////////////////////
class DLL_PUBLIC Plugin : public Resource
{
public:
    
    POOLED(Pools::Resource)
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Creates the Resource, but do not load it.
    /// It loads the file, but do not launch the plugin using the StartPlugin()
    /// function.
    //////////////////////////////////////////////////////////////////////
    Plugin(const std::string& name, const std::string& file);
    
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
    const std::string& getName() const;
    
    ////////////////////////////////////////////////////////////////////////
    /// @brief Unloads the Resource.
    ////////////////////////////////////////////////////////////////////////
    virtual void unload () ;
    
private:
    
    std::string         _file;
    std::string         _mName;
    PluginStartFunction _mStart;
    PluginStopFunction  _mStop;
    DYNLIB_HANDLE       _mHandle;
    bool                _mIsStarted;
    
public:
    
    class BadPluginConception : public std::exception {
    public:
        BadPluginConception(const std::string& file) : _file(file) { }
        const char* what() const throw() { return _file.c_str(); }
        std::string _file;
    };
    
    class BadPluginExtension : public std::exception {
    public:
        BadPluginExtension(const std::string& file) : _file(file) { }
        const char* what() const throw() { return _file.c_str(); }
        std::string _file;
    };
    
    class BadPluginFile : public std::exception {
    public:
        BadPluginFile(const std::string& file) : _file(file) { }
        const char* what() const throw() { return _file.c_str(); }
        std::string _file;
    };
};

/// @brief SpecializedCountedObjectHolder for PluginPrivate.
typedef SpecializedCountedObjectHolder<Plugin> PluginHolder;

/// @brief SpecializedResourceHolderList for PluginPrivate.
typedef SpecializedResourceHolderList<Plugin> PluginHolderList;

/// @brief SpecializedCountedObjectUser.
typedef SpecializedCountedObjectUser<Plugin> PluginUser;

//////////////////////////////////////////////////////////////////////
/// @brief Loads PluginPrivate Objects.
//////////////////////////////////////////////////////////////////////
class DLL_PUBLIC PluginLoader : public ResourceLoader
{
public:
    
    POOLED(Pools::Loader)
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    PluginLoader ();
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual ~PluginLoader() noexcept(false);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Loads the desired plugin file.
    //////////////////////////////////////////////////////////////////////
    virtual PluginHolder load(const std::string& name, const std::string& file) const;
    
    ////////////////////////////////////////////////////////////////////////
    /// @brief Returns a clone of this object.
    /// Typically, this function is implemented as 'return new MyLoaderClass();',
    /// but you are free to do whatever you want.
    ////////////////////////////////////////////////////////////////////////
    virtual ResourceLoader* clone() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns true if the file given is loadable by this loader.
    /// Basically opens the file and if success, returns true.
    //////////////////////////////////////////////////////////////////////
    virtual bool isLoadable( const std::string& filepath ) const;
};

/// @brief ResourceLoaderFactory for PluginLoader.
typedef ResourceLoaderFactory<PluginLoader> PluginLoaderFactory;

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
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Loads a Plugin into this Manager.
    //////////////////////////////////////////////////////////////////////
    virtual PluginUser load ( const PluginHolder& holder );
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Loads a Plugin from a given file.
    //////////////////////////////////////////////////////////////////////
    virtual PluginUser load ( const std::string& name , const std::string& filepath );
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns Plugin with given 'name'.
    //////////////////////////////////////////////////////////////////////
    virtual PluginUser get ( const std::string& name );
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns Plugin with given 'name'.
    //////////////////////////////////////////////////////////////////////
    virtual const PluginUser get ( const std::string& name ) const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Removes a Plugin.
    //////////////////////////////////////////////////////////////////////
    virtual void remove ( const std::string& name );
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Removes every Plugin.
    //////////////////////////////////////////////////////////////////////
    virtual void clearPlugins();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the PluginLoaderFactory.
    //////////////////////////////////////////////////////////////////////
    virtual PluginLoaderFactory& getPluginLoaderFactory();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the PluginLoaderFactory.
    //////////////////////////////////////////////////////////////////////
    virtual const PluginLoaderFactory& getPluginLoaderFactory() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Clears every Plugins and the Factory.
    //////////////////////////////////////////////////////////////////////
    virtual void clear();
    
    ////////////////////////////////////////////////////////////////////////
    /// @brief Unloads the Resource.
    ////////////////////////////////////////////////////////////////////////
    virtual void unload () ;
    
protected:
    
    /// @brief Installed Plugins.
    PluginHolderList iPlugins;
    
    /// @brief Installed Loaders.
    PluginLoaderFactory iFactory;
};

/// @brief SpecializedCountedObjectHolder for PluginManager.
typedef SpecializedCountedObjectHolder < PluginManager > PluginManagerHolder ;

GreEndNamespace

#endif
