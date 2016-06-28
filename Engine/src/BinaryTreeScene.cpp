//
//  BinaryTreeScene.cpp
//  GRE
//
//  Created by Jacques Tronconi on 21/12/2015.
//
//
/*
#include "BinaryTreeScene.h"

GreBeginNamespace

BinaryTreeNodePrivate::BinaryTreeNodePrivate(const std::string& name)
: NodePrivate(name), _mMesh(Mesh::Null), _mCamera(Camera::Null), _mScene(), _mNodeParent(), _mNodeLeft(), _mNodeRight()
{
    
}

BinaryTreeNodePrivate::~BinaryTreeNodePrivate()
{
    
}

Node BinaryTreeNodePrivate::addChild(const Node& child)
{
    if(!child.lock())
        return Node::Null;
    
    Vector3 Position1(_mMatrix[3]);
    Vector3 Position2(child.getNodeMatrix()[3]);
    
    float distance1 = glm::distance(Vector3(0,0,0), Position1);
    float distance2 = glm::distance(Vector3(0,0,0), Position2);
    Node ret;
    
    if(distance1 > distance2) {
        if(_mNodeLeft) {
            ret = _mNodeLeft->addChild(child);
        } else {
            _mNodeLeft = std::dynamic_pointer_cast<BinaryTreeNodePrivate>(child.lock());
            _mNodeLeft->setThisNode(Node(_mNodeLeft));
            _mNodeLeft->setParent(this->toNode());
            _mNodeLeft->setScene(_mScene);
            ret = Node(_mNodeLeft);
        }
    }
    
    else {
        if(_mNodeRight) {
            ret = _mNodeRight->addChild(child);
        } else {
            _mNodeRight = std::dynamic_pointer_cast<BinaryTreeNodePrivate>(child.lock());
            _mNodeRight->setThisNode(Node(_mNodeRight));
            _mNodeRight->setParent(this->toNode());
            _mNodeRight->setScene(_mScene);
            ret = Node(_mNodeRight);
        }
    }
    
    return ret;
}

Node BinaryTreeNodePrivate::addChild(NodePrivate *child)
{
    std::shared_ptr<BinaryTreeNodePrivate> futurChild(dynamic_cast<BinaryTreeNodePrivate*>(child));
    return this->addChild(Node(futurChild));
}

void BinaryTreeNodePrivate::translate(const Vector3 &vector)
{
    NodePrivate::translate(vector);
    _calculateTranslation();
}

void BinaryTreeNodePrivate::setMesh(const Mesh &mesh)
{
    _mMesh = mesh;
}

Mesh& BinaryTreeNodePrivate::getMesh()
{
    return _mMesh;
}

const Mesh& BinaryTreeNodePrivate::getMesh() const
{
    return _mMesh;
}

void BinaryTreeNodePrivate::setCamera(const Camera &camera)
{
    _mCamera = camera;
}

Camera& BinaryTreeNodePrivate::getCamera()
{
    return _mCamera;
}

const Camera& BinaryTreeNodePrivate::getCamera() const
{
    return _mCamera;
}

void BinaryTreeNodePrivate::_calculateTranslation()
{
    std::shared_ptr<BinaryTreeNodePrivate> NodeLeft = _mNodeLeft;
    std::shared_ptr<BinaryTreeNodePrivate> NodeRight = _mNodeRight;
    std::weak_ptr<BinaryTreeNodePrivate>   NodeParentWeak = getParentTyped();
    std::shared_ptr<BinaryTreeNodePrivate> NodeParent = NodeParentWeak.lock();
    std::shared_ptr<BinaryTreeNodePrivate> NodeThis;
    
    bool isleft = false;
    bool isright = false;
    
    if(NodeParent)
    {
        if(NodeParent->_mNodeRight.get() == this) {
            NodeThis = NodeParent->_mNodeRight;
            isleft = false; isright = true;
        } else if(NodeParent->_mNodeLeft.get() == this) {
            NodeThis = NodeParent->_mNodeLeft;
            isleft = true; isright = false;
        }
    }
    
    if(NodeRight && NodeParent)
    {
        if(isright)
        {
            NodeParent->_mNodeRight = NodeRight;
            NodeRight->_mNodeParent = _mNodeParent;
            
            if(NodeRight->_mNodeLeft)
            {
                NodeLeft->addChild(NodeRight->_mNodeLeft->toNode());
                NodeRight->_mNodeLeft = NodeLeft;
            }
            
            if(NodeRight->_mNodeRight)
            {
                
            }
        }
        
        if(isleft)
        {
            NodeParent->_mNodeLeft = NodeRight;
            NodeRight->_mNodeParent = _mNodeParent;
            
            if(NodeRight->_mNodeLeft)
            {
                NodeLeft->addChild(NodeRight->_mNodeLeft->toNode());
                NodeRight->_mNodeLeft = NodeLeft;
            }
            
            if(NodeRight->_mNodeRight)
            {
                
            }
        }
    }
    
    if(NodeParent && NodeThis)
    {
        Node NodeRoot = _mScene.getRoot();
        NodeRoot.addChild(Node(ResourceUser(NodeThis)));
    }
}

Node BinaryTreeNodePrivate::findParent(const Node& child, Node thisNode)
{
    if(child.lock() && thisNode.lock())
    {
        auto ptr = _mNodeLeft.get();
        if(ptr == child.lock().get()) {
            return thisNode;
        }
        
        ptr = _mNodeRight.get();
        if(ptr == child.lock().get()) {
            return thisNode;
        }
        
        Node rettmp = _mNodeLeft->findParent(child, Node(ResourceUser(_mNodeLeft)));
        if(rettmp.lock())
            return rettmp;
        rettmp = _mNodeRight->findParent(child, Node(ResourceUser(_mNodeRight)));
        if(rettmp.lock())
            return rettmp;
    }
    
    return Node::Null;
}

void BinaryTreeNodePrivate::setParent(const Node& node)
{
    _mNodeParent = node;
}

Node BinaryTreeNodePrivate::getNodeMostRight() const
{
    NodeTyped NodeMostRight = _mNodeRight;
    std::weak_ptr<BinaryTreeNodePrivate> NodeMostRightWeak;
    while(NodeMostRight) {
        NodeMostRightWeak = NodeMostRight;
        NodeMostRight = NodeMostRight->_mNodeRight;
    }
    
    return Node(NodeMostRightWeak);
}

const std::weak_ptr<BinaryTreeNodePrivate> BinaryTreeNodePrivate::getParentTyped() const
{
    return std::dynamic_pointer_cast<BinaryTreeNodePrivate>(_mNodeParent.toWeakPtr().lock());
}

Node BinaryTreeNodePrivate::getNodeLeft() const
{
    return Node(_mNodeLeft);
}

BinaryTreeScenePrivate::BinaryTreeScenePrivate(const std::string& name)
: ScenePrivate(name), _mNodeRootWeak(), _mNodeRoot(std::make_shared<BinaryTreeNodePrivate>(getName()+"-node-root")), _mActiveCamera()
{
    _mNodeRootWeak = Node(ResourceUser(_mNodeRoot));
    _mNodeRoot->setThisNode(Node(_mNodeRoot));
}

BinaryTreeScenePrivate::~BinaryTreeScenePrivate()
{
    
}

Node& BinaryTreeScenePrivate::getRoot()
{
    return _mNodeRootWeak;
}

const Node& BinaryTreeScenePrivate::getRoot() const
{
    return _mNodeRootWeak;
}

NodePrivate* BinaryTreeScenePrivate::createNode(const Mesh &mesh)
{
    BinaryTreeNodePrivate* NodeReturn = new BinaryTreeNodePrivate(getName() + "-node");
    NodeReturn->setMesh(mesh);
    
    return NodeReturn;
}

NodePrivate* BinaryTreeScenePrivate::createNode(const Camera &camera)
{
    BinaryTreeNodePrivate* NodeReturn = new BinaryTreeNodePrivate(getName() + "-node");
    NodeReturn->setCamera(camera);
    
    return NodeReturn;
}

void BinaryTreeScenePrivate::clear()
{
    _mNodeRoot.reset();
    _mNodeRootWeak = Node::Null;
    _mActiveCamera.reset();
}

Camera& BinaryTreeScenePrivate::getCamera()
{
    auto ptr = _mActiveCamera.lock();
    if(ptr)
        return ptr->getCamera();
    return Camera::Null;
}

const Camera& BinaryTreeScenePrivate::getCamera() const
{
    auto ptr = _mActiveCamera.lock();
    if(ptr)
        return ptr->getCamera();
    return Camera::Null;
}

void NodeByFilterToVector(std::shared_ptr<BinaryTreeNodePrivate> curNode, std::vector<const Node>& retVector, std::shared_ptr<BinaryTreeNodePrivate> NodeUntill)
{
    retVector.push_back(Node(ResourceUser(curNode)));
    std::shared_ptr<BinaryTreeNodePrivate> parenttmp = curNode->getParentTyped().lock();
    if(parenttmp)
    {
        if(parenttmp->getNodeLeft().lock()) {
            retVector.push_back(Node(ResourceUser(parenttmp->getNodeLeft())));
        }
        
        if(parenttmp.get() != NodeUntill.get())
            NodeByFilterToVector(parenttmp, retVector, NodeUntill);
    }
}

std::vector<const Node> BinaryTreeScenePrivate::getNodesByFilter(Node::Filter filter) const
{
    std::vector<const Node> NodeVectorReturn;
    std::shared_ptr<BinaryTreeNodePrivate> NodeCamera = _mActiveCamera.lock();
    
    if(NodeCamera)
    {
        std::shared_ptr<BinaryTreeNodePrivate> ptr = std::dynamic_pointer_cast<BinaryTreeNodePrivate>(NodeCamera->getNodeMostRight().lock());
        if(ptr)
        {
            NodeByFilterToVector(ptr, NodeVectorReturn, NodeCamera);
        }
    }
    
    return NodeVectorReturn;
}

void BinaryTreeScenePrivate::setActiveCamera(const Node &cameraNode)
{
    _mActiveCamera = std::dynamic_pointer_cast<BinaryTreeNodePrivate>(cameraNode.lock());
}

Node BinaryTreeScenePrivate::_retrieveNodeParent(const Node &child) const
{
    Node ret = Node::Null;
    
    if(child.lock())
        ret = _mNodeRoot->findParent(child, Node(ResourceUser(_mNodeRoot)));
    
    return ret;
}

BinaryTreeSceneLoader::BinaryTreeSceneLoader()
{
    
}

BinaryTreeSceneLoader::~BinaryTreeSceneLoader()
{
    
}

bool BinaryTreeSceneLoader::isTypeSupported(Resource::Type type) const
{
    return type == Resource::Type::Scene;
}

Resource* BinaryTreeSceneLoader::load(Resource::Type type, const std::string &name) const
{
    return new BinaryTreeScenePrivate(name);
}

ResourceLoader* BinaryTreeSceneLoader::clone() const
{
    return (ResourceLoader*) new BinaryTreeSceneLoader;
}

GreEndNamespace
 */
