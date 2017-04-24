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

RenderPass::RenderPass ( const std::string & name ) : Gre::Renderable ( name )
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
        //////////////////////////////////////////////////////////////////////
        // When the technique is Self-Rendered , we bind only the params hold
        // by iSelfUsedParams. When not , we only bind the Viewport and the
        // clear colors and depth.

        if ( technique -> isSelfRendered() )
        {
            for ( const auto & param : iSelfUsedParams )
            if ( !param.isInvalid() )
            param -> use ( technique ) ;
        }

        else
        {
            technique -> setAliasedParameterValue ( TechniqueParam::ViewportLeft , HdwProgVarType::Int1 , iViewport.left ) ;
            technique -> setAliasedParameterValue ( TechniqueParam::ViewportTop , HdwProgVarType::Int1 , iViewport.top ) ;
            technique -> setAliasedParameterValue ( TechniqueParam::ViewportWidth , HdwProgVarType::Int1 , iViewport.width ) ;
            technique -> setAliasedParameterValue ( TechniqueParam::ViewportHeight , HdwProgVarType::Int1 , iViewport.height ) ;

            technique -> setAliasedParameterValue ( TechniqueParam::ClearColor , HdwProgVarType::Float4 , iClearColor.toFloat4() ) ;
            technique -> setAliasedParameterValue ( TechniqueParam::ClearDepth , HdwProgVarType::Float1 , iClearDepth ) ;
        }
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

        //////////////////////////////////////////////////////////////////////
        // Uses preprocessing techniques here. Those techniques will use adequate
        // binding to be draw by the renderer.

        for ( auto tech : iPreProcessTechniques )
        renderTechnique ( renderer , tech ) ;

        //////////////////////////////////////////////////////////////////////
        // Uses the main technique.

        renderTechnique ( renderer , iTechnique ) ;

        //////////////////////////////////////////////////////////////////////
        // Uses postprocessing techniques.

        for ( auto tech : iPostProcessTechniques )
        renderTechnique ( renderer , tech ) ;
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

void RenderPass::addSelfUsedRenderable ( const RenderableHolder & renderable )
{
    GreAutolock ; iSelfUsedParams.push_back ( renderable ) ;
}

const std::vector < RenderableHolder > & RenderPass::getSelfUsedRenderables () const
{
    GreAutolock ; return iSelfUsedParams ;
}

void RenderPass::renderTechnique(const Gre::Renderer *renderer, const TechniqueHolder &technique) const
{
    if ( technique.isInvalid() )
    return ;

    GreAutolock ;

    //////////////////////////////////////////////////////////////////////
    // First binds the technique. Binding the technique should also binds
    // the framebuffer if it has one.

    technique -> bind () ;

    //////////////////////////////////////////////////////////////////////
    // If technique is Self-Rendered , use it and use the renderer to draw
    // it normally. It should inform the Renderer on how to render it.

    if ( technique -> isSelfRendered() )
    {
        use ( technique ) ;
        renderer -> draw ( technique ) ;
    }

    else
    {
        RenderNodeHolderList nodes ;

        //////////////////////////////////////////////////////////////////////
        // When the Scene is invalid , we choose to use an empty node list and
        // so call the renderer draw function with the technique as argument. This
        // let the renderer choose if it should draw the technique as a Self-Rendered
        // or just left blank screen.

        if ( !iScene.isInvalid() && !iCamera.isInvalid() )
        nodes = iScene -> getNodesForCamera ( iCamera ) ;

        if ( !iCamera.isInvalid() )
        iCamera -> use ( technique ) ;

        if ( !iScene.isInvalid() )
        iScene -> use ( technique ) ;

        //////////////////////////////////////////////////////////////////////
        // Computes lights depending on scene , camera and technique informations.

        std::list<LightRenderNodeHolder> lights ;

        if (technique -> getLightingMode() != TechniqueLightingMode::None &&
            !iScene.isInvalid() && !iCamera.isInvalid() )
        lights = iScene -> getActivatedLightsForCamera ( iCamera ) ;

        if ( !nodes.empty() )
        {
            //////////////////////////////////////////////////////////////////////
            // For each nodes , bind lights , bind material , draw it with renderer.

            for ( auto node : nodes )
            renderTechniqueWithNodeAndLights ( renderer , technique , node , lights ) ;
        }

        else
        {
            //////////////////////////////////////////////////////////////////////
            // Depending on lighting mode , bind lights and call technique.

            renderTechniqueWithLights ( renderer , technique , RenderNodeHolder ( nullptr ) , lights ) ;
        }

    }

    //////////////////////////////////////////////////////////////////////
    // Unbinding the technique should unbind the framebuffer , reste lights
    // and textures counter.

    technique -> reset () ;
    technique -> unbind () ;
}

