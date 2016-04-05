//
//  Renderer.h
//  GResource
//
//  Created by Jacques Tronconi on 08/11/2015.
//  Copyright (c) 2015 Atlanti's Corporation. All rights reserved.
//

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

class DLL_PUBLIC RendererResource : public Resource
{
public:
    
    POOLED(Pools::Resource)
    
    RendererResource (const std::string& name);
    
    virtual ~RendererResource ();
    
    virtual void render();
    
public:
    
    void setFramerate(float fps) { _wantedFps = fps; }
    
    float getCurrentFramerate() const { return _currentFps; }
    
    ElapsedTime getElapsedTime() const;
    void resetElapsedTime();
    
    virtual void beginRender();
    virtual void endRender();
    
    virtual void setClearColor(const Color& color) { }
    virtual void setClearDepth(float depth) { }
    
    bool isActive() const;
    void setActive(bool active);
    
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
    Scene loadSceneByName(const std::string& name, const std::string& sname);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Loads a Scene object already created using the Resource Manager
    /// system.
    /// @param scene    The scene to load.
    //////////////////////////////////////////////////////////////////////
    Scene loadSceneByResource(Scene scene);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Push the given matrix to save data.
    //////////////////////////////////////////////////////////////////////
    virtual void pushMatrix(MatrixType matrix);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Pop the given matrix to restore data.
    //////////////////////////////////////////////////////////////////////
    virtual void popMatrix(MatrixType matrix);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the Scene object.
    //////////////////////////////////////////////////////////////////////
    Scene& getScene();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the Scene object currently binded.
    //////////////////////////////////////////////////////////////////////
    const Scene& getScene() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Apply the transformation in the given node.
    /// You should always call ::pushMatrix() before applying transformations
    /// and ::popMatrix() to preserve Matrix transformations.
    //////////////////////////////////////////////////////////////////////
    virtual void transform(const Node& node);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Draw given Mesh and updates the set of HardwareProgramVariables
    /// provided.
    /// Normally, this function is called from ::drawPass().
    //////////////////////////////////////////////////////////////////////
    virtual void draw(const Mesh& mesh, HardwareProgramVariables& variables);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Draw a mesh using given Node Matrix.
    //////////////////////////////////////////////////////////////////////
    void draw(const Mesh& mesh, const Matrix4& nodeMatrix, const HardwareProgram& activProgram);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Constructs a HardwareProgram using given Vertex Shader,
    /// FragmentShader.
    /// @note You can set every value to HardwareShader::PassThrough if you
    /// just want default Program.
    //////////////////////////////////////////////////////////////////////
    virtual HardwareProgram createHardwareProgram(const std::string& name, const HardwareShader& vertexShader, const HardwareShader& fragmentShader);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Constructs a HardwareProgram using given Vertex and Fragment
    /// Shader's files.
    //////////////////////////////////////////////////////////////////////
    HardwareProgram createHardwareProgram(const std::string& name, const std::string& vertexShaderPath, const std::string& fragmentShaderPath);
    
public:
    
    virtual HardwareVertexBuffer createVertexBuffer();
    virtual HardwareIndexBuffer createIndexBuffer(PrimitiveType ptype, StorageType stype);
    virtual Mesh createMeshFromBuffers(const std::string& name, const HardwareVertexBuffer& vbuf, const HardwareIndexBufferBatch& ibufs);
    virtual Texture createTexture(const std::string& name, const std::string& file);
    virtual Camera createCamera(const std::string& name);
    
protected:
    
    virtual void _renderImmediate();
    
public:
    
    virtual void translate(float x, float y, float z) { }
    virtual void rotate(float angle, float x, float y, float z) { }
    
