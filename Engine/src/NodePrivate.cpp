//
//  NodePrivate.cpp
//  GRE
//
//  Created by Jacques Tronconi on 06/01/2016.
//
//

#include "NodePrivate.h"

#include "Node.h"
#include "Scene.h"

GRE_BEGIN_NAMESPACE

NodePrivate::NodePrivate(const std::string& name)
: Resource(name), _mIsVisible(false), _mThisNode()
{
    
}

NodePrivate::~NodePrivate()
{
    
}

Node NodePrivate::addChild(const Node& child)
{
    GreDebugFunctionNotImplemented();
    return Node::Null;
}

Node NodePrivate::addChild(NodePrivate *child)
{
    GreDebugFunctionNotImplemented();
    return Node::Null;
}

void NodePrivate::translate(const Vector3 &vector)
{
    _mMatrix = glm::translate(_mMatrix, vector);
}

void NodePrivate::rotate(float angle, const Vector3 &axe)
{
    _mMatrix = glm::rotate(_mMatrix, glm::radians(angle), axe);
}

void NodePrivate::setVisible(bool visible)
{
    GreDebugFunctionNotImplemented();
    _mIsVisible = visible;
}

bool NodePrivate::isVisible() const
{
    GreDebugFunctionNotImplemented();
    return _mIsVisible;
}

void NodePrivate::setMesh(const Mesh &mesh)
{
    GreDebugFunctionNotImplemented();
}

Mesh& NodePrivate::getMesh()
{
    GreDebugFunctionNotImplemented();
    return Mesh::Null;
}

const Mesh& NodePrivate::getMesh() const
{
    GreDebugFunctionNotImplemented();
    return Mesh::Null;
}

void NodePrivate::setCamera(const Camera &camera)
{
    GreDebugFunctionNotImplemented();
}

Camera& NodePrivate::getCamera()
{
    GreDebugFunctionNotImplemented();
    return Camera::Null;
}

const Camera& NodePrivate::getCamera() const
{
    GreDebugFunctionNotImplemented();
    return Camera::Null;
}

void NodePrivate::setParent(const Node& node)
{
    GreDebugFunctionNotImplemented();
}

void NodePrivate::setScene(const Gre::Scene &scene)
{
    GreDebugFunctionNotImplemented();
}

Scene NodePrivate::getScene() const
{
    GreDebugFunctionNotImplemented();
    return Scene::Null;
}

void NodePrivate::setThisNode(const Node &thisNode)
{
    _mThisNode = thisNode.toWeakPtr();
}

Node NodePrivate::toNode() const
{
    return Node(_mThisNode);
}

Matrix4& NodePrivate::getNodeMatrix()
{
    return _mMatrix;
}

const Matrix4& NodePrivate::getNodeMatrix() const
{
    return _mMatrix;
}

void NodePrivate::setNodeMatrix(const Matrix4 &mat4)
{
    _mMatrix = mat4;
}

GRE_END_NAMESPACE
