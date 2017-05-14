//////////////////////////////////////////////////////////////////////
//
//  SpecializedResourceManager.h
//  This source file is part of Gre
//		(Gang's Resource Engine)
//
//  Copyright (c) 2015 - 2017 Luk2010
//  Created on 12/11/2016.
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

#ifndef SpecializedResourceManager_h
#define SpecializedResourceManager_h

#include "Pools.h"
#include "Resource.h"
#include "ResourceLoader.h"

GreBeginNamespace

////////////////////////////////////////////////////////////////////////
/// @brief Manages only Resource objects.
////////////////////////////////////////////////////////////////////////
template < typename Class >
class ResourceManagerBase : public IndependentResource
{
public:

    POOLED ( Pools::Referenced )

    typedef Holder<Class> ClassHolder ;
    typedef SpecializedResourceHolderList<Class> ClassHolderList ;

    ////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////
    ResourceManagerBase ( ) : IndependentResource ( )
    {

    }

    ////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////
    ResourceManagerBase ( const std::string & name ) : IndependentResource ( name )
    {

    }

    ////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////
    ResourceManagerBase ( const ResourceIdentifier & identifier , const std::string & name ) : IndependentResource (identifier, name)
    {

    }

    ////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////
    virtual ~ResourceManagerBase ( ) noexcept ( false )
    {

    }

    ////////////////////////////////////////////////////////////////////////
    /// @brief Returns a Resource from given identifier.
    ////////////////////////////////////////////////////////////////////////
    virtual ClassHolder find ( const ResourceIdentifier & identifier )
    {
        GreAutolock ;

        for ( ClassHolder& holder : iHolders )
        {
            if ( !holder.isInvalid() )
            {
                if ( holder->getIdentifier() == identifier )
                {
                    return ClassHolder ( holder ) ;
                }
            }
        }

        return ClassHolder ( nullptr ) ;
    }

    ////////////////////////////////////////////////////////////////////////
    /// @brief Returns a Resource from given identifier.
    ////////////////////////////////////////////////////////////////////////
    virtual const ClassHolder find ( const ResourceIdentifier & identifier ) const
    {
        GreAutolock ;

        for ( const ClassHolder& holder : iHolders )
        {
            if ( !holder.isInvalid() )
            {
                if ( holder->getIdentifier() == identifier )
                {
                    return ClassHolder ( holder ) ;
                }
            }
        }

        return ClassHolder ( nullptr ) ;
    }

    ////////////////////////////////////////////////////////////////////////
    /// @brief Finds a Resource and return its holder.
    ////////////////////////////////////////////////////////////////////////
    virtual const ClassHolder findHolder ( const ResourceIdentifier & identifier ) const
    {
        GreAutolock ;

        for ( const ClassHolder & holder : iHolders )
        {
            if ( !holder.isInvalid() )
            {
                if ( holder->getIdentifier() == identifier )
                {
                    return holder ;
                }
            }
        }

        return ClassHolder ( nullptr ) ;
    }

    ////////////////////////////////////////////////////////////////////////
    /// @brief Returns the first Resource with given name.
    ////////////////////////////////////////////////////////////////////////
    virtual ClassHolder findFirst ( const std::string & name )
    {
        GreAutolock ;

        for ( ClassHolder& holder : iHolders )
        {
            if ( !holder.isInvalid() )
            {
                if ( holder->getName() == name )
                {
                    return ClassHolder ( holder ) ;
                }
            }
        }

        return ClassHolder ( nullptr ) ;
    }

    ////////////////////////////////////////////////////////////////////////
    /// @brief Returns the first Resource with given name.
    ////////////////////////////////////////////////////////////////////////
    virtual const ClassHolder findFirst ( const std::string & name ) const
    {
        GreAutolock ;

        for ( const ClassHolder& holder : iHolders )
        {
            if ( !holder.isInvalid() )
            {
                if ( holder->getName() == name )
                {
                    return ClassHolder ( holder ) ;
                }
            }
        }

        return ClassHolder ( nullptr ) ;
    }

    ////////////////////////////////////////////////////////////////////////
    /// @brief Returns the first Resource with given name and its holder.
    ////////////////////////////////////////////////////////////////////////
    virtual ClassHolder findFirstHolder ( const std::string & name )
    {
        GreAutolock ;

        for ( ClassHolder& holder : iHolders )
        {
            if ( !holder.isInvalid() )
            {
                if ( holder->getName() == name )
                {
                    return holder ;
                }
            }
        }

        return ClassHolder ( nullptr ) ;
    }

    ////////////////////////////////////////////////////////////////////////
    /// @brief Returns the first Resource with given name and its holder.
    ////////////////////////////////////////////////////////////////////////
    virtual const ClassHolder findFirstHolder ( const std::string & name ) const
    {
        GreAutolock ;

        for ( const ClassHolder& holder : iHolders )
        {
            if ( !holder.isInvalid() )
            {
                if ( holder->getName() == name )
                {
                    return holder ;
                }
            }
        }

        return ClassHolder ( nullptr ) ;
    }

    ////////////////////////////////////////////////////////////////////////
    /// @brief Removes a Resource.
    ////////////////////////////////////////////////////////////////////////
    virtual void remove ( const ClassHolder & holder )
    {
        GreAutolock ;

        if ( !holder.isInvalid() )
        {
            remove ( holder->getIdentifier() ) ;
        }
    }