    virtual void drawTriangle(float sz, const Color& color1, const Color& color2, const Color& color3) { }
    virtual void drawQuad(float sz, const Color& color1, const Color& color2, const Color& color3, const Color& color4) { }
    virtual void draw(const Mesh& mesh, const HardwareProgram& activProgram);
    virtual void prepare(const Camera& cam) { }
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Prepare the Pass object to be drew.
    /// @note This function prepare every api-specific content of the Pass
    /// to be binded, for example on OpenGl for Vertex Array Objects, as they
    /// do not have any objects we must use the VariantData tables and this
    /// function should create a VAO for the Pass.
    //////////////////////////////////////////////////////////////////////
    virtual void prepare(PassPrivate* privPass);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Prepare given fbo to be drawed on.
    //////////////////////////////////////////////////////////////////////
    virtual void prepare(const FrameBuffer& fbo);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Finish use of given FrameBuffer.
    /// @note Usually this function make the Pipeline to render again to
    /// the default framebuffer, the screen.
    //////////////////////////////////////////////////////////////////////
    virtual void finish(const FrameBuffer& fbo);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Renders Framebuffer's in the Queue to the screen (default
    /// frame buffer), and clear the queue.
    //////////////////////////////////////////////////////////////////////
    virtual void renderFrameBuffers(std::queue<FrameBuffer>& fboQueue);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Creates a FrameBuffer object and returns it.
    //////////////////////////////////////////////////////////////////////
    virtual FrameBuffer createFrameBuffer(const std::string& name) const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Creates a blank Texture object and returns it.
    //////////////////////////////////////////////////////////////////////
    virtual Texture createTexture(const std::string& name) const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Creates a RenderContext using given Info.
    /// The RendererResource should keep a copy of this RenderContext, in
    /// order to destroy it.
    //////////////////////////////////////////////////////////////////////
    virtual RenderContext createRenderContext(const std::string& name, const RenderContextInfo& info, Renderer caller);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Notifiates the Renderer that the RenderContext currently
    /// binded has changed.
    ///
    /// This method is called by RenderContext::bind() and renderCtxt points
    /// to the RenderContextPrivate* structure currently binded, or this
    /// method is called by RenderContext::unbind() and renderCtxt is null.
    ///
    /// You should use this method to proceed to Context internal resource
    /// changes.
    //////////////////////////////////////////////////////////////////////
    virtual void onCurrentContextChanged(RenderContextPrivate* renderCtxt);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the currently binded RenderContext object.
    /// This can be RenderContext::Null if you are not between Renderer::_preRender()
    /// and Renderer::_postRender().
    //////////////////////////////////////////////////////////////////////
    RenderContext getCurrentContext();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Adds a new RenderTarget to the Renderer.
    /// The RenderTarget may be a Window, a Framebuffer, or any object where
    /// the Renderer should draw into.
    ///
    /// In the case of a Window object, the Renderer will bind the Window
    /// (for the RenderContext to be binded), and will draw each Pass in
    /// a separate Framebuffer which will be blended into the final Pass.
    ///
    /// To draw in a Framebuffer using a RenderTarget, the Renderer has
    /// to call RenderTarget::bindFramebuffer(). This function also has the
    /// purpose to let the RenderTarget sets its own Framebuffer, even for
    /// a Window.
    //////////////////////////////////////////////////////////////////////
    void addRenderTarget(const RenderTarget& renderTarget);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Specifically adds a Window as a RenderTarget.
    ///
    /// @param caller
    /// This param is filled by the user who call this function. This permit
    /// to set a Renderer parent for the RenderContext object.
    ///
    /// @param autoCreateContext
    ///  If the Window does not already have a setted RenderContext, the
    /// Renderer automatically creates a RenderContext and set it to this
    /// Window object. The parameters of this RenderContext are unbehavioured,
    /// but generally this should be the best one.
    //////////////////////////////////////////////////////////////////////
    void addRenderTarget(Window& window, Renderer caller, bool autoCreateContext = true);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Select the default Scene to draw a RenderTarget object if this
    /// one does not already have one.
    //////////////////////////////////////////////////////////////////////
    void selectDefaultScene(const Scene& defScene);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the Default Scene set by ::selectDefaultScene().
    //////////////////////////////////////////////////////////////////////
    Scene getDefaultScene();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Render given RenderTarget.
    /// @note If this RenderTarget implies a Context change, you should have
    /// called RenderTarget::bind() before this call.
    /// If this does not imply a Context change, RenderTarget::bindFramebuffer()
    /// is always called in this function.
    //////////////////////////////////////////////////////////////////////
    void render(RenderTarget& rtarget);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Select the Scene that will be used during rendering.
    /// Normally this is set by ::render(). The Scene is either the Scene
    /// selected in the RenderTarget either the default Scene.
    //////////////////////////////////////////////////////////////////////
    void selectScene(const Scene& rhs);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Saves the Current Scene object into a private std::deque.
    //////////////////////////////////////////////////////////////////////
    void pushCurrentScene();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Restores the Current Scene from previously saved ones.
    /// If no Scene can be restore, this function has no effect.
    //////////////////////////////////////////////////////////////////////
    void popCurrentScene();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Launches the Drawing Loop Thread.
    /// This means that the Renderer begins to draw every RenderTarget it
    /// has to, and update to every RenderContext.
    //////////////////////////////////////////////////////////////////////
    void launchDrawingThread();
    
protected:
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the RenderContext object that points to the given
    /// RenderContextPrivate object.
    //////////////////////////////////////////////////////////////////////
    virtual RenderContext findRenderContextFromPrivate(RenderContextPrivate* renderCtxtPriv) = 0;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Initializes the RenderContextData structure.
    /// @note As of today, this function has no use.
    //////////////////////////////////////////////////////////////////////
    virtual void initializeRenderContextDataPrivate(RenderContextData& renderCtxtData);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief The inifinite threaded drawing loop.
    //////////////////////////////////////////////////////////////////////
    void threadedLoop();
    
protected:
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Renders the Queue of Framebuffer objects to the RenderContext
    /// surface, depending on the Viewport currently active.
    //////////////////////////////////////////////////////////////////////
    virtual void _renderFramebuffers(std::queue<FrameBuffer> &fboQueue);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Sets the current Viewport active.
    //////////////////////////////////////////////////////////////////////
    virtual void _setCurrentViewport(const Viewport& viewport);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Pre-render the Current Scene.
    /// This means in this function, you can do whatever you want BEFORE
    /// rendering the Scene. Default behaviour does nothing.
    //////////////////////////////////////////////////////////////////////
    virtual void _preRenderCurrentScene();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Render the Currently active Scene.
    //////////////////////////////////////////////////////////////////////
    virtual void _renderCurrentScene();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Post-render the Current Scene.
    /// Here you can do whatever you want AFTER rendering the Scene. 
    //////////////////////////////////////////////////////////////////////
    virtual void _postRenderCurrentScene();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Render a Viewport object.
    /// Here the Renderer bind the selected Scene, then render the Scene
    /// in the Viewport.
    //////////////////////////////////////////////////////////////////////
    virtual void _renderViewport(const Viewport& viewport);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Render a RenderTarget, but using the Current Context.
    /// This is the process for every RenderTarget.
    //////////////////////////////////////////////////////////////////////
    virtual void _renderRenderTargetWithCurrentContext(RenderTarget& renderTarget);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Render a RenderTarget which has a RenderContext object.
    /// This function only ensure binding and unbinding of the context,
    /// then calls ::_renderRenderTargetWithCurrentContext().
    //////////////////////////////////////////////////////////////////////
    virtual void _renderRenderTargetWithContext(RenderTarget& renderTarget);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Simply sets if the Renderer is active or not.
    //////////////////////////////////////////////////////////////////////
    virtual void _setActive(bool activ);
    
protected:

