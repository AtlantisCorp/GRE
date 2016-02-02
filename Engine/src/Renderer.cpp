//
//  Renderer.cpp
//  GResource
//
//  Created by Jacques Tronconi on 08/11/2015.
//  Copyright (c) 2015 Atlanti's Corporation. All rights reserved.
//

#include "Renderer.h"
#include "ResourceManager.h"
#include "Pass.h"

GRE_BEGIN_NAMESPACE

typedef std::chrono::high_resolution_clock Clock;

RendererResource::RendererResource (const std::string& name)
: Resource(name), Transmitter(name)
{
    _sum_frames = 0;
    _wantedFps  = 0.0f;
    _currentFps = 0.0f;
    _mIsActive = false;
    _mIsImmediateMode = false;
    _mMaxFps = 0.0f;
    _mMinFps = 0.0f;
    
    _mClockReset              = Clock::now();
    _mClockAtPreviousFrameEnd = Clock::now();
    _mClockElapsedSinceReset  = Clock::duration::zero();
}

void RendererResource::beginRender()
{
    
}

void RendererResource::endRender()
{
    // Calculate std::chrono
    
    _sum_frames++;
    _mClockElapsedSinceReset = std::chrono::duration_cast<std::chrono::milliseconds>(Clock::now() - _mClockReset);
    auto timeElapsedSinceLastRefresh = std::chrono::duration_cast<std::chrono::milliseconds>(Clock::now() - _mClockAtPreviousFrameEnd);
    
    if(timeElapsedSinceLastRefresh >= std::chrono::milliseconds(1000))
    {
        _currentFps = (float)_sum_frames / (((float)timeElapsedSinceLastRefresh.count()) / 1000.0f);
        _mClockAtPreviousFrameEnd = Clock::now();
        _sum_frames = 0;
        
        if(_currentFps > _mMaxFps || _mMaxFps <= 0.2f)
            _mMaxFps = _currentFps;
        if(_currentFps < _mMinFps || _mMinFps <= 0.2f)
            _mMinFps = _currentFps;
    }
    
}

ElapsedTime RendererResource::getElapsedTime() const
{
    return std::chrono::duration_cast<ElapsedTime>(_mClockElapsedSinceReset);
}

void RendererResource::resetElapsedTime()
{
    _mClockReset             = Clock::now();
    _mClockElapsedSinceReset = Clock::duration::zero();
    _mMaxFps                 = 0.0f;
    _mMinFps                 = 0.0f;
    _currentFps              = 0.0f;
    _sum_frames              = 0;
}

void RendererResource::setActive(bool active)
{
    _mIsActive = active;
}

bool RendererResource::isActive() const
{
    return _mIsActive;
}

void RendererResource::render()
{
    if(isActive())
    {
        _preRender();
        
        if(!isImmediate()) {
            _render();
        } else {
            _renderImmediate();
        }

        _postRender();
    }
}

void RendererResource::_preRender()
{
    GreDebugFunctionNotImplemented();
}

