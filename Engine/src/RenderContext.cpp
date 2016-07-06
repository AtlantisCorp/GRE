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
#include "WindowEvent.h"

GreBeginNamespace

RenderContextPrivate::RenderContextPrivate(const std::string& name, const RenderContextInfo& ctxtInfo)
: Resource(name), iContextInfo(ctxtInfo)
{
    iViewports.push_back(Viewport(name+"/DefaultViewport"));
}

RenderContextPrivate::~RenderContextPrivate()
{
    if(isBinded()) {
        unbind();
    }
}

const RenderContextInfo& RenderContextPrivate::getInfo() const
{
    return iContextInfo;
}

bool RenderContextPrivate::isBinded() const
{
    return iIsBinded;
}

void RenderContextPrivate::bind()
{
    GreDebugFunctionNotImplemented();
}

void RenderContextPrivate::unbind()
{
    GreDebugFunctionNotImplemented();
}

void RenderContextPrivate::flush()
{
    GreDebugFunctionNotImplemented();
}

void RenderContextPrivate::addViewport(const Gre::Viewport &viewport)
{
    auto it = std::find_if(iViewports.begin(), iViewports.end(), [&](const Viewport& v) -> bool { return viewport.getName() == v.getName(); });
    
    if(it == iViewports.end())
    {
        iViewports.push_back(viewport);
    }
    
    else
    {
#ifdef GreIsDebugMode
        GreDebugPretty() << "Sorry, Viewport '" << viewport.getName() << "' is already present in RenderContext '" << getName() << "'." << std::endl;
#endif
    }
}

void RenderContextPrivate::createViewport(const std::string &name, float topratio, float leftratio, float widthratio, float heightratio, bool activated)
{
    RenderContextPrivate::addViewport(Viewport(name, topratio, leftratio, widthratio, heightratio, activated));
}

const ViewportList& RenderContextPrivate::getViewports() const
{
    return iViewports;
}

Viewport RenderContextPrivate::getDefaultViewport() const
{
    return iViewports.front();
}

void RenderContextPrivate::removeViewport(const std::string &name)
{
    auto it = std::find_if(iViewports.begin(), iViewports.end(), [&](const Viewport& v) -> bool { return name == v.getName(); });
    
    if(it != iViewports.end())
    {
        iViewports.erase(it);
    }
    
    else
    {
#ifdef GreIsDebugMode
        GreDebugPretty() << "Sorry, Viewport '" << name << "' is not present in RenderContext '" << getName() << "'." << std::endl;
#endif
    }
}

std::string RenderContextPrivate::toString() const
{
    std::string ret;
    ret += "RenderContext (name='"; ret += getName();
    ret += "', Viewports="; ret += std::to_string(getViewports().size());
    ret += ", isBinded="; ret += std::to_string(iIsBinded); ret += "')";
    return ret;
}

void RenderContextPrivate::onEvent(const Event &e)
{
    if(e.getType() == EventType::WindowSized)
    {
        WindowSizedEvent wse = e.to<WindowSizedEvent>();
        _onWindowSizedEvent(wse);
    }
    
    Resource::onEvent(e);
}

void RenderContextPrivate::_onWindowSizedEvent(WindowSizedEvent &event)
{
    for(auto& viewport : iViewports)
    {
        viewport.onBordersChanged(event.getSurface());
    }
}

// ---------------------------------------------------------------------------------------------------

RenderContext::RenderContext(const RenderContextPrivate* pointer)
: SpecializedResourceUser<RenderContextPrivate>(pointer)
{
    
}

RenderContext::RenderContext(const RenderContextHolder& holder)
: SpecializedResourceUser<RenderContextPrivate>(holder)
{
    
}

RenderContext::RenderContext(const RenderContext& user)
: SpecializedResourceUser<RenderContextPrivate>(user)
{
    
}

RenderContext::~RenderContext()
{
    
}

const RenderContextInfo& RenderContext::getInfo() const
{
    auto ptr = lock();
    if(ptr)
        return ptr->getInfo();
    return RenderContextInfo::Null;
}

bool RenderContext::isBinded() const
{
    auto ptr = lock();
    if(ptr)
        return ptr->isBinded();
    return false;
}

void RenderContext::bind()
{
    auto ptr = lock();
    if(ptr)
        ptr->bind();
}

void RenderContext::unbind()
{
    auto ptr = lock();
    if(ptr)
        ptr->unbind();
}

void RenderContext::flush()
{
    auto ptr = lock();
    if(ptr)
        ptr->flush();
}

void RenderContext::addViewport(const Gre::Viewport &viewport)
{
    auto ptr = lock();
    if(ptr)
        ptr->addViewport(viewport);
}

void RenderContext::createViewport(const std::string &name, float topratio, float leftratio, float widthratio, float heightratio, bool activated)
{
    auto ptr = lock();
    if(ptr)
        ptr->createViewport(name, topratio, leftratio, widthratio, heightratio, activated);
}

const ViewportList& RenderContext::getViewports() const
{
    auto ptr = lock();
    if(ptr)
        return ptr->getViewports();
    return Viewport::EmptyList;
}

Viewport RenderContext::getDefaultViewport() const
{
    auto ptr = lock();
    if ( ptr )
        return ptr->getDefaultViewport();
    return Viewport();
}

void RenderContext::removeViewport(const std::string &name)
{
    auto ptr = lock();
    if(ptr)
        ptr->removeViewport(name);
}

std::string RenderContext::toString() const
{
    auto ptr = lock();
    if(ptr)
        return ptr->toString();
    return "";
}

RenderContext RenderContext::Null = RenderContext(nullptr);

GreEndNamespace
