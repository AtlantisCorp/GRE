//
//  RenderContext.cpp
//  GRE
//
//  Created by Jacques Tronconi on 15/02/2016.
//
//

#include "RenderContext.h"

GreBeginNamespace

RenderContextPrivate::RenderContextPrivate(const std::string& name, const RenderContextInfo& ctxtInfo)
: Resource(name), _mContextInfo(ctxtInfo)
{
    _mViewports.push_back(Viewport(name+"/DefaultViewport"));
}

RenderContextPrivate::~RenderContextPrivate()
{
    if(isBinded()) {
        unbind();
    }
}

const RenderContextInfo& RenderContextPrivate::getInfo() const
{
    return _mContextInfo;
}

bool RenderContextPrivate::isBinded() const
{
    return _mIsBinded;
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

Renderer& RenderContextPrivate::getRenderer()
{
    return _mParentRenderer;
}

const Renderer& RenderContextPrivate::getRenderer() const
{
    return _mParentRenderer;
}

void RenderContextPrivate::addViewport(const Gre::Viewport &viewport)
{
    auto it = std::find_if(_mViewports.begin(), _mViewports.end(), [&](const Viewport& v) -> bool { return viewport.getName() == v.getName(); });
    
    if(it == _mViewports.end())
    {
        _mViewports.push_back(viewport);
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

const std::vector<Viewport>& RenderContextPrivate::getViewports() const
{
    return _mViewports;
}

void RenderContextPrivate::removeViewport(const std::string &name)
{
    auto it = std::find_if(_mViewports.begin(), _mViewports.end(), [&](const Viewport& v) -> bool { return name == v.getName(); });
    
    if(it != _mViewports.end())
    {
        _mViewports.erase(it);
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
    std::string rendname = getRenderer().expired() ? "Null" : getRenderer().getName();
    std::string ret;
    ret += "RenderContext (name='"; ret += getName();
    ret += "', Viewports="; ret += std::to_string(getViewports().size());
    ret += ", isBinded="; ret += std::to_string(_mIsBinded);
    ret += ", parentRenderer='"; ret += rendname; ret += "')";
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
    for(auto& viewport : _mViewports)
    {
        viewport.onBordersChanged(event.surface);
    }
}

// ---------------------------------------------------------------------------------------------------

RenderContext::RenderContext()
: ResourceUser(), _mCtxt()
{
    
}

RenderContext::RenderContext(const RenderContext& rhs)
: ResourceUser(rhs), _mCtxt(rhs._mCtxt)
{
    
}

RenderContext::RenderContext(RenderContext&& rhs)
: ResourceUser(rhs), _mCtxt(std::move(rhs._mCtxt))
{
    
}

RenderContext::RenderContext(const ResourceUser& rhs)
: ResourceUser(rhs), _mCtxt(std::dynamic_pointer_cast<RenderContextPrivate>(rhs.lock()))
{
    
}

RenderContext& RenderContext::operator=(const RenderContext &rhs)
{
    ResourceUser::operator=(rhs);
    _mCtxt = rhs._mCtxt;
    return *this;
}

bool RenderContext::operator==(const RenderContext &rhs) const
{
    return _mCtxt.lock() == rhs._mCtxt.lock();
}

bool RenderContext::operator!=(const RenderContext &rhs) const
{
    return !(*this == rhs);
}

RenderContext::~RenderContext()
{
    
}

const RenderContextInfo& RenderContext::getInfo() const
{
    auto ptr = _mCtxt.lock();
    if(ptr)
        return ptr->getInfo();
    return RenderContextInfo::Null;
}

bool RenderContext::isBinded() const
{
    auto ptr = _mCtxt.lock();
    if(ptr)
        return ptr->isBinded();
    return false;
}

void RenderContext::bind()
{
    auto ptr = _mCtxt.lock();
    if(ptr)
        ptr->bind();
}

void RenderContext::unbind()
{
    auto ptr = _mCtxt.lock();
    if(ptr)
        ptr->unbind();
}

void RenderContext::flush()
{
    auto ptr = _mCtxt.lock();
    if(ptr)
        ptr->flush();
}

Renderer& RenderContext::getRenderer()
{
    auto ptr = _mCtxt.lock();
    if(ptr)
        return ptr->getRenderer();
    return Renderer::Null;
}

const Renderer& RenderContext::getRenderer() const
{
    auto ptr = _mCtxt.lock();
    if(ptr)
        return ptr->getRenderer();
    return Renderer::Null;
}

void RenderContext::addViewport(const Gre::Viewport &viewport)
{
    auto ptr = _mCtxt.lock();
    if(ptr)
        ptr->addViewport(viewport);
}

void RenderContext::createViewport(const std::string &name, float topratio, float leftratio, float widthratio, float heightratio, bool activated)
{
    auto ptr = _mCtxt.lock();
    if(ptr)
        ptr->createViewport(name, topratio, leftratio, widthratio, heightratio, activated);
}

const std::vector<Viewport>& RenderContext::getViewports() const
{
    auto ptr = _mCtxt.lock();
    if(ptr)
        return ptr->getViewports();
    return Viewport::EmptyVector;
}

void RenderContext::removeViewport(const std::string &name)
{
    auto ptr = _mCtxt.lock();
    if(ptr)
        ptr->removeViewport(name);
}

std::string RenderContext::toString() const
{
    auto ptr = _mCtxt.lock();
    if(ptr)
        return ptr->toString();
    return "";
}

RenderContext RenderContext::Null = RenderContext();

GreEndNamespace
