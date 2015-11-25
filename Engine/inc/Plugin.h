//
//  Plugin.h
//  GResource
//
//  Created by Jacques Tronconi on 01/11/2015.
//  Copyright (c) 2015 Atlanti's Corporation. All rights reserved.
//

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

GRE_BEGIN_NAMESPACE

typedef void  (*PluginStartFunction)   (void);
typedef void  (*PluginStopFunction)    (void);
typedef void* (*PluginGetNameFunction) (void);

/// @brief A Resource object holding the Plugin data.
class DLL_PUBLIC PluginResource : public Resource
{
public:
    
    POOLED(Pools::Resource)
    
    /// @brief Creates the Resource, but do not load it.
    /// It loads the file, but do not launch the plugin using the StartPlugin()
    /// function.
    PluginResource(const std::string& name, const std::string& file);
    
    /// @brief Destroys the Resource. If the plugin was loaded, it will unload
    /// it.
    ~PluginResource();
    
    /// @brief Start the plugin (call StartPlugin() function).
    bool start ();
    
    /// @brief Stop the plugin (call StopPlugin() function).
    void stop ();
    
    /// @brief Returns the name of this plugin.
    const std::string& getName() const;
    
protected:
    
    /// @deprecated
    const void* _getData() const;
    
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

/// @brief A loadable add-on to the Engine.
/// A plugin can do everything to the Engine. Its mainly purpose, is to add
/// more ResourceLoader object to factories present in the Engine, for the final user
/// to be able to load and to do more things.
class DLL_PUBLIC Plugin : public ResourceUser
{
public:
    
    POOLED(Pools::Resource)
    
    Plugin (Plugin&& movref);
    Plugin (const Plugin& plugin);
    explicit Plugin (const ResourceUser& ruser);
    
    virtual ~Plugin ();
    
    Plugin& operator = (const ResourceUser& ruser);
    Plugin& operator = (const Plugin& rhs);
    
    /// @brief Start the plugin.
    bool start();
    
    /// @brief Stop the plugin.
    void stop();
    
    /// @brief Returns the name of the plugin, not the name of the resource.
    const std::string& getName() const;
    
protected:
    
    std::weak_ptr<PluginResource> _mPlugin;
};

class DLL_PUBLIC PluginLoader : public FileLoader
{
public:
    
    POOLED(Pools::Loader)
    
    PluginLoader ();
    
    /// @brief Returns true if the Resource desired is Resource::Plugin.
    bool isTypeSupported(Resource::Type type) const;
    
    /// @brief Loads the desired plugin file.
    Resource* load(Resource::Type type, const std::string& name, const std::string& file) const;
    
    /// @brief Clones the Loader.
    ResourceLoader* clone() const;
};

GRE_END_NAMESPACE

#endif
