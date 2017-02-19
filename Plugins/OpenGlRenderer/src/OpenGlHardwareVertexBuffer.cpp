//////////////////////////////////////////////////////////////////////
//
//  OpenGlHardwareVertexBuffer.cpp
//  This source file is part of Gre
//		(Gang's Resource Engine)
//
//  Copyright (c) 2015 - 2017 Luk2010
//  Created on 11/02/2017.
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

#include "OpenGlRenderer.h"

OpenGlHardwareVertexBuffer::OpenGlHardwareVertexBuffer ( const void* data , size_t sz , const Gre::VertexDescriptor& desc )
: Gre::HardwareVertexBuffer("")
{
    glGenBuffers(1, &iGlBuffer);
    if ( iGlBuffer == 0 ) {
        GreDebug("[WARN] Can't create OpenGlHardwareVertexBuffer:iGlBuffer.") << Gre::gendl ;
    }
    
    else
    {
        glBindBuffer(GL_ARRAY_BUFFER, iGlBuffer);
        glBufferData(GL_ARRAY_BUFFER, sz, data, GL_STATIC_DRAW);
        iSize = sz ; setVertexDescriptor(desc);
    }
}

OpenGlHardwareVertexBuffer::~OpenGlHardwareVertexBuffer() noexcept ( false )
{
    if ( iGlBuffer != 0 )
    {
        glDeleteBuffers(1, &iGlBuffer);
    }
}

void OpenGlHardwareVertexBuffer::bind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, iGlBuffer);
}

void OpenGlHardwareVertexBuffer::unbind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void OpenGlHardwareVertexBuffer::addData(const char *vdata, size_t sz)
{
    GreAutolock ;
    
    if ( iGlBuffer )
    {
        GLuint oldbuffer = iGlBuffer ;
        glGenBuffers(1, &iGlBuffer);
        
        if ( iGlBuffer )
        {
            GLint size = 0 ;
            
            glBindBuffer(GL_COPY_READ_BUFFER, oldbuffer);
            glGetBufferParameteriv(GL_COPY_READ_BUFFER, GL_BUFFER_SIZE, &size);
            
            glBindBuffer(GL_COPY_WRITE_BUFFER, iGlBuffer);
            glBufferData(GL_COPY_WRITE_BUFFER, size + sz, nullptr, GL_STATIC_DRAW);
            
            glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, size);
            glBufferSubData(GL_COPY_WRITE_BUFFER, size, sz, vdata);
            
            glBindBuffer(GL_COPY_READ_BUFFER, 0);
            glBindBuffer(GL_COPY_WRITE_BUFFER, 0);
            iSize = size + sz ;
        }
        
        glDeleteBuffers(1, &oldbuffer);
    }
    
    else
    {
        glGenBuffers(1, &iGlBuffer);
        if ( iGlBuffer )
        {
            glBindBuffer(GL_ARRAY_BUFFER, iGlBuffer);
            glBufferData(GL_ARRAY_BUFFER, sz, vdata, GL_STATIC_DRAW);
        }
    }
}

const char* OpenGlHardwareVertexBuffer::getData() const
{
    return 0 ;
}

void OpenGlHardwareVertexBuffer::clearData()
{
    GreAutolock ;
    
    if ( iGlBuffer )
    {
        glDeleteBuffers(1, &iGlBuffer);
        iGlBuffer = 0 ;
        iSize = 0 ;
    }
}

size_t OpenGlHardwareVertexBuffer::getSize() const
{
    return iSize ;
}

size_t OpenGlHardwareVertexBuffer::count() const
{
    return iSize / getVertexDescriptor().getSize() ;
}
