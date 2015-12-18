//
//  Renderer.cpp
//  GResource
//
//  Created by Jacques Tronconi on 08/11/2015.
//  Copyright (c) 2015 Atlanti's Corporation. All rights reserved.
//

#include "Renderer.h"
#include "ResourceManager.h"

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
    
}

void RendererResource::_render()
{
    
}

void RendererResource::_postRender()
{
    
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
    
}

void RendererResource::popMatrix(MatrixType matrix)
{
    
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
    return HardwareVertexBuffer::Null;
}

HardwareIndexBuffer RendererResource::createIndexBuffer(PrimitiveType ptype, StorageType stype)
{
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
    return Texture::Null;
}

Camera RendererResource::createCamera(const std::string& name)
{
    return Camera(name);
}

void RendererResource::draw(const Mesh &mesh)
{
    
}

void RendererResource::transform(const Node &node)
{
    
}

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

void Renderer::draw(const Mesh &mesh)
{
    auto ptr = _mRenderer.lock();
    if(ptr)
        ptr->draw(mesh);
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
