//////////////////////////////////////////////////////////////////////
//
//  RenderPass.h
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

#ifndef GRE_RENDERPASS_H
#define GRE_RENDERPASS_H

#include "Pools.h"
#include "Renderable.h"

#include "RenderTarget.h"
#include "Viewport.h"
#include "Scene.h"

GreBeginNamespace

class Renderer ;

//////////////////////////////////////////////////////////////////////
/// @brief Defines a rendering pass as a rendering action constructed
/// by the user.
///
/// A RenderPass is a set of informations defined by the user on how
/// to render something somewhere. You can set here everything the renderer
/// needs to render something.
///
/// A RenderPass can also holds constant uniforms values that will be
/// bound to the used Technique before drawing. Those values can be
/// changed separately from the rendering loop.
///
/// A RenderPass should be used by a User to define a step in the
/// rendering process. For example , take the case one wants to draw the
/// entire scene but one object has a post processing technique. The
/// main renderpass will render pre processing technique using the whole
/// scene , then the main technique with the whole scene. Then for post
/// processing technique , for each node , it will draw the post processing
/// techniques of the node . Then it will use the post processing techniques
/// of the pass for the whole scene.
///
/// use technique -> if self rendered -> bind / draw
///               -> else -> bind objects -> for each node -> pre processing
///                                                        -> draw
///                                                        -> post processing
/// preprocessing -> use prerender techniques.
/// postprocessing -> use postrender techniques.
///
/// main render pass
///             -> bind viewport/clarbuffers
///             -> use preprocessing techniques
///             -> use main technique
///             -> use postprocessing techniques
///
///
///
//////////////////////////////////////////////////////////////////////
class DLL_PUBLIC RenderPass : public Renderable
{
public:

    POOLED ( Pools::Render ) ;

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    RenderPass ( const std::string & name ) ;

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual ~RenderPass () noexcept ( false ) ;

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    void setRenderTarget ( const RenderTargetHolder & target ) ;

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    const RenderTargetHolder & getRenderTarget () const ;

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    void setViewport ( const Viewport & viewport ) ;

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    const Viewport & getViewport () const ;

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    void setClearColor ( const Color & color ) ;

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    const Color & getClearColor () const ;

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    void setClearDepth ( float value ) ;

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    float getClearDepth () const ;

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    void setClearBuffers ( const ClearBuffers & buff ) ;

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    const ClearBuffers & getClearBuffers () const ;

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    void setScene ( const RenderSceneHolder & scene ) ;

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    const RenderSceneHolder & getScene () const ;

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    void setTechnique ( const TechniqueHolder & technique ) ;

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    const TechniqueHolder & getTechnique () const ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Binds the parameters to the given technique for this render
    /// pass. Those parameters are the viewport's parameters , the clear
    /// color , the clear depth , and every custom parameters.
    //////////////////////////////////////////////////////////////////////
    virtual void use ( const TechniqueHolder & technique ) const ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Renders the pass using the given renderer.
    //////////////////////////////////////////////////////////////////////
    virtual void render ( const Renderer * renderer ) const ;

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual void setCamera ( const CameraHolder & camera ) ;

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual const CameraHolder & getCamera () const ;

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual void setClearViewport ( bool value ) ;

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual bool isClearViewport () const ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Adds a Self-Rendered Renderable.
    //////////////////////////////////////////////////////////////////////
    virtual void addSelfUsedRenderable ( const RenderableHolder & renderable ) ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Returns 'iSelfUsedParams'.
    //////////////////////////////////////////////////////////////////////
    virtual const std::vector < RenderableHolder > & getSelfUsedRenderables () const ;

protected:

    //////////////////////////////////////////////////////////////////////
    /// @brief Renders once using the given technique.
    //////////////////////////////////////////////////////////////////////
    virtual void renderTechnique ( const Renderer * renderer , const TechniqueHolder & technique ) const ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Binds node , lights and material and render the node.
    //////////////////////////////////////////////////////////////////////
    virtual void renderTechniqueWithNodeAndLights (const Renderer* renderer ,
                                                   const TechniqueHolder & technique ,
                                                   const RenderNodeHolder & node ,
                                                   const std::list < LightRenderNodeHolder > & lights) const ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Binds lights and render the technique. Notes the technique
    /// should inform the renderer on how to render it if node is invalid.
    //////////////////////////////////////////////////////////////////////
    virtual void renderTechniqueWithLights (const Renderer* renderer ,
                                            const TechniqueHolder & technique ,
                                            const RenderNodeHolder & node ,
                                            const std::list < LightRenderNodeHolder > & lights ) const ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Renders a technique only for given node. The previous technique
    /// should have been unbinded. Every components may be used in this function,
    /// except the scene as only one node is drew.
    //////////////////////////////////////////////////////////////////////
    virtual void renderTechniqueWithNode (const Renderer* renderer ,
                                          const TechniqueHolder & technique ,
                                          const RenderNodeHolder & node ,
                                          const std::list < LightRenderNodeHolder > & lights) const ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Binds this renderable , and use the renderer to draw the
    /// node. If node is invalid , the technique should inform the renderer
    /// how to draw it.
    //////////////////////////////////////////////////////////////////////
    virtual void renderTechniqueWithNode (const Renderer* renderer ,
                                          const TechniqueHolder & technique ,
                                          const RenderNodeHolder & node) const ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Changes the viewport values when the rendertarget changes
    /// its surface. Notes that this function is called only if the
    /// rendertarget is a Window. This function use the new window size to
    /// configure the viewport from its relatives values.
    //////////////////////////////////////////////////////////////////////
    virtual void onWindowSizedEvent ( const WindowSizedEvent & e ) ;

protected:

    /// @brief RenderTarget to draw.
    RenderTargetHolder iTarget ;

    /// @brief Viewport we draw.
    Viewport iViewport ;

    /// @brief Clear Color to clear the color buffer. (Applicable only if cleared buffers
    /// holds ClearBuffer::Color)
    Color iClearColor ;

    /// @brief Depth value to clear the depth buffer. (Applicable only if cleared buffers
    /// holds ClearBuffer::Depth)
    float iClearDepth ;

    /// @brief Cleared buffers. Default contains color, depth and stencil buffers.
    ClearBuffers iClearBuffers ;

    /// @brief Scene to draw.
    RenderSceneHolder iScene ;

    /// @brief Technique to use when drawing this pass.
    TechniqueHolder iTechnique ;

    /// @brief Camera's used to draw everything.
    CameraHolder iCamera ;

    /// @brief Sets this option to 'true' if you want that the Viewport acts as a Scissor
    /// test for clearing buffers. This lets the user clears only regions of a framebuffer
    /// using scissor commands.
    bool iClearViewport ;

    /// @brief Self Used Parameters are TechniqueParamBinders that will be called when a
    /// Self-Rendered technique is bound. Notes this applies to the current technique ,
    /// applying to its pre and post processing techniques. In order to set Self Used Params
    /// to a sub-technique only , creates a RenderSubPass.
    std::vector < RenderableHolder > iSelfUsedParams ;
};

/// @brief
typedef SpecializedCountedObjectUser<RenderPass> RenderPassUser ;

/// @brief
typedef SpecializedCountedObjectHolder<RenderPass> RenderPassHolder ;

/// @brief
typedef SpecializedResourceHolderList<RenderPass> RenderPassHolderList ;

GreEndNamespace

#endif // GRE_RENDERPASS_H
