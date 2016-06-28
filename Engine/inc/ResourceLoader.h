////////////////////////////////////////////////////
//  File    : ResourceLoader.h
//  Project : GRE
//
//  Created by Jacques Tronconi on 07/05/2016.
//  
//
////////////////////////////////////////////////////

#ifndef GRE_ResourceLoader_h
#define GRE_ResourceLoader_h

#include "Resource.h"

GreBeginNamespace

////////////////////////////////////////////////////////////////////////
/// @class ResourceLoader
/// @brief Used to load a Resource from something.
///
/// The ResourceLoader is an atypic object, because this class defines the
/// basic interface but not the main loading function.
///
/// The main loading function is of the form :
/// Resource* load (Resource::Type type, const std::string& name, ...) const
/// where '...' is a list of other argues that must be known from the user.
///
/// The ResourceLoader object must not save data, because of the factory
/// system.
/// At each loading, the factory constructs a new loader. This loader is
/// used by the ResourceManager, then is destroyed by the ResourceManager.
///
/// @see Resource, ResourceUser
////////////////////////////////////////////////////////////////////////
class DLL_PUBLIC ResourceLoader : public Clonable<ResourceLoader>
{
public:
    
    POOLED(Pools::Loader)
    
    ////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////
    ResourceLoader();
    
    ////////////////////////////////////////////////////////////////////////
    /// @brief Copies the object.
    ////////////////////////////////////////////////////////////////////////
    ResourceLoader(const ResourceLoader&);
    
    ////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////
    virtual ~ResourceLoader();
    
    ////////////////////////////////////////////////////////////////////////
    /// @brief Returns true if given Resource::Type is supported.
    ////////////////////////////////////////////////////////////////////////
    virtual bool isTypeSupported (Resource::Type type) const;
    
    ////////////////////////////////////////////////////////////////////////
    /// @brief Returns a clone of this object.
    /// Typically, this function is implemented as 'return new MyLoaderClass();',
    /// but you are free to do whatever you want.
    ////////////////////////////////////////////////////////////////////////
    virtual ResourceLoader* clone() const;
};

////////////////////////////////////////////////////////////////////////
/// @class ResourceLoaderFactory
/// @brief Constructs a Loader given its name.
///
/// In fact, this class just use the ResourceFactory::clone() function to
/// clone th registered factory for the user to use it.
///
/// # The ResourceFactory process
///
/// 2 steps :
/// - A plugin or the user creates a new Factory class. This Factory should
/// inherit from one of the typical factory (Window factory, Renderer factory,
/// ..., see Resource::Type for all factories types).
/// - The plugin 'registers' the Factory class using the ResourceLoaderFactory::registers()
/// function, giving the Factory name, and a new Factory object.
///
/// - The Factory user can use the registered Factory in using the ResourceLoaderFactory::get()
/// function.
/// - The user is responsible of the deletion of the factory, but you have to keep
/// in mind that ResourceManager::load() destroys the Factory object.
///
/// @see ResourceLoader
////////////////////////////////////////////////////////////////////////
template <typename T>
class ResourceLoaderFactory
{
public:
    
    POOLED(Pools::Factory)
    
    ////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////
    ResourceLoaderFactory () { }
    
    ////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////
    virtual ~ResourceLoaderFactory () { }
    
    ////////////////////////////////////////////////////////////////////////
    /// @brief Registers a new Loader in this Factory.
    ////////////////////////////////////////////////////////////////////////
    void registers(const std::string& name, T* loader)
    { _loaders[name].reset(loader); }
    
    ////////////////////////////////////////////////////////////////////////
    /// @brief Return Loader identified by given name.
    ////////////////////////////////////////////////////////////////////////
    T* get(const std::string& name)
    {
        auto it = _loaders.find(name);
        if(it != _loaders.end())
            return reinterpret_cast<T*>(_loaders[name]->clone());
        else
            return nullptr;
    }
    
    ////////////////////////////////////////////////////////////////////////
    /// @brief Returns first Loader in this Factory.
    ////////////////////////////////////////////////////////////////////////
    T* getFirst ()
    {
        if(!_loaders.empty())
            return reinterpret_cast<T*>(_loaders.begin()->second->clone());
        return nullptr;
    }
    
    ////////////////////////////////////////////////////////////////////////
    /// @brief Reset the Loader's list.
    ////////////////////////////////////////////////////////////////////////
    void clear()
    { _loaders.clear(); }
    
    ////////////////////////////////////////////////////////////////////////
    /// @brief Returns a list with the name of every Loader.
    ////////////////////////////////////////////////////////////////////////
    StringList getLoadersName() const
    {
        StringList ret;
        for(auto it = _loaders.begin(); it != _loaders.end(); it++) {
            ret.push_back(it->first);
        }
        
        return ret;
    }
    
protected:
    
    /// @brief Loader's list, by name.
    std::map<std::string, std::shared_ptr<T> > _loaders;
};

GreEndNamespace

#endif
