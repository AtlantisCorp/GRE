//
//  Node.cpp
//  GRE
//
//  Created by Jacques Tronconi on 14/12/2015.
//
//

#include "Node.h"

GRE_BEGIN_NAMESPACE

Node::Node()
: ResourceUser(), Transmitter(std::string()), _mNode()
{
    
}

Node::Node(Node&& rhs)
: ResourceUser(rhs), Transmitter(rhs.Listener::getName()), _mNode(rhs._mNode)
{
    
}

Node::Node(const Node& rhs)
: ResourceUser(rhs), Transmitter(rhs.Listener::getName()), _mNode(rhs._mNode)
{
    
}

Node::Node(const ResourceUser& rhs)
: ResourceUser(rhs), Transmitter(std::string()), _mNode(std::dynamic_pointer_cast<NodePrivate>(rhs.lock()))
{
    
}

Node::Node(std::weak_ptr<NodePrivate> rhs)
: ResourceUser(rhs), Transmitter(std::string()), _mNode(rhs)
{
    
}

Node::~Node()
{
    
}

Node& Node::operator = (const Node& node)
{
    ResourceUser::operator=(node);
    _mNode = node._mNode;
    return *this;
}

Node Node::addChild(const Node& child)
{
    auto ptr = _mNode.lock();
    
    if(ptr) {
        if(ptr->addChild(child).expired()) {
            return Node::Null;
        }
        else {
            return child;
        }
    }
    
    return Node::Null;
}

Node Node::addChild(NodePrivate *child)
{
    auto ptr = _mNode.lock();
    
    if(ptr)
        return ptr->addChild(child);
    return Node::Null;
}

void Node::translate(const Vector3 &vector)
{
    auto ptr = _mNode.lock();
    
    if(ptr)
        ptr->translate(vector);
}

void Node::rotate(float angle, const Vector3 &axe)
{
    auto ptr = _mNode.lock();
    
    if(ptr)
        ptr->rotate(angle, axe);
}

void Node::setVisible(bool visible)
{
    auto ptr = _mNode.lock();
    
    if(ptr)
        ptr->setVisible(visible);
}

bool Node::isVisible() const
{
    auto ptr = _mNode.lock();
    
    if(ptr)
        return ptr->isVisible();
    return false;
}

void Node::setMesh(const Mesh &mesh)
{
    auto ptr = _mNode.lock();
    
    if(ptr)
        ptr->setMesh(mesh);
}

Mesh& Node::getMesh()
{
    auto ptr = _mNode.lock();
    
    if(ptr)
        return ptr->getMesh();
    
    return Mesh::Null;
}

const Mesh& Node::getMesh() const
{
    auto ptr = _mNode.lock();
    
    if(ptr)
        return ptr->getMesh();
    
    return Mesh::Null;
}

void Node::setCamera(const Camera &camera)
{
    auto ptr = _mNode.lock();
    
    if(ptr)
        ptr->setCamera(camera);
}

Camera& Node::getCamera()
{
    auto ptr = _mNode.lock();
    
    if(ptr)
        return ptr->getCamera();
    
    return Camera::Null;
}

const Camera& Node::getCamera() const
{
    auto ptr = _mNode.lock();
    
    if(ptr)
        return ptr->getCamera();
    
    return Camera::Null;
}

std::weak_ptr<NodePrivate> Node::toWeakPtr()
{
    return _mNode;
}

const std::weak_ptr<NodePrivate> Node::toWeakPtr() const
{
    return _mNode;
}

void Node::setParent(const Node &node)
{
    auto ptr = _mNode.lock();
    
    if(ptr)
        ptr->setParent(node);
}

Matrix4& Node::getNodeMatrix()
{
    auto ptr = _mNode.lock();
    if(ptr)
        return ptr->getNodeMatrix();
    return MatrixUtils::Identity;
}

const Matrix4& Node::getNodeMatrix() const
{
    auto ptr = _mNode.lock();
    if(ptr)
        return ptr->getNodeMatrix();
    return MatrixUtils::Identity;
}

void Node::setNodeMatrix(const Matrix4 &mat4)
{
    auto ptr = _mNode.lock();
    if(ptr)
        ptr->setNodeMatrix(mat4);
}

Node Node::Null = Node();

GRE_END_NAMESPACE