    ////////////////////////////////////////////////////////////////////////
    /// @brief Removes a Resource from its Identifier.
    ////////////////////////////////////////////////////////////////////////
    virtual void remove ( const ResourceIdentifier & identifier )
    {
        GreAutolock ;

        for ( auto it = iHolders.begin() ; it != iHolders.end() ; it++ )
        {
            if ( !it->isInvalid() )
            {
                if ( (*it)->getIdentifier() == identifier )
                {
                    removeListener( EventProceederHolder( (*it) ) ) ;
                    iHolders.erase(it);
                    return ;
                }
            }
        }
    }

    ////////////////////////////////////////////////////////////////////////
    /// @brief Returns the Resource list.
    ////////////////////////////////////////////////////////////////////////
    virtual const ClassHolderList & getResources () const
    {
        GreAutolock ;
        return iHolders ;
    }

    ////////////////////////////////////////////////////////////////////////
    /// @brief Sets a new Resource list.
    ////////////////////////////////////////////////////////////////////////
    virtual void setResources ( const ClassHolderList & holders )
    {
        GreAutolock ; iHolders = holders ;
    }

    ////////////////////////////////////////////////////////////////////////
    /// @brief Loads the Resource with the given arguments passed as a string.
    ////////////////////////////////////////////////////////////////////////
    virtual bool loadResource ( const std::string & arguments )
    {
        GreAutolock ; iLoadStatus = true ;
        return true ;
    }

    ////////////////////////////////////////////////////////////////////////
    /// @brief Unloads the Resource.
    ////////////////////////////////////////////////////////////////////////
    virtual void unload ()
    {
        GreAutolock ; iHolders.clear(); iLoadStatus = false;
    }

protected:

    /// @brief Holds the Resources managed objects.
    ClassHolderList iHolders ;
};

////////////////////////////////////////////////////////////////////////
/// @brief A Specialized Manager that can load / unload Resource.
///
/// The first template argument is the base class for this type of Resource ,
/// for example 'HardwareProgram' .
/// The second template argument is the base class for every loader for this
/// Resource.
///
/// A subclass should follow the following pattern to create a new manager :
///   - To create a new Resource , the function 'virtual ClassHolder load ( ... )'
/// should always be implemented.
///   - You can also add any function of your choice.
///
////////////////////////////////////////////////////////////////////////
template < typename Class , typename ClassLoader >
class DLL_PUBLIC SpecializedResourceManager : public ResourceManagerBase < Class >
{
public:

    POOLED ( Pools::Manager )

    typedef ResourceLoaderFactory<ClassLoader> ClassLoaderFactory ;

    ////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////
    SpecializedResourceManager ( ) : ResourceManagerBase<Class> ( )
    {

    }

    ////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////
    SpecializedResourceManager ( const std::string & name )
    : Gre::ResourceManagerBase<Class> ( ResourceIdentifier::New() , name )
    {

    }

    ////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////
    virtual ~SpecializedResourceManager () noexcept ( false )
    {

    }

    ////////////////////////////////////////////////////////////////////////
    /// @brief Returns 'iFactory'.
    ////////////////////////////////////////////////////////////////////////
    virtual ClassLoaderFactory & getFactory ()
    {
        GreAutolock ; return iLoaders ;
    }

    ////////////////////////////////////////////////////////////////////////
    /// @brief Returns the loader factory.
    ////////////////////////////////////////////////////////////////////////
    virtual const ClassLoaderFactory & getFactory () const
    {
        GreAutolock ; return iLoaders ;
    }

    ////////////////////////////////////////////////////////////////////////
    /// @brief Unloads the Resource.
    ////////////////////////////////////////////////////////////////////////
    virtual void unload ()
    {
        GreAutolock ; iLoaders.clear () ;
        ResourceManagerBase < Class > :: unload () ;
    }

    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the first loader with given name.
    /// If name is empty, returns the first loader.
    //////////////////////////////////////////////////////////////////////
    virtual ClassLoader * findLoader ( const std::string & name = std::string() )
    {
        auto & loaderlist = iLoaders.getLoaders () ;

		if ( name.empty() && loaderlist.size() ) {
			return loaderlist.begin()->second.get() ;
		}

        for ( auto it = loaderlist.begin() ; it != loaderlist.end() ; it++ )
        {
            if ( it->first == name )
            {
                return it->second.get() ;
            }
        }

        return nullptr ;
    }

    //////////////////////////////////////////////////////////////////////
    /// @brief Finds the loader which name is given by the option field
    /// 'Loader.Name' . If no field is found, returns nullptr.
    //////////////////////////////////////////////////////////////////////
    virtual ClassLoader* findLoaderFromOptions ( const ResourceLoaderOptions & ops )
    {
        auto loadernameit = ops.find ( "Loader.Name" ) ;

        if ( loadernameit == ops.end() )
        return nullptr ;

        std::string loadername = loadernameit -> second.to<std::string> () ;

        if ( loadername.empty() )
        return nullptr ;

        return findLoader ( loadername ) ;
    }

protected:

    ////////////////////////////////////////////////////////////////////////
    /// @brief Find the best loader depending on the 'isLoadable' arg.
    /// This loader should not be destroyed by the user.
    ////////////////////////////////////////////////////////////////////////
    virtual ClassLoader * iFindBestLoader ( const std::string & filepath )
    {
        auto & loaderlist = iLoaders.getLoaders () ;

        for ( auto it = loaderlist.begin() ; it != loaderlist.end() ; it++ )
        {
            if ( it->second->isLoadable(filepath) )
            {
                return it->second.get() ;
            }
        }

        return nullptr ;
    }

protected:

    /// @brief Holds the Loaders in a factory.
    ClassLoaderFactory iLoaders ;
};

GreEndNamespace

#endif /* SpecializedResourceManager_h */
