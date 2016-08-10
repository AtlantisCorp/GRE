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
#include "FileLoader.h"

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
class DLL_PUBLIC PluginPrivate : public Resource
{
public:
    
    POOLED(Pools::Resource)
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Creates the Resource, but do not load it.
    /// It loads the file, but do not launch the plugin using the StartPlugin()
    /// function.
    //////////////////////////////////////////////////////////////////////
    PluginPrivate(const std::string& name, const std::string& file);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Destroys the Resource. If the plugin was loaded, it will unload
    /// it.
    //////////////////////////////////////////////////////////////////////
    ~PluginPrivate();
    
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

/// @brief SpecializedResourceHolder for PluginPrivate.
typedef SpecializedResourceHolder<PluginPrivate> PluginHolder;

/// @brief SpecializedResourceHolderList for PluginPrivate.
typedef SpecializedResourceHolderList<PluginPrivate> PluginHolderList;

//////////////////////////////////////////////////////////////////////
/// @brief SpecializedResourceUser for PluginPrivate.
//////////////////////////////////////////////////////////////////////
class DLL_PUBLIC Plugin : public SpecializedResourceUser<PluginPrivate>
{
public:
    
    POOLED(Pools::Resource)
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    Plugin(const PluginPrivate* pointer);
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    Plugin(const PluginHolder& holder);
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    Plugin(const Plugin& user);
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual ~Plugin ();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Start the plugin.
    //////////////////////////////////////////////////////////////////////
    bool start();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Stop the plugin.
    //////////////////////////////////////////////////////////////////////
    void stop();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the name of the plugin, not the name of the resource.
    //////////////////////////////////////////////////////////////////////
    const std::string& getName() const;
    
    /// @brief Null Plugin.
    static Plugin Null;
};

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

GreEndNamespace

#endif
