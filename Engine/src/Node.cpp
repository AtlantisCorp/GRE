//
//  Node.cpp
//  GRE
//
//  Created by Jacques Tronconi on 14/12/2015.
//
//

#include "Node.h"

GRE_BEGIN_NAMESPACE

NodePrivate::NodePrivate(const std::string& name)
: Resource(name), _mIsVisible(false)
{
    
}

NodePrivate::~NodePrivate()
{
    
}

NodePrivate::NodePtr NodePrivate::addChild(NodePrivate::NodePtr child)
{
    return NodePtr();
}

void NodePrivate::translate(const Vector3 &vector)
{
    
}

void NodePrivate::setVisible(bool visible)
{
    _mIsVisible = visible;
}

bool NodePrivate::isVisible() const
{
    return _mIsVisible;
}

const Matrix4& NodePrivate::getTransformation() const
{
    return MatrixUtils::Zero4;
}

Mesh& NodePrivate::getMesh()
{
    return Mesh::Null;
}

const Mesh& NodePrivate::getMesh() const
{
    return Mesh::Null;
}

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

Node::~Node()
{
    
}

Node Node::addChild(Node child)
{
    auto ptr = _mNode.lock();
    if(ptr) {
        if(ptr->addChild(child._mNode).expired())
            return Node::Null;
        else {
            child.listen(*this);
            return child;
        }
    }
    
    return Node::Null;
}

void Node::translate(const Vector3 &vector)
{
    auto ptr = _mNode.lock();
    if(ptr)
        ptr->translate(vector);
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

const Matrix4& Node::getTransformation() const
{
    auto ptr = _mNode.lock();
    if(ptr)
        return ptr->getTransformation();
    return MatrixUtils::Zero4;
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

Node Node::Null = Node();

GRE_END_NAMESPACE
