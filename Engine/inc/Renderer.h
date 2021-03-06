//////////////////////////////////////////////////////////////////////
//
//  Renderer.h
//  This source file is part of Gre
//		(Gang's Resource Engine)
//
//  Copyright (c) 2015 - 2017 Luk2010
//  Created on 08/11/2015.
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

#ifndef GResource_Renderer_h
#define GResource_Renderer_h

#include "Resource.h"
#include "ResourceLoader.h"
#include "SpecializedResourceManager.h"

#include "Color.h"
#include "Mesh.h"
#include "HardwareProgram.h"
#include "HardwareProgramManager.h"
#include "FrameBuffer.h"
#include "Viewport.h"
#include "RenderContext.h"
#include "Variant.h"
#include "RenderPipeline.h"

GreBeginNamespace

/// @brief Info structure for Renderer.
typedef Dictionnary RendererOptions ;

/// @brief Matrix Types used by the Engine.
enum class MatrixType
{
    Projection,
    ModelView
};

//////////////////////////////////////////////////////////////////////
/// @class Renderer
/// @brief Base class for Renderer Objects.
///
/// A 'Renderer' is an object, that use a defined Rendering API (as
/// OpenGl, DirectX, Vulkan) to draw a defined 'RenderingQuery'. Normally,
/// this 'RenderingQuery' is defined by a 'RenderScene'.
///
/// The Renderer is not responsible for RenderContext creation/management.
/// This is done in order to have the following situation : an OpenGl
/// Renderer has to be written once in a plugin. Then, different plugins
/// can create OpenGl Window with RenderContext creation depending on
/// the platform, for example Cocoa OpenGl RenderContext or GlX Contexts.
///
/// @note
/// The Renderer can install some managers into the Engine. Those
/// managers are dependent from him.
///
//////////////////////////////////////////////////////////////////////
class DLL_PUBLIC Renderer : public Resource
{
public:

    POOLED(Pools::Resource)

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    Renderer ( const std::string& name , const RendererOptions& options );

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual ~Renderer () noexcept(false);

public:

    //////////////////////////////////////////////////////////////////////
    /// @brief Renders the pipeline binding the render context.
    //////////////////////////////////////////////////////////////////////
    virtual void render () const ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Sets only the given region for clearing the buffers. Also
    /// called a Scissor - box .
    //////////////////////////////////////////////////////////////////////
    virtual void setClearRegion ( const Surface & box ) const = 0 ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Sets the current viewport.
    //////////////////////////////////////////////////////////////////////
    virtual void setViewport ( const Viewport & viewport ) const = 0 ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Sets the binded framebuffer color used to clear the color
    /// buffer.
    //////////////////////////////////////////////////////////////////////
    virtual void setClearColor ( const Color & color ) const = 0 ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Sets the default's current framebuffer depth value when the
    /// depth buffer is cleared.
    //////////////////////////////////////////////////////////////////////
    virtual void setClearDepth ( float value ) const = 0 ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Clears the binded framebuffer buffers.
    //////////////////////////////////////////////////////////////////////
    virtual void clearBuffers ( const ClearBuffers & buffers ) const = 0 ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Draw generally a quad to the screen using the given technique.
    ///
    /// @note
    /// No binding is done in this function. Generally, this function is called
    /// when the technique is Self-Rendered. The Renderer may draw a static
    /// fullscreen quad using the given technique. The technique should already
    /// be binded when calling this function , and thus no binding is required.
    ///
    //////////////////////////////////////////////////////////////////////
    virtual void draw ( const Gre::TechniqueHolder & technique ) const = 0 ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Draw the designated submesh. Notes that this submesh should
    /// already be binded by RenderPass , and the renderer only has to call
    /// the draw command directly from its API ( for example glDrawElements ).
    ///
    /// This function can also bind an index buffer , and judge how to draw
    /// the submesh.
    ///
    //////////////////////////////////////////////////////////////////////
    virtual void drawSubMesh ( const SubMeshHolder & submesh ) const = 0 ;

public:

    //////////////////////////////////////////////////////////////////////
    /// @brief Installs managers this Renderer has to the ResourceManager .
    //////////////////////////////////////////////////////////////////////
    virtual bool installManagers ( ) ;

    ////////////////////////////////////////////////////////////////////////
    /// @brief Unloads the Resource.
    /// Also uninstall every managers from the ResourceManager.
    ////////////////////////////////////////////////////////////////////////
    virtual void unload () ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Creates a MeshManager for this Renderer.
    //////////////////////////////////////////////////////////////////////
    virtual MeshManagerHolder iCreateMeshManager ( ) const = 0 ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Creates a HardwareProgramManager for this Renderer.
    //////////////////////////////////////////////////////////////////////
    virtual HardwareProgramManagerInternalCreator* iCreateProgramManagerCreator ( ) const = 0 ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Creates a TextureManager for this Renderer.
    //////////////////////////////////////////////////////////////////////
    virtual TextureInternalCreator* iCreateTextureCreator ( ) const = 0 ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Creates the Framebuffer internal creator.
    //////////////////////////////////////////////////////////////////////
    virtual RenderFramebufferInternalCreator* iCreateFramebufferCreator ( ) const = 0 ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Changes 'iContext'.
    //////////////////////////////////////////////////////////////////////
    virtual void setRenderContext ( const RenderContextHolder& context ) ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Returns 'iContext'.
    //////////////////////////////////////////////////////////////////////
    virtual const RenderContextHolder& getRenderContext () const ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the surface associated to the renderer's context. If
    /// this context is invalid, it returns '{ 0 , 0 , 0 , 0 }' .
    //////////////////////////////////////////////////////////////////////
    virtual Surface getRenderContextSurface () const ;

public:

