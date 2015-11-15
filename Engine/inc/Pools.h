//
//  Pools.h
//  GResource
//
//  Created by Jacques Tronconi on 28/10/2015.
//  Copyright (c) 2015 Atlanti's Corporation. All rights reserved.
//

#ifndef GResource_Pools_h
#define GResource_Pools_h

#include <iostream>
#include <memory>
#include <map>
#include <new>
#include <fstream>
#include <dirent.h>
#include <utility>

#if defined _WIN32 || defined __CYGWIN__
#ifdef BUILDING_DLL
#ifdef __GNUC__
#define DLL_PUBLIC __attribute__ ((dllexport))
#else
#define DLL_PUBLIC __declspec(dllexport) // Note: actually gcc seems to also supports this syntax.
#endif
#else
#ifdef __GNUC__
#define DLL_PUBLIC __attribute__ ((dllimport))
#else
#define DLL_PUBLIC __declspec(dllimport) // Note: actually gcc seems to also supports this syntax.
#endif
#endif
#define DLL_LOCAL
#else
#if __GNUC__ >= 4
#define DLL_PUBLIC __attribute__ ((visibility ("default")))
#define DLL_LOCAL  __attribute__ ((visibility ("hidden")))
#else
#define DLL_PUBLIC
#define DLL_LOCAL
#endif
#endif

template<class T>
class Clonable
{
public:
    
    virtual ~Clonable() { }
    virtual T* clone () const = 0;
};

enum class DLL_PUBLIC Pools
{
    Default,
    Resource,
    Manager,
    Loader,
    Factory
};

#define POOLED(pooltype) \
void* operator new (size_t sz) { \
if(Pool< pooltype > ::Get().canAttach(sz) == false) throw std::bad_alloc(); \
void* ptr = malloc (sz) ; \
Pool< pooltype > :: Get().attach(ptr, sz); \
return ptr; } \
void  operator delete (void* p) noexcept { \
if(p) free (p); \
Pool< pooltype > :: Get().detach(p); }

template < Pools pooltype >
class DLL_PUBLIC Pool
{
public:
    
    static Pool& Get () {
        static Pool pool;
        return pool;
    }
    
    void attach(void* p, size_t sz)
    {
        _ptrs[p] = sz;
        _totalsz += sz;
    }
    
    void detach(void* p)
    {
        auto it = _ptrs.find(p);
        if(it != _ptrs.end()) {
            _totalsz -= it->second;
            _ptrs.erase(it);
        }
    }
    
    bool canAttach(size_t sz)
    {
        return _totalsz + sz <= _maxsz;
    }
    
    unsigned getCurrentSize () const
    {
        return _totalsz;
    }
    
    void setMaximumSize(size_t szinbytes)
    {
        _maxsz = szinbytes;
    }
    
private:
    
    Pool () : _totalsz(0), _maxsz(100000) { }
    
    std::map<void*, size_t> _ptrs;
    unsigned                _totalsz;
    unsigned                _maxsz;
};

#endif
