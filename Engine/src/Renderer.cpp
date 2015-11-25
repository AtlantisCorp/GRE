//
//  Renderer.cpp
//  GResource
//
//  Created by Jacques Tronconi on 08/11/2015.
//  Copyright (c) 2015 Atlanti's Corporation. All rights reserved.
//

#include "Renderer.h"

GRE_BEGIN_NAMESPACE

RendererResource::RendererResource (const std::string& name)
: Resource(name)
{
    _begin_time = 0;
    _end_time   = 0;
    _sum_time   = 0;
    _sum_frames = 0;
    _wantedFps  = 0.0f;
    _currentFps = 0.0f;
    _previous.tv_sec = 0;
    _previous.tv_usec = 0;
    _mIsActive = false;
    _mIsImmediateMode = false;
}

void RendererResource::beginRender()
{
    
}

float timedifference_msec(struct timeval t1, struct timeval t0)
{
    return (t1.tv_sec - t0.tv_sec) * 1000.0f + (t1.tv_usec - t0.tv_usec) / 1000.0f;
}

void RendererResource::endRender()
{
    // Calculate the Framerate
    _sum_frames++;
    struct timeval now;
    gettimeofday(&now, 0);
    
    double diff_time = timedifference_msec(now, _previous);
    //std::cout << diff_time << std::endl;
    if(diff_time >= 1000)
    {
        _currentFps = _sum_frames / (diff_time / 1000);
        _begin_time = _end_time;
        _previous = now;
        _sum_frames = 0;
    }
}

void RendererResource::setActive(bool active)
{
    _mIsActive = active;
}

bool RendererResource::isActive() const
{
    return _mIsActive;
}

void RendererResource::render()
{
    if(isActive())
    {
        _preRender();
        
        if(!isImmediate()) {
            _render();
        } else {
            _renderImmediate();
        }

        _postRender();
    }
}

void RendererResource::_preRender()
{
    
}

void RendererResource::_render()
{
    
}

void RendererResource::_postRender()
{
    
}

bool RendererResource::isImmediate() const
{
    return _mIsImmediateMode;
}

void RendererResource::setImmediateMode(bool mode)
{
    _mIsImmediateMode = mode;
}

void RendererResource::_renderImmediate()
{
    for(auto it = _mImmediateFunctions.begin(); it != _mImmediateFunctions.end(); it++)
    {
        (*it) ();
    }
}

void RendererResource::addImmediateAction(std::function<void ()> action)
{
    _mImmediateFunctions.push_back(action);
}

void RendererResource::resetImmediateActions()
{
    _mImmediateFunctions.clear();
}

Renderer::Renderer(Renderer&& movref)
: ResourceUser(movref), _mRenderer(std::move(movref._mRenderer))
{
    
}

Renderer::Renderer (const Renderer& renderer)
: ResourceUser(renderer.lock()), _mRenderer(renderer._mRenderer)
{
    
}

Renderer::Renderer (const ResourceUser& renderer)
: ResourceUser(renderer.lock()), _mRenderer(std::dynamic_pointer_cast<RendererResource>(renderer.lock()))
{
    
}

Renderer::~Renderer()
{
    
}

Renderer& Renderer::operator=(const ResourceUser &ruser)
{
    ResourceUser::_resource = ruser.lock();
    _mRenderer = std::dynamic_pointer_cast<RendererResource>(ruser.lock());
    return *this;
}

Renderer& Renderer::operator=(const Renderer &ruser)
{
    ResourceUser::_resource = ruser.lock();
    _mRenderer = ruser._mRenderer;
    return *this;
}

void Renderer::render()
{
    auto ptr = _mRenderer.lock();
    if(ptr)
        ptr->render();
}

void Renderer::renderExample()
{
    auto ptr = _mRenderer.lock();
    if(ptr)
        ptr->renderExample();
}

void Renderer::associateWindow(Window &window)
{
    auto ptr = _mRenderer.lock();
    if(ptr)
        ptr->associateWindow(window);
}

void Renderer::setFramerate(float fps)
{
    auto ptr = _mRenderer.lock();
    if(ptr)
        ptr->setFramerate(fps);
}

float Renderer::getCurrentFramerate() const
{
    auto ptr = _mRenderer.lock();
    if(ptr)
        return ptr->getCurrentFramerate();
    
    return 0.0f;
}

void Renderer::beginRender()
{
    auto ptr = _mRenderer.lock();
    if(ptr)
        ptr->beginRender();
}

void Renderer::endRender()
{
    auto ptr = _mRenderer.lock();
    if(ptr)
        ptr->endRender();
}

void Renderer::setClearColor(const Color& color)
{
    auto ptr = _mRenderer.lock();
    if(ptr)
        ptr->setClearColor(color);
}

void Renderer::setClearDepth(float depth)
{
    auto ptr = _mRenderer.lock();
    if(ptr)
        ptr->setClearDepth(depth);
}

void Renderer::setActive(bool active)
{
    auto ptr = _mRenderer.lock();
    if(ptr)
        ptr->setActive(active);
}

bool Renderer::isActive() const
{
    auto ptr = _mRenderer.lock();
    if(ptr)
        return ptr->isActive();
    return false;
}

bool Renderer::isImmediate() const
{
    auto ptr = _mRenderer.lock();
    if(ptr)
        return ptr->isImmediate();
    return false;
}

void Renderer::setImmediateMode(bool mode)
{
    auto ptr = _mRenderer.lock();
    if(ptr)
        ptr->setImmediateMode(mode);
}

void Renderer::addImmediateAction(std::function<void ()> action)
{
    auto ptr = _mRenderer.lock();
    if(ptr)
        ptr->addImmediateAction(action);
}

void Renderer::resetImmediateActions()
{
    auto ptr = _mRenderer.lock();
    if(ptr)
        ptr->resetImmediateActions();
}

RendererLoader::RendererLoader()
{
    
}

RendererLoader::~RendererLoader()
{
    
}

bool RendererLoader::isTypeSupported (Resource::Type type) const
{
    return type == Resource::Type::Renderer;
}

Resource* RendererLoader::load (Resource::Type type, const std::string& name) const
{
    return nullptr;
}

ResourceLoader* RendererLoader::clone() const
{
    return new RendererLoader();
}

GRE_END_NAMESPACE
