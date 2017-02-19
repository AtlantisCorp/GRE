//////////////////////////////////////////////////////////////////////
//
//  OpenGlRenderer.cpp
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
#include <ResourceManager.h>

// ---------------------------------------------------------------------------------------------------------
// GRE to OpenGl translations.

GLenum translateGlMode ( const Gre::IndexDrawmode& mode )
{
    if ( mode == Gre::IndexDrawmode::Points )
        return GL_POINTS ;
    if ( mode == Gre::IndexDrawmode::Lines )
        return GL_LINES ;
    if ( mode == Gre::IndexDrawmode::Patches )
        return GL_PATCHES ;
    return GL_TRIANGLES ;
}

GLenum translateGlType ( const Gre::IndexType& type )
{
    if ( type == Gre::IndexType::UnsignedByte )
        return GL_UNSIGNED_BYTE ;
    if ( type == Gre::IndexType::UnsignedShort )
        return GL_UNSIGNED_SHORT ;
    return GL_UNSIGNED_INT ;
}

// ---------------------------------------------------------------------------------------------------------

OpenGlRenderer::OpenGlRenderer ( const std::string& name , const Gre::RendererOptions& options )
: Gre::Renderer(name, options)
{
    
}

OpenGlRenderer::~OpenGlRenderer() noexcept ( false )
{
    
}

void OpenGlRenderer::_setClearColor(const Gre::Color &color) const
{
    glClearColor(color.getRed(), color.getGreen(), color.getBlue(), color.getAlpha());
}

void OpenGlRenderer::_preRender() const
{
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
}

void OpenGlRenderer::_postRender() const
{
    
}

void OpenGlRenderer::_setViewport(const Gre::Viewport &viewport) const
{
    Gre::Surface view = viewport.getSurface();
    glViewport(view.left, view.top, view.width, view.height);
}

void OpenGlRenderer::_setCamera(const Gre::Matrix4 &projection, const Gre::Matrix4 &view) const
{
    
}

void OpenGlRenderer::_setNode(const Gre::RenderNodeHolder &node) const
{
    
}

void OpenGlRenderer::_drawNodeMesh(const Gre::MeshUser &mesh, const Gre::HardwareProgramHolder &program) const
{
    if ( mesh.isInvalid() || program.isInvalid() )
        return ;
    
    Gre::MeshHolder holder = mesh.lock() ;
    
    GLuint vao ;
    
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    
    // Gets every VertexComponents locations. Those locations can be changed by the user,
    // by using 'HardwareProgram::setAttribLocation()'.
    
    GLuint locpos = program->getAttribLocation(Gre::VertexComponentType::Position) ;
    GLuint loccol = program->getAttribLocation(Gre::VertexComponentType::Color) ;
    GLuint locnor = program->getAttribLocation(Gre::VertexComponentType::Normal) ;
    
    // Enables every VertexComponents.
    
    glEnableVertexAttribArray(locpos);
    glEnableVertexAttribArray(loccol);
    glEnableVertexAttribArray(locnor);
    
    // Loop through every HardwareVertexBuffer to set the correct pointer to OpenGl. Notes
    // that if two buffers are enabled to the same component, the second one will overwrite
    // the first one. You should use only one buffer by component.
    
    // NOTES : In all gl functions wich require the buffer a pointer to data, as those buffers
    // can be on the GPU, GPU buffers should return a null pointer (0) .
    
    for ( const Gre::HardwareVertexBufferHolder& buffer : holder->getVertexBuffers() )
    {
        if ( !buffer.isInvalid() && buffer->isEnabled() )
        {
            const Gre::VertexDescriptor& desc = buffer->getVertexDescriptor () ;
            buffer->bind() ;
            
            for ( const Gre::VertexComponentType& component : desc.getComponents() )
            {
                if ( component == Gre::VertexComponentType::Position )
                {
                    glVertexAttribPointer(locpos,
                                          3,
                                          GL_FLOAT, GL_FALSE,
                                          desc.getStride(component),
                                          buffer->getData() + desc.getOffset(component) ) ;
                }
                
                else if ( component == Gre::VertexComponentType::Color )
                {
                    glVertexAttribPointer(loccol,
                                          4,
                                          GL_UNSIGNED_INT, GL_FALSE,
                                          desc.getStride(component),
                                          buffer->getData() + desc.getOffset(component) ) ;
                }
                
                else if ( component == Gre::VertexComponentType::Normal )
                {
                    glVertexAttribPointer(locnor,
                                          3,
                                          GL_FLOAT, GL_TRUE,
                                          desc.getStride(component),
                                          buffer->getData() + desc.getOffset(component) ) ;
                }
                
                else
                {
                    GreDebug("[WARN] Invalid VertexComponentType in HardwareVertexBuffer '") << buffer->getName() << "'." << Gre::gendl ;
                }
            }
        }
    }
    
    // Bind the index buffer and send elements to OpenGl.
    
    const Gre::HardwareIndexBufferHolder& index = holder->getIndexBuffer();
    index->bind() ;
    
    glDrawElements(translateGlMode(index->getIndexDescriptor().getMode()),
                   index->count(),
                   translateGlType(index->getIndexDescriptor().getType()),
                   index->getData());
    
    // Disables every components.
    
    glDisableVertexAttribArray(locpos);
    glDisableVertexAttribArray(loccol);
    glDisableVertexAttribArray(locnor);
    
    glBindVertexArray(0);
}

void OpenGlRenderer::_postNode(const Gre::RenderNodeHolder &node, const Gre::CameraUser &camera, const Gre::HardwareProgramHolder &program) const
{
    
}

void OpenGlRenderer::installDefaultFeatures()
{
    {
        // Try to locate default programs.
        
        Gre::ResourceManager::Get().getHardwareProgramManager()->_loadDefaultProgram() ;
        
        Gre::HardwareProgramUser defprog = Gre::ResourceManager::Get().getHardwareProgramManager()->getDefaultProgram();
        if ( !defprog.isInvalid() ) {
            iDefaultProgram = defprog ;
            setFeature(Gre::RendererFeature::LoadDefaultProgram);
        }
    }
    
    iFeaturesInstalled = true ;
}

Gre::MeshManagerHolder OpenGlRenderer::iCreateMeshManager() const
{
    return Gre::MeshManagerHolder ( new OpenGlMeshManager(this) ) ;
}

Gre::HardwareProgramManagerHolder OpenGlRenderer::iCreateProgramManager() const
{
    return Gre::HardwareProgramManagerHolder ( new OpenGlProgramManager(this) ) ;
}

Gre::TextureManagerHolder OpenGlRenderer::iCreateTextureManager() const
{
    return Gre::TextureManagerHolder ( new OpenGlTextureManager("OpenGlTextureManager" , this) ) ;
}













