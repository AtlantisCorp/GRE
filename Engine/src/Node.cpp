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

Node::Node(const std::string& name)
: Resource(ResourceIdentifier::New() , name)
{
    iParent = nullptr;
    iType = Type::Root;
    iParentingBehaviour = ParentingBehaviour::TakePlace;
}

Node::Node(const std::string& name, Node* parent)
: Resource(ResourceIdentifier::New() , name)
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

Node::~Node()
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

Node* Node::getParent()
{
    return iParent;
}

const Node* Node::getParent() const
{
    return iParent;
}

Node* Node::findChild(const std::string &name)
{
    for(auto child : iChildren)
    {
        if(child->getName() == name)
            return child;
    }
    
    return nullptr;
}

const Node* Node::findChild(const std::string &name) const
{
    for(auto child : iChildren)
    {
        if(child->getName() == name)
            return child;
    }
    
    return nullptr;
}

bool Node::isRoot() const
{
    return iType == Type::Root;
}

Node::Type Node::getType() const
{
    return iType;
}

void Node::setParent(Gre::Node *parent)
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

void Node::addChild(Gre::Node *node)
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

void Node::removeChild(const std::string &name)
{
    for(auto it = iChildren.begin(); it != iChildren.end(); it++)
    {
        Node* child = *it;
        if(child->getName() == name)
        {
            iChildren.erase(it);
            child->onParentRemoved(this);
        }
    }
}

void Node::removeChild(Gre::Node *node)
{
    for(auto it = iChildren.begin(); it != iChildren.end(); it++)
    {
        Node* child = *it;
        if(child == node)
        {
            iChildren.erase(it);
            child->onParentRemoved(this);
            break;
        }
    }
}

void Node::removeAllChildren()
{
    while(!iChildren.empty())
    {
        Node* node = (*iChildren.begin());
        iChildren.erase(iChildren.begin());
        node->onParentRemoved(this);
    }
}

void Node::onChildConstructed(Gre::Node *child)
{
    if(!child)
        return;
    
    // Child has been constructed with argument us for parent.
    // We can add it without much assertions. We must avoid using
    // Node::addChild() because it would call onParentAdded(), which
    // would call setParent() which would call onChildRemoved() then onChildAdded(),
    // and this would call addChild()...
    
    if(child->getParent() == this)
    {
        iChildren.push_back(child);
    }
}

void Node::onChildDestroyed(Gre::Node *child)
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

void Node::onParentDestroyed(Gre::Node *parent)
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

void Node::onChildRemoved(Gre::Node *child)
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

void Node::onChildAdded(Gre::Node *child)
{
    if(!child)
        return;
    
    if(std::find(iChildren.begin(), iChildren.end(), child) == iChildren.end() &&
       child->getParent() == this)
    {
        iChildren.push_back(child);
    }
}

void Node::onParentAdded(Gre::Node *parent)
{
    iParent = parent;
}

void Node::onParentRemoved(Gre::Node *parent)
{
    // We assume 'destroying' a Parent and 'removing' a parent from the
    // Node's tree has, for a Child Node, the same meaning but subclass
    // can act differently.
    
    onParentDestroyed(parent);
}

GreEndNamespace
