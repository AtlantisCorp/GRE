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
#include "Node.h"
#include "Mesh.h"
#include "Transformation.h"

GreBeginNamespace

class SceneManager;

//////////////////////////////////////////////////////////////////////
/// @class SceneNode
/// @brief Specialized Node to contains particular Objects as Mesh.
/// 
/// The SceneNode is a Node (i.e. contains a parent node and one or more
/// children nodes), also holding some informations as transformations.
/// The SceneNode is not guaranteed to be hold in the bounding box from
/// its parent. This behaviour is implementation dependent. 
///
/// The SceneNode can creates child using the same creator used to create
/// this SceneNode. 
///
/// Every SceneNode has a generated Id. This Id is dependent from the
/// SceneManager that created it. @see SceneManager::createIdentifier().
//////////////////////////////////////////////////////////////////////
class DLL_PUBLIC SceneNodePrivate : public NodePrivate
{
public:
    
    POOLED(Pools::Scene)
	
	/// @brief Identifies a node.
	typedef uint64_t Identifier;
	
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
	/// @brief Creates a SceneNodePrivate.
	//////////////////////////////////////////////////////////////////////
	SceneNodePrivate(const SceneManager& creator);
	
	//////////////////////////////////////////////////////////////////////
	/// @brief Creates a SceneNodePrivate with name.
	//////////////////////////////////////////////////////////////////////
	SceneNodePrivate(const SceneManager& creator, const std::string& name);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Creates a SceneNodePrivate with a name and a Mesh shared
    /// object.
    //////////////////////////////////////////////////////////////////////
    SceneNodePrivate(const SceneManager& creator, const std::string& name, const Mesh& renderable);
	
	//////////////////////////////////////////////////////////////////////
	/// @brief Destroys a SceneNodePrivate.
	//////////////////////////////////////////////////////////////////////
	virtual ~SceneNodePrivate();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns true if this SceneNode contains a Mesh.
    //////////////////////////////////////////////////////////////////////
    virtual bool isRenderable() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the Mesh rendered in this Node.
    //////////////////////////////////////////////////////////////////////
    virtual const Mesh& getRenderable() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the Mesh rendered in this Node.
    //////////////////////////////////////////////////////////////////////
    virtual Mesh& getRenderable();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Sets a new Mesh to render in this Node.
    //////////////////////////////////////////////////////////////////////
    virtual void setRenderable(const Mesh& mesh);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns true if the Mesh object has changed since the last
    /// update.
    //////////////////////////////////////////////////////////////////////
    virtual bool isRenderableChanged() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Change the local transformation applied to this Node.
    //////////////////////////////////////////////////////////////////////
    virtual void setTransformation(const Transformation& transformation);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the local transformation applied to this Node.
    //////////////////////////////////////////////////////////////////////
    virtual const Transformation& getTransformation() const;
	
protected:
	
	/// @brief Creator of this SceneNodePrivate.
	SceneManager* iCreator;
	
	/// @brief Identifier of this node.
	Identifier iIdentifier;
    
    /// @brief Contains a Mesh if this Node is renderable. Else, contains
    /// Mesh::Null.
    Mesh iRenderable;
    
    /// @brief Flags set to true when the Mesh object has changed and no update
    /// has been done yet.
    mutable bool iRenderableChanged;
    
    /// @brief Holds transformation properties.
    Transformation iTransformation;
    
    /// @brief Flags set to true when the iTransformation property has changed.
    mutable bool iTransformationChanged;
};

/// @brief Common typedef to specialize ResourceHolder.
typedef SpecializedResourceHolder<SceneNodePrivate> SceneNodeHolder;

/// @brief Common typedef to specialize SpecializedResourceHolderList.
typedef SpecializedResourceHolderList<SceneNodePrivate> SceneNodeHolderList;

//////////////////////////////////////////////////////////////////////
/// @brief Specialized Node for SceneNodePrivate.
//////////////////////////////////////////////////////////////////////
class SceneNode : public Node, public SpecializedResourceUser<SceneNodePrivate>
{
public:
    
    POOLED(Pools::Scene)
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Constructs a SceneNode from raw pointer.
    //////////////////////////////////////////////////////////////////////
    SceneNode(SceneNodePrivate* node);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Constructs a SceneNode from Holder.
    //////////////////////////////////////////////////////////////////////
    SceneNode(const SceneNodeHolder& holder);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Constructs a SceneNode from User.
    //////////////////////////////////////////////////////////////////////
    SceneNode(const SceneNode& user);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Destroys the SceneNode.
    //////////////////////////////////////////////////////////////////////
    virtual ~SceneNode();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Creates a new ResourceHolder in order to use the Resource.
    //////////////////////////////////////////////////////////////////////
    SpecializedResourceHolder<SceneNodePrivate> lock();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Creates a new ResourceHolder in order to use the Resource.
    //////////////////////////////////////////////////////////////////////
    const SpecializedResourceHolder<SceneNodePrivate> lock() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns true if this SceneNode contains a Mesh.
    //////////////////////////////////////////////////////////////////////
    bool isRenderable() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the Mesh rendered in this Node.
    //////////////////////////////////////////////////////////////////////
    const Mesh& getRenderable() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the Mesh rendered in this Node.
    //////////////////////////////////////////////////////////////////////
    Mesh& getRenderable();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Sets a new Mesh to render in this Node.
    //////////////////////////////////////////////////////////////////////
    void setRenderable(const Mesh& mesh);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns true if the Mesh object has changed since the last
    /// update.
    //////////////////////////////////////////////////////////////////////
    bool isRenderableChanged() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Change the local transformation applied to this Node.
    //////////////////////////////////////////////////////////////////////
    void setTransformation(const Transformation& transformation);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the local transformation applied to this Node.
    //////////////////////////////////////////////////////////////////////
    const Transformation& getTransformation() const;
    
    /// @brief Null Object.
    static SceneNode Null;
};

/// @brief std::list for SceneNode.
typedef std::list<SceneNode> SceneNodeList;

GreEndNamespace

#endif // __SCENENODE_H__

