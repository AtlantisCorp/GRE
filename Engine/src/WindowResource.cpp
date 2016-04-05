//
//  WindowResource.cpp
//  GRE
//
//  Created by Jacques Tronconi on 21/02/2016.
//
//

#include "WindowResource.h"

GreBeginNamespace

WindowResource::WindowResource(const std::string& name)
: Resource(name)
{
    _mTitle = name;
    _mSurface.top = 0;
    _mSurface.left = 0;
    _mSurface.width = 0;
    _mSurface.height = 0;
    _mExposed = false;
    _mClosed = true;
    _lastUpdate = _lastUpdate.min();
}

WindowResource::~WindowResource()
{
    // Note : A subclass should always check that
    // the Window has correctly been closed before
    // destruction.
}

bool WindowResource::pollEvent()
{
    GreDebugFunctionNotImplemented();
    return false;
}

bool WindowResource::isClosed() const
{
    return _mClosed;
}

const std::string WindowResource::recommendedRenderer() const
{
    GreDebugFunctionNotImplemented();
    return "none";
}

void WindowResource::setTitle(const std::string &title)
{
    GreDebugFunctionNotImplemented();
    _mTitle = title;
}

Surface WindowResource::getSurface() const
{
    return _mSurface;
}

void WindowResource::update()
{
    // Here we send an update event to listeners.
    if(_lastUpdate != _lastUpdate.min())
    {
        UpdateTime now = UpdateChrono::now();
        UpdateEvent ue;
        ue.elapsedTime = std::chrono::duration_cast<std::chrono::nanoseconds>(now - _lastUpdate);
        sendEvent(ue);
        _lastUpdate = now;
    }
    else
    {
        _lastUpdate = UpdateChrono::now();
    }
    
    // Then, we must call every LoopBehaviour functions.
    _mLoopBehaviours.call();
}

bool WindowResource::isExposed() const
{
    return _mExposed;
}

bool WindowResource::hasRenderContext() const
{
    return !_mRenderContext.expired();
}

void WindowResource::setRenderContext(const RenderContext &renderCtxt)
{
    if(hasRenderContext())
    {
        _mRenderContext.unbind();
        removeListener(_mRenderContext.getName());
    }
    
    _mRenderContext = renderCtxt;
    addListener(_mRenderContext);
    
    onRenderContextChanged();
}

RenderContext& WindowResource::getRenderContext()
{
    return _mRenderContext;
}

const RenderContext& WindowResource::getRenderContext() const
{
    return _mRenderContext;
}

void WindowResource::onRenderContextChanged()
{
    GreDebugFunctionNotImplemented();
}

void WindowResource::addLoopBehaviour(LoopBehaviour behaviour)
{
    _mLoopBehaviours.add(behaviour);
}

void WindowResource::clearLoopBehaviour()
{
    _mLoopBehaviours.clear();
}

bool WindowResource::hasBeenClosed() const
{
    GreDebugFunctionNotImplemented();
    return true;
}

bool WindowResource::holdsRenderContext() const
{
    return true;
}

GreEndNamespace
