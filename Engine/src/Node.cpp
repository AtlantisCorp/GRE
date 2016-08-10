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

#include "Node.h"

GreBeginNamespace

NodePrivate::NodePrivate(const std::string& name)
: Resource(name)
{
    iParent = nullptr;
    iType = Type::Root;
    iParentingBehaviour = ParentingBehaviour::TakePlace;
}

NodePrivate::NodePrivate(const std::string& name, NodePrivate* parent)
: Resource(name)
{
    iParentingBehaviour = ParentingBehaviour::TakePlace;
    iParent = parent;
    if(parent)
    {
        iType = Type::Leaf;
        parent->onChildConstructed(this);
    }
    
    else
    {
        iType = Type::Root;
    }
}

NodePrivate::~NodePrivate()
{
    if(iParent)
    {
        iParent->onChildDestroyed(this);
    }
    
    if(iChildren.size())
    {
        for(auto child : iChildren)
        {
            child->onParentDestroyed(this);
        }
    }
}

NodePrivate* NodePrivate::getParent()
{
    return iParent;
}

const NodePrivate* NodePrivate::getParent() const
{
    return iParent;
}

NodePrivate* NodePrivate::findChild(const std::string &name)
{
    for(auto child : iChildren)
    {
        Node childnode (child);
        if(childnode.getName() == name)
            return child;
    }
    
    return nullptr;
}

const NodePrivate* NodePrivate::findChild(const std::string &name) const
{
    for(auto child : iChildren)
    {
        Node childnode (child);
        if(childnode.getName() == name)
            return child;
    }
    
    return nullptr;
}

bool NodePrivate::isRoot() const
{
    return iType == Type::Root;
}

NodePrivate::Type NodePrivate::getType() const
{
    return iType;
}

void NodePrivate::setParent(Gre::NodePrivate *parent)
{
    if(iParent)
    {
        iParent->onChildRemoved(this);
    }
    
    iParent = parent;
    
    if(iParent)
    {
        iParent->onChildAdded(this);
    }
}

void NodePrivate::addChild(Gre::NodePrivate *node)
{
    if(node)
    {
        if(std::find(iChildren.begin(), iChildren.end(), node) == iChildren.end())
        {
            iChildren.push_back(node);
            node->onParentAdded(this);
        }
    }
}

void NodePrivate::removeChild(const std::string &name)
{
    for(auto it = iChildren.begin(); it != iChildren.end(); it++)
    {
        NodePrivate* child = *it;
        if(child->getName() == name)
        {
            iChildren.erase(it);
            child->onParentRemoved(this);
        }
    }
}

void NodePrivate::removeChild(Gre::NodePrivate *node)
{
    for(auto it = iChildren.begin(); it != iChildren.end(); it++)
    {
        NodePrivate* child = *it;
        if(child == node)
        {
            iChildren.erase(it);
            child->onParentRemoved(this);
            break;
        }
    }
}

void NodePrivate::removeAllChildren()
{
    while(!iChildren.empty())
    {
        NodePrivate* node = (*iChildren.begin());
        iChildren.erase(iChildren.begin());
        node->onParentRemoved(this);
    }
}

void NodePrivate::onChildConstructed(Gre::NodePrivate *child)
{
    if(!child)
        return;
    
    // Child has been constructed with argument us for parent.
    // We can add it without much assertions. We must avoid using
    // NodePrivate::addChild() because it would call onParentAdded(), which
    // would call setParent() which would call onChildRemoved() then onChildAdded(),
    // and this would call addChild()...
    
    if(child->getParent() == this)
    {
        iChildren.push_back(child);
    }
}

void NodePrivate::onChildDestroyed(Gre::NodePrivate *child)
{
    if(!child)
        return;
    
    for(auto it = iChildren.begin(); it != iChildren.end(); it++)
    {
        if((*it) == child)
        {
            iChildren.erase(it);
            break;
        }
    }
}

