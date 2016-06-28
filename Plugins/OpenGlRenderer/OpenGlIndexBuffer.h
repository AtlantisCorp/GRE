//
//  OpenGlIndexBuffer.h
//  GRE
//
//  Created by Jacques Tronconi on 11/12/2015.
//
//

#ifndef GRE_OpenGlIndexBuffer_h
#define GRE_OpenGlIndexBuffer_h

#include "OpenGlIncludes.h"

class OpenGlIndexBuffer : public HardwareIndexBufferPrivate
{
public:
    
    POOLED(Pools::HwdBuffer)
    
    OpenGlIndexBuffer();
    OpenGlIndexBuffer(PrimitiveType ptype, StorageType stype);
    ~OpenGlIndexBuffer();
    
    void bind() const;
    void unbind() const;
    bool isInvalid() const;
    void update() const;
    
private:
    
    GLuint _mIndexBufferId;
};

#endif
