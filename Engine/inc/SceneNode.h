//////////////////////////////////////////////////////////////////////
//
//  SceneNode.h
//  This source file is part of Gre
//		(Gang's Resource Engine)
//
//  Copyright (c) 2015 - 2017 Luk2010
//  Created on 11/06/2016.
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

#ifndef __SCENENODE_H__
#define __SCENENODE_H__

#include "Pools.h"
#include "Renderable.h"
#include "Mesh.h"
#include "Transformation.h"
#include "BoundingBox.h"
#include "Camera.h"
#include "TechniqueParamBinder.h"

GreBeginNamespace

/// @brief Identifies a node.
typedef uint64_t RenderNodeIdentifier;

////////////////////////////////////////////////////////////////////////
/// @brief Defines a type a RenderNode may have. Notes that a RenderNode
/// is always renderable if it has a Mesh.
enum class RenderNodeType : int
{
    Default ,
    Light
};

/// @brief Uses this definition here because RenderScene should be able
/// to access nodes protected methods.
class RenderScene ;

//////////////////////////////////////////////////////////////////////
/// @brief Specialized Node to contains particular Objects as Mesh.
///
/// The RenderNode is an updatable Object. It can holds a Mesh, or only
/// describe a Part of a Space for a Scene Division.
///
/// Every RenderNode has a generated Id using 'Identificator::Create()'.
///
/// You can add a RenderNode using 'RenderScene::addNode()' or
/// 'RenderNode::addNode()'. In the default implementation, if the
/// BoundingBox from the futur child node is in the BoundingBox of
/// the Node, the Child will be added. If not, it will be added to the
/// Parent's Node using same technic.
///
//////////////////////////////////////////////////////////////////////
class DLL_PUBLIC RenderNode : public Renderable
{
public:

    /// @brief RenderNode can be accessed by the render scene.
    friend class RenderScene ;

    POOLED ( Pools::Scene )

    /// @brief Pre-holder typedef.
    typedef SpecializedCountedObjectHolder<RenderNode> RenderNodeHolder;

    /// @brief Pre-holder list.
    typedef SpecializedResourceHolderList<RenderNode> RenderNodeHolderList;

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    RenderNode ();

	//////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////
    RenderNode ( const std::string& name );

	//////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////
	virtual ~RenderNode () noexcept ( false ) ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Returns true if this RenderNode contains a Mesh.
    //////////////////////////////////////////////////////////////////////
    virtual bool isRenderable() const;

    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the Mesh rendered in this Node.
    //////////////////////////////////////////////////////////////////////
    virtual const MeshHolder& getMesh() const;

    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the Mesh rendered in this Node.
    //////////////////////////////////////////////////////////////////////
    virtual MeshHolder& getMesh();

    //////////////////////////////////////////////////////////////////////
    /// @brief Sets a new Mesh to render in this Node.
    //////////////////////////////////////////////////////////////////////
    virtual void setMesh(const MeshHolder& mesh);

    //////////////////////////////////////////////////////////////////////
    /// @brief Change the local transformation applied to this Node.
    //////////////////////////////////////////////////////////////////////
    virtual void setTransformation(const Transformation& transformation);

    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the local transformation applied to this Node.
    //////////////////////////////////////////////////////////////////////
    virtual const Transformation& getTransformation() const;

    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the Model Matrix applied to this Node and the
    /// parents.
    //////////////////////////////////////////////////////////////////////
    virtual Matrix4 getModelMatrix() const;

    //////////////////////////////////////////////////////////////////////
    /// @brief Adds a Child to this Node.
    //////////////////////////////////////////////////////////////////////
    virtual void addNode ( const RenderNodeHolder& node );

    //////////////////////////////////////////////////////////////////////
    /// @brief Returns this Parent's Node, if it has one.
    //////////////////////////////////////////////////////////////////////
    virtual const RenderNodeHolder & getParent () const ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Returns every Children this Node has.
    //////////////////////////////////////////////////////////////////////
    virtual RenderNodeHolderList& getChildren();

    //////////////////////////////////////////////////////////////////////
    /// @brief Returns every Children this Node has.
    //////////////////////////////////////////////////////////////////////
    virtual const RenderNodeHolderList& getChildren() const;

    //////////////////////////////////////////////////////////////////////
    /// @brief True if has one or more children.
    //////////////////////////////////////////////////////////////////////
    virtual bool hasChildren () const ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Finds RenderNode from its identifier.
    //////////////////////////////////////////////////////////////////////
    virtual RenderNodeHolder find ( ResourceIdentifier identifier );

    //////////////////////////////////////////////////////////////////////
    /// @brief Finds RenderNode from its identifier.
    //////////////////////////////////////////////////////////////////////
    virtual const RenderNodeHolder find ( ResourceIdentifier identifier ) const;

    //////////////////////////////////////////////////////////////////////
    /// @brief Removes given Child.
    /// This function also removes every Child's Children. To unremove this
    /// Child's Children, use 'RenderNode::removeNotRecursive()' .
    //////////////////////////////////////////////////////////////////////
    virtual void remove ( const ResourceIdentifier& identifier );

    //////////////////////////////////////////////////////////////////////
    /// @brief Removes given Child but not its Children.
    /// Its Children are added to this Node using 'RenderNode::addNode()' .
    //////////////////////////////////////////////////////////////////////
    virtual void removeNotRecursive ( const ResourceIdentifier& identifier );

