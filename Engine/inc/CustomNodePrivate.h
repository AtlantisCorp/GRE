//
//  CustomNodePrivate.h
//  GRE
//
//  Created by Jacques Tronconi on 11/04/2016.
//
//

#ifndef GRE_CustomNodePrivate_h
#define GRE_CustomNodePrivate_h

#include "Pools.h"
#include "Resource.h"

#include "Mesh.h"
#include "Camera.h"
#include "AxisAlignedBoundingBox.h"

GreBeginNamespace

enum class CustomNodeType
{
    /// @brief Defines the Root Node of the Custom Tree.
    Root = 0,
    
    /// @brief Defines a Node that contains a Mesh.
    Mesh,
    
    /// @brief Defines a Node that contains a Camera.
    Camera
};

//////////////////////////////////////////////////////////////////////
/// @brief Defines the CustomNodePrivate process.
///
/// # The Custom Node Process
///
/// Each Node should be linked with 26 neighbourgs, as in a cube divided
/// for each face, in 3x3.
///
/// A 3x3 Cube is a Custom Node. The Custom Node's tree contains only
/// the 3x3 Cube, and objects that does not completly rely in one of those
/// subtrees.
///
/// The Object is a Custom Node, contained in a ZoneCustomNode which holds
/// a 3x3 Cube.
//////////////////////////////////////////////////////////////////////
class DLL_PUBLIC CustomNodePrivate : public Resource
{
public:
    
    CustomNodePrivate(const std::string& name, CustomNodeType type, CustomNodePrivate* parent = nullptr);
    virtual ~CustomNodePrivate();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Sets the Volume occupied by this Node.
    //////////////////////////////////////////////////////////////////////
    void setVolume(const Volume& v);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns a newly created CustomNode of type CustomNodeType::Mesh.
    //////////////////////////////////////////////////////////////////////
    ResourceUser addMeshNode(const Mesh& mesh, const Vector3& position);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns a newly created CustomNode of type CustomNodeType::Camera.
    //////////////////////////////////////////////////////////////////////
    ResourceUser addCameraNode(const Camera& camera, const Vector3& position);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the Center of the Node's space, without transformations.
    //////////////////////////////////////////////////////////////////////
    Vector3 getCenter() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Sets the Position of this Node, i.e. the translation's
    /// component.
    //////////////////////////////////////////////////////////////////////
    void setPosition(const Vector3& position);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the AxisAlignedBoundingBox containing this Node.
    //////////////////////////////////////////////////////////////////////
    AxisAlignedBoundingBox getAABoundingBox() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the BoundingBox for a Zone child.
    //////////////////////////////////////////////////////////////////////
    AxisAlignedBoundingBox getPartialAABoundingBox() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns true if this Node has one or more children.
    //////////////////////////////////////////////////////////////////////
    bool hasChildren() const;
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    
    
protected:
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Creates a persistent ResourceUser which must hold the desired
    /// CustomNode.
    //////////////////////////////////////////////////////////////////////
    virtual ResourceUser _createNode(const std::string& name, CustomNodeType type);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Adds the given node to this Node subtree, depending on its
    /// position.
    //////////////////////////////////////////////////////////////////////
    virtual bool _addCustomNode(ResourceUser& node, const Vector3& position);
    
protected:
    
    typedef std::vector<CustomNodePrivate*> ChildrenVector;
    
    /// @brief Holds the Type of CustomNode.
    CustomNodeType _mType;
    
    /// @brief Holds the Volume occupied by this CustomNode.
    Volume _mVolume;
    
    /// @brief Holds a pointer to the Parent Node.
    /// This raw pointer may be invalidated without any intervention.
    CustomNodePrivate* _mParent;
    
    /// @brief Holds the Position of this Node.
    Vector3 _mPosition;
    
    /// @brief Holds Children.
    ChildrenVector _mChildren;
};

GreEndNamespace

#endif
