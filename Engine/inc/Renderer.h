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
#include "Color.h"
#include "HardwareVertexBuffer.h"
#include "HardwareIndexBuffer.h"
#include "Mesh.h"
#include "Camera.h"
#include "Scene.h"
#include "HardwareProgram.h"
#include "HardwareProgramManager.h"
#include "RenderContextInfo.h"
#include "RenderTarget.h"
#include "FrameBuffer.h"
#include "Viewport.h"
#include "RenderContext.h"
#include "RenderingApiDescriptor.h"

GreBeginNamespace

class RenderContextPrivate;
class RenderContext;
class Window;
class Renderer;

typedef std::chrono::milliseconds ElapsedTime;

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

//////////////////////////////////////////////////////////////////////
/// @class RendererPrivate
/// @brief Base class for Renderer Objects.
///
/// The Renderer is the object responsible for, basically, drawing the
/// Scene to one or more RenderTarget.
///
/// Basically, the job of the Renderer is :
///   - Get the RenderScene to draw for a RenderTarget.
///   - Get the Pass's objects from the RenderScene.
///   - Drawing every Pass with each Pass settings.
///   - Mixing the result from those Pass.
///   - Drawing the result to the RenderTarget.
///
/// The RenderTarget can be a texture, a window, anything.
///
/// The Renderer is called by RenderContext. In fact, each RenderTarget
/// is updated by the ResourceManager. For each RenderTarget, the associated
/// Renderer will be called to draw those RenderTarget with Renderer::drawRenderTarget().
/// The RenderTarget then call Renderer::drawScene().
/// The Scene call Renderer::drawPasses().
/// The Pass call Renderer::drawObjects().
/// The objects call Renderer::drawBuffer().
///
/// Objects should be divided in four categories :
///    - Static Objects
///        - Non-transparent ( = non-sorted )
///        - Transparent ( = sorted or need sorting )
///    - Dynamic Objects
///        - Non-transparent
///        - Transparent ( = need sorting )
///
/// 21/06/2016 : The choice has been made to let the ResourceManager
/// updates the RenderTarget objects. Those will have an associated
/// Renderer. The Renderer objects should not be updated and should be
/// only used to draw in real time to the RenderTarget.
///              Also, the Renderer now do not have the meaning of time.
/// Now you have to use a Listener to count ElapsedTime.
//////////////////////////////////////////////////////////////////////
class DLL_PUBLIC RendererPrivate : public Resource
{
public:
    
