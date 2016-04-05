//
//  FrameBuffer.cpp
//  GRE
//
//  Created by Jacques Tronconi on 25/01/2016.
//
//

#include "FrameBuffer.h"

GreBeginNamespace

FrameBufferPrivate::FrameBufferPrivate(const std::string& name)
: Resource(name)
{
    _mMaxWidth = 0;
    _mMaxHeight = 0;
    _mMaxSamples = 0;
    _mMaxColorAttachement = 0;
}

FrameBufferPrivate::~FrameBufferPrivate()
{
    
}

void FrameBufferPrivate::bind() const
{
    GreDebugFunctionNotImplemented();
}

void FrameBufferPrivate::unbind() const
{
    GreDebugFunctionNotImplemented();
}

Texture& FrameBufferPrivate::getDefaultTextureAccessible()
{
    auto it = _mHdwFboTextures.find(Attachement::Color);
    if(it != _mHdwFboTextures.end())
        return (*it).second;
    
    return Texture::Null;
}

const Texture& FrameBufferPrivate::getDefaultTextureAccessible() const
{
    auto it = _mHdwFboTextures.find(Attachement::Color);
    if(it != _mHdwFboTextures.end())
        return (*it).second;
    
    return Texture::Null;
}

void FrameBufferPrivate::attachTexture(FrameBufferPrivate::Attachement attachement, const Texture &texture)
{
    GreDebugFunctionNotImplemented();
}

// ---------------------------------------------------------------------------------------------------

FrameBuffer::FrameBuffer()
: ResourceUser(), _mfbo()
{
    
}

FrameBuffer::FrameBuffer(const FrameBuffer& rhs)
: ResourceUser(rhs), _mfbo(rhs._mfbo)
{
    
}

FrameBuffer::FrameBuffer(FrameBuffer&& rhs)
: ResourceUser(rhs), _mfbo(std::move(rhs._mfbo))
{
    
}

FrameBuffer::FrameBuffer(const ResourceUser& rhs)
: ResourceUser(rhs), _mfbo(std::dynamic_pointer_cast<FrameBufferPrivate>(rhs.lock()))
{
    
}

FrameBuffer& FrameBuffer::operator=(const FrameBuffer &rhs)
{
    ResourceUser::operator=(rhs);
    _mfbo = rhs._mfbo;
    return *this;
}

bool FrameBuffer::operator==(const FrameBuffer &rhs) const
{
    return _mfbo.lock() == rhs._mfbo.lock();
}

bool FrameBuffer::operator!=(const FrameBuffer &rhs) const
{
    return !(*this == rhs);
}

FrameBuffer::~FrameBuffer()
{
    
}

void FrameBuffer::bind() const
{
    auto ptr = _mfbo.lock();
    if(ptr)
        ptr->bind();
}

void FrameBuffer::unbind() const
{
    auto ptr = _mfbo.lock();
    if(ptr)
        ptr->unbind();
}

Texture& FrameBuffer::getDefaultTextureAccessible()
{
    auto ptr = _mfbo.lock();
    if(ptr)
        return ptr->getDefaultTextureAccessible();
    return Texture::Null;
}

const Texture& FrameBuffer::getDefaultTextureAccessible() const
{
    auto ptr = _mfbo.lock();
    if(ptr)
        return ptr->getDefaultTextureAccessible();
    return Texture::Null;
}

void FrameBuffer::attachTexture(FrameBuffer::Attachement attachement, const Texture &texture)
{
    auto ptr = _mfbo.lock();
    if(ptr)
        ptr->attachTexture(attachement, texture);
}

FrameBuffer FrameBuffer::Null = FrameBuffer();

// ---------------------------------------------------------------------------------------------------

FrameBufferLoader::FrameBufferLoader()
{
    
}

FrameBufferLoader::~FrameBufferLoader()
{
    
}

bool FrameBufferLoader::isTypeSupported(Resource::Type type) const
{
    return type == Resource::Type::FrameBuff;
}

Resource* FrameBufferLoader::load(Resource::Type type, const std::string &name) const
{
    return nullptr;
}

GreEndNamespace
