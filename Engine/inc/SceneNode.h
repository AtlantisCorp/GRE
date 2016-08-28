//////////////////////////////////////////////////////////////////////
//
//  SceneNode.h
//  This source file is part of Gre 
//		(Gang's Resource Engine)
//
//  Copyright (c) 2015 - 2016 Luk2010
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
#include "Resource.h"
#include "Mesh.h"
#include "Transformation.h"
#include "BoundingBox.h"
#include "Camera.h"

GreBeginNamespace

/// @brief Identifies a node.
typedef uint64_t RenderNodeIdentifier;

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
class DLL_PUBLIC RenderNodePrivate : public Resource
{
public:
    
    POOLED(Pools::Scene)
    
    /// @brief Pre-holder typedef.
    typedef SpecializedResourceHolder<RenderNodePrivate> RenderNodeHolder;
    
    /// @brief Pre-holder list.
    typedef SpecializedResourceHolderList<RenderNodePrivate> RenderNodeHolderList;
    
    /// @brief Easier Id.
    typedef RenderNodeIdentifier Identifier;
	
	/// @brief A simple class to generate Identifiers.
	class Identificator
	{
	public:
		
		/// @brief Returns a valid Identifier.
		static Identifier Create();
		
		/// @brief Returns last created Identifier, or invalid.
		static Identifier Last();
		
		/// @brief Returns true if Identifier is valid.
		static bool IsValid(Identifier id);
		
		/// @brief Identifier counter.
		static Identifier identifier;
	};
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    RenderNodePrivate();
	
	//////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////
    RenderNodePrivate(const std::string& name);
	
	//////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////
	virtual ~RenderNodePrivate();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns true if this RenderNode contains a Mesh.
    //////////////////////////////////////////////////////////////////////
    virtual bool isRenderable() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the Mesh rendered in this Node.
    //////////////////////////////////////////////////////////////////////
    virtual const Mesh& getMesh() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the Mesh rendered in this Node.
    //////////////////////////////////////////////////////////////////////
    virtual Mesh& getMesh();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Sets a new Mesh to render in this Node.
    //////////////////////////////////////////////////////////////////////
    virtual void setMesh(const Mesh& mesh);
    
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
    /// @brief Returns every Childs this Node has.
    //////////////////////////////////////////////////////////////////////
    virtual RenderNodeHolderList& getChilds();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns every Childs this Node has.
    //////////////////////////////////////////////////////////////////////
    virtual const RenderNodeHolderList& getChilds() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Finds RenderNode from its identifier.
    //////////////////////////////////////////////////////////////////////
    virtual RenderNodeHolder find ( RenderNodeIdentifier identifier );
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Finds RenderNode from its identifier.
    //////////////////////////////////////////////////////////////////////
    virtual const RenderNodeHolder find ( RenderNodeIdentifier identifier ) const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns Children visible by given CameraHolder.
    //////////////////////////////////////////////////////////////////////
    virtual RenderNodeHolderList getVisibleChildren ( const CameraHolder& camera ) const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Removes given Child.
    /// This function also removes every Child's Children. To unremove this
    /// Child's Children, use 'RenderNode::removeNotRecursive()' .
    //////////////////////////////////////////////////////////////////////
    virtual void remove ( const Identifier& identifier );
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Removes given Child but not its Children.
    /// Its Children are added to this Node using 'RenderNode::addNode()' .
    //////////////////////////////////////////////////////////////////////
    virtual void removeNotRecursive ( const Identifier& identifier );
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Removes every Child.
    //////////////////////////////////////////////////////////////////////
    virtual void clear();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the BoundingBox for this Node.
    //////////////////////////////////////////////////////////////////////
    virtual const BoundingBox& getBoundingBox() const;
    
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
    /// @brief Returns 'iIdentifier' property.
    //////////////////////////////////////////////////////////////////////
    virtual Identifier getIdentifier() const;
    
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
	
protected:
	
	/// @brief Identifier of this node.
	Identifier iIdentifier;
    
    /// @brief Contains a Mesh if this Node is renderable. Else, contains
    /// Mesh::Null.
    Mesh iRenderable;
    
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
    
    /// @brief Flags set to true when the iTransformation property has changed.
    mutable bool iTransformationChanged;
    
    /// @brief Flag set to true when 'iRenderable' has been changed.
    mutable bool iRenderableChanged;
};

/// @brief Common typedef to specialize ResourceHolder.
typedef SpecializedResourceHolder<RenderNodePrivate> RenderNodeHolder;

/// @brief Common typedef to specialize SpecializedResourceHolderList.
typedef SpecializedResourceHolderList<RenderNodePrivate> RenderNodeHolderList;

//////////////////////////////////////////////////////////////////////
/// @brief Specialized Node for RenderNodePrivate.
//////////////////////////////////////////////////////////////////////
class RenderNode : public SpecializedResourceUser<RenderNodePrivate>
{
public:
    
    POOLED(Pools::Scene)
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    RenderNode(const RenderNodePrivate* node);
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    RenderNode(const RenderNodeHolder& holder);
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    RenderNode(const RenderNode& user);
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual ~RenderNode();
    
    /// @brief Null Object.
    static RenderNode Null;
};

/// @brief std::list for RenderNode.
typedef std::list<RenderNode> RenderNodeList;

/*
RenderScene scene = ResourceManager::Get().getRenderSceneManager().load("MyScene", "Default");

RenderNodeHolder cube = scene.createNode("MyCube");
cube.setMesh(mycubemesh);
cube.translate(Vector3(0,0,1));
scene.addNode(cube);

cube.translate()
{
    iPosition = position;
    iParent->onChildNodeMoved(this)
    {
        if ( !node->getBoundingBox()->in(getBoundingBox()) )
        {
            RenderNodeHolder node ( node );
            this->remove(node);
            iParent->addNode(node);
            {
                if ( !node->getBoundingBox()->in(getBoundingBox()) )
                {
                    iParent->addNode(node);
                }
                else
                {
                    for ( child : nodes )
                    {
                        if ( node->boundingbox->in (child->boundingbox) )
                        {
                            child->addNode(node)
                            return;
                        }
                    }
                    
                    iChilds.push_back(node);
                    return;
                }
            }
        }
    }
}

Later, while updating...
 
 MyRenderSceneManager.onUpdateEvent ()
 {
    updateseveryscene();
    {
        scene.onUpdateEvent();
        {
            scene.updaterootnode();
        }
    }
 }
 
 To make a player :
 
 Camera::connectRenderNode(myplayerbignode);
 Camera::onKeyUp, onKeyDown -> myplayerbignode.translate, scale, rotate, ...
 
*/

GreEndNamespace

#endif // __SCENENODE_H__

