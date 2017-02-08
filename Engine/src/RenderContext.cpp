//////////////////////////////////////////////////////////////////////
//
//  RenderContext.cpp
//  This source file is part of Gre
//		(Gang's Resource Engine)
//
//  Copyright (c) 2015 - 2016 Luk2010
//  Created on 15/02/2016.
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

#include "RenderContext.h"
#include "Window.h"

GreBeginNamespace

RenderContext::RenderContext ( const std::string& name )
: Gre::Resource(ResourceIdentifier::New() , name)
{
    Viewport defaultviewport ( name + "/DefaultViewport" , 0.0f , 0.0f , 1.0f , 1.0f ) ;
    iViewports.push_back(defaultviewport);
}

RenderContext::RenderContext(const std::string& name, const RenderContextInfo& ctxtInfo)
: Resource(ResourceIdentifier::New() , name)
, iContextInfo(ctxtInfo)
{
    Viewport defaultviewport ( name + "/DefaultViewport" , 0.0f , 0.0f , 1.0f , 1.0f ) ;
    iViewports.push_back(defaultviewport);
}

RenderContext::~RenderContext() noexcept ( false )
{
    
}

const RenderContextInfo& RenderContext::getInfo() const
{
    GreAutolock ;
    return iContextInfo;
}

bool RenderContext::isBinded() const
{
    GreAutolock ;
    return iIsBinded;
}

void RenderContext::addViewport(const Gre::Viewport &viewport)
{
    GreAutolock ;
    
    auto it = std::find_if(iViewports.begin(), iViewports.end(),
                           [&](const Viewport& v) -> bool { return viewport.getName() == v.getName(); });
    
    if(it == iViewports.end())
    {
        iViewports.push_back(viewport);
    }
    
    else
    {
#ifdef GreIsDebugMode
        GreDebugPretty() << "Sorry, Viewport '" << viewport.getName() << "' is already present in RenderContext '" << getName() << "'." << Gre::gendl;
#endif
    }
}

void RenderContext::createViewport(const std::string &name, float topratio, float leftratio, float widthratio, float heightratio, bool activated)
{
    GreAutolock ;
    RenderContext::addViewport(Viewport(name, topratio, leftratio, widthratio, heightratio, activated));
}

const ViewportList& RenderContext::getViewports() const
{
    GreAutolock ;
    return iViewports;
}

Viewport RenderContext::getDefaultViewport() const
{
    GreAutolock ;
    return iViewports.front();
}

void RenderContext::removeViewport(const std::string &name)
{
    GreAutolock ;
    
    auto it = std::find_if(iViewports.begin(), iViewports.end(), [&](const Viewport& v) -> bool { return name == v.getName(); });
    
    if(it != iViewports.end())
    {
        iViewports.erase(it);
    }
    
    else
    {
#ifdef GreIsDebugMode
        GreDebugPretty() << "Sorry, Viewport '" << name << "' is not present in RenderContext '" << getName() << "'." << Gre::gendl;
#endif
    }
}

std::string RenderContext::toString() const
{
    GreAutolock ;
    
    std::string ret;
    ret += "RenderContext (name='"; ret += getName();
    ret += "', Viewports="; ret += std::to_string(getViewports().size());
    ret += ", isBinded="; ret += std::to_string(iIsBinded); ret += "')";
    return ret;
}

RenderFramebufferHolder RenderContext::createFramebuffer( const std::string& name )
{
    GreAutolock ;
    
    RenderFramebufferHolder holder = iCreateFramebuffer(name);
    
    if ( holder.isInvalid() )
    {
#ifdef GreIsDebugMode
        GreDebugPretty() << "Can't create RenderFramebuffer '" << name << "'." << Gre::gendl;
#endif
        return RenderFramebufferHolder ( nullptr );
    }
    
    else
    {
#ifdef GreIsDebugMode
        GreDebugPretty() << "RenderFramebuffer '" << name << "'." << Gre::gendl;
#endif
        iFramebuffers.add(holder);
        return holder;
    }
}

RenderFramebufferHolderList RenderContext::getFramebuffers(int sz)
{
    if ( sz > 0 )
    {
        GreAutolock ;
        RenderFramebufferHolderList ret;
        
        for ( auto it = iFramebuffers.begin(); it != iFramebuffers.end(); it++ )
        {
            ret.add((*it));
            sz--;
            
            if ( sz <= 0 )
                break;
        }
        
        while ( sz > 0 )
        {
            RenderFramebufferHolder fbo = createFramebuffer( getName() + "/fbo#" + std::to_string(sz) );
            sz--;
            
            if ( fbo.isInvalid() )
            {
#ifdef GreIsDebugMode
                GreDebugPretty() << "Can't create RenderFramebuffer '" << getName() + "/fbo#" + std::to_string(sz) << "'." << Gre::gendl;
#endif
            }
            
            else
            {
                ret.add(fbo);
            }
        }
        
        return ret;
    }
    
    return RenderFramebufferHolderList () ;
}

void RenderContext::onWindowSizedEvent(const Gre::WindowSizedEvent &e)
{
    iUpdatesViewportsBorders( { 0 , 0 , e.Width , e.Height } );
}

void RenderContext::onWindowAttachContextEvent(const Gre::WindowAttachContextEvent &e)
{
    GreAutolock ;
    
    ResourceUser winuser = e.getEmitter() ;
    ResourceHolder winholder = winuser.lock() ;
    WindowHolder window = WindowHolder ( reinterpret_cast<Window*>(winholder.getObject()) ) ;
    
    if ( !window.isInvalid() )
    {
        iUpdatesViewportsBorders ( window->getSurface() ) ;
    }
}

void RenderContext::iUpdatesViewportsBorders ( const Surface& surface )
{
    GreAutolock ;
    
    for(Viewport& viewport : iViewports)
    {
        viewport.onBordersChanged({viewport.getSurface().left , viewport.getSurface().top ,
            surface.width , surface.height });
    }
}

GreEndNamespace
