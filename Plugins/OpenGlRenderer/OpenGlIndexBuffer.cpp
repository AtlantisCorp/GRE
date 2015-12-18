//
//  OpenGlIndexBuffer.cpp
//  GRE
//
//  Created by Jacques Tronconi on 11/12/2015.
//
//

#include "OpenGlIndexBuffer.h"

OpenGlIndexBuffer::OpenGlIndexBuffer()
: _mIndexBufferId(0)
{
    glGenBuffers(1, &_mIndexBufferId);
}

OpenGlIndexBuffer::OpenGlIndexBuffer(PrimitiveType ptype, StorageType stype)
: HardwareIndexBufferPrivate(ptype, stype), _mIndexBufferId(0)
{
    glGenBuffers(1, &_mIndexBufferId);
}

OpenGlIndexBuffer::~OpenGlIndexBuffer()
{
    if(_mIndexBufferId)
    {
        glDeleteBuffers(1, &_mIndexBufferId);
    }
}

void OpenGlIndexBuffer::bind() const
{
    if(_mIndexBufferId)
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _mIndexBufferId);
}

void OpenGlIndexBuffer::unbind() const
{
    if(_mIndexBufferId)
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

bool OpenGlIndexBuffer::isInvalid() const
{
    return _mIndexBufferId == 0;
}

void OpenGlIndexBuffer::update() const
{
    if(_mIndexBufferId)
    {
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, getSize(), nullptr, GL_STREAM_DRAW);
        char* ptr = 0;
        for(auto indexedFace : _mFaces.indexedFaces)
        {
            glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, (GLintptr) ptr, sizeof(GLint) * indexedFace.indices.size(), (void*) indexedFace.indices.data());
            ptr += sizeof(GLint) * indexedFace.indices.size();
        }
        setDirty(false);
    }
}