void NodePrivate::onParentDestroyed(Gre::NodePrivate *parent)
{
    if(parent == iParent)
    {
        if(iParentingBehaviour == ParentingBehaviour::TakePlace)
        {
            iParent = parent->getParent();
        }
        
        else if(iParentingBehaviour == ParentingBehaviour::BecomeRoot)
        {
            iParent = nullptr;
        }
        
        if(iParent)
        {
            iParent->onChildAdded(this);
        }
        
        else
        {
            iType = Type::Root;
        }
    }
}

void NodePrivate::onChildRemoved(Gre::NodePrivate *child)
{
    if(!child)
        return;
    
    for(auto it = iChildren.begin(); it != iChildren.end(); it++)
    {
        if(*it == child)
        {
            iChildren.erase(it);
            break;
        }
    }
}

void NodePrivate::onChildAdded(Gre::NodePrivate *child)
{
    if(!child)
        return;
    
    if(std::find(iChildren.begin(), iChildren.end(), child) == iChildren.end() &&
       child->getParent() == this)
    {
        iChildren.push_back(child);
    }
}

void NodePrivate::onParentAdded(Gre::NodePrivate *parent)
{
    iParent = parent;
}

void NodePrivate::onParentRemoved(Gre::NodePrivate *parent)
{
    // We assume 'destroying' a Parent and 'removing' a parent from the
    // Node's tree has, for a Child Node, the same meaning but subclass
    // can act differently.
    
    onParentDestroyed(parent);
}

// ---------------------------------------------------------------------------------------------------

Node::Node(const NodePrivate* node)
: ResourceUser(node)
, SpecializedResourceUser<Gre::NodePrivate>(node)
{
    
}

Node::Node(const NodeHolder& holder)
: ResourceUser(holder)
, SpecializedResourceUser<Gre::NodePrivate>(holder)
{
    
}

Node::Node(const Node& user)
: ResourceUser(user)
, SpecializedResourceUser<Gre::NodePrivate>(user)
{
    
}

Node::~Node()
{
    
}

Node Node::getParent()
{
    auto ptr = lock();
    if(ptr)
    {
        return Node (ptr->getParent());
    }
    
    return Node::Null;
}

const Node Node::getParent() const
{
    auto ptr = lock();
    if(ptr)
    {
        return Node (ptr->getParent());
    }
    
    return Node::Null;
}

Node Node::findChild(const std::string &name)
{
    auto ptr = lock();
    if(ptr)
    {
        return Node (ptr->findChild(name));
    }
    
    return Node::Null;
}

const Node Node::findChild(const std::string &name) const
{
    auto ptr = lock();
    if(ptr)
    {
        return Node (ptr->findChild(name));
    }
    
    return Node::Null;
}

bool Node::isRoot() const
{
    auto ptr = lock();
    if(ptr)
    {
        return ptr->isRoot();
    }
    
    return Node::Null;
}

NodePrivate::Type Node::getType() const
{
    auto ptr = lock();
    if(ptr)
    {
        return ptr->getType();
    }
    
    return NodePrivate::Type::Root;
}

void Node::setParent(Gre::Node parent)
{
    auto ptr = lock();
    if(ptr)
    {
        ptr->setParent(parent.lock().get());
    }
}

void Node::addChild(Gre::Node node)
{
    auto ptr = lock();
    if(ptr)
    {
        ptr->addChild(node.lock().get());
    }
}

void Node::removeChild(const std::string &name)
{
    auto ptr = lock();
    if(ptr)
    {
        ptr->removeChild(name);
    }
}

void Node::removeChild(Gre::Node node)
{
    auto ptr = lock();
    if(ptr)
    {
        ptr->removeChild(node.lock().get());
    }
}

void Node::removeAllChildren()
{
    auto ptr = lock();
    if(ptr)
    {
        ptr->removeAllChildren();
    }
}

Node Node::Null = Node (nullptr);

GreEndNamespace
