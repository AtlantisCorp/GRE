//////////////////////////////////////////////////////////////////////
//
//  Renderer.h
//  This source file is part of Gre
//		(Gang's Resource Engine)
//
//  Copyright (c) 2015 - 2016 Luk2010
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
#include "HardwareVertexBuffer.h"
#include "HardwareIndexBuffer.h"
#include "Mesh.h"
#include "Camera.h"
#include "HardwareProgram.h"
#include "HardwareProgramManager.h"
#include "RenderContextInfo.h"
#include "RenderTarget.h"
#include "FrameBuffer.h"
#include "Viewport.h"
#include "RenderContext.h"
#include "RenderingApiDescriptor.h"
#include "Variant.h"
#include "SceneNode.h"

GreBeginNamespace

class RenderContextPrivate;
class RenderContext;
class Window;
class Renderer;
class RenderingQuery ;

typedef std::chrono::milliseconds ElapsedTime;

/// @brief Info structure for Renderer.
typedef VariantDictionnary RendererOptions ;

/// @brief Matrix Types used by the Engine.
enum class MatrixType
{
    Projection,
    ModelView
};

/// @brief Holds some data the Renderer may need during Context changing.
typedef struct
{
    
} RenderContextData;

/// @brief Experimental Enumeration for Renderer's Features.
enum class RendererFeature
{
    /// @brief Renderer auto-load default HardwareProgram for Pass objects, if those do not already
    /// have one valid. This feature ignore the 'const' of Pass objects when drawing them.
    LoadDefaultProgram ,
    
    /// @brief The Renderer can use a default Material if no Material is set in the current drawed RenderNode.
    /// This feature is needed if you want that each Node always erase previous node material.
    LoadDefaultMaterial
};

//////////////////////////////////////////////////////////////////////
/// @class RendererPrivate
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
    
    // Drawing functions 2.0
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Draw a RenderingQuery submitted by the RenderScene.
    //////////////////////////////////////////////////////////////////////
    virtual void draw ( const RenderingQuery & query ) const ;
    
    ////////////////////////////////////////////////////////////////////////
    /// @brief Prepare a new rendering process.
    ////////////////////////////////////////////////////////////////////////
    virtual void preRender ( const Color& clearcolor ) const ;
    
    ////////////////////////////////////////////////////////////////////////
    /// @brief Ends the current rendering process.
    ////////////////////////////////////////////////////////////////////////
    virtual void postRender () const ;
    
protected:
    
    ////////////////////////////////////////////////////////////////////////
    /// @brief Sets the actual Framebuffer.
    ////////////////////////////////////////////////////////////////////////
    virtual void _setFramebuffer ( const RenderFramebufferHolder& framebuffer ) const ;
    
    ////////////////////////////////////////////////////////////////////////
    /// @brief Unsets the actual framebuffer.
    ////////////////////////////////////////////////////////////////////////
    virtual void _unsetFramebuffer ( const RenderFramebufferHolder & framebuffer ) const ;
    
    ////////////////////////////////////////////////////////////////////////
    /// @brief Sets the clearing color.
    ////////////////////////////////////////////////////////////////////////
    virtual void _setClearColor ( const Color& color ) const = 0 ;
    
    ////////////////////////////////////////////////////////////////////////
    /// @brief Do some action before rendering.
    ////////////////////////////////////////////////////////////////////////
    virtual void _preRender () const = 0 ;
    
    ////////////////////////////////////////////////////////////////////////
    /// @brief Do some action after rendering.
    ////////////////////////////////////////////////////////////////////////
    virtual void _postRender () const = 0 ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Sets the current Viewport.
    //////////////////////////////////////////////////////////////////////
    virtual void _setViewport ( const Viewport& viewport ) const = 0 ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Sets the current HardwareProgram.
    //////////////////////////////////////////////////////////////////////
    virtual void setHardwareProgram ( const HardwareProgramHolder& program ) const ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Sets the current camera to the used hardware program.
    //////////////////////////////////////////////////////////////////////
    virtual void setCamera ( const CameraUser& camera , const HardwareProgramHolder& program ) const ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Sets the current camera.
    //////////////////////////////////////////////////////////////////////
    virtual void _setCamera ( const Matrix4& projection , const Matrix4& view ) const = 0 ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Sets the current node.
    //////////////////////////////////////////////////////////////////////
    virtual void setNode ( const RenderNodeHolder & node , const HardwareProgramHolder& program ) const ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Sets the current node.
    //////////////////////////////////////////////////////////////////////
    virtual void _setNode ( const RenderNodeHolder& node ) const = 0 ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Draws the Mesh using its buffers.
    //////////////////////////////////////////////////////////////////////
    virtual void _drawNodeMesh ( const MeshUser& mesh , const HardwareProgramHolder& program ) const = 0 ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief If the node has children, draws it too.
    //////////////////////////////////////////////////////////////////////
    virtual void postNode ( const RenderNodeHolder & node , const CameraUser& camera , const HardwareProgramHolder& program ) const ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Post-drawing function.
    //////////////////////////////////////////////////////////////////////
    virtual void _postNode ( const RenderNodeHolder & node , const CameraUser& camera , const HardwareProgramHolder& program ) const = 0 ;
    
