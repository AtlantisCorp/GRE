//////////////////////////////////////////////////////////////////////
//
//  Scene.cpp
//  This source file is part of Gre
//		(Gang's Resource Engine)
//
//  Copyright (c) 2015 - 2016 Luk2010
//  Created on 14/12/2015.
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

#include "Scene.h"
#include "ResourceManager.h"

GreBeginNamespace

SceneManagerPrivate::SceneManagerPrivate(const std::string& name)
: Resource(name), iRootNode(nullptr), iCurrentCamera(nullptr)
{
    // We always add the Default first Pass.
    createAndAddPass("Default", (PassNumber) PassPurpose::First);
    // By default, a Pass should always load a new passtrough shader program
    // when the Renderer ask it to draw.
}

SceneManagerPrivate::~SceneManagerPrivate()
{
    
}

SceneNode& SceneManagerPrivate::getRoot()
{
    return iRootNode;
}

const SceneNode& SceneManagerPrivate::getRoot() const
{
    return iRootNode;
}

SceneNode SceneManagerPrivate::createNode()
{
	SceneNodePrivate* node = new SceneNodePrivate(this);
	iSceneNodes.add(node);
	return SceneNode(node);
}

SceneNode SceneManagerPrivate::createNode(const std::string& name)
{
	SceneNodePrivate* node = new SceneNodePrivate(this, name);
	iSceneNodes.add(node);
	return SceneNode(node);
}

void SceneManagerPrivate::destroyNode(const std::string& name)
{
	auto it = iSceneNodes.find(name);
	
	if(it == iSceneNodes.end())
	{
#ifdef GreIsDebugMode
		GreDebugPretty() << "SceneNode '" << name << "' not found." << std::endl;
#endif // GreIsDebugMode
	}
	
	else
	{
		SceneNode parent = (*it)->getParent();
		if(parent)
		{
			parent.removeChild((*it).get());
		}
		
		iSceneNodes.erase(it);
	}
}

void SceneManagerPrivate::destroyNode(SceneNode& node)
{
	if(node)
	{
		destroyNode(node.getName());
	}
	
	else
	{
#ifdef GreIsDebugMode
		GreDebugPretty() << "Can't destroy null SceneNode." << std::endl;
#endif // GreIsDebugMode
	}
}

void Gre::SceneManagerPrivate::destroyEveryNode()
{
	iRootNode = nullptr;
	iSceneNodes.clear();
}

void SceneManagerPrivate::clear()
{
    iPasses.clear();
    iPassesByNumber.clear();
    iRootNode = nullptr;
    iSceneNodes.clear();
}

Camera& SceneManagerPrivate::addCamera(const Camera& camera)
{
	if(camera)
	{
		iCameras.add(camera.lock());
		return camera;
	}
	
	else
	{
#ifdef GreIsDebugMode
		GreDebugPretty() << "A null Camera cannot be added." << std::endl;
#endif // GreIsDebugMode
		return Camera::Null;
	}
}

Camera& SceneManagerPrivate::addCamera(const std::string& name)
{
	CameraHolder camera = Gre::SpecializedResourceHolder<CameraPrivate>(new Camera(name));
	iCameras.add(camera);
	return Camera(camera.get());
}

Camera& SceneManagerPrivate::getCurrentCamera()
{
    return iCurrentCamera;
}

const Camera& SceneManagerPrivate::getCurrentCamera() const
{
    return iCurrentCamera;
}

std::vector<const Node> SceneManagerPrivate::getNodesByFilter(NodePrivate::Filter filter) const
{
    GreDebugFunctionNotImplemented();
    return std::vector<const Node> ();
}

SceneNodeHolderList SceneManagerPrivate::getNodesForPass(Gre::PassPurpose pass, NodePrivate::Filter filter) const
{
	if(filter == NodePrivate::Filter::None)
	{
		return iSceneNodes;
	}
	
	return SceneNodeHolderList();
}

SceneNodeList SceneManagerPrivate::getNodesFrom(const Gre::Camera &camera) const
{
    SceneNodeList ret;
    
    for(auto it = iSceneNodes.begin(); it != iSceneNodes.end(); it++)
    {
        Vector3 pos = (*it)->getTransformation().getTranslation();
        if( camera.contains(pos) )
        {
            ret.push_back(SceneNode( (*it) ));
        }
    }
    
    return ret;
}

void SceneManagerPrivate::setActiveCamera(const Camera &camera)
{
	iCurrentCamera = camera;
}