void RendererResource::_render()
{
    // ScenePrivate::getActivPasses should returns the
    // list of Pass objects, currently activated, and by
    // order of draw.
    PassList passes = _mScene.getActivePasses();
    
    // This Queue contains the FrameBuffer objects that will have to
    // be blended at the end of the process.
    std::queue<FrameBuffer> framebufQueue;
    
    // For each Pass object, we use the PassPrivate::renderWith()
    // function. Each Pass object has the possibility to customize
    // the Render process as they want.
    for(Pass pass : passes)
    {
        // Each Pass should render to a FrameBuffer object.
        // This object is added to the list of FrameBuffer to blend
        // at the end of the process, in the order first-in/first-out.
        
        // Note : If the Pass doesn't have any FrameBuffer, we draw
        // the FrameBuffers in memory and then we draw this pass.
        
        FrameBuffer& fbo = pass.getFrameBuffer();
        if(!pass.isFrameBufferedRendering())
        {
            // We render every Framebuffers in order to draw the Pass directly
            // on it.
            renderFrameBuffers(framebufQueue);
            
            // This pass will be rendered on the default FrameBuffer (i.e. the screen),
            // but the pass objects before it will be already draw on screen.
            pass.renderWith(this);
        }
        
        if(pass.isFrameBufferedRendering())
        {
            if(fbo.expired())
            {
                // Here we have to create the FrameBuffer object for this Pass.
                fbo = createFrameBuffer(std::string("PassFbo-") + pass.getName());
                Texture colorTexture = createTexture(fbo.getName() + "-Color0Tex"); // Create the Blank Texture object.
                fbo.attachTexture(FrameBuffer::Attachement::Color, colorTexture); // Attach the Texture to Color0.
            }
            
            // Prepare the fbo to be draw on. (Usually binding)
            prepare(fbo);
            
            // Draw the pass in the fbo.
            pass.renderWith(this);
            
            // Store the fbo to the Queue in order to be draw later, with blending.
            framebufQueue.push(pass.getFrameBuffer());
            
            // We need to unbind the FrameBuffer, so just do it.
            finish(fbo);
        }
    }
    
    // Now, we should render all FrameBuffer objects, blended, from the first one
    // to the last one.
    renderFrameBuffers(framebufQueue);
}

void RendererResource::prepare(PassPrivate* privPass)
{
    GreDebugFunctionNotImplemented();
}

void RendererResource::prepare(const FrameBuffer& fbo)
{
    if(fbo != FrameBuffer::Null)
    {
        // Here, we just bind the FrameBuffer in order to be able
        // to draw on it.
        fbo.bind();
    }
}

void RendererResource::finish(const FrameBuffer& fbo)
{
    if(fbo != FrameBuffer::Null)
    {
        fbo.unbind();
    }
}

void RendererResource::renderFrameBuffers(std::queue<FrameBuffer> &fboQueue)
{
    GreDebugFunctionNotImplemented();
}

void RendererResource::_postRender()
{
    GreDebugFunctionNotImplemented();
}

bool RendererResource::isImmediate() const
{
    return _mIsImmediateMode;
}

void RendererResource::setImmediateMode(bool mode)
{
    _mIsImmediateMode = mode;
}

void RendererResource::_renderImmediate()
{
    for(auto it = _mImmediateFunctions.begin(); it != _mImmediateFunctions.end(); it++)
    {
        (*it) ();
    }
}

void RendererResource::addImmediateAction(std::function<void ()> action)
{
    _mImmediateFunctions.push_back(action);
}

void RendererResource::resetImmediateActions()
{
    _mImmediateFunctions.clear();
}

Scene RendererResource::loadSceneByName(const std::string& name, const std::string& sname)
{
    SceneLoader* sceneLoader = ResourceManager::Get().getSceneLoaderFactory().get(name);
    if(sceneLoader) {
        return loadSceneByResource(Scene(ResourceManager::Get().loadResourceWith(sceneLoader, Resource::Type::Scene, sname)));
    }
    
    return Scene::Null;
}

Scene RendererResource::loadSceneByResource(Scene scene)
{
    if(!_mScene.expired()) {
        Emitter::removeListener(_mScene.getName());
        _mScene.clear();
    }
    
    _mScene = scene;
    if(!_mScene.expired())
        _mScene.getRoot().listen(*this);
    return _mScene;
}

void RendererResource::pushMatrix(MatrixType matrix)
{
    GreDebugFunctionNotImplemented();
}

void RendererResource::popMatrix(MatrixType matrix)
{
    GreDebugFunctionNotImplemented();
}

Scene& RendererResource::getScene()
{
    return _mScene;
}

const Scene& RendererResource::getScene() const
{
    return _mScene;
}

HardwareVertexBuffer RendererResource::createVertexBuffer()
{
    GreDebugFunctionNotImplemented();
    return HardwareVertexBuffer::Null;
}

HardwareIndexBuffer RendererResource::createIndexBuffer(PrimitiveType ptype, StorageType stype)
{
    GreDebugFunctionNotImplemented();
    return HardwareIndexBuffer::Null;
}