public:
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Installs managers this Renderer has to the ResourceManager .
    //////////////////////////////////////////////////////////////////////
    virtual bool installManagers ( ) ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Should installs as many default features it can have.
    //////////////////////////////////////////////////////////////////////
    virtual void installDefaultFeatures () = 0 ;
    
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
    virtual HardwareProgramManagerHolder iCreateProgramManager ( ) const = 0 ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Creates a TextureManager for this Renderer.
    //////////////////////////////////////////////////////////////////////
    virtual TextureInternalCreator* iCreateTextureCreator ( ) const = 0 ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Changes 'iContext'.
    //////////////////////////////////////////////////////////////////////
    virtual void setRenderContext ( const RenderContextUser& context ) ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns 'iContext'.
    //////////////////////////////////////////////////////////////////////
    virtual const RenderContextHolder& getRenderContext () const ;
    
public:
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns true if the Renderer has successfully installed every
    /// of its managers to the ResourceManager.
    //////////////////////////////////////////////////////////////////////
    virtual bool isInstalled ( ) const ;
    
public:
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns true if the Renderer has given feature.
    //////////////////////////////////////////////////////////////////////
    virtual bool hasFeature ( const RendererFeature& feature ) const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Forces given feature to be enabled.
    //////////////////////////////////////////////////////////////////////
    virtual void setFeature ( const RendererFeature& feature ) ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Forces given feature to be disabled.
    //////////////////////////////////////////////////////////////////////
    virtual void removeFeature ( const RendererFeature& feature ) ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns every feature.
    //////////////////////////////////////////////////////////////////////
    virtual const std::vector < RendererFeature > & getFeatures () const;
    
protected:
    
    /// @brief A default program that can be used to draw RenderPass which does not have any program
    /// already set .
    HardwareProgramUser iDefaultProgram ;
    
    /// @brief The default program used to draw textured objects .
    HardwareProgramUser iDefaultTexturedProgram ;
    
    /// @brief A default material that can be used to draw a Mesh which does not have any material already set.
    MaterialUser iDefaultMaterial ;
    
    /// @brief Default Orthografic Projection Matrix.
    Matrix4 iDefaultOrthoProjectionMatrix ;
    
    /// @brief Default View Matrix.
    Matrix4 iDefaultViewMatrix ;
    
    /// @brief Default Model Matrix.
    Matrix4 iDefaultModelMatrix ;
    
    /// @brief Holds every features.
    std::vector < RendererFeature > iFeatures;
    
    /// @brief True if this renderer has installed every managers to the resource manager.
    bool iInstalled ;
    
    /// @brief True if 'installDefaultFeatures' has been called successfully.
    bool iFeaturesInstalled ;
    
    /// @brief The RenderContext this Renderer is attached to.
    RenderContextHolder iContext ;
};

/// @brief SpecializedCountedObjectHolder for RendererPrivate.
typedef SpecializedCountedObjectHolder<Renderer> RendererHolder;

/// @brief SpecializedResourceHolderList for RendererPrivate.
typedef SpecializedResourceHolderList<Renderer> RendererHolderList;

/// @brief SpecializedResourceUser.
typedef SpecializedCountedObjectUser<Renderer> RendererUser;

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
    virtual RendererUser load ( const std::string & name , const RendererOptions & options ) ;
};

/// @brief SpecializedCountedObjectHolder for RendererManager.
typedef SpecializedCountedObjectHolder < RendererManager > RendererManagerHolder ;

GreEndNamespace
#endif
