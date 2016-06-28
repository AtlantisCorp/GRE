//
//  WindowResource.cpp
//  GRE
//
//  Created by Jacques Tronconi on 21/02/2016.
//
//

#include "WindowResource.h"

GreBeginNamespace

WindowPrivate::WindowPrivate(const std::string& name)
: RenderTargetPrivate(name)
{
    iTitle = name;
    iSurface.top = 0;
    iSurface.left = 0;
    iSurface.width = 0;
    iSurface.height = 0;
    iExposed = false;
    iClosed = true;
    iLastUpdate = iLastUpdate.min();
}

WindowPrivate::~WindowPrivate()
{
    // Note : A subclass should always check that
    // the Window has correctly been closed before
    // destruction.
}

bool WindowPrivate::pollEvent()
{
    return false;
}

bool WindowPrivate::isClosed() const
{
    return iClosed;
}

const std::string WindowPrivate::recommendedRenderer() const
{
    return "none";
}

void WindowPrivate::setTitle(const std::string &title)
{
    iTitle = title;
}

Surface WindowPrivate::getSurface() const
{
    return iSurface;
}

void WindowPrivate::update()
{
    // Here we send an update event to listeners.
    if(iLastUpdate != iLastUpdate.min())
    {
        UpdateEvent ue; UpdateTime now = UpdateChrono::now();
        ue.elapsedTime = std::chrono::duration_cast<std::chrono::nanoseconds>(now - iLastUpdate);
        
        sendEvent(ue);
        
        iLastUpdate = now;
    }
    else
    {
        iLastUpdate = UpdateChrono::now();
    }
    
    // Then, we must call every LoopBehaviour functions.
    iLoopBehaviours.call();
}

bool WindowPrivate::isExposed() const
{
    return iExposed;
}

bool WindowPrivate::hasRenderContext() const
{
    return !iRenderContext.expired();
}

void WindowPrivate::setRenderContext(const RenderContext &renderCtxt)
{
    if(hasRenderContext())
    {
        iRenderContext.unbind();
        removeListener(iRenderContext.getName());
    }
    
    iRenderContext = renderCtxt;
    addListener(iRenderContext);
    
    onRenderContextChanged();
}

RenderContext& WindowPrivate::getRenderContext()
{
    return iRenderContext;
}

const RenderContext& WindowPrivate::getRenderContext() const
{
    return iRenderContext;
}

void WindowPrivate::onRenderContextChanged()
{
    
}

void WindowPrivate::addLoopBehaviour(LoopBehaviour behaviour)
{
    iLoopBehaviours.add(behaviour);
}

void WindowPrivate::clearLoopBehaviour()
{
    iLoopBehaviours.clear();
}

bool WindowPrivate::hasBeenClosed() const
{
    return true;
}

bool WindowPrivate::holdsRenderContext() const
{
    return true;
}

GreEndNamespace
