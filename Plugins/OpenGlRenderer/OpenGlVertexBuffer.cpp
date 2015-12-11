//
//  OpenGlVertexBuffer.cpp
//  GRE
//
//  Created by Jacques Tronconi on 11/12/2015.
//
//

#include "OpenGlVertexBuffer.h"

OpenGlVertexBuffer::OpenGlVertexBuffer()
: _mVertexBufferId(0)
{
    glGenBuffers(1, &_mVertexBufferId);
}

OpenGlVertexBuffer::~OpenGlVertexBuffer()
{
    if(_mVertexBufferId)
    {
        glDeleteBuffers(1, &_mVertexBufferId);
    }
}

void OpenGlVertexBuffer::bind() const
{
    if(_mVertexBufferId)
        glBindBuffer(GL_ARRAY_BUFFER, _mVertexBufferId);
}

void OpenGlVertexBuffer::unbind() const
{
    if(_mVertexBufferId)
        glBindBuffer(GL_ARRAY_BUFFER, 0);
}

bool OpenGlVertexBuffer::isInvalid() const
{
    return _mVertexBufferId == 0;
}

void OpenGlVertexBuffer::update() const
{
    if(_mVertexBufferId)
    {
        glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * _mVertexs.size(), _mVertexs.data(), GL_STREAM_DRAW);
        setDirty(false);
    }
}