    float  _wantedFps; ///< @brief Hold the desired framerate.
    float  _currentFps; ///< @brief Hold the current fps.
    bool   _mIsActive;
    bool   _mIsImmediateMode; ///< @brief True if renderer is in immediate mode.
    std::vector<std::function<void(void)> > _mImmediateFunctions; ///< @brief Vector of functions to be called in immediate mode.
    HardwareProgramManager _mProgramManager; ///< @brief The Shader Program Manager.
    
    /// @brief FBO data : Holds a Mesh to draw FrameBuffers on screen.
    Mesh _mPlaneFboMesh;
    
    /// @brief FBO data : Holds the shader to draw the FrameBuffers on screen.
    HardwareProgram _mfboHdwProgram;
    
    /// @brief List of RenderTarget's objects.
    std::vector<RenderTarget> _mRenderTargets;
    
    /// @brief The default Scene used by this Renderer to draw on RenderTarget.
    Scene _mDefaultScene;
    
    /// @brief Current Scene used to render.
    /// You can modify this Scene using Renderer::selectScene() but this is used
    /// also by the RenderTarget. You should set a Scene to a RenderTarget using
    /// RenderTarget::selectScene().
    Scene _mCurrentScene;
    
    /// @brief Holds the Scene objects saved by push/pop method.
    /// This is used currently to save the Context's Scene when changing to a
    /// Viewport's Scene. it follows the pattern first-in/last-out.
    std::deque<Scene> _mSavedScenes;
    
