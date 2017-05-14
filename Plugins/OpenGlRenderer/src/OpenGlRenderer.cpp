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

std::string translateGlError ( GLenum err )
{
    if ( err == GL_NO_ERROR )
        return "GL_NO_ERROR" ;
    else if ( err == GL_INVALID_ENUM )
        return "GL_INVALID_ENUM" ;
    else if ( err == GL_INVALID_VALUE )
        return "GL_INVALID_VALUE" ;
    else if ( err == GL_INVALID_OPERATION )
        return "GL_INVALID_OPERATION" ;
    else if ( err == GL_INVALID_FRAMEBUFFER_OPERATION )
        return "GL_INVALID_FRAMEBUFFER_OPERATION" ;
    else if ( err == GL_OUT_OF_MEMORY )
        return "GL_OUT_OF_MEMORY" ;

#ifdef GL_STACK_UNDERFLOW
    else if ( err == GL_STACK_UNDERFLOW )
        return "GL_STACK_UNDERFLOW" ;
#endif

#ifdef GL_STACK_OVERFLOW
    else if ( err == GL_STACK_OVERFLOW )
        return "GL_STACK_OVERFLOW" ;
#endif

    return "" ;
}

// ---------------------------------------------------------------------------------------------------------
// OpenGL Error handler.

void checkOpenGlError ( void )
{
    GLenum error ;
    while ( (error = glGetError()) != GL_NO_ERROR )
    {

    }
}

// ---------------------------------------------------------------------------------------------------------

OpenGlRenderer::OpenGlRenderer ( const std::string& name , const Gre::RendererOptions& options )
: Gre::Renderer(name, options)
{
    iDefaultQuadVAO = 0 ;
    iDefaultQuadVBO = 0 ;
}

OpenGlRenderer::~OpenGlRenderer() noexcept ( false )
{

}

void OpenGlRenderer::setClearRegion(const Gre::Surface &box) const
{
    glEnable(GL_SCISSOR_TEST);
    glScissor(box.left, box.top, box.width, box.height);
}

void OpenGlRenderer::setViewport(const Gre::Viewport &viewport) const
{
    if ( iContext.isInvalid() )
    return ;

    // Notes the framebuffer should update the viewport values. Here we disable
    // the scissor test to be sure no external scissor will destroy our render.

    glDisable ( GL_SCISSOR_TEST ) ;
    glViewport(viewport.left, viewport.top, viewport.width, viewport.height);

    // Now we should enable scissor test if the viewport has a special region.

    if ( viewport.regioned() )
    {
        Gre::Surface region = viewport.region () ;

        glEnable ( GL_SCISSOR_TEST ) ;
        glScissor ( region.left , region.top , region.width , region.height ) ;
    }

    // Now set clear depth and clear colors.

    glClearColor ( viewport.clearcolor().getRed() , viewport.clearcolor().getGreen() ,
                   viewport.clearcolor().getBlue() , viewport.clearcolor().getAlpha() ) ;

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glClearDepth ( viewport.cleardepth() ) ;

    // Now compute clear buffers.

    clearBuffers ( viewport.clearbuffers() ) ;
}

void OpenGlRenderer::setClearDepth(float value) const
{
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glClearDepth(value);
}

void OpenGlRenderer::setClearColor(const Gre::Color &color) const
{
    glClearColor(color.getRed(), color.getGreen(), color.getBlue(), color.getAlpha());
}

void OpenGlRenderer::clearBuffers ( const Gre::ClearBuffers & buffers ) const
{
    GLbitfield buffs = 0 ;

    if ( buffers.test((int)Gre::ClearBuffer::Color) )
        buffs = buffs | GL_COLOR_BUFFER_BIT ;
    if ( buffers.test((int)Gre::ClearBuffer::Depth) )
        buffs = buffs | GL_DEPTH_BUFFER_BIT ;
    if ( buffers.test((int)Gre::ClearBuffer::Stencil) )
        buffs = buffs | GL_STENCIL_BUFFER_BIT ;

    if ( buffs ) glClear(buffs) ;

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void OpenGlRenderer::drawSubMesh(const Gre::SubMeshHolder & submesh) const
{
    GreAutolock ;

    if ( submesh.isInvalid() )
        return ;

    //////////////////////////////////////////////////////////////////////
    // Binds the index buffer and send elements to OpenGl.

    const Gre::HardwareIndexBufferHolder& index = submesh->getIndexBuffer();
    index->bind() ;

    glDrawElements(translateGlMode(index->getIndexDescriptor().getMode()),
                   index->count(),
                   translateGlType(index->getIndexDescriptor().getType()),
                   index->getData());
}

void OpenGlRenderer::draw ( const Gre::TechniqueHolder & technique ) const
{
    GreAutolock ;

    //////////////////////////////////////////////////////////////////////
    // If VAO and VBO are not initialized , initializes them , const_casting
    // this object. This way , the quad can be initialized at first time this
    // function is used , but normally this function should not change anything
    // in the renderer (and so the 'const' parameter).

    if ( !iDefaultQuadVAO || !iDefaultQuadVBO )
    {
        if ( ! const_cast < OpenGlRenderer* > (this) -> loadDefaultQuad () )
        GreDebug ( "[WARN] Can't load default Quad to render technique." ) << Gre::gendl ;
    }

    glBindVertexArray ( iDefaultQuadVAO ) ;
    glDrawArrays ( GL_TRIANGLE_STRIP , 0 , 4 ) ;

    glBindVertexArray ( 0 ) ;
}

Gre::MeshManagerHolder OpenGlRenderer::iCreateMeshManager() const
{
    return Gre::MeshManagerHolder ( new OpenGlMeshManager(this) ) ;
}

Gre::HardwareProgramManagerInternalCreator* OpenGlRenderer::iCreateProgramManagerCreator() const
{
    return new OpenGlProgramManagerCreator(this) ;
}

Gre::TextureInternalCreator* OpenGlRenderer::iCreateTextureCreator() const
{
    return new OpenGlTextureCreator(this) ;
}

Gre::RenderFramebufferInternalCreator* OpenGlRenderer::iCreateFramebufferCreator() const
{
    return new OpenGlFramebufferCreator(this) ;
}

bool OpenGlRenderer::loadDefaultQuad ()
{
    GreAutolock ;

    if ( iContext.isInvalid() )
    return false ;

    GLfloat defaultQuadVertices [] =
    {
        -1.0f , -1.0f , 0.0f ,
         1.0f , -1.0f , 0.0f ,
        -1.0f ,  1.0f , 0.0f ,
         1.0f ,  1.0f , 0.0f
    };

    glGenVertexArrays ( 1 , &iDefaultQuadVAO ) ;
    glBindVertexArray ( iDefaultQuadVAO ) ;

    glGenBuffers ( 1 , &iDefaultQuadVBO ) ;
    glBindBuffer ( GL_ARRAY_BUFFER , iDefaultQuadVBO ) ;
    glBufferData ( GL_ARRAY_BUFFER , sizeof(defaultQuadVertices) , defaultQuadVertices , GL_STATIC_DRAW ) ;

    glEnableVertexAttribArray ( 0 ) ;
    glVertexAttribPointer ( (GLuint) 0 , 3 , GL_FLOAT , GL_FALSE , 0 , 0 ) ;

    glBindVertexArray ( 0 ) ;
    return true ;
}
