//
//  Scene.cpp
//  GRE
//
//  Created by Jacques Tronconi on 14/12/2015.
//
//

#include "Scene.h"
#include "ResourceManager.h"

GreBeginNamespace

ScenePrivate::ScenePrivate(const std::string& name)
: Resource(name)
{
    // We always add the Default first Pass.
    createAndAddPass("Default", (PassNumber) PassPurpose::First);
    // By default, a Pass should always load a new passtrough shader program
    // when the Renderer ask it to draw.
}

ScenePrivate::~ScenePrivate()
{
    
}

Node& ScenePrivate::getRoot()
{
    GreDebugFunctionNotImplemented();
    return Node::Null;
}

const Node& ScenePrivate::getRoot() const
{
    GreDebugFunctionNotImplemented();
    return Node::Null;
}

NodePrivate* ScenePrivate::createNode(const Mesh &mesh)
{
    GreDebugFunctionNotImplemented();
    return nullptr;
}

NodePrivate* ScenePrivate::createNode(const Camera &camera)
{
    GreDebugFunctionNotImplemented();
    return nullptr;
}

void ScenePrivate::clear()
{
    GreDebugFunctionNotImplemented();
}

Camera& ScenePrivate::getCamera()
{
    GreDebugFunctionNotImplemented();
    return Camera::Null;
}

const Camera& ScenePrivate::getCamera() const
{
    GreDebugFunctionNotImplemented();
    return Camera::Null;
}

std::vector<const Node> ScenePrivate::getNodesByFilter(Node::Filter filter) const
{
    GreDebugFunctionNotImplemented();
    return std::vector<const Node> ();
}

void ScenePrivate::setActiveCamera(const Node &cameraNode)
{
    GreDebugFunctionNotImplemented();
}

Pass ScenePrivate::createAndAddPass(const std::string &name, const PassNumber &passNumber)
{
    if(passNumber < (PassNumber)PassPurpose::First || passNumber > (PassNumber)PassPurpose::Last)
    {
#ifdef GreIsDebugMode
        GreDebugPretty() << "PassNumber '" << passNumber << "' is invalid." << std::endl;
#endif
        return Pass::Null;
    }
    
    PassPrivate* check = _mPassesByNumber[passNumber].lock().get();
    if(check)
    {
        GreDebugPretty() << "PassNumber '" << passNumber << "' was already allocated." << std::endl;
        return Pass::Null;
    }
    
    PassPrivate* newPass = new PassPrivate(name, passNumber);
    std::shared_ptr<PassPrivate> shrdPass (newPass);
    
    _mPasses.push_back(shrdPass);
    _mPassesByNumber[passNumber] = shrdPass;
    
#ifdef GreIsDebugMode
    GreDebugPretty() << "Pass successfully created and added at PassNumber '" << passNumber << "'." << std::endl;
#endif
    
    return Pass(ResourceUser(shrdPass));
}

Pass ScenePrivate::addPass(PassPrivate *customPass)
{
    if(!customPass)
    {
#ifdef GreIsDebugMode
        GreDebugPretty() << "Provided arg 'customPass' is null." << std::endl;
#endif
        return Pass::Null;
    }
    
    PassNumber passNumber = customPass->getPassNumber();
    if(passNumber < (PassNumber)PassPurpose::First || passNumber > (PassNumber)PassPurpose::Last)
    {
#ifdef GreIsDebugMode
        GreDebugPretty() << "PassNumber '" << passNumber << "' is invalid." << std::endl;
#endif
        return Pass::Null;
    }
    
    PassPrivate* check = _mPassesByNumber[customPass->getPassNumber()].lock().get();
    if(check)
    {
        GreDebugPretty() << "PassNumber '" << customPass->getPassNumber() << "' was already allocated." << std::endl;
        return Pass::Null;
    }
    
    std::shared_ptr<PassPrivate> shrdPass (customPass);
    _mPasses.push_back(shrdPass);
    _mPassesByNumber[customPass->getPassNumber()] = shrdPass;
    
#ifdef GreIsDebugMode
    GreDebugPretty() << "Pass successfully added at PassNumber '" << customPass->getPassNumber() << "'." << std::endl;
#endif
    
    return Pass(ResourceUser(shrdPass));
}

Pass ScenePrivate::getPassByName(const std::string &name)
{
    for(std::shared_ptr<PassPrivate>& passShrd : _mPasses)
    {
        if(passShrd->getName() == name)
            return Pass(ResourceUser(passShrd));
    }
    
#ifdef GreIsDebugMode
    GreDebugPretty() << "Pass '" << name << "' not found." << std::endl;
#endif
    
    return Pass::Null;
}

