//
//  CustomNodePrivate.cpp
//  GRE
//
//  Created by Jacques Tronconi on 11/04/2016.
//
//
/*
#include "CustomNodePrivate.h"

#include "MeshCustomNode.h"
#include "CameraCustomNode.h"

GreBeginNamespace

CustomNodePrivate::CustomNodePrivate(const std::string& name, CustomNodeType type, CustomNodePrivate* parent)
: Resource(name), _mType(type), _mParent(parent), _mPosition(0,0,0)
{
    _mVolume.x = _mVolume.y = _mVolume.z = 0.0f;
    
}

CustomNodePrivate::~CustomNodePrivate()
{
    _mParent = nullptr;
}

void CustomNodePrivate::setVolume(const Volume& v)
{
    _mVolume = v;
}

ResourceUser CustomNodePrivate::addMeshNode(const Gre::Mesh &mesh, const Vector3& position)
{
    ResourceUser newNode = _createNode(mesh.getName()+"-node", CustomNodeType::Mesh);
    _addCustomNode(newNode, position);
    
    MeshCustomNode newMeshNode = MeshCustomNode(newNode);
    newMeshNode.setMesh(mesh);
    
    return newMeshNode;
}

ResourceUser CustomNodePrivate::addCameraNode(const Camera& camera, const Vector3& position)
{
    ResourceUser newNode = _createNode(camera.getName()+"-node", CustomNodeType::Camera);
    _addCustomNode(newNode, position);
    
    CameraCustomNode newCameraNode = CameraCustomNode(newNode);
    newCameraNode.setCamera(camera);
    
    return newCameraNode;
}

Vector3 CustomNodePrivate::getCenter() const
{
    return Vector3(_mVolume.x/2, _mVolume.y/2, _mVolume.z/2);
}

void CustomNodePrivate::setPosition(const Vector3& position)
{
    _mPosition = position;
}

ResourceUser CustomNodePrivate::_createNode(const std::string& name, CustomNodeType type)
{
    ResourceUser node;
    
    // This function should create the Node depending on its type.
    if(type == CustomNodeType::Root)
    {
        // Root node is simply a raw CustomNode.
        node = ResourceUser(std::make_shared<CustomNodePrivate>(name, type, nullptr), true);
    }
    
    else if(type == CustomNodeType::Mesh)
    {
        node = ResourceUser(std::make_shared<MeshCustomNodePrivate>(name, type, nullptr), true);
    }
    
    else if(type == CustomNodeType::Camera)
    {
        node = ResourceUser(std::make_shared<CameraCustomNodePrivate>(name, type, nullptr), true);
    }
    
    else
    {
        node = CustomNode::Null;
    }
    
    return node;
}

bool CustomNodePrivate::_addCustomNode(Gre::ResourceUser &node, const Vector3 &position)
{
    // For now, Position is an Absolute position in space. We must check if this Position
    // might be or might not be in the 3x3 Cube.
    
    CustomNode cnode = CustomNode(node);
    
    AxisAlignedBoundingBox aabb = getAABoundingBox();
    if(aabb.hasVolume())
    {
        if(aabb.contains(cnode.getAABoundingBox().translate(position)))
        {
            if(cnode.getAABoundingBox().hasOneComponentSuperior(getPartialAABoundingBox()))
            {
                _mChildren.push_back(cnode);
                cnode._setParent(this);
                addListener(cnode);
                return true;
            }
            
            else
            {
                // Here we must determine which of the subcube will have the node.
                for(auto subcube : _mSubNodes)
                {
                    if(subcube.addCustomNode(node, position))
                    {
                        return true;
                    }
                }
            }
        }
    }
}
           
AxisAlignedBoundingBox CustomNodePrivate::getAABoundingBox() const
{
    return AxisAlignedBoundingBox(_mPosition, _mPosition + _mVolume.vec3);
}

AxisAlignedBoundingBox CustomNodePrivate::getPartialAABoundingBox() const
{
    if(hasChildren())
    {
        return getFirstChild().getAABoundingBox();
    }
}

bool CustomNodePrivate::hasChildren() const
{
    return _mChildren.size() > 0;
}

GreEndNamespace
*/