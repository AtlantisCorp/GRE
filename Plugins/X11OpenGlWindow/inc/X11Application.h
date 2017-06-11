//////////////////////////////////////////////////////////////////////
//
//  X11Application.h
//  This source file is part of Gre
//      (Gang's Resource Engine)
//
//  Copyright (c) 2015 - 2017 Luk2010
//  Created on 30/05/2017.
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

#ifndef GRE_X11APPLICATION_H
#define GRE_X11APPLICATION_H

#include <Application.h>
#include <X11OpenGl.h>

/// @brief Global used to define a X11Display connection for every Window and
/// Application present in the Engine.
extern ::Display* gX11Display ;

//////////////////////////////////////////////////////////////////////
/// @brief Informations about XKB Extension.
typedef struct {
	int major ;
	int minor ;
	int majorOpcode ;
	int eventBase ;
	int errorBase ;
	bool detectable ;
  	Bool available ;
} X11XKB ;

//////////////////////////////////////////////////////////////////////
/// @brief Manages an X11 running event loop.
//////////////////////////////////////////////////////////////////////
class X11Application : public Gre::Application
{
public:

    POOLED ( Gre::Pools::Referenced )

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    X11Application ( const std::string & name ,
                     const std::string & author = std::string() ,
                     const std::string & description = std::string()
                   ) ;

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual ~X11Application () noexcept ( false ) ;

protected:

    //////////////////////////////////////////////////////////////////////
    /// @brief While terminate condition is not filled , process events
    /// in the X11 queue , update the Window Manager and let the Renderer
    /// Manager render.
    //////////////////////////////////////////////////////////////////////
    virtual void iMainThreadLoop () ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Handles X11 Events and send them to appropriated Windows.
    //////////////////////////////////////////////////////////////////////
    virtual void iX11HandleEvents ( const Gre::EventHolder & updateevent ) ;

public:

    //////////////////////////////////////////////////////////////////////
    /// @brief Initialize X11.
    //////////////////////////////////////////////////////////////////////
    static bool X11Initialize () ;

protected:

    /// @brief Connection to the default X11 Display.
    ::Display* iX11Display ;

  	/// @brief XKB Extension informations.
	X11XKB iX11XKB ;

};

//////////////////////////////////////////////////////////////////////
/// @brief X11Application Loader.
//////////////////////////////////////////////////////////////////////
class X11ApplicationLoader : public Gre::ApplicationLoader
{
public:

    POOLED ( Gre::Pools::Loader )

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    X11ApplicationLoader();

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual ~X11ApplicationLoader() noexcept(false) ;

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual Gre::ApplicationHolder load( const std::string& name , const std::string& author , const std::string& desc ) const ;

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual Gre::ResourceLoader* clone() const ;

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual bool isLoadable ( const std::string& file ) const ;
};

#endif // GRE_X11APPLICATION_H