Pass ScenePrivate::getPassByNumber(const PassNumber &passNumber)
{
    std::weak_ptr<PassPrivate> passWeak = _mPassesByNumber[passNumber];
    if(passWeak.expired())
    {
#ifdef GreIsDebugMode
        GreDebugPretty() << "Pass with PassNumber '" << passNumber << "' not found." << std::endl;
#endif
        return Pass::Null;
    }
    
    return Pass(ResourceUser(passWeak));
}

void ScenePrivate::removePassByName(const std::string &name)
{
    PassPrivateOwnedList::const_iterator end = _mPasses.end();
    PassPrivateOwnedList::iterator it;
    
    for(it = _mPasses.begin(); it != end; it++)
    {
        if( (*it)->getName() == name) {
            _mPassesByNumber[(*it)->getPassNumber()].reset();
            _mPasses.erase(it);
            return;
        }
    }
    
#ifdef GreIsDebugMode
    GreDebugPretty() << "Pass '" << name << "' not found." << std::endl;
#endif
}

void ScenePrivate::removePassByNumber(const PassNumber &passNumber)
{
    PassPrivateOwnedList::const_iterator end = _mPasses.end();
    PassPrivateOwnedList::iterator it;
    
    for(it = _mPasses.begin(); it != end; it++)
    {
        if( (*it)->getPassNumber() == passNumber) {
            _mPassesByNumber[(*it)->getPassNumber()].reset();
            _mPasses.erase(it);
            return;
        }
    }
    
#ifdef GreIsDebugMode
    GreDebugPretty() << "Pass '" << passNumber << "' not found." << std::endl;
#endif
}

PassList ScenePrivate::getActivePasses() const
{
    PassList retPass;
    
    for(auto const& passIt : _mPassesByNumber)
    {
        if(!passIt.second.expired()) {
            retPass.push_back(Pass(ResourceUser(passIt.second)));
        }
    }
    
#ifdef GreIsDebugMode
    if(retPass.size() == 0) {
        GreDebugPretty() << "No active pass detected." << std::endl;
    }
#endif
    
    return retPass;
}

bool ScenePrivate::isDirty() const
{
    return _dirty;
}

// ---------------------------------------------------------------------------------------------------

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

bool Scene::operator==(const Scene &rhs) const
{
    return _mScene.lock() == rhs._mScene.lock();
}

bool Scene::operator!=(const Scene &rhs) const
{
    return !(*this == rhs);
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

NodePrivate* Scene::createNode(const Mesh &mesh)
{
    auto ptr = _mScene.lock();
    if(ptr)
        return ptr->createNode(mesh);
    return nullptr;
}

NodePrivate* Scene::createNode(const Camera &camera)
{
    auto ptr = _mScene.lock();
    if(ptr)
        return ptr->createNode(camera);
    return nullptr;
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

void Scene::setActiveCamera(const Node &cameraNode)
{
    auto ptr = _mScene.lock();
    if(ptr)
        ptr->setActiveCamera(cameraNode);
}

Pass Scene::createAndAddPass(const std::string &name, const PassNumber &passNumber)
{
    auto ptr = _mScene.lock();
    
    if(ptr)
        return ptr->createAndAddPass(name, passNumber);
    return Pass::Null;
}

Pass Scene::addPass(Gre::PassPrivate *customPass)
{
    auto ptr = _mScene.lock();
    
    if(ptr)
        return ptr->addPass(customPass);
    return Pass::Null;
}

Pass Scene::getPassByName(const std::string &name)
{
    auto ptr = _mScene.lock();
    
    if(ptr)
        return ptr->getPassByName(name);
    return Pass::Null;
}

Pass Scene::getPassByNumber(const PassNumber &passNumber)
{
    auto ptr = _mScene.lock();
    
    if(ptr)
        return ptr->getPassByNumber(passNumber);
    return Pass::Null;
}

void Scene::removePassByName(const std::string &name)
{
    auto ptr = _mScene.lock();
    
    if(ptr)
        ptr->removePassByName(name);
}

void Scene::removePassByNumber(const PassNumber &passNumber)
{
    auto ptr = _mScene.lock();
    
    if(ptr)
        ptr->removePassByNumber(passNumber);
}

PassList Scene::getActivePasses() const
{
    auto ptr = _mScene.lock();
    
    if(ptr)
        return ptr->getActivePasses();
    return PassList();
}

bool Scene::isDirty() const
{
    auto ptr = _mScene.lock();
    if(ptr)
        return ptr->isDirty();
    return false;
}

// ---------------------------------------------------------------------------------------------------

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
    GreDebugFunctionNotImplemented();
    return nullptr;
}

Scene Scene::Null = Scene();

GreEndNamespace
