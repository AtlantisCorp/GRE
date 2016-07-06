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
///   - Get the SceneManager to draw for a RenderTarget.
///   - Get the Pass's objects from the SceneManager.
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
    virtual ~RendererPrivate ();
    
public:
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Draw the given RenderTarget.
    //////////////////////////////////////////////////////////////////////
    virtual void drawRenderTarget(const RenderTarget& rendertarget);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Draw the given SceneManager.
    /// @note A RenderTarget should already be binded.
    //////////////////////////////////////////////////////////////////////
    virtual void drawSceneManager(const SceneManager& scenemanager);
    
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
    /// @brief Draws the List of SceneNode using given HardwareProgram.
    //////////////////////////////////////////////////////////////////////
    virtual void drawSceneNodeList(SceneNodeList& nodes, const HardwareProgramHolder& program);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Draw a list of RenderFramebuffer onto the binded RenderTarget.
    /// Those RenderFramebuffer's should be blended together.
    //////////////////////////////////////////////////////////////////////
    virtual void drawFramebufferList(RenderFramebufferHolderList& fbolist);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Sets the buffer's base color when clearing it.
    /// @sa glSetClearColor()
    //////////////////////////////////////////////////////////////////////
    virtual void setClearColor(const Color& color);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Sets the buffer's base depth when clearing it.
    /// @sa glSetClearDepth()
    //////////////////////////////////////////////////////////////////////
    virtual void setClearDepth(float depth);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the HardwareProgramManager for this Renderer.
    //////////////////////////////////////////////////////////////////////
    virtual HardwareProgramManager& getHardwareProgramManager();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the HardwareProgramManager for this Renderer.
    //////////////////////////////////////////////////////////////////////
    virtual const HardwareProgramManager& getHardwareProgramManager() const;
    
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
    /// @brief Creates a VertexBuffer using the HardwareProgramManager.
    //////////////////////////////////////////////////////////////////////
    virtual HardwareVertexBuffer createVertexBuffer();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Creates an IndexBuffer using the HardwareProgramManager.
    //////////////////////////////////////////////////////////////////////
    virtual HardwareIndexBuffer createIndexBuffer(PrimitiveType ptype, StorageType stype);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Loads a Mesh to GPU memory, organize its Vertex Buffers and
    /// Index Buffers.
    //////////////////////////////////////////////////////////////////////
    virtual void loadMesh(Mesh& mesh);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Creates an empty Texture and return its holder.
    //////////////////////////////////////////////////////////////////////
    virtual TextureHolder createEmptyTexture(int width, int height);
    
    virtual Texture createTexture(const std::string& name, const std::string& file);
    
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
    virtual RenderContextHolder createRenderContext(const std::string& name, const RenderContextInfo& info, Renderer caller);
    
protected:
    
    /// @brief The Last binded / used RenderContext.
    RenderContextHolder iLastRenderContext;
    
    /// @brief A structure representing a FrameContext.
    typedef struct _framecontext {
        
        /// @brief If not-null, will be used as the final Framebuffer to render the Scene.
        RenderFramebufferHolder SpecialFramebuffer;
        
        /// @brief The Scene we have to render.
        SceneManagerHolder RenderedScene;
        
        /// @brief The Viewport used to draw the Scene. Available from the call to ::drawSceneManager().
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
};

class DLL_PUBLIC Renderer : public ResourceUser
{
public:
    
    Renderer ();
    Renderer (Renderer&& movref);
    Renderer (const Renderer& renderer);
    explicit Renderer (const ResourceUser& ruser);
    
    virtual ~Renderer();
    
    Renderer& operator = (const ResourceUser& ruser);
    Renderer& operator = (const Renderer& ruser);
    
    void render();
    
    void setFramerate (float fps);
    
    float getCurrentFramerate () const;
    ElapsedTime getElapsedTime() const;
    void resetElapsedTime();
    
    void beginRender();
    void endRender();
    
    void setClearColor(const Color& color);
    void setClearDepth(float depth);
    
    void setActive(bool active);
    bool isActive() const;
    
    bool isImmediate() const;
    void setImmediateMode(bool mode);
    
    void addImmediateAction(std::function<void(void)> action);
    void resetImmediateActions();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Loads a Scene object, loading it using the Resource Manager
    /// system.
    /// @param name     Name of the loader to use to create the Scene Object.
    /// @param sname    Name of the Scene that will be loaded.
    //////////////////////////////////////////////////////////////////////
    SceneManager loadSceneByName(const std::string& name, const std::string& sname);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Loads a Scene object already created using the Resource Manager
    /// system.
    /// @param scene    The scene to load.
    //////////////////////////////////////////////////////////////////////
    SceneManager loadSceneByResource(SceneManager scene);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Push the given matrix to save data.
    //////////////////////////////////////////////////////////////////////
    void pushMatrix(MatrixType matrix);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Pop the given matrix to restore data.
    //////////////////////////////////////////////////////////////////////
    void popMatrix(MatrixType matrix);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the Scene object.
    //////////////////////////////////////////////////////////////////////
    SceneManager& getScene();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the Scene object.
    //////////////////////////////////////////////////////////////////////
    const SceneManager& getScene() const;
    
