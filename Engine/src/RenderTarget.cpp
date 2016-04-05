//
//  RenderTarget.cpp
//  GRE
//
//  Created by Jacques Tronconi on 20/02/2016.
//
//

#include "RenderTarget.h"

GreBeginNamespace

RenderTargetPrivate::RenderTargetPrivate()
{
    
}

RenderTargetPrivate::~RenderTargetPrivate()
{
    
}

void RenderTargetPrivate::selectScene(const Scene& scene)
{
    _mSelectedScene = scene;
}

Scene RenderTargetPrivate::getSelectedScene()
{
    return _mSelectedScene;
}

bool RenderTargetPrivate::needsDrawing() const
{
    return _mNeedsDrawing || _mSelectedScene.isDirty();
}

void RenderTargetPrivate::forceRedraw()
{
    _mNeedsDrawing = true;
}

void RenderTargetPrivate::onRenderFinished() const
{
    _mNeedsDrawing = false;
}

// ---------------------------------------------------------------------------------------------------

RenderTarget::RenderTarget()
: _mRenderTarget()
{
    
}

RenderTarget::~RenderTarget()
{
    
}

void RenderTarget::bind()
{
    auto ptr = _mRenderTarget.lock();
    if(ptr)
        ptr->bind();
}

void RenderTarget::unbind()
{
    auto ptr = _mRenderTarget.lock();
    if(ptr)
        ptr->unbind();
}

void RenderTarget::bindFramebuffer()
{
    auto ptr = _mRenderTarget.lock();
    if(ptr)
        ptr->bindFramebuffer();
}

void RenderTarget::unbindFramebuffer()
{
    auto ptr = _mRenderTarget.lock();
    if(ptr)
        ptr->unbindFramebuffer();
}

void RenderTarget::selectScene(const Scene &scene)
{
    auto ptr = _mRenderTarget.lock();
    if(ptr)
        ptr->selectScene(scene);
}

Scene RenderTarget::getSelectedScene()
{
    auto ptr = _mRenderTarget.lock();
    if(ptr)
        return ptr->getSelectedScene();
    return Scene::Null;
}

bool RenderTarget::holdsRenderContext() const
{
    auto ptr = _mRenderTarget.lock();
    if(ptr)
        return ptr->holdsRenderContext();
    return false;
}

bool RenderTarget::needsDrawing() const
{
    auto ptr = _mRenderTarget.lock();
    if(ptr)
        return ptr->needsDrawing();
    return false;
}

void RenderTarget::forceRedraw()
{
    auto ptr = _mRenderTarget.lock();
    if(ptr)
        ptr->forceRedraw();
}

void RenderTarget::onRenderFinished() const
{
    auto ptr = _mRenderTarget.lock();
    if(ptr)
        ptr->onRenderFinished();
}

GreEndNamespace
