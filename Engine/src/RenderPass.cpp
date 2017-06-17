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

RenderPass::RenderPass ( const std::string & name )
: Gre::Renderable ( name )
{

}

RenderPass::~RenderPass () noexcept ( false )
{

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
        // by iSelfUsedParams.

        if ( technique -> isSelfRendered() )
        {
            for ( const auto & param : iSelfUsedParams )
            if ( !param.isInvalid() )
            param -> use ( technique ) ;
        }
    }

    TechniqueParamBinder::use(technique);
}

void RenderPass::setCamera(const RenderNodeHolder &camera)
{
    GreAutolock ; iCamera = camera ;
}

const RenderNodeHolder & RenderPass::getCamera() const
{
    GreAutolock ; return iCamera ;
}

void RenderPass::render ( const Renderer * renderer ) const
{
    GreAutolock ;

    if ( renderer && !iTechnique.isInvalid() )
    {
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
    // the framebuffer. Notes a technique should always have a framebuffer
    // object to bind. In fact , even the null framebuffer should be the
    // default framebuffer.

    auto framebuffer = technique -> getFramebuffer () ;

    if ( framebuffer.isInvalid() )
    return ;

    technique -> bind () ;

    Surface ctxtsurf = renderer -> getRenderContextSurface () ;
    Surface viewsurf = framebuffer -> getViewport().makeSurface( ctxtsurf ) ;

    if ( viewsurf.isZero() )
    {
        technique -> unbind () ;
        return ;
    }

    renderer -> setViewport ( framebuffer->getViewport() ) ;

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
        //////////////////////////////////////////////////////////////////////
        // When the Scene is invalid , we choose to use an empty node list and
        // so call the renderer draw function with the technique as argument. This
        // let the renderer choose if it should draw the technique as a Self-Rendered
        // or just left blank screen.

        if ( iCamera.isInvalid() )
        {
            technique -> reset () ;
            technique -> unbind () ;
            return ;
        }

        const Matrix4 & view = iCamera -> getViewMatrix () ;
        const Matrix4 & projection = technique -> getProjectionMatrix () ;
        const Matrix4 viewprojection = projection * view ;

        technique -> setAliasedParameterValue ( TechniqueParam::CameraPosition , HdwProgVarType::Float3 , iCamera -> getPosition() ) ;
        technique -> setAliasedParameterValue ( TechniqueParam::CameraDirection , HdwProgVarType::Float3 , iCamera -> getDirection() ) ;
        technique -> setAliasedParameterValue ( TechniqueParam::ProjectionMatrix , HdwProgVarType::Matrix4 , projection ) ;
        technique -> setAliasedParameterValue ( TechniqueParam::ViewMatrix , HdwProgVarType::Matrix4 , view ) ;
        technique -> setAliasedParameterValue ( TechniqueParam::ProjectionViewMatrix , HdwProgVarType::Matrix4 , viewprojection ) ;

        RenderNodeHolderList nodes ;
        RenderNodeHolderList lights ;

        if ( !iScene.isInvalid() )
        {
            if ( technique->getLightingMode() != TechniqueLightingMode::None )
            lights = iScene -> lights ( viewprojection ) ;

            nodes = iScene -> sort ( viewprojection ) ;
            iScene -> use ( technique ) ;
        }

        if ( !nodes.empty() )
        {
            //////////////////////////////////////////////////////////////////////
            // For each nodes , bind lights , bind material , draw it with renderer.
            //GreDebug ( "[WARN] :: SIZE = " ) << nodes.size () << gendl ;
            for ( auto node : nodes )
            renderTechniqueWithNodeAndLights ( renderer , technique , node , lights ) ;
        }

        else
        {
            GreDebug ( "[WARN] :: NODES_EMPTY" ) << gendl ;
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
                                                   const RenderNodeHolderList & lights) const
{
    if ( node.isInvalid() || technique.isInvalid() )
    return ;

    if ( node->getMesh().isInvalid() )
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
    {
        technique -> bind () ;
        renderer -> setViewport ( technique->getFramebuffer()->getViewport() ) ;
    }

    //////////////////////////////////////////////////////////////////////
    // As we also need to make the 'NormalMatrix' parameter , we should calculate
    // it here and directly sets it to the technique.

    if ( !iCamera.isInvalid() )
    {
        const Matrix4 & view = iCamera -> getViewMatrix () ;
        const Matrix4 & model = node -> getModelMatrix () ;
        const Matrix4 modelview = model * view ;
        const Matrix3 normal = glm::transpose(glm::inverse( Matrix3(modelview) )) ;

        technique -> setAliasedParameterValue ( TechniqueParam::ModelMatrix , HdwProgVarType::Matrix4 , model ) ;
        technique -> setAliasedParameterValue ( TechniqueParam::NormalMatrix3 , HdwProgVarType::Matrix3 , normal ) ;
    }

    //////////////////////////////////////////////////////////////////////
    // Binds the node. Notes that if the node does not contains any renderable,
    // we do not render it.

    node -> use ( technique ) ;

    //////////////////////////////////////////////////////////////////////
    // Binds every lights and render the node.

    renderTechniqueWithLights ( renderer , technique , node , lights ) ;

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
                                            const RenderNodeHolderList & lights ) const
{
    if ( technique.isInvalid() )
    return ;

    //////////////////////////////////////////////////////////////////////
    // Binds lights.

    if ( technique -> getLightingMode() == TechniqueLightingMode::AllLights )
    {
        for ( auto light : lights )
        if ( !light.isInvalid() )
        light -> bindEmissiveMaterial ( technique ) ;

        renderTechniqueWithNode ( renderer , technique , node ) ;
    }

    else if ( technique -> getLightingMode() == TechniqueLightingMode::PerLight )
    {
        for ( auto light : lights )
        {
            if ( light.isInvalid() )
            continue ;

            light -> bindEmissiveMaterial ( technique ) ;

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
                                          const RenderNodeHolderList & lights) const
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
        {
            const Matrix4 & view = iCamera -> getViewMatrix () ;
            const Matrix4 & projection = technique -> getProjectionMatrix () ;
            const Matrix4 viewprojection = projection * view ;

            technique -> setAliasedParameterValue ( TechniqueParam::CameraPosition , HdwProgVarType::Float3 , iCamera -> getPosition() ) ;
            technique -> setAliasedParameterValue ( TechniqueParam::CameraDirection , HdwProgVarType::Float3 , iCamera -> getDirection() ) ;
            technique -> setAliasedParameterValue ( TechniqueParam::ProjectionMatrix , HdwProgVarType::Matrix4 , projection ) ;
            technique -> setAliasedParameterValue ( TechniqueParam::ViewMatrix , HdwProgVarType::Matrix4 , view ) ;
            technique -> setAliasedParameterValue ( TechniqueParam::ProjectionViewMatrix , HdwProgVarType::Matrix4 , viewprojection ) ;
        }

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
    {
        if ( node->getMesh().isInvalid() )
        return ;

        auto mesh = node -> getMesh () ;
        mesh -> bind ( technique ) ;

        while ( mesh -> bindNextSubMesh ( technique ) )
        {
            auto submesh = mesh -> getCurrentSubMesh () ;
            auto material = submesh -> getDefaultMaterial () ;

            if ( node -> getMaterial().isInvalid() && !material.isInvalid() )
            material -> use ( technique ) ;

            renderer -> drawSubMesh ( submesh ) ;

            mesh -> unbindCurrentSubMesh ( technique ) ;
        }

        mesh -> unbind ( technique ) ;
    }
}

GreEndNamespace