    void translate(float x, float y, float z);
    void rotate(float angle, float x, float y, float z);
    
    void drawTriangle(float sz, const Color& color1 = Color::White, const Color& color2 = Color::White, const Color& color3 = Color::White);
    void drawQuad(float sz, const Color& color1 = Color::White, const Color& color2 = Color::White, const Color& color3 = Color::White, const Color& color4 = Color::White);
    void draw(const Mesh& mesh, const HardwareProgram& activProgram);
    void prepare(const Camera& cam);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Prepare the Pass object to be drew.
    //////////////////////////////////////////////////////////////////////
    void prepare(PassPrivate* privPass);
    
    HardwareVertexBuffer createVertexBuffer();
    HardwareIndexBuffer createIndexBuffer(PrimitiveType ptype, StorageType stype);
    Mesh createMeshFromBuffers(const std::string& name, const HardwareVertexBuffer& vbuf, const HardwareIndexBufferBatch& ibufs);
    Texture createTexture(const std::string& name, const std::string& file);
    Camera createCamera(const std::string& name);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Constructs a HardwareProgram using given Vertex Shader,
    /// IndexShader.
    /// @note You can set every value to HardwareShader::PassThrough if you
    /// just want default Program.
    //////////////////////////////////////////////////////////////////////
    HardwareProgram createHardwareProgram(const std::string& name, const HardwareShader& vertexShader, const HardwareShader& fragmentShader);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Constructs a HardwareProgram using given Vertex and Fragment
    /// Shader's files.
    //////////////////////////////////////////////////////////////////////
    HardwareProgram createHardwareProgram(const std::string& name, const std::string& vertexShaderPath, const std::string& fragmentShaderPath);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Apply the transformation in the given node.
    //////////////////////////////////////////////////////////////////////
    void transform(const Node& node);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Draw given Mesh and updates the set of HardwareProgramVariables
    /// provided.
    /// Normally, this function is called from ::drawPass().
    //////////////////////////////////////////////////////////////////////
    void draw(const Mesh& mesh, HardwareProgramVariables& variables);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Creates a blank Texture object and returns it.
    //////////////////////////////////////////////////////////////////////
    Texture createTexture(const std::string& name) const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Creates a RenderContext using given Info.
    /// The RendererPrivate should keep a copy of this RenderContext, in
    /// order to destroy it.
    //////////////////////////////////////////////////////////////////////
    RenderContext createRenderContext(const std::string& name, const RenderContextInfo& info);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Notifiates the Renderer that the RenderContext currently
    /// binded has changed.
    //////////////////////////////////////////////////////////////////////
    void onCurrentContextChanged(RenderContextPrivate* renderCtxt);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Adds a new RenderTarget to the Renderer.
    /// The RenderTarget may be a Window, a Framebuffer, or any object where
    /// the Renderer should draw into.
    //////////////////////////////////////////////////////////////////////
    void addRenderTarget(const RenderTarget& renderTarget);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Specifically adds a Window as a RenderTarget.
    //////////////////////////////////////////////////////////////////////
    void addRenderTarget(Window& window, bool autoCreateContext = true);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Select the default Scene to draw a RenderTarget object if this
    /// one does not already have one.
    //////////////////////////////////////////////////////////////////////
    void selectDefaultScene(const SceneManager& defScene);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the Default Scene set by ::selectDefaultScene().
    //////////////////////////////////////////////////////////////////////
    SceneManager getDefaultScene();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Render given RenderTarget.
    //////////////////////////////////////////////////////////////////////
    void render(RenderTarget& rtarget);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Launches the Drawing Loop Thread.
    //////////////////////////////////////////////////////////////////////
    void launchDrawingThread();
    
    /// @brief Holds the Null Renderer User.
    static Renderer Null;
    
private:
    
    std::weak_ptr<RendererPrivate> _mRenderer;
};

class DLL_PUBLIC RendererLoader : public ResourceLoader
{
public:
    
    POOLED(Pools::Loader)
    
    RendererLoader();
    virtual ~RendererLoader();
    
    virtual bool isTypeSupported (Resource::Type type) const;
    virtual Resource* load (Resource::Type type, const std::string& name) const;
    virtual ResourceLoader* clone() const;
    
protected:
    
    
};

//////////////////////////////////////////////////////////////////////
/// @brief A factory that stores every Renderer Loaders.
//////////////////////////////////////////////////////////////////////
class DLL_PUBLIC RendererLoaderFactory : public ResourceLoaderFactory<RendererLoader>
{
public:
    
    RendererLoaderFactory();
    ~RendererLoaderFactory();
};

typedef GreException                          RendererInvalidVersion;
typedef GreException                          RendererInvalidApi;
typedef GreExceptionWithText                  RendererNoProgramManagerException;
typedef GreExceptionWithText                  RendererNoCapacityException;

GreEndNamespace
#endif
