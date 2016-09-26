//////////////////////////////////////////////////////////////////////
//
//  ResourceHolder.h
//  This source file is part of Gre
//		(Gang's Resource Engine)
//
//  Copyright (c) 2015 - 2016 Luk2010
//  Created on 02/05/2016.
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

#ifndef GRE_ResourceHolder_h
#define GRE_ResourceHolder_h

#include "Resource.h"

GreBeginNamespace

//////////////////////////////////////////////////////////////////////
/// @class ResourceHolder
/// @brief Scoped Resource Holder.
///
/// At construction, the ResourceHolder acquire the Resource. When
/// destruction is requested, calls Resource::release(). It also holds
/// a pointer to the Resource.
///
//////////////////////////////////////////////////////////////////////
class DLL_PUBLIC ResourceHolder
{
public:
    
    POOLED(Pools::Resource)
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Constructs a ResourceHolder using given Resource.
    /// @note Calls Resource::acquire.
    //////////////////////////////////////////////////////////////////////
    ResourceHolder(Resource* resource);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Constructs a ResourceHolder, given the resource and the
    /// ReferenceCounter it is normally attached to.
    ///
    /// This constructor is provided in order to manage that, when a
    /// ResourceUser locks the Resource, it constructs a ResourceHolder.
    /// But the ResourceUser can do it even if the Resource pointer is
    /// invalidated. So, we handle this case by using this constructor :
    /// if counter->getHolderCount() == 0, this means there isn't any
    /// ResourceHolder in place, so we should use resource as a nullptr.
    //////////////////////////////////////////////////////////////////////
    ResourceHolder(Resource* resource, ReferenceCounter* counter);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Copy the ResourceHolder.
    //////////////////////////////////////////////////////////////////////
    ResourceHolder(const ResourceHolder& holder);
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual ~ResourceHolder();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the raw Resource.
    //////////////////////////////////////////////////////////////////////
    Resource* get();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the raw Resource.
    //////////////////////////////////////////////////////////////////////
    const Resource* get() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns true if iResource is null.
    //////////////////////////////////////////////////////////////////////
    bool isNull() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns true if Resource is null.
    //////////////////////////////////////////////////////////////////////
    bool isInvalid() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns false if iResource is null.
    //////////////////////////////////////////////////////////////////////
    operator bool () const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Equivalent to ::get().
    //////////////////////////////////////////////////////////////////////
    Resource* operator -> ();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Equivalent to ::get().
    //////////////////////////////////////////////////////////////////////
    const Resource* operator -> () const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Reset the pointers.
    //////////////////////////////////////////////////////////////////////
    virtual void reset();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Copies given ResourceHolder.
    //////////////////////////////////////////////////////////////////////
    ResourceHolder& operator = ( const ResourceHolder& rhs );
    
private:
    
    /// @brief The Holded Resource.
    Resource* iResource;
    
    /// @brief The Reference Counter.
    ReferenceCounter* iCounter;
};

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
template<typename Class>
class SpecializedResourceHolder : public ResourceHolder
{
public:
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    SpecializedResourceHolder ( Class* resource )
    : ResourceHolder ( resource )
    {
        iClass = resource;
    }
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    SpecializedResourceHolder(Class* resource, ReferenceCounter* counter)
    : ResourceHolder(resource, counter)
    {
        iClass = resource;
    }
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    SpecializedResourceHolder(const SpecializedResourceHolder<Class>& holder)
    : ResourceHolder(holder)
    {
        iClass = holder.iClass;
    }
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the raw Resource.
    //////////////////////////////////////////////////////////////////////
    Class* get() { return iClass; }
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the raw Resource.
    //////////////////////////////////////////////////////////////////////
    const Class* get() const { return iClass; }
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Equivalent to ::get().
    //////////////////////////////////////////////////////////////////////
    Class* operator -> () { return get(); }
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Equivalent to ::get().
    //////////////////////////////////////////////////////////////////////
    const Class* operator -> () const { return get(); }
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Reset the Holder.
    //////////////////////////////////////////////////////////////////////
    virtual void reset() { iClass = nullptr; ResourceHolder::reset(); }
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    SpecializedResourceHolder<Class>& operator = (const SpecializedResourceHolder<Class>& rhs)
    {
        ResourceHolder::operator=(rhs);
        iClass = rhs.iClass;
        return *this;
    }
    
private:
    
    /// @brief Holds the specialized version.
    Class* iClass;
};

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
template<typename Class>
class SpecializedResourceHolderList : public std::list< SpecializedResourceHolder<Class> >
{
public:
    
    /// @brief Common typedef to avoid typename use.
    typedef typename std::list< SpecializedResourceHolder<Class> > list_t;
	
	//////////////////////////////////////////////////////////////////////
	/// @brief Adds a SpecializedResourceHolder to the list.
	//////////////////////////////////////////////////////////////////////
	void add(const SpecializedResourceHolder<Class>& obj)
	{
		this->push_back(obj);
	}
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Adds a SpecializedResourceHolderList to the list.
    //////////////////////////////////////////////////////////////////////
    void add ( const SpecializedResourceHolderList<Class>& objs )
    {
        for ( auto obj : objs )
        {
            add(obj);
        }
    }
	
	//////////////////////////////////////////////////////////////////////
	/// @brief Adds a Resource Pointer to the list.
	//////////////////////////////////////////////////////////////////////
	void add(Class* obj)
	{
		this->push_back(SpecializedResourceHolder<Class>(obj));
	}
    
    ////////////////////////////////////////////////////////////////////////
    /// @brief Removes a Resource Holder from its name.
    ////////////////////////////////////////////////////////////////////////
    void remove(const std::string& name)
    {
        auto it = find(name);
        if(it != list_t::end())
        {
            this->erase(it);
        }
    }
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns an iterator corresponding to the first match for
    /// given object.
    //////////////////////////////////////////////////////////////////////
    typename list_t::iterator find(const SpecializedResourceHolder<Class>& obj)
    {
        for(auto it = list_t::begin(); it != list_t::end(); it++)
        {
            if( (*it).get() == obj.get() )
            {
                return it;
            }
        }
        
        return list_t::end();
    }
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns an iterator corresponding to the first match for
    /// given object.
    //////////////////////////////////////////////////////////////////////
    typename list_t::const_iterator find(const SpecializedResourceHolder<Class>& obj) const
    {
        for(auto it = list_t::begin(); it != list_t::end(); it++)
        {
            if( (*it).get() == obj.get() )
            {
                return it;
            }
        }
        
        return list_t::end();
    }
	
	//////////////////////////////////////////////////////////////////////
	/// @brief Returns an iterator corresponding to the first match for
	/// given name.
	//////////////////////////////////////////////////////////////////////
    typename list_t::iterator find(const std::string& name)
	{
        for(auto it = list_t::begin(); it != list_t::end(); it++)
		{
			if((*it)->getName() == name)
				return it;
		}
        
        return list_t::end();
	}
	
	//////////////////////////////////////////////////////////////////////
	/// @brief Returns an iterator corresponding to the first match for
	/// given name.
	//////////////////////////////////////////////////////////////////////
	typename list_t::const_iterator find(const std::string& name) const
	{
		for(auto it = list_t::begin(); it != list_t::end(); it++) 
		{
			if((*it)->getName() == name)
				return it;
		}
        
        return list_t::end();
	}
};

GreEndNamespace

#endif
