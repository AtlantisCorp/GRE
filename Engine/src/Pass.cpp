//
//  Pass.cpp
//  GRE
//
//  Created by Jacques Tronconi on 06/01/2016.
//
//

#include "Pass.h"
#include "Renderer.h"

GRE_BEGIN_NAMESPACE

PassPrivate::PassPrivate(const std::string& name, const PassNumber& passNumber)
: Resource(name), _mNumber(passNumber), _mIsActivated(true), _mLinkedProgram(HardwareProgram::Null), _mAutoCreateProgram(true)
{
    
}

PassPrivate::~PassPrivate()
{
    
}

void PassPrivate::renderWith(RendererResource *renderer)
{
    if(autoCreateProgram() && _mLinkedProgram.expired())
    {
        // We tell the Renderer to create the passthrough Program,
        // then we compile it, finalize it and set it.
        HardwareProgram ptprogram = renderer->createHardwareProgram(getName() + "-program-passthrough", HardwareShader::VertexPassThrough, HardwareShader::FragmentPassThrough);
        if(!ptprogram.expired())
        {
            if(ptprogram.isReady())
            {
                _mLinkedProgram = ptprogram;
            }
            else
            {
#ifdef GreIsDebugMode
                GreDebugPretty() << "Can't create Pass-through Program Shader."  << std::endl;
#endif
            }
        }
    }
    
    if(renderer && !getHardwareProgram().expired() && isActivated())
    {
        // First of all, we set the Shader on !
        _mLinkedProgram.bind();
        
        // Currently, the function RendererResource::drawPass() has the
        // only purpose to 'draw' the objects using the Pass HardwareProgram.
        // So we set the current active camera by default, but a subclass can
        // configure this as wanted.
        renderer->prepare(renderer->getScene().getCamera());
        
        // We also get every Node objects for farthest to nearest from the Scene
        // to give some Mesh to draw for the Renderer.
        std::vector<const Node> nodes = renderer->getScene().getNodesByFilter(Node::Filter::FarthestToNearest);
        
        // This function permit to the Renderer to directly draw the Pass,
        // without having to make assertion about the Pass validity.
        // Note those assertions should be made in Debug mode.
        for(auto node : nodes)
        {
            renderer->draw(node.getMesh(), node.getNodeMatrix(), _mLinkedProgram);
        }
        
        // We unbind the Shader program, finalizing the Pass.
        _mLinkedProgram.unbind();
    }
    
#ifdef GreIsDebugMode
    else if(!renderer)
    {
        GreDebugPretty() << "Pass arg 'renderer' was null." << std::endl;
    }
#endif
}

void PassPrivate::setActivated(bool activate)
{
    _mIsActivated = activate;
}

bool PassPrivate::isActivated() const
{
    return _mIsActivated;
}

void PassPrivate::setHardwareProgram(const HardwareProgram& hwdProgram)
{
    _mLinkedProgram = hwdProgram;
}

HardwareProgram PassPrivate::getHardwareProgram() const
{
    return _mLinkedProgram;
}

PassNumber PassPrivate::getPassNumber() const
{
    return _mNumber;
}

void PassPrivate::setAutoCreateProgram(bool enabled)
{
    _mAutoCreateProgram = enabled;
}

bool PassPrivate::autoCreateProgram() const
{
    return _mAutoCreateProgram;
}

// ---------------------------------------------------------------------------------------------------

Pass::Pass()
: ResourceUser(), _mPass()
{
    
}

Pass::Pass(const Pass& rhs)
: ResourceUser(rhs), _mPass(rhs._mPass)
{
    
}

Pass::Pass(Pass&& rhs)
: ResourceUser(rhs), _mPass(std::move(rhs._mPass))
{
    
}

Pass::Pass(const ResourceUser& rhs)
: ResourceUser(rhs), _mPass(std::dynamic_pointer_cast<PassPrivate>(rhs.lock()))
{
    
}

Pass::~Pass()
{
    
}

void Pass::renderWith(RendererResource *renderer) const
{
    auto ptr = _mPass.lock();
    
    if(ptr)
        ptr->renderWith(renderer);
}

void Pass::setActivated(bool activate)
{
    auto ptr = _mPass.lock();
    
    if(ptr)
        ptr->setActivated(activate);
}

bool Pass::isActivated() const
{
    auto ptr = _mPass.lock();
    
    if(ptr)
        return ptr->isActivated();
    return false;
}

void Pass::setHardwareProgram(const HardwareProgram& hwdProgram)
{
    auto ptr = _mPass.lock();
    
    if(ptr)
        ptr->setHardwareProgram(hwdProgram);
}

HardwareProgram Pass::getHardwareProgram() const
{
    auto ptr = _mPass.lock();
    
    if(ptr)
        return ptr->getHardwareProgram();
    return HardwareProgram::Null;
}

PassNumber Pass::getPassNumber() const
{
    auto ptr = _mPass.lock();
    
    if(ptr)
        return ptr->getPassNumber();
    return (PassNumber) 0;
}

void Pass::setAutoCreateProgram(bool enabled)
{
    auto ptr = _mPass.lock();
    
    if(ptr)
        ptr->setAutoCreateProgram(enabled);
}

bool Pass::autoCreateProgram() const
{
    auto ptr = _mPass.lock();
    if(ptr)
        return ptr->autoCreateProgram();
    return false;
}

Pass Pass::Null = Pass();

GRE_END_NAMESPACE
