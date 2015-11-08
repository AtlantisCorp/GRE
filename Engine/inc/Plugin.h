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

typedef void  (*PluginStartFunction)   (void);
typedef void  (*PluginStopFunction)    (void);
typedef void* (*PluginGetNameFunction) (void);

class DLL_PUBLIC Plugin : public ResourceUser
{
public:
    
    POOLED(Pools::Resource)
    
    struct Data
    {
        std::string         name;
        PluginStartFunction start;
        PluginStopFunction  stop;
        DYNLIB_HANDLE       handle;
    };
    
    Plugin (const Plugin& plugin);
    explicit Plugin (const ResourceUser& ruser);
    
    virtual ~Plugin ();
    
    const std::string getName() const;
};

class DLL_PUBLIC PluginResource : public Resource
{
public:
    
    POOLED(Pools::Resource)
    
    PluginResource (const std::string& name, const std::string& file);
    
    bool start ();
    
    void stop ();
    
protected:
    
    const void* _getData() const;
    
private:
    
    std::string  _file;
    Plugin::Data _data;
    
public:
    
    class BadPluginConception : public std::exception {
    public:
        BadPluginConception(const std::string& file) : _file(file) { }
        const char* what() const throw() { return _file.c_str(); }
        std::string _file;
    };
};

class DLL_PUBLIC PluginLoader : public FileLoader
{
public:
    
    POOLED(Pools::Loader)
    
    PluginLoader ();
    
    bool isTypeSupported(Resource::Type type) const;
    
    Resource* load(Resource::Type type, const std::string& name, const std::string& file) const;
    
    ResourceLoader* clone() const;
};

#endif
