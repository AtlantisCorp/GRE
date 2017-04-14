//////////////////////////////////////////////////////////////////////
//
//  RenderPass.cpp
//  This source file is part of Gre
//		(Gang's Resource Engine)
//
//  Copyright (c) 2015 - 2017 Luk2010
//  Created on 24/03/2017.
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

#include "RenderPass.h"
#include "Renderer.h"

GreBeginNamespace

RenderPass::RenderPass ( const std::string & name ) : Gre::Resource ( name )
{
    iClearDepth = 1.0f ;
    iClearBuffers.set((int)ClearBuffer::Color, true);
    iClearBuffers.set((int)ClearBuffer::Depth, true);
    iClearBuffers.set((int)ClearBuffer::Stencil, true);
}

RenderPass::~RenderPass () noexcept ( false )
{

}

void RenderPass::setRenderTarget ( const RenderTargetHolder & target )
{
    GreAutolock ;

    if ( !iTarget.isInvalid() ) {
        iTarget -> removeListener (this) ;
    }

    iTarget = target ;

    if ( !iTarget.isInvalid() ) {
        iTarget -> addFilteredListener ( this , { EventType::WindowSized } ) ;
    }
}

const RenderTargetHolder & RenderPass::getRenderTarget () const
{
    GreAutolock ; return iTarget ;
}

void RenderPass::setViewport ( const Viewport & viewport )
{
    GreAutolock ; iViewport = viewport ;
}

const Viewport & RenderPass::getViewport () const
{
    GreAutolock ; return iViewport ;
}

void RenderPass::setClearColor ( const Color & color )
{
    GreAutolock ; iClearColor = color ;
}

const Color & RenderPass::getClearColor () const
{
    GreAutolock ; return iClearColor ;
}

void RenderPass::setClearDepth ( float value )
{
    GreAutolock ; iClearDepth = value ;
}

float RenderPass::getClearDepth () const
{
    GreAutolock ; return iClearDepth ;
}

void RenderPass::setClearBuffers ( const ClearBuffers & buff )
{
    GreAutolock ; iClearBuffers = buff ;
}

const ClearBuffers & RenderPass::getClearBuffers () const
{
    GreAutolock ; return iClearBuffers ;
}

void RenderPass::setScene ( const RenderSceneHolder & scene )
{
    GreAutolock ; iScene = scene ;
}

const RenderSceneHolder & RenderPass::getScene () const
{
    GreAutolock ; return iScene ;
}

void RenderPass::setTechnique ( const TechniqueHolder & technique )
{
    GreAutolock ; iTechnique = technique ;
}

const TechniqueHolder & RenderPass::getTechnique () const
{
    GreAutolock ; return iTechnique ;
}

void RenderPass::use ( const TechniqueHolder & technique ) const
{
    GreAutolock ;

    if ( !technique.isInvalid() )
    {
        technique -> setAliasedParameterValue ( TechniqueParam::ViewportLeft , HdwProgVarType::Int1 , iViewport.left ) ;
        technique -> setAliasedParameterValue ( TechniqueParam::ViewportTop , HdwProgVarType::Int1 , iViewport.top ) ;
        technique -> setAliasedParameterValue ( TechniqueParam::ViewportWidth , HdwProgVarType::Int1 , iViewport.width ) ;
        technique -> setAliasedParameterValue ( TechniqueParam::ViewportHeight , HdwProgVarType::Int1 , iViewport.height ) ;

        technique -> setAliasedParameterValue ( TechniqueParam::ClearColor , HdwProgVarType::Float4 , iClearColor.toFloat4() ) ;
        technique -> setAliasedParameterValue ( TechniqueParam::ClearDepth , HdwProgVarType::Float1 , iClearDepth ) ;
    }

    TechniqueParamBinder::use(technique);
}

void RenderPass::setCamera(const CameraHolder &camera)
{
    GreAutolock ; iCamera = camera ;
}

const CameraHolder & RenderPass::getCamera() const
{
    GreAutolock ; return iCamera ;
}

void RenderPass::render ( const Renderer * renderer ) const
{
    GreAutolock ;

    if ( renderer && !iTechnique.isInvalid() )
    {
        
        //////////////////////////////////////////////////////////////////////
        // Checks the viewport area : If it is null , we don't need to render
        // the pass.
        
        if ( !iViewport.getArea() )
        return ;
        
        renderer -> setViewport ( iViewport ) ;
        
        if ( iClearViewport )
        renderer -> setClearRegion ( iViewport ) ;

        renderer -> setClearColor ( iClearColor ) ;
        renderer -> setClearDepth ( iClearDepth ) ;
        renderer -> clearBuffers ( iClearBuffers ) ;

        renderTechnique ( renderer , iTechnique ) ;
    }
}

void RenderPass::setClearViewport(bool value)
{
    GreAutolock ; iClearViewport = value ;
}

bool RenderPass::isClearViewport() const
{
    GreAutolock ; return iClearViewport ;
}

void RenderPass::prerender(const Gre::Renderer *renderer, const TechniqueHolder &technique) const
{
    if ( !technique.isInvalid() )
    {
        for ( auto tech : technique -> getPreTechniques() )
        {
            renderTechnique ( renderer , tech ) ;
        }
    }
}

void RenderPass::postrender(const Gre::Renderer *renderer, const TechniqueHolder &technique) const
{
    if ( !technique.isInvalid() )
    {
        for ( auto tech : technique -> getPostTechniques() )
        {
            renderTechnique ( renderer , technique ) ;
        }
    }
}