Pass SceneManagerPrivate::createAndAddPass(const std::string &name, const PassNumber &passNumber)
{
    if(passNumber < (PassNumber)PassPurpose::First || passNumber > (PassNumber)PassPurpose::Last)
    {
#ifdef GreIsDebugMode
        GreDebugPretty() << "PassNumber '" << passNumber << "' is invalid." << std::endl;
#endif
        return Pass::Null;
    }
    
    PassHolder check = _mPassesByNumber[passNumber].lock();
    if(check)
    {
        GreDebugPretty() << "PassNumber '" << passNumber << "' was already allocated." << std::endl;
        return Pass::Null;
    }
    
    PassHolder newPass = PassHolder(new PassPrivate(name, passNumber));
    iPasses.add(newPass);
    iPassesByNumber[passNumber] = Pass(newPass);
    
#ifdef GreIsDebugMode
    GreDebugPretty() << "Pass successfully created and added at PassNumber '" << passNumber << "'." << std::endl;
#endif
    
    return iPassesByNumber[passNumber];
}

Pass SceneManagerPrivate::addPass(PassHolder& customPass)
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
    
    PassHolder check = iPassesByNumber[passNumber].lock();
    if(check)
    {
        GreDebugPretty() << "PassNumber '" << passNumber << "' was already allocated." << std::endl;
        return Pass::Null;
    }
    
    iPasses.add(customPass);
    iPassesByNumber[passNumber] = Pass(customPass);
    
#ifdef GreIsDebugMode
    GreDebugPretty() << "Pass successfully added at PassNumber '" << customPass->getPassNumber() << "'." << std::endl;
#endif
    
    return iPassesByNumber[passNumber];
}

Pass SceneManagerPrivate::getPassByName(const std::string &name)
{
    for(auto passShrd : iPasses)
    {
        if(passShrd->getName() == name)
            return Pass(passShrd);
    }
    
#ifdef GreIsDebugMode
    GreDebugPretty() << "Pass '" << name << "' not found." << std::endl;
#endif
    
    return Pass::Null;
}

Pass SceneManagerPrivate::getPassByNumber(const PassNumber &passNumber)
{
    auto passWeak = iPassesByNumber[passNumber];
    if(passWeak.expired())
    {
#ifdef GreIsDebugMode
        GreDebugPretty() << "Pass with PassNumber '" << passNumber << "' not found." << std::endl;
#endif
        return Pass::Null;
    }
    
    return passWeak;
}

void SceneManagerPrivate::removePassByName(const std::string &name)
{
    PassHolderList::const_iterator end = iPasses.end();
    PassHolderList::iterator it;
    
    for(it = iPasses.begin(); it != end; it++)
    {
        if( (*it)->getName() == name) {
            iPassesByNumber[(*it)->getPassNumber()].reset();
            iPasses.erase(it);
            return;
        }
    }
    
#ifdef GreIsDebugMode
    GreDebugPretty() << "Pass '" << name << "' not found." << std::endl;
#endif
}

void SceneManagerPrivate::removePassByNumber(const PassNumber &passNumber)
{
    PassHolderList::const_iterator end = iPasses.end();
    PassHolderList::iterator it;
    
    for(it = iPasses.begin(); it != end; it++)
    {
        if( (*it)->getPassNumber() == passNumber) {
            iPassesByNumber[(*it)->getPassNumber()].reset();
            iPasses.erase(it);
            return;
        }
    }
    
#ifdef GreIsDebugMode
    GreDebugPretty() << "Pass '" << passNumber << "' not found." << std::endl;
#endif
}

PassHolderList SceneManagerPrivate::getActivePasses() const
{
    PassHolderList retPass;
    
    for(auto const& passIt : iPassesByNumber)
    {
        if(!passIt.second.expired()) {
            retPass.push_back(passIt.second);
        }
    }
    
#ifdef GreIsDebugMode
    if(retPass.size() == 0) {
        GreDebugPretty() << "No active pass detected." << std::endl;
    }
#endif
    
    return retPass;
}

// ---------------------------------------------------------------------------------------------------

SceneManager::SceneManager(SceneManagerPrivate* scene)
: SpecializedResourceUser<SceneManagerPrivate>(scene)
{
	
}

SceneManager::SceneManager(const SceneManagerHolder& holder)
: SpecializedResourceUser<SceneManagerPrivate>(holder)
{
	
}

SceneManager::SceneManager(const SceneManager& user)
: SpecializedResourceUser<SceneManagerPrivate>(user)
{
	
}

SceneManager::~SceneManager()
{
	
}

SceneNode& SceneManager::getRoot()
{
	auto ptr = lock();
	if(ptr)
	{
		return ptr->getRoot();
	}
	
	return SceneNode::Null;
}

