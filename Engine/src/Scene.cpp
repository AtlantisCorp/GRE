//
//  Scene.cpp
//  GRE
//
//  Created by Jacques Tronconi on 14/12/2015.
//
//

#include "Scene.h"

GRE_BEGIN_NAMESPACE

ScenePrivate::ScenePrivate(const std::string& name)
: Resource(name)
{
    
}

ScenePrivate::~ScenePrivate()
{
    
}

Node& ScenePrivate::getRoot()
{
    return Node::Null;
}

const Node& ScenePrivate::getRoot() const
{
    return Node::Null;
}

Node ScenePrivate::createNode(const Mesh &mesh)
{
    return Node::Null;
}

void ScenePrivate::clear()
{
    
}

Camera& ScenePrivate::getCamera()
{
    return Camera::Null;
}

const Camera& ScenePrivate::getCamera() const
{
    return Camera::Null;
}

std::vector<const Node> ScenePrivate::getNodesByFilter(Node::Filter filter) const
{
    return std::vector<const Node> ();
}

Scene::Scene()
: ResourceUser(), _mScene()
{
    
}

Scene::Scene(const Scene& rhs)
: ResourceUser(rhs), _mScene(rhs._mScene)
{
    
}

Scene::Scene(Scene&& rhs)
: ResourceUser(rhs), _mScene(std::move(rhs._mScene))
{
    
}

Scene::Scene(const ResourceUser& rhs)
: ResourceUser(rhs), _mScene(std::dynamic_pointer_cast<ScenePrivate>(rhs.lock()))
{
    
}

Scene::~Scene()
{
    
}

Scene& Scene::operator = (const Scene& rhs)
{
    ResourceUser::operator=(rhs);
    _mScene = rhs._mScene;
    return *this;
}

Node& Scene::getRoot()
{
    auto ptr = _mScene.lock();
    if(ptr)
        return ptr->getRoot();
    return Node::Null;
}

const Node& Scene::getRoot() const
{
    auto ptr = _mScene.lock();
    if(ptr)
        return ptr->getRoot();
    return Node::Null;
}

Node Scene::createNode(const Mesh &mesh)
{
    auto ptr = _mScene.lock();
    if(ptr)
        return ptr->createNode(mesh);
    return Node::Null;
}

void Scene::clear()
{
    auto ptr = _mScene.lock();
    if(ptr)
        ptr->clear();
}

Camera& Scene::getCamera()
{
    auto ptr = _mScene.lock();
    if(ptr)
        return ptr->getCamera();
    return Camera::Null;
}

const Camera& Scene::getCamera() const
{
    auto ptr = _mScene.lock();
    if(ptr)
        return ptr->getCamera();
    return Camera::Null;
}

std::vector<const Node> Scene::getNodes() const
{
    return getNodesByFilter(Node::Filter::Default);
}

std::vector<const Node> Scene::getNodesByFilter(Node::Filter filter) const
{
    auto ptr = _mScene.lock();
    if(ptr)
        return ptr->getNodesByFilter(filter);
    return std::vector<const Node> ();
}

SceneLoader::SceneLoader()
{
    
}

SceneLoader::~SceneLoader()
{
    
}

bool SceneLoader::isTypeSupported(Resource::Type type) const
{
    return type == Resource::Type::Scene;
}

Resource* SceneLoader::load(Resource::Type type, const std::string& name) const
{
    return nullptr;
}

Scene Scene::Null = Scene();

GRE_END_NAMESPACE