Mesh RendererResource::createMeshFromBuffers(const std::string& name, const HardwareVertexBuffer &vbuf, const HardwareIndexBufferBatch &ibufs)
{
    if(vbuf.isInvalid())
        return Mesh::Null;
    
    // In this function, we create a mesh using the default MeshLoader.
    Mesh ret = Mesh(ResourceManager::Get().loadResourceWith(ResourceManager::Get().getMeshLoaderFactory().get("DefaultLoader"),
                                                       Resource::Type::Mesh, name,
                                                       vbuf, ibufs));
    return std::move(ret);
}

Texture RendererResource::createTexture(const std::string &name, const std::string &file)
{
    GreDebugFunctionNotImplemented();
    return Texture::Null;
}

Camera RendererResource::createCamera(const std::string& name)
{
    return Camera(name);
}

void RendererResource::draw(const Mesh &mesh, const HardwareProgram& activProgram)
{
    GreDebugFunctionNotImplemented();
}

void RendererResource::transform(const Node &node)
{
    GreDebugFunctionNotImplemented();
}

void RendererResource::draw(const Mesh& mesh, const Matrix4& mat4, const HardwareProgram& activProgram)
{
    // If we arrive here, the Pass object should have set already the ShaderProgram,
    // the Projection and View matrix.
    
    // Set the Model Matrix.
    _mProgramManager.setSdkUniformMat4("ModelMatrix", mat4);
    
    // Now, we draw the Mesh.
    draw(mesh, activProgram);
}

void RendererResource::draw(const Mesh &mesh, HardwareProgramVariables &variables)
{
    GreDebugFunctionNotImplemented();
}

HardwareProgram RendererResource::createHardwareProgram(const std::string& name, const HardwareShader &vertexShader, const HardwareShader &fragmentShader)
{
    GreDebugFunctionNotImplemented();
    return HardwareProgram::Null;
}

HardwareProgram RendererResource::createHardwareProgram(const std::string &name, const std::string &vertexShaderPath, const std::string &fragmentShaderPath)
{
    HardwareShader vertexShader = _mProgramManager.loadShader(ShaderType::Vertex, vertexShaderPath, vertexShaderPath);
    HardwareShader fragmentShader = _mProgramManager.loadShader(ShaderType::Fragment, fragmentShaderPath, fragmentShaderPath);
    return createHardwareProgram(name, vertexShader, fragmentShader);
}

FrameBuffer RendererResource::createFrameBuffer(const std::string &name) const
{
    auto rloader = ResourceManager::Get().getFrameBufferLoaderFactory().get("OpenGlFrameBufferLoader");
    if(rloader)
    {
        ResourceUser ruser = ResourceManager::Get().loadResourceWith(rloader, Resource::Type::FrameBuff, name);
        return FrameBuffer(ruser);
    }
#ifdef GreIsDebugMode
    else
    {
        GreDebugPretty() << "No loader to create FrameBuffer '" << name << "'." << std::endl;
        throw GreNoLoaderFound("OpenGlFrameBufferLoader");
    }
#endif
    
    return FrameBuffer::Null;
}

Texture RendererResource::createTexture(const std::string &name) const
{
    GreDebugFunctionNotImplemented();
    return Texture::Null;
}

// ---------------------------------------------------------------------------------------------------

Renderer::Renderer(Renderer&& movref)
: ResourceUser(movref), _mRenderer(std::move(movref._mRenderer))
{
    
}

Renderer::Renderer (const Renderer& renderer)
: ResourceUser(renderer.lock()), _mRenderer(renderer._mRenderer)
{
    
}

Renderer::Renderer (const ResourceUser& renderer)
: ResourceUser(renderer.lock()), _mRenderer(std::dynamic_pointer_cast<RendererResource>(renderer.lock()))
{
    
}

Renderer::~Renderer()
{
    
}

Renderer& Renderer::operator=(const ResourceUser &ruser)
{
    ResourceUser::_resource = ruser.lock();
    _mRenderer = std::dynamic_pointer_cast<RendererResource>(ruser.lock());
    return *this;
}

