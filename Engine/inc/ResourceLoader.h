//////////////////////////////////////////////////////////////////////
//
//  ResourceLoader.h
//  This source file is part of Gre
//		(Gang's Resource Engine)
//
//  Copyright (c) 2015 - 2016 Luk2010
//  Created on 07/05/2016.
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
/// CustomResourceHolder load ( const std::string& name , ... ) ;
///
/// Where '...' are variables arguments for the function.
/// The loaders should not store data because one can clone the Loader and
/// use this cloned object with the ResourceManager to load a new Resource.
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
    ////////////////////////////////////////////////////////////////////////
    virtual ~ResourceLoader() noexcept(false);
    
    ////////////////////////////////////////////////////////////////////////
    /// @brief Returns a clone of this object.
    /// Typically, this function is implemented as 'return new MyLoaderClass();',
    /// but you are free to do whatever you want.
    ////////////////////////////////////////////////////////////////////////
    virtual ResourceLoader* clone() const = 0;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns true if the file given is loadable by this loader.
    //////////////////////////////////////////////////////////////////////
    virtual bool isLoadable( const std::string& filepath ) const = 0;
};

////////////////////////////////////////////////////////////////////////
/// @class ResourceLoaderFactory
/// @brief Constructs a Loader given its name.
///
/// In fact, this class just use the ResourceFactory::clone() function to
/// clone the registered factory for the user to use it.
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
/// in mind that ResourceManager::loadResourceWith() destroys the Factory object.
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
    
    ////////////////////////////////////////////////////////////////////////
    /// @brief Returns the Loader's list.
    ////////////////////////////////////////////////////////////////////////
    std::map<std::string, std::shared_ptr<T> > getLoaders()
    {
        return _loaders;
    }
    
protected:
    
    /// @brief Loader's list, by name.
    std::map<std::string, std::shared_ptr<T> > _loaders;
};

GreEndNamespace

#endif
