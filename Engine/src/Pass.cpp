//////////////////////////////////////////////////////////////////////
//
//  Pass.cpp
//  This source file is part of Gre
//		(Gang's Resource Engine)
//
//  Copyright (c) 2015 - 2016 Luk2010
//  Created on 06/01/2016.
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

#include "Pass.h"
#include "Renderer.h"

GreBeginNamespace

PassPrivate::PassPrivate(const std::string& name, const PassNumber& passNumber)
: Resource(name), iNumber(passNumber), iIsActivated(true), iLinkedProgram(HardwareProgram::Null), iAutoCreateProgram(true), iFbo()
{
    if((uint32_t) passNumber > (uint32_t) PassPurpose::First)
        iFrameBufferedRendering = true;
    else
        iFrameBufferedRendering = false;
}

PassPrivate::~PassPrivate()
{
    
}

void PassPrivate::renderWith(RendererResource *renderer)
{
    if(autoCreateProgram() && iLinkedProgram.isExpired())
    {
        // We tell the Renderer to create the passthrough Program,
        // then we compile it, finalize it and set it.
        HardwareProgram ptprogram = renderer->createHardwareProgram("shader-program-passthrough", HardwareShader::VertexPassThrough, HardwareShader::FragmentPassThrough);
        if(!ptprogram.isExpired())
        {
            if(ptprogram.isReady())
            {
                iLinkedProgram = ptprogram;
            }
            else
            {
#ifdef GreIsDebugMode
                GreDebugPretty() << "Can't create Pass-through Program Shader."  << std::endl;
#endif
            }
        }
    }
    
    if(renderer && !getHardwareProgram().isExpired() && isActivated())
    {
        // We tell the Renderer to prepare the Pass.
        renderer->prepare(this);
        
        // We set the Shader on !
        iLinkedProgram.bind();
        
        // Currently, the function RendererResource::drawPass() has the
        // only purpose to 'draw' the objects using the Pass HardwareProgram.
        // So we set the current active camera by default, but a subclass can
        // configure this as wanted.
        renderer->prepare(renderer->getScene().getCurrentCamera());
        
        // We also get every Node objects for farthest to nearest from the Scene
        // to give some Mesh to draw for the Renderer.
        std::vector<const SceneNode> nodes = renderer->getScene().getNodesForPass((PassPurpose)iNumber, NodePrivate::Filter::FarthestToNearest);
        
        // This function permit to the Renderer to directly draw the Pass,
        // without having to make assertion about the Pass validity.
        // Note those assertions should be made in Debug mode.
        for(auto node : nodes)
        {
            renderer->draw(node.getRenderable(), node.getTransformation().get(), iLinkedProgram);
        }
        
        // We unbind the Shader program, finalizing the Pass.
        iLinkedProgram.unbind();
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
    iIsActivated = activate;
}

bool PassPrivate::isActivated() const
{
    return iIsActivated;
}

void PassPrivate::setHardwareProgram(const HardwareProgram& hwdProgram)
{
    iLinkedProgram = hwdProgram;
}

HardwareProgram PassPrivate::getHardwareProgram() const
{
    return iLinkedProgram;
}

PassNumber PassPrivate::getPassNumber() const
{
    return iNumber;
}

void PassPrivate::setAutoCreateProgram(bool enabled)
{
    iAutoCreateProgram = enabled;
}

bool PassPrivate::autoCreateProgram() const
{
    return iAutoCreateProgram;
}

FrameBuffer& PassPrivate::getFrameBuffer()
{
    return iFbo;
}

const FrameBuffer& PassPrivate::getFrameBuffer() const
{
    return iFbo;
}

void PassPrivate::setFrameBufferedRendering(bool fborender)
{
    iFrameBufferedRendering = fborender;
}

bool PassPrivate::isFrameBufferedRendering() const
{
    return iFrameBufferedRendering;
}

// ---------------------------------------------------------------------------------------------------

Pass::Pass(PassPrivate* pointer)
: SpecializedResourceUser<Gre::PassPrivate>(pointer)
{
    
}

Pass::Pass(const PassHolder& holder)
: SpecializedResourceUser<Gre::PassPrivate>(holder)
{
    
}

Pass::Pass(const Pass& user)
: SpecializedResourceUser<Gre::PassPrivate>(user)
{
    
}

Pass::~Pass()
{
    
}

void Pass::renderWith(RendererResource *renderer) const
{
    auto ptr = lock();
    
    if(ptr)
        ptr->renderWith(renderer);
}

void Pass::setActivated(bool activate)
{
    auto ptr = lock();
    
    if(ptr)
        ptr->setActivated(activate);
}

bool Pass::isActivated() const
{
    auto ptr = lock();
    
    if(ptr)
        return ptr->isActivated();
    return false;
}

void Pass::setHardwareProgram(const HardwareProgram& hwdProgram)
{
    auto ptr = lock();
    
    if(ptr)
        ptr->setHardwareProgram(hwdProgram);
}

HardwareProgram Pass::getHardwareProgram() const
{
    auto ptr = lock();
    
    if(ptr)
        return ptr->getHardwareProgram();
    return HardwareProgram::Null;
}

PassNumber Pass::getPassNumber() const
{
    auto ptr = lock();
    
    if(ptr)
        return ptr->getPassNumber();
    return (PassNumber) 0;
}

void Pass::setAutoCreateProgram(bool enabled)
{
    auto ptr = lock();
    
    if(ptr)
        ptr->setAutoCreateProgram(enabled);
}

bool Pass::autoCreateProgram() const
{
    auto ptr = lock();
    if(ptr)
        return ptr->autoCreateProgram();
    return false;
}

FrameBuffer& Pass::getFrameBuffer()
{
    auto ptr = lock();
    if(ptr)
        return ptr->getFrameBuffer();
    return FrameBuffer::Null;
}

const FrameBuffer& Pass::getFrameBuffer() const
{
    auto ptr = lock();
    if(ptr)
        return ptr->getFrameBuffer();
    return FrameBuffer::Null;
}

void Pass::setFrameBufferedRendering(bool fborender)
{
    auto ptr = lock();
    if(ptr)
        return ptr->setFrameBufferedRendering(fborender);
}

bool Pass::isFrameBufferedRendering() const
{
    auto ptr = lock();
    if(ptr)
        return ptr->isFrameBufferedRendering();
    return false;
}

Pass Pass::Null = Pass(nullptr);

GreEndNamespace