void RenderPass::renderTechnique(const Gre::Renderer *renderer, const TechniqueHolder &technique) const
{
    if ( technique.isInvalid() )
        return ;

    prerender ( renderer , technique ) ;

    technique -> resetLights () ;
    technique -> bind () ;

    iCamera -> use (technique) ;

    auto nodes = iScene -> getNodesForCamera ( iCamera ) ;
    iScene -> use ( technique ) ;

    if ( technique->getLightingMode() == TechniqueLightingMode::PerLight )
    {
        auto lights = iScene -> getActivatedLightsForCamera ( iCamera ) ;

        for ( const LightRenderNodeHolder & lightnode : lights )
        {
            lightnode -> use ( technique , false ) ;

            for ( auto n : nodes )
            {
                if ( !n -> isRenderable() )
                    continue ;
                
                //////////////////////////////////////////////////////////////////////
                // As we also need to make the 'NormalMatrix' parameter , we should calculate
                // it here and directly sets it to the technique.
                
                Matrix3 modelViewMat = Matrix3 ( iCamera->getViewMatrix() * n->getModelMatrix() ) ;
                Matrix3 normalMat = glm::transpose(glm::inverse(modelViewMat)) ;
                
                technique -> setAliasedParameterValue ( TechniqueParam::NormalMatrix3 , HdwProgVarType::Matrix3 , normalMat ) ;
                
                
                //////////////////////////////////////////////////////////////////////
                // Lets the node uses the technique and let the renderer draw the node.
                // Notes on OpenGl , drawing anything should be done under a VAO object ,
                // we should make the mesh bindable. When the mesh binds its attributes ,
                // it should also binds the VAO object. Unbinding unbinds the VAO object.
                
                n -> use ( technique ) ;
                use ( technique ) ;
                
                n -> getMesh() -> bind ( technique ) ;
                
                renderer -> draw (n) ;
                
                n -> getMesh() -> unbind ( technique ) ;
                
                //////////////////////////////////////////////////////////////////////
                // Undo the textures for the node. The mesh actually don't set textures,
                // as the node already sets everything in 'use'. Even for light's , shadowmap
                // is in the LightRenderNode object.
                
                technique -> resetTextures () ;
            }

            technique -> resetLights () ;
        }
    }

    else if ( technique->getLightingMode() == TechniqueLightingMode::AllLights )
    {
        auto lights = iScene -> getActivatedLightsForCamera ( iCamera ) ;

        for ( auto n : nodes )
        {
            if ( !n -> isRenderable() )
            continue ;
            
            //////////////////////////////////////////////////////////////////////
            // We have to set every lights for each node in order to rebuild the
            // texture units correctly.
            
            for ( const LightRenderNodeHolder & lightnode : lights ) {
                lightnode -> use ( technique , false ) ;
            }
            
            //////////////////////////////////////////////////////////////////////
            // As we also need to make the 'NormalMatrix' parameter , we should calculate
            // it here and directly sets it to the technique.
            
            Matrix3 modelViewMat = Matrix3 ( n->getModelMatrix() * iCamera->getViewMatrix() ) ;
            Matrix3 normalMat = glm::transpose(glm::inverse(modelViewMat)) ;
            
            technique -> setAliasedParameterValue ( TechniqueParam::NormalMatrix3 , HdwProgVarType::Matrix3 , normalMat ) ;


            //////////////////////////////////////////////////////////////////////
            // Lets the node uses the technique and let the renderer draw the node.
            // Notes on OpenGl , drawing anything should be done under a VAO object ,
            // we should make the mesh bindable. When the mesh binds its attributes ,
            // it should also binds the VAO object. Unbinding unbinds the VAO object.

            n -> use ( technique ) ;
            use ( technique ) ;
            
            n -> getMesh() -> bind ( technique ) ;
            
            renderer -> draw (n) ;
            
            n -> getMesh() -> unbind ( technique ) ;
            
            //////////////////////////////////////////////////////////////////////
            // Undo the textures for the node. The mesh actually don't set textures,
            // as the node already sets everything in 'use'. Even for light's , shadowmap
            // is in the LightRenderNode object.
            
            technique -> resetTextures () ;
        }

        technique -> resetLights () ;
    }

    else if ( technique->getLightingMode() == TechniqueLightingMode::None )
    {
        for ( auto n : nodes )
        {
            //////////////////////////////////////////////////////////////////////
            // As we also need to make the 'NormalMatrix' parameter , we should calculate
            // it here and directly sets it to the technique.

            Matrix4 normalMatrix = glm::inverseTranspose ( iCamera -> getViewMatrix() * n -> getModelMatrix() ) ;
            technique -> setAliasedParameterValue ( TechniqueParam::NormalMatrix , HdwProgVarType::Matrix4 , normalMatrix ) ;

            //////////////////////////////////////////////////////////////////////
            // Lets the node uses the technique and let the renderer draw the node.

            n -> use ( technique ) ;

            use ( technique ) ;
            renderer -> draw (n) ;
        }
    }

    technique -> unbind () ;
    postrender ( renderer , technique ) ;
}

void RenderPass::onWindowSizedEvent ( const WindowSizedEvent & e )
{
    iViewport.adaptRealArea(e.Width, e.Height);
    iViewport.adaptRealCorner(e.Width, e.Height) ;
}

GreEndNamespace