    //////////////////////////////////////////////////////////////////////
    /// @brief Returns true if the Renderer has successfully installed every
    /// of its managers to the ResourceManager.
    //////////////////////////////////////////////////////////////////////
    virtual bool isInstalled ( ) const ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Returns 'iEnabled' .
    //////////////////////////////////////////////////////////////////////
    virtual bool isEnabled () const ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Changes 'iEnabled' .
    //////////////////////////////////////////////////////////////////////
    virtual void setEnabled ( bool b ) ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Sets the pipeline used by the renderer.
    //////////////////////////////////////////////////////////////////////
    virtual void setPipeline ( const RenderPipelineHolder & pipeline ) ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Returns 'iPipeline'.
    //////////////////////////////////////////////////////////////////////
    virtual const RenderPipelineHolder & getPipeline () const ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Returns 'iPipeline'.
    //////////////////////////////////////////////////////////////////////
    virtual RenderPipelineHolder & getPipeline () ;

protected:

    /// @brief True if this renderer has installed every managers to the resource manager.
    bool iInstalled ;

    /// @brief The RenderContext this Renderer is attached to.
    RenderContextHolder iContext ;

    /// @brief Default Material used by this Renderer. The default Material is used only if
    /// the current node, mesh and technique does not have any default or set material.
    MaterialHolder iDefaultMaterial ;

    /// @brief Sets if the renderer may be enable or not. This may be use , for example , to
    /// switch from a renderer to another already ready and loaded. By default , this property is
    /// set to true but you can set it directly at construction to 'false' using the option 'Enabled'.
    bool iEnabled ;

    /// @brief Hold the pipeline currently used by the renderer.
    RenderPipelineHolder iPipeline ;
};

/// @brief Holder for RendererPrivate.
typedef Holder<Renderer> RendererHolder;

/// @brief SpecializedResourceHolderList for RendererPrivate.
typedef SpecializedResourceHolderList<Renderer> RendererHolderList;

//////////////////////////////////////////////////////////////////////
/// @brief ResourceLoader for Renderer.
///
/// Properties that can be given in 'RendererInfo' :
///
///   - 'Driver.name' : Name the driver should have (generally 'OpenGl'
/// or 'DirectX').
///   - 'Driver.minversion' : Version the driver should be at least, or
/// better.
///   - 'Driver.version' : Version the driver must have.
///
//////////////////////////////////////////////////////////////////////
class DLL_PUBLIC RendererLoader : public ResourceLoader
{
public:

    POOLED(Pools::Loader)

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    RendererLoader();

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual ~RendererLoader();

    //////////////////////////////////////////////////////////////////////
    /// @brief Returns 'true' if the given RenderingApiDescriptor is compatible
    /// with the Renderer loaded by this object.
    //////////////////////////////////////////////////////////////////////
    virtual bool isCompatible ( const RendererOptions& options ) const = 0;

    //////////////////////////////////////////////////////////////////////
    /// @brief Creates a new Renderer Object.
    //////////////////////////////////////////////////////////////////////
    virtual RendererHolder load ( const std::string& name , const RendererOptions& options ) const = 0;
};

//////////////////////////////////////////////////////////////////////
/// @brief A factory that stores every Renderer Loaders.
//////////////////////////////////////////////////////////////////////
typedef ResourceLoaderFactory < RendererLoader > RendererLoaderFactory;

//////////////////////////////////////////////////////////////////////
/// @brief Renderer Manager.
//////////////////////////////////////////////////////////////////////
class DLL_PUBLIC RendererManager : public SpecializedResourceManager<Renderer, RendererLoader>
{
public:

    POOLED(Pools::Manager)

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    RendererManager( const std::string& name = "DefaultRendererManager" );

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual ~RendererManager() noexcept ( false ) ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Creates a new Renderer , following given rules.
    //////////////////////////////////////////////////////////////////////
    virtual RendererHolder load ( const std::string & name , const RendererOptions & options ) ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Call's 'Renderer::render()' for every activated Renderer's.
    /// This function will so calls the RenderPass'es registered in every
    /// Renderer's by the user.
    //////////////////////////////////////////////////////////////////////
    virtual void render () const ;
};

/// @brief Holder for RendererManager.
typedef Holder < RendererManager > RendererManagerHolder ;

GreEndNamespace
#endif