Renderer& Renderer::operator=(const Renderer &ruser)
{
    ResourceUser::_resource = ruser.lock();
    _mRenderer = ruser._mRenderer;
    return *this;
}

void Renderer::render()
{
    auto ptr = _mRenderer.lock();
    if(ptr)
        ptr->render();
}

void Renderer::renderExample()
{
    auto ptr = _mRenderer.lock();
    if(ptr)
        ptr->renderExample();
}

void Renderer::associateWindow(Window &window)
{
    auto ptr = _mRenderer.lock();
    if(ptr)
        ptr->associateWindow(window);
}

void Renderer::setFramerate(float fps)
{
    auto ptr = _mRenderer.lock();
    if(ptr)
        ptr->setFramerate(fps);
}

float Renderer::getCurrentFramerate() const
{
    auto ptr = _mRenderer.lock();
    if(ptr)
        return ptr->getCurrentFramerate();
    
    return 0.0f;
}

ElapsedTime Renderer::getElapsedTime() const
{
    auto ptr = _mRenderer.lock();
    if(ptr)
        return ptr->getElapsedTime();
    return ElapsedTime::zero();
}

void Renderer::resetElapsedTime()
{
    auto ptr = _mRenderer.lock();
    if(ptr)
        ptr->resetElapsedTime();
}

void Renderer::beginRender()
{
    auto ptr = _mRenderer.lock();
    if(ptr)
        ptr->beginRender();
}

void Renderer::endRender()
{
    auto ptr = _mRenderer.lock();
    if(ptr)
        ptr->endRender();
}

void Renderer::setClearColor(const Color& color)
{
    auto ptr = _mRenderer.lock();
    if(ptr)
        ptr->setClearColor(color);
}

void Renderer::setClearDepth(float depth)
{
    auto ptr = _mRenderer.lock();
    if(ptr)
        ptr->setClearDepth(depth);
}

void Renderer::setActive(bool active)
{
    auto ptr = _mRenderer.lock();
    if(ptr)
        ptr->setActive(active);
}

bool Renderer::isActive() const
{
    auto ptr = _mRenderer.lock();
    if(ptr)
        return ptr->isActive();
    return false;
}

bool Renderer::isImmediate() const
{
    auto ptr = _mRenderer.lock();
    if(ptr)
        return ptr->isImmediate();
    return false;
}

void Renderer::setImmediateMode(bool mode)
{
    auto ptr = _mRenderer.lock();
    if(ptr)
        ptr->setImmediateMode(mode);
}

void Renderer::addImmediateAction(std::function<void ()> action)
{
    auto ptr = _mRenderer.lock();
    if(ptr)
        ptr->addImmediateAction(action);
}

void Renderer::resetImmediateActions()
{
    auto ptr = _mRenderer.lock();
    if(ptr)
        ptr->resetImmediateActions();
}

void Renderer::translate(float x, float y, float z)
{
    auto ptr = _mRenderer.lock();
    if(ptr)
        ptr->translate(x, y, z);
}

void Renderer::rotate(float angle, float x, float y, float z)
{
    auto ptr = _mRenderer.lock();
    if(ptr)
        ptr->rotate(angle, x, y, z);
}

void Renderer::drawTriangle(float sz, const Color& color1, const Color& color2, const Color& color3)
{
    auto ptr = _mRenderer.lock();
    if(ptr)
        ptr->drawTriangle(sz, color1, color2, color3);
}

void Renderer::drawQuad(float sz, const Color& color1, const Color& color2, const Color& color3, const Color& color4)
{
    auto ptr = _mRenderer.lock();
    if(ptr)
        ptr->drawQuad(sz, color1, color2, color3, color4);
}

HardwareVertexBuffer Renderer::createVertexBuffer()
{
    auto ptr = _mRenderer.lock();
    if(ptr)
        return ptr->createVertexBuffer();
    return HardwareVertexBuffer::Null;
}

HardwareIndexBuffer Renderer::createIndexBuffer(PrimitiveType ptype, StorageType stype)
{
    auto ptr = _mRenderer.lock();
    if(ptr)
        return ptr->createIndexBuffer(ptype, stype);
    return HardwareIndexBuffer::Null;
}

