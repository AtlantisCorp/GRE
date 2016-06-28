//
//  CustomScene.cpp
//  GRE
//
//  Created by Jacques Tronconi on 07/04/2016.
//
//

#include "CustomScene.h"

GreBeginNamespace

CustomScenePrivate::CustomScenePrivate(const std::string& name)
: Resource(name)
{
    std::shared_ptr<CustomNodePrivate> rootPriv = std::make_shared<CustomNodePrivate>("Root", CustomNodeType::Root);
    _mRootNode = CustomNode(ResourceUser(rootPriv, true));
}

void CustomScenePrivate::setDefaultActiveCamera(const Gre::CameraCustomNode &node)
{
    _mDefaultActiveCamera = node;
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

void CustomScenePrivate::onEvent(const Gre::Event &e)
{
    Resource::onEvent(e);
    
    if(!_mRootNode.expired())
    {
        _mRootNode.onEvent(e);
    }
}

void ExampleUseCustomScene()
{
    CustomScenePrivate* myCustomScene = new CustomScenePrivate("MyScene");
    
    CustomNode rootNode = myCustomScene->getRoot();
    rootNode.setVolume({1024, 1024, 1024});
    
    Mesh myMesh;
    CustomNode myMeshNode = rootNode.addMeshNode(myMesh, rootNode.getCenter());
    
    Camera myCamera;
    CameraCustomNode myCameraNode = CameraCustomNode(rootNode.addCameraNode(myCamera));
    myCameraNode.setPosition({rootNode.getCenter().x, rootNode.getCenter().y, rootNode.getCenter().z - 3.0f});
    myCameraNode.setLookAt(rootNode.getCenter());
    
    myCustomScene->setDefaultActiveCamera(myCameraNode);
    
    // Renderer::_renderScene
    {
        const PassList& passes = myCustomScene->getActivePasses();
    }
    
    // ResourceManager::update
    {
        UpdateEvent e;
        myCustomScene->onEvent(e);
    }
}

GreEndNamespace
