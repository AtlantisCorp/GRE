//////////////////////////////////////////////////////////////////////
//
//  Animator.h
//  This source file is part of Gre
//		(Gang's Resource Engine)
//
//  Copyright (c) 2015 - 2016 Luk2010
//  Created on 08/12/2016.
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

#ifndef Animator_h
#define Animator_h

#include "Pools.h"
#include "Resource.h"
#include "SpecializedResourceManager.h"

GreBeginNamespace

//////////////////////////////////////////////////////////////////////
/// @brief An Object holding a function and called at each update
/// of the AnimatorManager.
///
/// This object can be used to update other objects independently from
/// the rendering loop , and permanently.
///
/// You can load an Animator using the 'AnimatorManager::load' function.
/// This Animator will have to be provided a AnimatorCallback function
/// in orer to update the property. It can also be linked to a Resource,
/// and the given Animator will stop update the property when the
/// Resource is unloaded.
///
//////////////////////////////////////////////////////////////////////

/*
 
 AnimatorUser myanimator = AnimatorManager -> load ( "MemberAnimator" , & MyNodeUser::x , [] ( float* x ,
                                                float deltatime ) { *x += deltatime ; } ) ;
 myanimator .lock() -> listen ( mynodeuser ) ;
 
 */

class DLL_PUBLIC Animator : public Resource
{
public:
    
    POOLED ( Pools::Resource )
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    Animator ( ) ;
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    Animator ( const std::string & name ) ;
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual ~Animator ( ) noexcept ( false ) ;
    
    /*
    //////////////////////////////////////////////////////////////////////
    /// @brief Registers the listened Resource and calls
    /// 'EventProceeder::listen'.
    /// If a listener is already registered, this function also try to
    /// removes ourself from the resource listener 's list.
    //////////////////////////////////////////////////////////////////////
    virtual void listen ( ResourceUser & proceeder ) const
    {
        GreAutolock ;
        
        if ( !iListened.isInvalid() )
        {
            iListened.lock() -> removeListener ( ResourceUser(this) ) ;
        }
        
        iListened = resource ;
        
        if ( !iListened.isInvalid() )
        {
            iListened.lock() -> addListener ( ResourceUser(this) ) ;
        }
    }
    */
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the listened Resource.
    //////////////////////////////////////////////////////////////////////
    virtual const ResourceUser & getListenedResource ( ) const
    {
        GreAutolock ;
        return iListened ;
    }
    
protected:
    
    /// @brief Listened Resource. Null if none is listened.
    ResourceUser iListened ;
};

/// @brief SpecializedCountedObjectHolder for Animator.
typedef SpecializedCountedObjectHolder < Animator > AnimatorHolder ;

/// @brief SpecializedResourceHolderList for Animator.
typedef SpecializedResourceHolderList < Animator > AnimatorHolderList ;

/// @brief SpecializedCountedObjectUser for Animator.
typedef SpecializedCountedObjectUser < Animator > AnimatorUser ;

/// @brief Defines an AnimatorCallback.
/// Uses 'AnimatorCallbackI::type' to guess the correct function type, or
/// 'AnimatorCallback' .
template < typename T > struct AnimatorCallbackI {
    typedef void (*type) ( ResourceUser , T * , float ) ;
} ;

template < typename T >
using AnimatorCallback = typename AnimatorCallbackI<T>::type ;

//////////////////////////////////////////////////////////////////////
/// @brief A Specialized Animator for a given property type.
//////////////////////////////////////////////////////////////////////
template < typename MemberType >
class MemberAnimator : public Animator
{
public:
    
    POOLED ( Pools::Resource )
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    MemberAnimator ( MemberType * m , AnimatorCallback<MemberType> callback )
    : Animator ( ) , iProperty ( m ) , iCallback ( callback )
    {
#ifdef GreIsDebugMode
        if ( !m )
        {
            GreDebugPretty() << "Null Property Given." << std::endl;
        }
#endif
    }
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    MemberAnimator ( const std::string & name , MemberType * m , AnimatorCallback<MemberType> callback )
    : Animator ( name ) , iProperty ( m ) , iCallback ( callback )
    {
#ifdef GreIsDebugMode
        if ( !m )
        {
            GreDebugPretty() << "Null Property Given." << std::endl;
        }
#endif
    }
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual ~MemberAnimator ( ) noexcept ( false )
    {
        
    }
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Changes the property updated.
    //////////////////////////////////////////////////////////////////////
    virtual void setProperty ( MemberType * property )
    {
        GreAutolock ;
        iProperty = property ;
    }
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the property updated.
    //////////////////////////////////////////////////////////////////////
    virtual MemberType getProperty ( ) const
    {
        GreAutolock ;
        
        if ( iProperty )
            return * iProperty ;
        else
            return MemberType () ;
    }
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Changes the callback.
    //////////////////////////////////////////////////////////////////////
    virtual void setCallback ( AnimatorCallback<MemberType> callback )
    {
        GreAutolock ; iCallback = callback ;
    }
    
protected:
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Calls the Callback function, if 'iProperty' is non-null.
    //////////////////////////////////////////////////////////////////////
    virtual void onUpdateEvent ( const UpdateEvent & e )
    {
        GreAutolock ;
        
        if ( iProperty && iCallback )
        {
            iCallback ( iListened , iProperty , e.elapsedTime.count() ) ;
        }
    }
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Sets 'iProperty' and 'iListened' to null.
    //////////////////////////////////////////////////////////////////////
    virtual void onResourceUnloadedEvent ( const ResourceUnloadedEvent & e )
    {
        GreAutolock ;
        
        Animator::onResourceUnloadedEvent(e) ;
        iProperty = nullptr ;
    }

protected:
    
    /// @brief Property to update.
    MemberType * iProperty ;
    
    /// @brief Callback.
    AnimatorCallback<MemberType> iCallback ;
};

//////////////////////////////////////////////////////////////////////
/// @brief Loads a basic Animator.
//////////////////////////////////////////////////////////////////////
class DLL_PUBLIC AnimatorLoader : public ResourceLoader
{
public:
    
    POOLED ( Pools::Loader )
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    AnimatorLoader ( ) ;
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual ~AnimatorLoader ( ) noexcept ( false ) ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Loads a 'MemberAnimator' depending on the Member Property.
    //////////////////////////////////////////////////////////////////////
    template < typename MemberType >
    AnimatorHolder load ( const std::string & name , MemberType * property , ResourceUser listened ,
                         AnimatorCallback<MemberType> callback ) const ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns always true.
    //////////////////////////////////////////////////////////////////////
    virtual bool isLoadable ( const std::string & l ) const ;
};

//////////////////////////////////////////////////////////////////////
/// @brief The Animator Manager.
//////////////////////////////////////////////////////////////////////
class DLL_PUBLIC AnimatorManager : public SpecializedResourceManager < Animator , AnimatorLoader >
{
public:
    
    POOLED ( Pools::Manager )
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    AnimatorManager ( ) ;
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual ~AnimatorManager ( ) noexcept ( false ) ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Loads an Animator Object using the AnimatorLoader referenced.
    //////////////////////////////////////////////////////////////////////
    template < typename T >
    AnimatorUser load ( const std::string & name , T * property , ResourceUser listened ,
                        AnimatorCallback<T> callback )
    {
        return AnimatorUser ( nullptr ) ;
    }
};

/// @brief SpecializedCountedObjectHolder for AnimatorManager.
typedef SpecializedCountedObjectHolder < AnimatorManager > AnimatorManagerHolder ;

GreEndNamespace

#endif /* Animator_h */
