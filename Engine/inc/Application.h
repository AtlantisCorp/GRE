//////////////////////////////////////////////////////////////////////
//
//  Application.h
//  This source file is part of Gre
//		(Gang's Resource Engine)
//
//  Copyright (c) 2015 - 2016 Luk2010
//  Created on 08/10/2016.
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

#ifndef Application_h
#define Application_h

#include "Pools.h"
#include "Resource.h"

GreBeginNamespace

//////////////////////////////////////////////////////////////////////
/// @brief An Application Object.
//////////////////////////////////////////////////////////////////////
class DLL_PUBLIC ApplicationPrivate : public Resource
{
public:
    
    POOLED(Pools::Resource)
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    ApplicationPrivate ( const std::string& name , const std::string& author = std::string() , const std::string& description = std::string() ) ;
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual ~ApplicationPrivate () noexcept ( false ) ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Basically runs the Application.
    /// By default , this functions send an update event to every Window
    /// objects in the Window Manager. But , platform-dependtly , this should
    /// be avoided in order to use platform-dependent code . ( like NSApplication
    /// on macOS )
    //////////////////////////////////////////////////////////////////////
    virtual void run () ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Should terminate the run loop.
    //////////////////////////////////////////////////////////////////////
    virtual void terminate () ;
    
protected:
    
    /// @brief Author string.
    std::string iAuthor ;
    
    /// @brief Description string.
    std::string iDescription ;
    
    /// @brief Flag to indicate the loop must stop.
    bool iShouldTerminate ;
};

/// @brief SpecializedResourceHolder for ApplicationPrivate.
typedef SpecializedResourceHolder < ApplicationPrivate > ApplicationHolder ;

/// @brief SpecializedResourceHolderList for ApplicationPrivate.
typedef SpecializedResourceHolderList < ApplicationPrivate > ApplicationHolderList ;

//////////////////////////////////////////////////////////////////////
/// @brief SpecializedResourceUser for ApplicationPrivate.
//////////////////////////////////////////////////////////////////////
class DLL_PUBLIC Application : public SpecializedResourceUser < ApplicationPrivate >
{
public:
    
    POOLED(Pools::Resource)
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    Application ( const ApplicationPrivate* pointer ) ;
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    Application ( const ApplicationHolder& holder ) ;
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    Application ( const Application& user ) ;
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual ~Application() noexcept ( false ) ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Basically runs the Application.
    //////////////////////////////////////////////////////////////////////
    virtual void run () ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Should terminate the run loop.
    //////////////////////////////////////////////////////////////////////
    virtual void terminate () ;
    
    /// @brief A Null Application object.
    static Application Null ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Uses the ResourceManager to create an Application using the
    /// first loader encountered.
    //////////////////////////////////////////////////////////////////////
    static ApplicationHolder Create ( const std::string& name , const std::string& author = std::string() , const std::string& description = std::string() ) ;
};

//////////////////////////////////////////////////////////////////////
/// @brief ResourceLoader for ApplicationPrivate.
//////////////////////////////////////////////////////////////////////
class DLL_PUBLIC ApplicationLoader : public ResourceLoader
{
public:
    
    POOLED(Pools::Loader)
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    ApplicationLoader() ;
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual ~ApplicationLoader() noexcept ( false ) ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Loads an Application an returns its Holder.
    //////////////////////////////////////////////////////////////////////
    virtual ApplicationHolder load ( const std::string& name , const std::string& author , const std::string& desc ) const = 0 ;
};

/// @brief ResourceLoaderFactory for ApplicationLoader.
typedef ResourceLoaderFactory < ApplicationLoader > ApplicationLoaderFactory ;

GreEndNamespace

#endif /* Application_h */