    //////////////////////////////////////////////////////////////////////
    /// @brief Removes every Child.
    //////////////////////////////////////////////////////////////////////
    virtual void clear();

    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the BoundingBox for this Node.
    //////////////////////////////////////////////////////////////////////
    virtual const BoundingBox& getBoundingBox() const;

    //////////////////////////////////////////////////////////////////////
    /// @brief Changes the BoundingBox.
    //////////////////////////////////////////////////////////////////////
    virtual void setBoundingBox ( const BoundingBox& bbox ) ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Translates the RenderNode.
    //////////////////////////////////////////////////////////////////////
    virtual void translate ( const Vector3& vec );

    //////////////////////////////////////////////////////////////////////
    /// @brief Rotates the RenderNode.
    //////////////////////////////////////////////////////////////////////
    virtual void rotate ( float angle , const Vector3& axis );

    //////////////////////////////////////////////////////////////////////
    /// @brief Scales the RenderNode.
    //////////////////////////////////////////////////////////////////////
    virtual void scale ( float value );

    //////////////////////////////////////////////////////////////////////
    /// @brief Scales the RenderNode using the given 3 components.
    //////////////////////////////////////////////////////////////////////
    virtual void scale ( const Vector3& values ) ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Returns 'iMaterial'.
    //////////////////////////////////////////////////////////////////////
    virtual const MaterialHolder& getMaterial () const ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Returns 'iMaterial'.
    //////////////////////////////////////////////////////////////////////
    virtual MaterialHolder& getMaterial () ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Changes 'iMaterial'.
    //////////////////////////////////////////////////////////////////////
    virtual void setMaterial ( const MaterialHolder& material ) ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Returns true if bounding box intersects one frustrum plane,
    /// or if more than one plane intersects 'inside'.
    //////////////////////////////////////////////////////////////////////
    virtual bool isVisible ( const CameraHolder& camera ) const ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Listens to 'PositionChangedEvent' and 'DirectionChangedEvent'
    /// from the given object and adapts its position and direction.
    //////////////////////////////////////////////////////////////////////
    virtual void setPositionTracked ( const EventProceederUser & tracked ) ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Returns 'iPositionTracked'.
    //////////////////////////////////////////////////////////////////////
    virtual const EventProceederUser& getPositionTracked () const ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Binds customs and mesh parameters to the technique.
    //////////////////////////////////////////////////////////////////////
    virtual void use ( const TechniqueHolder & technique ) const ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the node's type.
    //////////////////////////////////////////////////////////////////////
    virtual const RenderNodeType & getNodeType () const ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Changes transformation's translation.
    //////////////////////////////////////////////////////////////////////
    virtual void setPosition ( const Vector3 & position ) ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Changes transformation's scale.
    //////////////////////////////////////////////////////////////////////
    virtual void setScale ( float value ) ;

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual void setVisible ( bool b ) ;

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual bool isVisible () const ;

protected:

    //////////////////////////////////////////////////////////////////////
    /// @brief Changes the RenderNode's parent.
    /// @note This function is protected in order to prevent anyone to change
    /// inaccurately the node's parent.
    //////////////////////////////////////////////////////////////////////
    virtual void setParent ( const RenderNodeHolder& parent );

    //////////////////////////////////////////////////////////////////////
    /// @brief Called when receiving Update Event.
    /// Updates 'iBoundingBox' and 'iModelMatrix' if needed.
    //////////////////////////////////////////////////////////////////////
    virtual void onUpdateEvent(const UpdateEvent& e);

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual void onPositionChangedEvent ( const PositionChangedEvent& e ) ;

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual void onDirectionChangedEvent ( const DirectionChangedEvent& e ) ;

protected:

    //////////////////////////////////////////////////////////////////////
    /// @brief Function used by the RenderScene to compute the children
    /// visible by the given camera in this node. The node should also add
    /// itself if visible.
    //////////////////////////////////////////////////////////////////////
    virtual void iComputeChildren (RenderNodeHolderList & result ,
                                   const CameraHolder & camera ,
                                   float elapsed) const ;

protected:

    /// @brief Node's type.
    RenderNodeType iNodeType ;

    /// @brief Contains a Mesh if this Node is renderable. Else, is invalid.
    MeshHolder iRenderable;

    /// @brief Holds transformation properties.
    Transformation iTransformation;

    /// @brief Childs' list.
    RenderNodeHolderList iChilds;

    /// @brief Parent Holder.
    RenderNodeHolder iParent;

    /// @brief Axis Aligned BoundingBox translated to this Node.
    BoundingBox iBoundingBox;

    /// @brief ModelMatrix used for the HardwareProgram.
    Matrix4 iModelMatrix;

    /// @brief
    MaterialHolder iMaterial ;

    /// @brief Holds a user to the tracked object.
    EventProceederUser iPositionTracked ;

    /// @brief Visible flag. If true , the node can compute its visibility from
    /// a given camera ('isVisible()' with a camera function).
    bool iVisible ;

    /// @brief Flags set to true when the iTransformation property has changed.
    mutable bool iTransformationChanged;

    /// @brief Flag set to true when 'iRenderable' has been changed.
    mutable bool iRenderableChanged;
};

/// @brief Common typedef to specialize ResourceHolder.
typedef SpecializedCountedObjectHolder < RenderNode > RenderNodeHolder;

/// @brief Common typedef to specialize SpecializedResourceHolderList.
typedef SpecializedResourceHolderList < RenderNode > RenderNodeHolderList;

/// @brief SpecializedCountedObjectUser for RenderNode.
typedef SpecializedCountedObjectUser < RenderNode > RenderNodeUser ;

GreEndNamespace

#endif // __SCENENODE_H__
