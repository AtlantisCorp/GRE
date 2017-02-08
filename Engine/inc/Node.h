//////////////////////////////////////////////////////////////////////
//
//  Node.h
//  This source file is part of Gre
//		(Gang's Resource Engine)
//
//  Copyright (c) 2015 - 2016 Luk2010
//  Created on 14/12/2015.
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

#ifndef GRE_Node_h
#define GRE_Node_h

#include "Pools.h"
#include "Resource.h"

GreBeginNamespace

//////////////////////////////////////////////////////////////////////
/// @class NodePrivate
/// @brief Base class for Node objects.
///
/// A Node can be described as an object which have a Parent, and one
/// or more children. If a Node has no Parent, he is Root. If a Node
/// has no Child, he is Leaf. If it has a Parent and one or more children,
/// he is a Branch.
///
/// The Node class is made to be customized. The RenderNode class is an
/// example of a customization, for example to handle Mesh objects, and
/// to be constructible by the RenderScene.
///
/// The Node class does not have any information like transformation about
/// the Node, it only holds the Parent, the Children and the Type of Node,
/// in addition of the normal properties of a Resource.
///
/// @note The Node class holding is not made by the ResourceManager. You
/// must either use a NodeHolderList to keep track of the Node objects, or
/// use NodeHolder objects directly and not Node objects. If a Node is
/// created using formula `Node n = Node( new NodePrivate(...) );` , the
/// Node ResourceUser object will not keep track of the Object. The behaviour
/// of this line is not defined. You should instead use
/// `NodeHolder n = NodeHolder( new NodePrivate(...) );`.
//////////////////////////////////////////////////////////////////////
class DLL_PUBLIC Node : public Resource
{
public:
    
    /// @brief Describes some filters used when accessing Nodes.
    enum class Filter
    {
    	None,               ///< @brief No filter.
        FarthestToNearest,  ///< @brief Filter Nodes from the farthest to the nearest.
        NearestToFarthest,  ///< @brief Filter Nodes from the nearest to the farthest.
        Default             ///< @brief No filters.
    };
    
    /// @brief Types a Node can be.
    enum class Type
    {
        Root    = 0, ///< @brief Node has no Parent.
        Leaf    = 1, ///< @brief Node has a Parent, but no Child.
        Branch  = 2  ///< @brief Node has a Parent and one or more Children.
    };
    
    /// @brief Behaviours when Parent dies.
    enum class ParentingBehaviour
    {
        TakePlace = 0, ///< @brief Default behaviour, the Child Node is added as Child to the parent
                        /// of its Parent.
        BecomeRoot = 1 ///< @brief The Node becomes Root.
    };
    
public:
    
    POOLED(Pools::Resource)
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Creates a Node with no parent.
    //////////////////////////////////////////////////////////////////////
    Node(const std::string& name);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Creates a Node with a parent.
    //////////////////////////////////////////////////////////////////////
    Node(const std::string& name, Node* parent);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Destroys the Node.
    ///
    /// This function will notifiate the Parent and the Children in order
    /// to make a correct deletion of the Node.
    //////////////////////////////////////////////////////////////////////
    virtual ~Node();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Return the Parent Node or nullptr.
    //////////////////////////////////////////////////////////////////////
    virtual Node* getParent();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the Parent Node or nullptr.
    //////////////////////////////////////////////////////////////////////
    virtual const Node* getParent() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Finds a Node with given name.
    //////////////////////////////////////////////////////////////////////
    virtual const Node* findChild(const std::string& name) const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Finds a Node with given name.
    //////////////////////////////////////////////////////////////////////
    virtual Node* findChild(const std::string& name);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns true if Root.
    //////////////////////////////////////////////////////////////////////
    virtual bool isRoot() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the Node's type.
    //////////////////////////////////////////////////////////////////////
    virtual Type getType() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Sets a new Parent.
    //////////////////////////////////////////////////////////////////////
    virtual void setParent(Node* parent);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Adds a Node to this one.
    //////////////////////////////////////////////////////////////////////
    virtual void addChild(Node* node);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Removes a Child from this Node.
    //////////////////////////////////////////////////////////////////////
    virtual void removeChild(const std::string& name);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Removes a Child from this Node.
    //////////////////////////////////////////////////////////////////////
    virtual void removeChild(Node* node);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Removes every Child in this Node.
    //////////////////////////////////////////////////////////////////////
    virtual void removeAllChildren();
    
protected:
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Called by a Child Node constructed with a parent arg.
    /// The parent should add the Node to its children list.
    //////////////////////////////////////////////////////////////////////
    virtual void onChildConstructed(Node* child);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Called by a Child Node when destroyed. The parent Node should
    /// act in consequence and assert the Child is removed from the
    /// children list.
    //////////////////////////////////////////////////////////////////////
    virtual void onChildDestroyed(Node* child);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Called by a Parent Node to all its children. The Child should
    /// either becoming a Root Node or getting the parent's Parent Node as
    /// its parent.
    //////////////////////////////////////////////////////////////////////
    virtual void onParentDestroyed(Node* parent);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Called by a Child Node when (1) it is removed from the Parent
    /// Node or (2) its Parent Node changes. The Parent Node should assert
    /// this Child Node is removed from the children list.
    //////////////////////////////////////////////////////////////////////
    virtual void onChildRemoved(Node* child);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Called by a Child Node when (1) it is added to the Parent Node
    /// manually or (2) when its Parent Node changes. The Parent Node should
    /// assert the Node is added to the children list.
    //////////////////////////////////////////////////////////////////////
    virtual void onChildAdded(Node* child);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Called by a Parent Node when NodePrivate::addChild is used.
    /// The Child Node should assert its parent is the given one.
    //////////////////////////////////////////////////////////////////////
    virtual void onParentAdded(Node* parent);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Called by a Parent Node when NodePrivate::removeChild is used.
    /// The Child Node should assert its parent is (1) nullptr or (2) the
    /// Parent Node's parent.
    //////////////////////////////////////////////////////////////////////
    virtual void onParentRemoved(Node* parent);
    
private:
    
    /// @brief The parent Node. If Node::Null, this Node is considered as
    /// a Root Node.
    Node* iParent;
    
    /// @brief A List of Children.
    std::list<Node*> iChildren;
    
    /// @brief Type of this Node.
    Type iType;
    
    /// @brief Behaviour when Parent dies.
    ParentingBehaviour iParentingBehaviour;
};

/// @brief Specialization of SpecializedCountedObjectHolder.
typedef SpecializedCountedObjectHolder<Node> NodeHolder;

/// @brief Specialization of SpecializedResourceHolderList.
typedef SpecializedResourceHolderList<Node> NodeHolderList;

/// @brief SpecializedCountedObjectUser.
typedef SpecializedCountedObjectUser<Node> NodeUser;

/// @brief Specialization of std::list for Node.
typedef std::list< NodeUser > NodeList;

GreEndNamespace

#endif
