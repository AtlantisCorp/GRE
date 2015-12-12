//
//  Window.h
//  GResource
//
//  Created by Jacques Tronconi on 07/11/2015.
//  Copyright (c) 2015 Atlanti's Corporation. All rights reserved.
//

#ifndef __GResource__Window__
#define __GResource__Window__

#include "Resource.h"
#include "Listener.h"

GRE_BEGIN_NAMESPACE

class Renderer;
class WindowResource;

typedef std::pair<int, int> WindowSize;

class DLL_PUBLIC Window : public ResourceUser
{
public:
    
    Window ();
    Window (Window&& rmove);
    Window (const Window& window);
    explicit Window (const ResourceUser& ruser);
    
    virtual ~Window();
    
    Window& operator = (const ResourceUser& ruser);
    Window& operator = (const Window& wuser);
    
    bool pollEvent();
    bool isClosed() const;
    const std::string recommendedRenderer() const;
    
    void associate(Renderer& renderer);
    
    Renderer getAssociatedRenderer();
    
    void setTitle(const std::string& title);
    void swapBuffers();
    
    WindowSize getWindowSize() const;
    
    void setVerticalSync (bool vsync);
    bool hasVerticalSync () const;
    
    Listener& addListener(const std::string& name);
    Listener getListener(const std::string& name);
    void removeListener(const std::string& name);
    
    operator Emitter& ();
    operator const Emitter& () const;
    
    void beginUpdate();
    void endUpdate();
    
    bool isExposed() const;
    
private:
    
    std::weak_ptr<WindowResource> _mWindow;
};

class DLL_PUBLIC WindowLoader : public ResourceLoader
{
public:
    
    POOLED(Pools::Loader)
    
    WindowLoader();
    virtual ~WindowLoader();
    
    virtual bool isTypeSupported (Resource::Type type) const;
    virtual Resource* load (Resource::Type type, const std::string& name, int x0, int y0, int wid, int height) const;
    virtual ResourceLoader* clone() const;
    
protected:
    
    
};

typedef ResourceLoaderFactory<WindowLoader> WindowLoaderFactory;

GRE_END_NAMESPACE
#endif /* defined(__GResource__Window__) */