const SceneNode& SceneManager::getRoot() const
{
	auto ptr = lock();
	if(ptr)
	{
		return ptr->getRoot();
	}
	
	return SceneNode::Null;
}

SceneNode SceneManager::createNode()
{
	auto ptr = lock();
	if(ptr)
	{
		return ptr->createNode();
	}
	
	return SceneNode::Null;
}

SceneNode SceneManager::createNode(const std::string& name)
{
	auto ptr = lock();
	if(ptr)
	{
		return ptr->createNode(name);
	}
	
	return SceneNode::Null;
}

void SceneManager::destroyNode(const std::string& name)
{
	auto ptr = lock();
	if(ptr)
	{
		ptr->destroyNode(name);
	}
}

void SceneManager::destroyNode(SceneNode& node)
{
	auto ptr = lock();
	if(ptr)
	{
		ptr->destroyNode(node);
	}
}

void SceneManager::destroyEveryNode()
{
	auto ptr = lock();
	if(ptr)
	{
		ptr->destroyEveryNode();
	}
}

void SceneManager::clear()
{
	auto ptr = lock();
	if(ptr)
	{
		ptr->clear();
	}
}

Camera& SceneManager::addCamera(const Camera& camera)
{
	auto ptr = lock();
	if(ptr)
	{
		return ptr->addCamera(camera);
	}
	
	return Camera::Null;
}

Camera& SceneManager::addCamera(const std::string& name)
{
	auto ptr = lock();
	if(ptr)
	{
		return ptr->addCamera(name);
	}
	
	return Camera::Null;
}

Camera& SceneManager::getCurrentCamera()
{
	auto ptr = lock();
	if(ptr)
	{
		return ptr->getCurrentCamera();
	}
	
	return Camera::Null;
}

const Camera& SceneManager::getCurrentCamera() const
{
	auto ptr = lock();
	if(ptr)
	{
		return ptr->getCurrentCamera();
	}
	
	return Camera::Null;
}

void SceneManager::setActiveCamera(const Camera& camera)
{
	auto ptr = lock();
	if(ptr)
	{
		ptr->setActiveCamera(camera);
	}
}

std::vector<const Node> SceneManager::getNodesByFilter(Node::Filter filter) const
{
	auto ptr = lock();
	if(ptr)
	{
		return ptr->getNodesByFilter(filter);
	}
	
	return std::vector<const Node>();
}

std::vector<const SceneNode> SceneManager::getNodesForPass(Gre::PassPurpose pass, Node::Filter filter) const
{
	auto ptr = lock();
	if(ptr)
	{
		return ptr->getNodesForPass(pass, filter);
	}
	
	return std::vector<const SceneNode>();
}

Pass SceneManager::createAndAddPass(const std::string& name, const PassNumber& passNumber)
{
	auto ptr = lock();
	if(ptr)
	{
		return ptr->createAndAddPass(name, passNumber);
	}
	
	return Pass::Null;
}

Pass SceneManager::addPass(PassHolder& customPass)
{
	auto ptr = lock();
	if(ptr)
	{
		return ptr->addPass(customPass);
	}
	
	return Pass::Null;
}

Pass SceneManager::getPassByName(const std::string& name)
{
	auto ptr = lock();
	if(ptr)
	{
		return ptr->getPassByName(name);
	}
	
	return Pass::Null;
}

Pass SceneManager::getPassByNumber(const PassNumber& passNumber)
{
	auto ptr = lock();
	if(ptr)
	{
		return ptr->getPassByNumber(passNumber);
	}
	
	return Pass::Null;
}

void SceneManager::removePassByName(const std::string& name)
{
	auto ptr = lock();
	if(ptr)
	{
		ptr->removePassByName(name);
	}
}

void SceneManager::removePassByNumber(const PassNumber& passNumber)
{
	auto ptr = lock();
	if(ptr)
	{
		ptr->removePassByNumber(passNumber);
	}
}

PassList SceneManager::getActivePasses() const
{
	auto ptr = lock();
	if(ptr)
	{
		return ptr->getActivePasses();
	}
	
	return PassList();
}

SceneManager SceneManager::Null = SceneManager(nullptr);

// ---------------------------------------------------------------------------------------------------

SceneManagerLoader::SceneManagerLoader()
{
    
}

SceneManagerLoader::~SceneManagerLoader()
{
    
}

bool SceneManagerLoader::isTypeSupported(Resource::Type type) const
{
    return type == Resource::Type::Scene;
}

Resource* SceneManagerLoader::load(Resource::Type type, const std::string& name) const
{
    GreDebugFunctionNotImplemented();
    return nullptr;
}

SceneManager SceneManager::Null = Scene();

GreEndNamespace