    POOLED(Pools::Resource)
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    RendererPrivate (const std::string& name);
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual ~RendererPrivate () noexcept(false);
    
public:
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Draw the given RenderTarget.
    //////////////////////////////////////////////////////////////////////
    virtual void drawRenderTarget(const RenderTarget& rendertarget);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Draw the given RenderScene.
    /// @note A RenderTarget should already be binded.
    //////////////////////////////////////////////////////////////////////
    virtual void drawRenderScene(const RenderScene& scenemanager);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Draw a list of Pass to given list of Framebuffers.
    /// @note A RenderTarget should be binded.
    //////////////////////////////////////////////////////////////////////
    virtual void drawPassList(const PassHolderList& passlist, RenderFramebufferHolderList& fbolist);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Draw given Pass using associated Framebuffer.
    //////////////////////////////////////////////////////////////////////
    virtual void drawPassWithFramebuffer(const PassHolder& passholder, RenderFramebufferHolder& fboholder);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Draws the List of RenderNode using given HardwareProgram.
    //////////////////////////////////////////////////////////////////////
    virtual void drawRenderNodeList(RenderNodeHolderList& nodes, HardwareProgramHolder& program);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Draw a list of RenderFramebuffer onto the binded RenderTarget.
    /// Those RenderFramebuffer's should be blended together.
    //////////////////////////////////////////////////////////////////////
    virtual void drawFramebufferList(RenderFramebufferHolderList& fbolist);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Draw one Mesh to the binded Framebuffer using given
    /// HardwareProgram.
    //////////////////////////////////////////////////////////////////////
    virtual void drawSimpleMesh ( Mesh& mesh , HardwareProgram& prog );
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Draw given Vertex Buffer using the given HardwareProgram.
    //////////////////////////////////////////////////////////////////////
    virtual void drawVertexBuffer(const HardwareVertexBuffer& vbuf, const HardwareProgramHolder& program);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Calls the API - dependent 'drawVertexBuffer' stuff.
    //////////////////////////////////////////////////////////////////////
    virtual void drawVertexBufferPrivate(const HardwareVertexBuffer& vbuf, const HardwareProgramHolder& program) = 0;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Calls the API - dependent 'drawIndexBuffer' stuff.
    //////////////////////////////////////////////////////////////////////
    virtual void drawIndexBufferPrivate(const HardwareIndexBuffer& ibuf, const HardwareVertexBuffer& vbuf, const HardwareProgramHolder& program) = 0;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Draw given Vertex Buffer using Index Buffer and Hardware
    /// Program.
    //////////////////////////////////////////////////////////////////////
    virtual void drawIndexBuffer(const HardwareIndexBuffer& ibuf, const HardwareVertexBuffer& vbuf, const HardwareProgramHolder& program);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Sets the buffer's base color when clearing it.
    /// @sa glSetClearColor()
    //////////////////////////////////////////////////////////////////////
    virtual void setClearColor(const Color& color) = 0;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Sets the buffer's base depth when clearing it.
    /// @sa glSetClearDepth()
    //////////////////////////////////////////////////////////////////////
    virtual void setClearDepth(float depth) = 0;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the HardwareProgramManager for this Renderer.
    //////////////////////////////////////////////////////////////////////
    virtual HardwareProgramManager getHardwareProgramManager();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the HardwareProgramManager for this Renderer.
    //////////////////////////////////////////////////////////////////////
    virtual const HardwareProgramManager getHardwareProgramManager() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Constructs a HardwareProgram using the HardwareProgramManager
    /// provided for this Renderer. If the vertexShader and fragmentShader
    /// parameters are null, a default Shader will be used.
    //////////////////////////////////////////////////////////////////////
    virtual HardwareProgram createHardwareProgram(const std::string& name, const HardwareShader& vertexShader, const HardwareShader& fragmentShader);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Constructs a HardwareProgram using given Vertex and Fragment
    /// Shader's files.
    //////////////////////////////////////////////////////////////////////
    virtual HardwareProgram createHardwareProgram(const std::string& name, const std::string& vertexShaderPath, const std::string& fragmentShaderPath);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Creates an empty HardwareVertexBuffer configured to run on
    /// user GPU.
    //////////////////////////////////////////////////////////////////////
    virtual HardwareVertexBufferHolder createVertexBuffer();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Creates an empty HardwareIndexBuffer configured to run on
    /// user GPU.
    //////////////////////////////////////////////////////////////////////
    virtual HardwareIndexBufferHolder createIndexBuffer();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Loads a Mesh to GPU memory, organize its Vertex Buffers and
    /// Index Buffers.
    //////////////////////////////////////////////////////////////////////
    virtual void loadMesh(Mesh& mesh, bool deleteCache);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Creates a new Texture object, with given name and null
    /// data.
    //////////////////////////////////////////////////////////////////////
    virtual TextureHolder createTexture(const std::string& name);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Creates a Texture object directly drawable or bindable.
    //////////////////////////////////////////////////////////////////////
    virtual TextureHolder createTexture(const std::string& name, const std::string& file);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Creates an empty Texture and return its holder.
    //////////////////////////////////////////////////////////////////////
    virtual TextureHolder createEmptyTexture(int width, int height);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the TextureManager.
    //////////////////////////////////////////////////////////////////////
    virtual TextureManager& getTextureManager();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the TextureManager.
    //////////////////////////////////////////////////////////////////////
    virtual const TextureManager& getTextureManager() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the 'sz' numbers of Framebuffers available. If the
    /// expected number is not available, it creates as Framebuffers as
    /// necessary.
    //////////////////////////////////////////////////////////////////////
    virtual RenderFramebufferHolderList getFramebuffers(int sz);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Creates a Framebuffer object and returns it.
    //////////////////////////////////////////////////////////////////////
    virtual RenderFramebufferHolder createFramebuffer() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Creates a RenderContext using given Info.
    //////////////////////////////////////////////////////////////////////
    virtual RenderContextHolder createRenderContext ( const std::string& name , const RenderContextInfo& info );
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Creates a HardwareVertexBuffer of given size, or
    /// 'HardwareVertexBufferHolder(nullptr)' if invalid.
    //////////////////////////////////////////////////////////////////////
    virtual HardwareVertexBufferHolder createVertexBufferWithSize ( const std::string& name , size_t sz ) const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Clears every RenderContext in this Renderer.
    //////////////////////////////////////////////////////////////////////
    virtual void clearRenderContexts();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Adds a RenderTarget for rendering.
    //////////////////////////////////////////////////////////////////////
    virtual void registerTarget ( const RenderTargetHolder& holder );
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Launch the Drawing Thread for this Renderer.
    /// The Drawing Thread draws every registered RenderTarget.
    /// Considering if the drew RenderTarget has a specific RenderContext,
    /// this RenderTarget can be rendered using a specific thread. Every
    /// RenderTarget that do not have a specific thread ( drawing in
    /// global context ) are drew in the launch thread loop.
    ///
    /// @note At the end of the loop, every launched thread are joined before
    /// entering a new loop.
    ///
    //////////////////////////////////////////////////////////////////////
    virtual void launch();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the 'iLaunchMustStop' boolean.
    //////////////////////////////////////////////////////////////////////
    virtual bool getLaunchMustStop() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns 'iTargets' property.
    //////////////////////////////////////////////////////////////////////
    virtual RenderTargetHolderList& getRegisteredTargets();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns 'iTargets' property.
    //////////////////////////////////////////////////////////////////////
    virtual const RenderTargetHolderList& getRegisteredTargets() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Attempts to join the launch thread.
    //////////////////////////////////////////////////////////////////////
    virtual void stop();
    
protected:
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Internal method to create a Vertex buffer.
    //////////////////////////////////////////////////////////////////////
    virtual HardwareVertexBufferHolder iCreateVertexBuffer ( const std::string& name , size_t sz ) const = 0;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Creates a Texture Object with given name, initialize it
    /// using API calls. ( like glGenTextures )
    //////////////////////////////////////////////////////////////////////
    virtual TextureHolder iCreateTexturePrivate ( const std::string& name ) const = 0;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Creates a RenderContext object.
    //////////////////////////////////////////////////////////////////////
    virtual RenderContextHolder iCreateRenderContext ( const std::string& name , const RenderContextInfo& info ) const = 0;
    
protected:
    