Mesh Renderer::createMeshFromBuffers(const std::string &name, const HardwareVertexBuffer &vbuf, const HardwareIndexBufferBatch &ibufs)
{
    auto ptr = _mRenderer.lock();
    if(ptr)
        return ptr->createMeshFromBuffers(name, vbuf, ibufs);
    return Mesh::Null;
}

Texture Renderer::createTexture(const std::string &name, const std::string &file)
{
    auto ptr = _mRenderer.lock();
    if(ptr)
        return ptr->createTexture(name, file);
    return Texture::Null;
}

Camera Renderer::createCamera(const std::string &name)
{
    auto ptr = _mRenderer.lock();
    if(ptr)
        return ptr->createCamera(name);
    return Camera(nullptr);
}

void Renderer::draw(const Mesh &mesh, const HardwareProgram& activProgram)
{
    auto ptr = _mRenderer.lock();
    if(ptr)
        ptr->draw(mesh, activProgram);
}

void Renderer::prepare(const Camera& cam)
{
    auto ptr = _mRenderer.lock();
    if(ptr)
        ptr->prepare(cam);
}

Scene Renderer::loadSceneByName(const std::string& name, const std::string& sname)
{
    auto ptr = _mRenderer.lock();
    if(ptr)
        return ptr->loadSceneByName(name, sname);
    return Scene::Null;
}

Scene Renderer::loadSceneByResource(Scene scene)
{
    auto ptr = _mRenderer.lock();
    if(ptr)
        return ptr->loadSceneByResource(scene);
    return Scene::Null;
}

void Renderer::pushMatrix(MatrixType matrix)
{
    auto ptr = _mRenderer.lock();
    if(ptr)
        ptr->pushMatrix(matrix);
}

void Renderer::popMatrix(MatrixType matrix)
{
    auto ptr = _mRenderer.lock();
    if(ptr)
        ptr->popMatrix(matrix);
}

Scene& Renderer::getScene()
{
    auto ptr = _mRenderer.lock();
    if(ptr)
        return ptr->getScene();
    return Scene::Null;
}

const Scene& Renderer::getScene() const
{
    auto ptr = _mRenderer.lock();
    if(ptr)
        return ptr->getScene();
    return Scene::Null;
}

void Renderer::transform(const Node &node)
{
    auto ptr = _mRenderer.lock();
    if(ptr)
        ptr->transform(node);
}

HardwareProgram Renderer::createHardwareProgram(const std::string& name, const HardwareShader &vertexShader, const HardwareShader &fragmentShader)
{
    auto ptr = _mRenderer.lock();
    if(ptr)
        return ptr->createHardwareProgram(name, vertexShader, fragmentShader);
    return HardwareProgram::Null;
}

void Renderer::prepare(Gre::PassPrivate *privPass)
{
    auto ptr = _mRenderer.lock();
    if(ptr)
        ptr->prepare(privPass);
}

void Renderer::prepare(const FrameBuffer &fbo)
{
    auto ptr = _mRenderer.lock();
    if(ptr)
        ptr->prepare(fbo);
}

void Renderer::finish(const FrameBuffer &fbo)
{
    auto ptr = _mRenderer.lock();
    if(ptr)
        ptr->finish(fbo);
}

void Renderer::renderFrameBuffers(std::queue<FrameBuffer> &fboQueue)
{
    auto ptr = _mRenderer.lock();
    if(ptr)
        ptr->renderFrameBuffers(fboQueue);
}

// ---------------------------------------------------------------------------------------------------

RendererLoader::RendererLoader()
{
    
}

RendererLoader::~RendererLoader()
{
    
}

bool RendererLoader::isTypeSupported (Resource::Type type) const
{
    return type == Resource::Type::Renderer;
}

Resource* RendererLoader::load (Resource::Type type, const std::string& name) const
{
    return nullptr;
}

ResourceLoader* RendererLoader::clone() const
{
    return new RendererLoader();
}

GRE_END_NAMESPACE