    /// @brief Current Context Binded.
    /// This RenderContext can be RenderContext::Null (in this case nothing is rendered
    /// to the screen). RenderContext::bind() should notifiate the Renderer with
    /// Renderer::onCurrentContextChanged(this) in order to modify this value.
    RenderContext* _mCurrentContext;
    
    /// @brief Holds RenderContextData stored by RenderContext objects.
    std::map<RenderContext*, RenderContextData> _mContextDatas;
    
    /// @brief Holds the Current Viewport.
    /// This value is correct in the ::render() function.
    Viewport _mCurrentViewport;
    
    /// @brief Holds the Drawing Loop Thread.
    /// The Drawing Loop Thread has in charge the infinite drawing of all the Rendering Targets.
    std::thread _mDrawThread;
    
    /// @brief A special bool that must be set to true when terminating the draw thread.
    bool _mMustStopThread;
    
private:
    
    int _sum_frames;
    float _mMaxFps;
    float _mMinFps;
    std::chrono::high_resolution_clock::time_point _mClockReset;
    std::chrono::high_resolution_clock::time_point _mClockAtPreviousFrameEnd;
    std::chrono::high_resolution_clock::duration   _mClockElapsedSinceReset;
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
    Scene loadSceneByName(const std::string& name, const std::string& sname);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Loads a Scene object already created using the Resource Manager
    /// system.
    /// @param scene    The scene to load.
    //////////////////////////////////////////////////////////////////////
    Scene loadSceneByResource(Scene scene);
    
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
    Scene& getScene();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the Scene object.
    //////////////////////////////////////////////////////////////////////
    const Scene& getScene() const;
    
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
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Prepare given fbo to be drawed on.
    //////////////////////////////////////////////////////////////////////
    void prepare(const FrameBuffer& fbo);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Finish use of given FrameBuffer.
    /// @note Usually this function make the Pipeline to render again to
    /// the default framebuffer, the screen.
    //////////////////////////////////////////////////////////////////////
    void finish(const FrameBuffer& fbo);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Renders Framebuffer's in the Queue to the screen (default
    /// frame buffer), and clear the queue.
    //////////////////////////////////////////////////////////////////////
    void renderFrameBuffers(std::queue<FrameBuffer>& fboQueue);
    
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
    /// @brief Creates a FrameBuffer object and returns it.
    //////////////////////////////////////////////////////////////////////
    FrameBuffer createFrameBuffer(const std::string& name) const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Creates a blank Texture object and returns it.
    //////////////////////////////////////////////////////////////////////
    Texture createTexture(const std::string& name) const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Creates a RenderContext using given Info.
    /// The RendererResource should keep a copy of this RenderContext, in
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
    void selectDefaultScene(const Scene& defScene);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the Default Scene set by ::selectDefaultScene().
    //////////////////////////////////////////////////////////////////////
    Scene getDefaultScene();
    
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
    
    std::weak_ptr<RendererResource> _mRenderer;
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
