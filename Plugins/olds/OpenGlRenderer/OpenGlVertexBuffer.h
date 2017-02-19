//
//  OpenGlVertexBuffer.h
//  GRE
//
//  Created by Jacques Tronconi on 11/12/2015.
//
//

#ifndef GRE_OpenGlVertexBuffer_h
#define GRE_OpenGlVertexBuffer_h

#include "OpenGlIncludes.h"

class OpenGlVertexBuffer : public HardwareVertexBufferPrivate
{
public:
    
    POOLED(Pools::HwdBuffer)
    
    OpenGlVertexBuffer();
    ~OpenGlVertexBuffer();
    
    void bind() const;
    void unbind() const;
    bool isInvalid() const;
    void update() const;
    
private:
    
    GLuint _mVertexBufferId;
    
};

#endif
