//////////////////////////////////////////////////////////////////////
//
//  Resource.h
//  This source file is part of Gre
//		(Gang's Resource Engine)
//
//  Copyright (c) 2015 - 2017 Luk2010
//  Created on 06/05/2016.
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

#ifndef GRE_Resource_h
#define GRE_Resource_h

#include "Pools.h"
#include "EventProceeder.h"

GreBeginNamespace

////////////////////////////////////////////////////////////////////////
/// @brief A Resource Property.
////////////////////////////////////////////////////////////////////////
struct ResourceProperty
{
    std::string name ;
    std::string description ;
    void* data ;
};

////////////////////////////////////////////////////////////////////////
/// @brief Singleton to generate Resource's ID .
////////////////////////////////////////////////////////////////////////
class DLL_PUBLIC ResourceIdentifier
{
public:

    ////////////////////////////////////////////////////////////////////////
    /// @brief Creates a new identifier.
    ////////////////////////////////////////////////////////////////////////
    static ResourceIdentifier New () ;

    ////////////////////////////////////////////////////////////////////////
    /// @brief Gets last identifier.
    ////////////////////////////////////////////////////////////////////////
    static ResourceIdentifier Last () ;

protected:

    ////////////////////////////////////////////////////////////////////////
    /// @brief Current ResourceIdentifier.
    ////////////////////////////////////////////////////////////////////////
    static ResourceIdentifier Current ;

public:

    ////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////
    ResourceIdentifier ( unsigned long int value ) ;

    ////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////
    ResourceIdentifier ( const ResourceIdentifier& identifier ) ;

    ////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////
    operator unsigned long int ( ) ;

    ////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////
    operator const unsigned long int ( ) const ;

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    ResourceIdentifier & operator ++ ( ) ;

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    ResourceIdentifier operator ++ ( int ) ;

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    ResourceIdentifier & operator -- ( ) ;

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    ResourceIdentifier operator -- ( int ) ;

    /// @brief The value.
    unsigned long int iValue ;
};

////////////////////////////////////////////////////////////////////////
/// @brief A Resource base object.
///
/// A Resource is an object that can be 'loaded' by a 'ResourceLoader' ,
/// 'unloaded' by itself ( generally at self destruction ) , written by
/// a 'ResourceWriter' , and managed by a 'ResourceManager' (taking in
/// account the 'ResourceManager' is itself a Resource) .
///
/// A Resource can have various property , even set by the user. A
/// ResourceProperty is a structure containing its name and description.
/// The user has the responsability of filling the 'data' field. This is
/// done in 'getProperty()' function. You can directly access 'data' field
/// using 'getPropertyData()' .
///
/// When a Resource is created by a ResourceLoader , its loaded status will
/// be 'true' . If the Resource is unloaded (using the function 'unload()') ,
/// its status will become 'false'.
///
/// You can't reload a Resource. You have to use the ResourceLoader again.
/// If you want to have ResourceObjects that are guaranteed to be reloadable,
/// use 'IndependentResource' . Those have 'load', 'reload' and 'unload'
/// functions.
///
/// A Resource does not have necessarly a name. Resource must be identified
/// using their unique ID , given by 'ResourceIdentifier::New()' . Those
/// ID are unique in the program , but not between sessions. Default name
/// for a Resource is 'Default' .
///
////////////////////////////////////////////////////////////////////////
class DLL_PUBLIC Resource : public EventProceeder
{
public:

    POOLED( Pools::Referenced )

    ////////////////////////////////////////////////////////////////////////
    /// @brief Default constructor.
    /// The Resource is initialized using name 'Default' and id generated
    /// using 'New()' .
    ////////////////////////////////////////////////////////////////////////
    Resource ( ) ;

    ////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////
    Resource ( const std::string & name ) ;

    ////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////
    Resource ( const ResourceIdentifier & identifier , const std::string & name = "Default" ) ;

    ////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////
    virtual ~Resource () noexcept ( false ) ;

    ////////////////////////////////////////////////////////////////////////
    /// @brief Return the Resource's identifier.
    ////////////////////////////////////////////////////////////////////////
    const ResourceIdentifier & getIdentifier () const ;

    ////////////////////////////////////////////////////////////////////////
    /// @brief Returns the Resource's name.
    ////////////////////////////////////////////////////////////////////////
    const std::string& getName () const ;

    ////////////////////////////////////////////////////////////////////////
    /// @brief Changes the Resource's name.
    ////////////////////////////////////////////////////////////////////////
    void setName ( const std::string& name ) ;

    ////////////////////////////////////////////////////////////////////////
    /// @brief Returns the property for given name.
    ////////////////////////////////////////////////////////////////////////
    virtual const ResourceProperty & getProperty ( const std::string& name ) const ;

    ////////////////////////////////////////////////////////////////////////
    /// @brief Returns the 'data' field for given property.
    ////////////////////////////////////////////////////////////////////////
    virtual const void * getPropertyData ( const std::string& name ) const ;

    ////////////////////////////////////////////////////////////////////////
    /// @brief Returns the 'iLoadStatus' property.
    ////////////////////////////////////////////////////////////////////////
    bool isLoaded () const ;

    ////////////////////////////////////////////////////////////////////////
    /// @brief Unloads the Resource.
    ////////////////////////////////////////////////////////////////////////
    virtual void unload () ;

    /// @brief A Null ResourceProperty.
    static ResourceProperty BadProperty ;

protected:

    /// @brief Identifier value.
    ResourceIdentifier iIdentifier ;

    /// @brief Name value.
    std::string iName ;

    /// @brief Propertie's list.
    std::vector < ResourceProperty > iProperties ;

    /// @brief Loading status. True if the Resource is loaded , false otherwise.
    bool iLoadStatus ;
};

