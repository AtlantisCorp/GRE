//
//  Resource.h
//  GResource
//
//  Created by Jacques Tronconi on 28/10/2015.
//  Copyright (c) 2015 Atlanti's Corporation. All rights reserved.
//

#ifndef GResource_Resource_h
#define GResource_Resource_h

#include "Pools.h"

class DLL_PUBLIC Resource
{
private:
    
    std::string _name;
    
public:
    
    enum class DLL_PUBLIC Type {
        Null,
        Text,
        Plugin,
        Window,
        Renderer
    };
    
    POOLED(Pools::Resource);
    
    Resource (const std::string& name);
    
    virtual ~Resource ();
    
    const std::string& getName() const;
    
    template<typename T>
    const T& getData() const
    { return *reinterpret_cast<const T*>(_getData()); }
    
protected:
    
    virtual const void* _getData() const;
};

class DLL_PUBLIC ResourceUser
{
protected:
    
    std::weak_ptr<Resource> _resource;
    
public:
    
    ResourceUser (const ResourceUser& rhs) : _resource(rhs._resource) { }
    ResourceUser (std::weak_ptr<Resource> r);
    
    virtual ~ResourceUser ();
    
    bool expired () const;
    
    const std::string& getName() const;
    
    template<typename T>
    const T getData() const { auto ptr = _resource.lock(); return ptr->getData<T>(); }
    
    std::shared_ptr<Resource> lock();
    const std::shared_ptr<Resource> lock() const;

};

class DLL_PUBLIC ResourceLoader : public Clonable<ResourceLoader>
{
public:
    
    POOLED(Pools::Loader)
    
    ResourceLoader();
    ResourceLoader(const ResourceLoader&);
    
    virtual ~ResourceLoader();
    
    virtual bool isTypeSupported (Resource::Type type) const;
    virtual ResourceLoader* clone() const;
};

template <typename T>
class ResourceLoaderFactory
{
public:
    
    POOLED(Pools::Factory)
    
    ResourceLoaderFactory () { }
    virtual ~ResourceLoaderFactory () { }
    
    void registers(const std::string& name, T* loader)
    { _loaders[name].reset(loader); }
    
    T* get(const std::string& name)
    { return reinterpret_cast<T*>(_loaders[name]->clone()); }
    
    T* getFirst ()
    { return reinterpret_cast<T*>(_loaders.begin()->second->clone()); }
    
private:
    
    std::map<std::string, std::shared_ptr<T> > _loaders;
};

#endif