    /// @brief The Last binded / used RenderContext.
    RenderContextHolder iLastRenderContext;
    
    /// @brief A list of RenderContextHolder in order to manage them.
    RenderContextHolderList iRenderContexts;
    
    /// @brief A structure representing a FrameContext.
    typedef struct _framecontext {
        
        /// @brief If not-null, will be used as the final Framebuffer to render the Scene.
        RenderFramebufferHolder SpecialFramebuffer;
        
        /// @brief The Scene we have to render.
        RenderSceneHolder RenderedScene;
        
        /// @brief The Viewport used to draw the Scene. Available from the call to ::drawRenderScene().
        Viewport RenderedViewport;
        
        /// @brief Framebuffers used to draw Pass objects.
        RenderFramebufferHolderList Framebuffers;
        
        /// @brief Constructs the FrameContext.
        _framecontext() : SpecialFramebuffer(nullptr), RenderedScene(nullptr) { }
        
    } FrameContext;
    
    /// @brief Holds per-frame useful informations. This member is valid only within the drawing.
    FrameContext iFrameContext;
    
    /// @brief The Hardware Program Manager for this Renderer.
    HardwareProgramManagerHolder iProgramManager;
    
    /// @brief The Mesh Manager.
    MeshManager iMeshManager;
    
    /// @brief The Texture Manager.
    TextureManager iTextureManager;
    