/// @brief Holder for Resource .
typedef Holder<Resource> ResourceHolder ;

////////////////////////////////////////////////////////////////////////
/// @brief A Specialized List for Resource objects .
////////////////////////////////////////////////////////////////////////
template < typename Class >
class SpecializedResourceHolderList : public std::list < Holder < Class > > , public Lockable
{
public:

    /// @brief Common typedef to avoid typename use.
    typedef Holder< Class > ClassHolder ;
    typedef typename std::list< ClassHolder > list_t ;

    ////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////
    SpecializedResourceHolderList ()
    {

    }

    ////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////
    SpecializedResourceHolderList ( const SpecializedResourceHolderList<Class> & rhs )
    : list_t (rhs) , Lockable ()
    {

    }

    ////////////////////////////////////////////////////////////////////////
    /// @brief Adds a new Holder .
    ////////////////////////////////////////////////////////////////////////
    void add ( const ClassHolder & holder )
    {
        GreAutolock ;
        list_t::push_back ( holder ) ;
    }

    ////////////////////////////////////////////////////////////////////////
    /// @brief Adds a new Object , creating a new Holder .
    ////////////////////////////////////////////////////////////////////////
    void add ( const Class * object )
    {
        GreAutolock ;
        list_t::push_back ( ClassHolder(object) ) ;
    }

    ////////////////////////////////////////////////////////////////////////
    /// @brief Finds the first object with the given identifier.
    ////////////////////////////////////////////////////////////////////////
    typename list_t::iterator find ( const ResourceIdentifier & identifier )
    {
        GreAutolock ;

        for ( auto it = list_t::begin() ; it != list_t::end() ; it++ )
        {
            if ( !it->isInvalid() )
            {
                auto object = it->getObject () ;

                if ( object )
                {
                    if ( object->getIdentifier() == identifier )
                    {
                        return it ;
                    }
                }
            }
        }

        return list_t::end() ;
    }

    ////////////////////////////////////////////////////////////////////////
    /// @brief Finds the first object with the given identifier.
    ////////////////////////////////////////////////////////////////////////
    typename list_t::const_iterator find ( const ResourceIdentifier & identifier ) const
    {
        GreAutolock ;

        for ( auto it = list_t::begin() ; it != list_t::end() ; it++ )
        {
            if ( !it->isInvalid() )
            {
                auto object = it->getObject () ;

                if ( object )
                {
                    if ( object->getIdentifier() == identifier )
                    {
                        return it ;
                    }
                }
            }
        }
    }

    ////////////////////////////////////////////////////////////////////////
    /// @brief Finds the first object with the given name.
    ////////////////////////////////////////////////////////////////////////
    typename list_t::iterator find ( const std::string & name )
    {
        GreAutolock ;

        for ( auto it = list_t::begin() ; it != list_t::end() ; it++ )
        {
            if ( !it->isInvalid() )
            {
                auto object = it->getObject () ;

                if ( object )
                {
                    if ( object->getName() == name )
                    {
                        return it ;
                    }
                }
            }
        }

        return list_t::end () ;
    }

    ////////////////////////////////////////////////////////////////////////
    /// @brief Finds the first object with the given name.
    ////////////////////////////////////////////////////////////////////////
    typename list_t::const_iterator find ( const std::string & name ) const
    {
        GreAutolock ;

        for ( auto it = list_t::begin() ; it != list_t::end() ; it++ )
        {
            if ( !it->isInvalid() )
            {
                auto object = it->getObject () ;

                if ( object )
                {
                    if ( object->getName() == name )
                    {
                        return it ;
                    }
                }
            }
        }

        return list_t::end () ;
    }

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    SpecializedResourceHolderList < Class > & operator = ( const SpecializedResourceHolderList<Class> & rhs )
    {
        GreAutolock ;
        list_t::operator=(rhs);
        return *this ;
    }

    /// @brief An empty list made for boolean states only.
    static const SpecializedResourceHolderList < Class > Empty ;
};

template < typename Class >
const SpecializedResourceHolderList < Class > SpecializedResourceHolderList < Class > :: Empty ;

////////////////////////////////////////////////////////////////////////
/// @brief An IndependentResource interface.
///
/// An IndependentResource object is a Resource where the 'load', 'reload'
/// and 'unload' actions can all be performed by the object itself.
///
/// By default , 'reload' calls 'unload' and then 'load' with the passed
/// arguments.
///
////////////////////////////////////////////////////////////////////////
class DLL_PUBLIC IndependentResource : public Resource
{
public:

    POOLED ( Pools::Referenced )

    ////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////
    IndependentResource () ;

    ////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////
    IndependentResource ( const std::string & name ) ;

    ////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////
    IndependentResource ( const ResourceIdentifier & identifier , const std::string & name = "Default" ) ;

    ////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////
    virtual ~IndependentResource () noexcept ( false ) ;

    ////////////////////////////////////////////////////////////////////////
    /// @brief Loads the Resource with the given arguments passed as a string.
    ////////////////////////////////////////////////////////////////////////
    virtual bool loadResource ( const std::string & arguments = std::string () ) = 0 ;

    ////////////////////////////////////////////////////////////////////////
    /// @brief Reloads the Resource with the given arguments passed as a string.
    ////////////////////////////////////////////////////////////////////////
    virtual bool reloadResource ( const std::string & arguments = std::string () ) ;
};

//////////////////////////////////////////////////////////////////////
/// @brief Creates typedef for holder and holder list.
#define GRE_MAKE_HOLDER( classname ) \
    typedef Holder < classname > classname ## Holder ; \
    typedef SpecializedResourceHolderList < classname > classname ## HolderList

GreEndNamespace

#endif
