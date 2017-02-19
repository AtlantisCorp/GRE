//////////////////////////////////////////////////////////////////////
//
//  GlXApplication.h
//  This source file is part of Gre
//		(Gang's Resource Engine)
//
//  Copyright (c) 2015 - 2016 Luk2010
//  Created on 17/10/2016.
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

#ifndef GlXApplication_h
#define GlXApplication_h

#include "GlX11Headers.h"

class GlXApplication : public Gre::ApplicationPrivate
{
public:
    
    POOLED(Gre::Pools::Resource)
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    GlXApplication ( const std::string& name , const std::string& author = std::string() , const std::string& description = std::string() ) ;
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual ~GlXApplication () noexcept ( false ) ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Runs the NSApplication object.
    //////////////////////////////////////////////////////////////////////
    virtual void run () ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Terminates the NSApplication run loop.
    //////////////////////////////////////////////////////////////////////
    virtual void terminate () ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the X11 Display associated with this Application.
    //////////////////////////////////////////////////////////////////////
    Display * getXDisplay () ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Return the current GlXApplication object .
    //////////////////////////////////////////////////////////////////////
    static GlXApplication * GetApplication () ;
    
protected:
    
    /// @brief X display.
    Display * iDisplay ;
};

//////////////////////////////////////////////////////////////////////
/// @brief GlXApplication Loader.
//////////////////////////////////////////////////////////////////////
class DLL_PUBLIC GlXApplicationLoader : public Gre::ApplicationLoader
{
public:
    
    POOLED(Gre::Pools::Loader)
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    GlXApplicationLoader () ;

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual ~GlXApplicationLoader () noexcept ( false ) ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Loads an Application an returns its Holder.
    //////////////////////////////////////////////////////////////////////
    virtual Gre::ApplicationHolder load ( const std::string& name , const std::string& author , const std::string& desc ) const;
    
    ////////////////////////////////////////////////////////////////////////
    /// @brief Returns a clone of this object.
    /// Typically, this function is implemented as 'return new MyLoaderClass();',
    /// but you are free to do whatever you want.
    ////////////////////////////////////////////////////////////////////////
    virtual Gre::ResourceLoader* clone() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns true if the file given is loadable by this loader.
    //////////////////////////////////////////////////////////////////////
    virtual bool isLoadable( const std::string& filepath ) const;
};

#endif /* GlXApplication_h */
