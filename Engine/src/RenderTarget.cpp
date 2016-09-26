//////////////////////////////////////////////////////////////////////
//
//  RenderTarget.cpp
//  This source file is part of Gre
//		(Gang's Resource Engine)
//
//  Copyright (c) 2015 - 2016 Luk2010
//  Created on 20/02/2016.
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

#include "RenderTarget.h"

GreBeginNamespace

RenderTargetPrivate::RenderTargetPrivate(const std::string& name)
: Resource(name), iSelectedScene(nullptr)
{
    
}

RenderTargetPrivate::~RenderTargetPrivate()
{
    
}

void RenderTargetPrivate::bind()
{
    
}

void RenderTargetPrivate::unbind()
{
    
}

void RenderTargetPrivate::bindFramebuffer()
{
    
}

void RenderTargetPrivate::unbindFramebuffer()
{
    
}

void RenderTargetPrivate::selectScene(const RenderScene& scene)
{
    iSelectedScene = scene;
}

RenderScene RenderTargetPrivate::getSelectedScene()
{
    return iSelectedScene;
}

bool RenderTargetPrivate::holdsRenderContext() const
{
    return false;
}

RenderContextHolder RenderTargetPrivate::getRenderContext()
{
    return RenderContextHolder(nullptr);
}

const RenderContextHolder RenderTargetPrivate::getRenderContext() const
{
    return RenderContextHolder(nullptr);
}

bool RenderTargetPrivate::holdsFramebuffer() const
{
    return false;
}

RenderFramebufferHolder RenderTargetPrivate::getFramebuffer()
{
    return RenderFramebufferHolder(nullptr);
}

const RenderFramebufferHolder RenderTargetPrivate::getFramebuffer() const
{
    return RenderFramebufferHolder(nullptr);
}

bool RenderTargetPrivate::needsDrawing() const
{
    return iNeedsDrawing;
}

void RenderTargetPrivate::forceRedraw()
{
    iNeedsDrawing = true;
}

void RenderTargetPrivate::onRenderFinished() const
{
    iNeedsDrawing = false;
}

bool RenderTargetPrivate::isAvailableForDrawing() const
{
    return true;
}

void RenderTargetPrivate::onUpdateEvent(const Gre::UpdateEvent &e)
{
    Resource::onUpdateEvent(e);
}

void RenderTargetPrivate::onKeyUpEvent(const Gre::KeyUpEvent &e)
{
    Resource::onKeyUpEvent(e);
}

void RenderTargetPrivate::onKeyDownEvent(const Gre::KeyDownEvent &e)
{
    Resource::onKeyDownEvent(e);
}

// ---------------------------------------------------------------------------------------------------

RenderTarget::RenderTarget(const RenderTargetPrivate* pointer)
: ResourceUser(pointer)
, SpecializedResourceUser<Gre::RenderTargetPrivate>(pointer)
{
    
}

RenderTarget::RenderTarget(const RenderTargetHolder& holder)
: ResourceUser(holder)
, SpecializedResourceUser<Gre::RenderTargetPrivate>(holder)
{
    
}

RenderTarget::RenderTarget(const RenderTarget& user)
: ResourceUser(user)
, SpecializedResourceUser<Gre::RenderTargetPrivate>(user)
{
    
}

RenderTarget::~RenderTarget()
{
    
}

void RenderTarget::bind()
{
    auto ptr = lock();
    if(ptr)
        ptr->bind();
}

void RenderTarget::unbind()
{
    auto ptr = lock();
    if(ptr)
        ptr->unbind();
}

void RenderTarget::bindFramebuffer()
{
    auto ptr = lock();
    if(ptr)
        ptr->bindFramebuffer();
}

void RenderTarget::unbindFramebuffer()
{
    auto ptr = lock();
    if(ptr)
        ptr->unbindFramebuffer();
}

void RenderTarget::selectScene(const RenderScene &scene)
{
    auto ptr = lock();
    if(ptr)
        ptr->selectScene(scene);
}

RenderScene RenderTarget::getSelectedScene()
{
    auto ptr = lock();
    if(ptr)
        return ptr->getSelectedScene();
    return RenderScene::Null;
}

bool RenderTarget::holdsRenderContext() const
{
    auto ptr = lock();
    if(ptr)
        return ptr->holdsRenderContext();
    return false;
}

RenderContextHolder RenderTarget::getRenderContext()
{
    auto ptr = lock();
    if( ptr )
        return ptr->getRenderContext();
    return RenderContextHolder(nullptr);
}

const RenderContextHolder RenderTarget::getRenderContext() const
{
    auto ptr = lock();
    if( ptr )
        return ptr->getRenderContext();
    return RenderContextHolder(nullptr);
}

bool RenderTarget::holdsFramebuffer() const
{
    auto ptr = lock();
    if ( ptr )
        return ptr->holdsFramebuffer();
    return false;
}

RenderFramebufferHolder RenderTarget::getFramebuffer()
{
    auto ptr = lock();
    if ( ptr )
        return ptr->getFramebuffer();
    return RenderFramebufferHolder(nullptr);
}

const RenderFramebufferHolder RenderTarget::getFramebuffer() const
{
    auto ptr = lock();
    if ( ptr )
        return ptr->getFramebuffer();
    return RenderFramebufferHolder(nullptr);
}

bool RenderTarget::needsDrawing() const
{
    auto ptr = lock();
    if(ptr)
        return ptr->needsDrawing();
    return false;
}

void RenderTarget::forceRedraw()
{
    auto ptr = lock();
    if(ptr)
        ptr->forceRedraw();
}

void RenderTarget::onRenderFinished() const
{
    auto ptr = lock();
    if(ptr)
        ptr->onRenderFinished();
}

GreEndNamespace