void RenderPass::renderTechniqueWithNodeAndLights (const Renderer* renderer ,
                                                   const TechniqueHolder & technique ,
                                                   const RenderNodeHolder & node ,
                                                   const std::list < LightRenderNodeHolder > & lights) const
{
    if ( node.isInvalid() || technique.isInvalid() )
    return ;
    
    if ( !node->isRenderable() )
    return ;

    //////////////////////////////////////////////////////////////////////
    // If node is not invalid , see if it has preprocessing techniques. If
    // this is the case , we unbind the current technique and binds the
    // preprocessed technique , making a rendering exclusively for the current
    // node.

    auto preprocess = node -> getPreProcessTechniques () ;

    if ( !preprocess.empty() )
    technique -> unbind () ;

    for ( auto tech : preprocess )
    renderTechniqueWithNode ( renderer , tech , node , lights ) ;

    if ( !preprocess.empty() )
    technique -> bind () ;

    //////////////////////////////////////////////////////////////////////
    // As we also need to make the 'NormalMatrix' parameter , we should calculate
    // it here and directly sets it to the technique.

    if ( !iCamera.isInvalid() )
    {
        Matrix3 modelViewMat = Matrix3 ( node->getModelMatrix() * iCamera->getViewMatrix() ) ;
        Matrix3 normalMat = glm::transpose(glm::inverse(modelViewMat)) ;

        technique -> setAliasedParameterValue ( TechniqueParam::NormalMatrix3 , HdwProgVarType::Matrix3 , normalMat ) ;
    }

    //////////////////////////////////////////////////////////////////////
    // Binds the node. Notes that if the node does not contains any renderable,
    // we do not render it.
    
    if ( node -> isRenderable() )
    node -> getMesh() -> bind ( technique ) ;

    node -> use ( technique ) ;

    //////////////////////////////////////////////////////////////////////
    // Binds every lights and render the node.

    renderTechniqueWithLights ( renderer , technique , node , lights ) ;
    
    if ( node -> isRenderable() )
    node -> getMesh() -> unbind ( technique ) ;

    //////////////////////////////////////////////////////////////////////
    // After using lights and textures , we should always reset the
    // technique counters.

    technique -> reset () ;

    //////////////////////////////////////////////////////////////////////
    // If node is not invalid , see if it has postprocessing techniques. If
    // this is the case , we unbind the current technique and binds the
    // postprocessed technique , making a rendering exclusively for the current
    // node.

    auto postprocess = node -> getPostProcessTechniques () ;

    if ( !postprocess.empty() )
    technique -> unbind () ;

    for ( auto tech : postprocess )
    renderTechniqueWithNode ( renderer , tech , node , lights ) ;

    if ( !postprocess.empty() )
    technique -> bind () ;
}

void RenderPass::renderTechniqueWithLights (const Renderer* renderer ,
                                            const TechniqueHolder & technique ,
                                            const RenderNodeHolder & node ,
                                            const std::list < LightRenderNodeHolder > & lights ) const
{
    if ( technique.isInvalid() )
    return ;

    //////////////////////////////////////////////////////////////////////
    // Binds lights.

    if ( technique -> getLightingMode() == TechniqueLightingMode::AllLights )
    {
        for ( auto light : lights )
        if ( !light.isInvalid() )
        light -> use ( technique , false ) ;

        renderTechniqueWithNode ( renderer , technique , node ) ;
    }

    else if ( technique -> getLightingMode() == TechniqueLightingMode::PerLight )
    {
        for ( auto light : lights )
        {
            if ( light.isInvalid() )
            continue ;

            light -> use ( technique , false ) ;

            renderTechniqueWithNode ( renderer , technique , node ) ;

            technique -> resetLights () ;
        }
    }

    else if ( technique -> getLightingMode() == TechniqueLightingMode::None )
    {
        renderTechniqueWithNode ( renderer , technique , node ) ;
    }
}

void RenderPass::renderTechniqueWithNode (const Renderer* renderer ,
                                          const TechniqueHolder & technique ,
                                          const RenderNodeHolder & node ,
                                          const std::list < LightRenderNodeHolder > & lights) const
{
    if ( technique.isInvalid() )
    return ;

    GreAutolock ;

    //////////////////////////////////////////////////////////////////////
    // First binds the technique. Binding the technique should also binds
    // the framebuffer if it has one.

    technique -> bind () ;

    //////////////////////////////////////////////////////////////////////
    // If technique is Self-Rendered , use it and use the renderer to draw
    // it normally. It should inform the Renderer on how to render it.

    if ( technique -> isSelfRendered() )
    {
        use ( technique ) ;
        renderer -> draw ( technique ) ;
    }

    else
    {
        if ( !iCamera.isInvalid() )
        iCamera -> use ( technique ) ;

        if ( !iScene.isInvalid() )
        iScene -> use ( technique ) ;

        renderTechniqueWithNodeAndLights ( renderer , technique , node , lights ) ;
    }

    //////////////////////////////////////////////////////////////////////
    // Unbinding the technique should unbind the framebuffer , reste lights
    // and textures counter.

    technique -> reset () ;
    technique -> unbind () ;
}

void RenderPass::renderTechniqueWithNode (const Renderer* renderer ,
                                          const TechniqueHolder & technique ,
                                          const RenderNodeHolder & node) const
{
    //////////////////////////////////////////////////////////////////////
    // Binds this renderable and draw using the renderer.

    use ( technique ) ;

    if ( node.isInvalid() )
    renderer -> draw ( technique ) ;

    else
    renderer -> draw ( node ) ;
}

void RenderPass::onWindowSizedEvent ( const WindowSizedEvent & e )
{
    iViewport.adaptRealArea(e.Width, e.Height);
    iViewport.adaptRealCorner(e.Width, e.Height) ;
}

GreEndNamespace
