//////////////////////////////////////////////////////////////////////
//
//  OpenGlMeshManager.cpp
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

// -----------------------------------------------------------------------------

class OpenGlMeshBinder : public Gre::MeshBinder
{
public:
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    OpenGlMeshBinder ( const OpenGlRenderer* renderer )
    : iGlRenderer ( renderer )
    {
        if ( iGlRenderer )
        iGlRenderer -> getRenderContext () -> bind () ;
        
        glGenVertexArrays(1, &iGlVao);
    }
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    ~OpenGlMeshBinder ()
    {
        if ( iGlRenderer )
        iGlRenderer -> getRenderContext () -> bind () ;
        
        glDeleteVertexArrays(1, &iGlVao);
    }
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    void bind ( const Gre::Mesh * mesh , const Gre::TechniqueHolder & technique ) const
    {
        if ( !mesh || technique.isInvalid() || !iGlVao )
        return ;
        
        glBindVertexArray(iGlVao);
    }
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    void unbind ( const Gre::Mesh * mesh , const Gre::TechniqueHolder & technique ) const
    {
        if ( !mesh || technique.isInvalid() || !iGlVao )
        return ;
        
        glBindVertexArray(0);
    }
    
protected:
    
    /// @brief Vertex Array Object id.
    GLuint iGlVao ;
    
    /// @brief Pointer to associated gl renderer.
    const OpenGlRenderer * iGlRenderer ;
};

// -----------------------------------------------------------------------------

OpenGlMeshManager::OpenGlMeshManager ( const OpenGlRenderer* renderer )
: Gre::MeshManager() , iRenderer(renderer)
{
    
}

OpenGlMeshManager::~OpenGlMeshManager() noexcept ( false )
{
    
}

Gre::HardwareVertexBufferHolder OpenGlMeshManager::createVertexBuffer(const void *data, size_t sz, const Gre::VertexDescriptor &desc) const
{
    GreAutolock ;
    
    if ( iRenderer && !iRenderer->getRenderContext().isInvalid() )
    {
        iRenderer->getRenderContext()->bind() ;
        return Gre::HardwareVertexBufferHolder ( new OpenGlHardwareVertexBuffer ( data , sz , desc ) ) ;
    }
    
    return Gre::HardwareVertexBufferHolder ( nullptr ) ;
}

Gre::HardwareIndexBufferHolder OpenGlMeshManager::createIndexBuffer(const void *data, size_t sz, const Gre::IndexDescriptor &desc) const
{
    GreAutolock ;
    
    if ( iRenderer && !iRenderer->getRenderContext().isInvalid() )
    {
        iRenderer->getRenderContext()->bind() ;
        return Gre::HardwareIndexBufferHolder ( new OpenGlHardwareIndexBuffer ( data , sz , desc ) ) ;
    }
    
    return Gre::HardwareIndexBufferHolder ( nullptr ) ;
}

Gre::MeshBinder* OpenGlMeshManager::loadBinder() const
{
    return new OpenGlMeshBinder ( iRenderer ) ;
}
