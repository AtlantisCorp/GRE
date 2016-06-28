//
//  CustomNode.cpp
//  GRE
//
//  Created by Jacques Tronconi on 11/04/2016.
//
//

#include "CustomNode.h"

GreBeginNamespace

void CustomNode::setVolume(const Volume& v)
{
    auto ptr = _mNode.lock();
    if(ptr)
        ptr->setVolume(v);
}

CustomNode CustomNode::addMeshNode(const Gre::Mesh &mesh)
{
    auto ptr = _mNode.lock();
    if(ptr)
        return CustomNode(ptr->addMeshNode(mesh, Vector3(0,0,0)));
    return CustomNode::Null;
}

CustomNode CustomNode::addMeshNode(const Gre::Mesh &mesh, const Vector3& position)
{
    auto ptr = _mNode.lock();
    if(ptr)
        return CustomNode(ptr->addMeshNode(mesh, position));
    return CustomNode::Null;
}

CustomNode CustomNode::addCameraNode(const Gre::Camera &camera)
{
    auto ptr = _mNode.lock();
    if(ptr)
        return CustomNode(ptr->addCameraNode(camera));
    return CustomNode::Null;
}

Vector3 CustomNode::getCenter() const
{
    auto ptr = _mNode.lock();
    if(ptr)
        return ptr->getCenter();
    return Vector3(0,0,0);
}

void CustomNode::setPosition(const Vector3& position)
{
    auto ptr = _mNode.lock();
    if(ptr)
        ptr->setPosition(position);
}

AxisAlignedBoundingBox CustomNode::getAABoundingBox() const
{
    auto ptr = _mNode.lock();
    if(ptr)
        return ptr->getAABoundingBox();
    return AxisAlignedBoundingBox();
}

CustomNode CustomNode::Null = CustomNode();


GreEndNamespace