    /// @brief Holds the RenderTarget this Renderer should draw.
    RenderTargetHolderList iTargets;
    
    /// @brief 'true' if the launch loop should stop.
    bool iLaunchMustStop;
    
    /// @brief The Launch thread.
    std::thread iLaunchThread;
};

/// @brief SpecializedResourceHolder for RendererPrivate.
typedef SpecializedResourceHolder<RendererPrivate> RendererHolder;

/// @brief SpecializedResourceHolderList for RendererPrivate.
typedef SpecializedResourceHolderList<RendererPrivate> RendererHolderList;

//////////////////////////////////////////////////////////////////////
/// @brief SpecializedResourceUser for Renderer.
//////////////////////////////////////////////////////////////////////
class DLL_PUBLIC Renderer : public SpecializedResourceUser<RendererPrivate>
{
public:
    
    POOLED(Pools::Resource)
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    Renderer(const RendererPrivate* pointer);
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    Renderer(const RendererHolder& holder);
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    Renderer(const Renderer& user);
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual ~Renderer();
    
    /// @brief Holds the Null Renderer User.
    static Renderer Null;
};

//////////////////////////////////////////////////////////////////////
/// @brief ResourceLoader for Renderer.
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
    virtual bool isCompatible ( const RenderingApiDescriptor& apidescriptor ) const = 0;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Creates a new Renderer Object.
    //////////////////////////////////////////////////////////////////////
    virtual RendererHolder load ( const std::string& name ) const = 0;
};

//////////////////////////////////////////////////////////////////////
/// @brief A factory that stores every Renderer Loaders.
//////////////////////////////////////////////////////////////////////
typedef ResourceLoaderFactory<RendererLoader> RendererLoaderFactory;

//////////////////////////////////////////////////////////////////////
/// @brief Renderer Manager.
//////////////////////////////////////////////////////////////////////
class DLL_PUBLIC RendererManager
{
public:
    
    POOLED(Pools::Manager)
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    RendererManager();
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual ~RendererManager();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Loads a Renderer into the Manager.
    //////////////////////////////////////////////////////////////////////
    virtual Renderer load ( const RendererHolder& renderer );
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Loads a Renderer into the Manager giving its name.
    //////////////////////////////////////////////////////////////////////
    virtual Renderer load ( const std::string& name );
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Loads a Renderer into the Manager given its name and API
    /// name.
    //////////////////////////////////////////////////////////////////////
    virtual Renderer load ( const std::string& name , const RenderingApiDescriptor& apidescriptor );
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the Renderer by its name.
    //////////////////////////////////////////////////////////////////////
    virtual Renderer get ( const std::string& name );
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the Renderer by its name.
    //////////////////////////////////////////////////////////////////////
    virtual const Renderer get ( const std::string& name ) const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns every Renderer loaded.
    //////////////////////////////////////////////////////////////////////
    virtual const RendererHolderList& getRenderers() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Removes given Renderer from this Manager.
    //////////////////////////////////////////////////////////////////////
    virtual void remove ( const std::string& name );
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Removes every Renderer.
    //////////////////////////////////////////////////////////////////////
    virtual void clearRenderers();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the RendererLoaderFactory.
    //////////////////////////////////////////////////////////////////////
    virtual RendererLoaderFactory& getRendererLoaderFactory();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the RendererLoaderFactory.
    //////////////////////////////////////////////////////////////////////
    virtual const RendererLoaderFactory& getRendererLoaderFactory() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Clears the Renderers and the Factory.
    //////////////////////////////////////////////////////////////////////
    virtual void clear();
    
protected:
    
    /// @brief Renderers loaded.
    RendererHolderList iRenderers;
    
    /// @brief RendererLoaderFactory.
    RendererLoaderFactory iFactory;
};

GreEndNamespace
#endif
