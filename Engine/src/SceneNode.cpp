//////////////////////////////////////////////////////////////////////
//
//  SceneNode.cpp
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

#include "SceneNode.h"

GreBeginNamespace

RenderNodePrivate::Identifier RenderNodePrivate::Identificator::identifier = 0;

RenderNodePrivate::Identifier RenderNodePrivate::Identificator::Create()
{
	return (identifier++);
}

RenderNodePrivate::Identifier RenderNodePrivate::Identificator::Last()
{
	return identifier;
}

bool RenderNodePrivate::Identificator::IsValid(RenderNodePrivate::Identifier id)
{
	return id > 0;
}

// ---------------------------------------------------------------------------------------------------

RenderNodePrivate::RenderNodePrivate()
: Gre::Resource(std::string("RenderNode#") + std::to_string(Identificator::Create()))
, iIdentifier(Identificator::Last())
, iRenderable(nullptr)
, iTransformation()
, iChilds()
, iParent(nullptr)
, iBoundingBox()
, iModelMatrix(0.0f)
, iTransformationChanged(false)
, iRenderableChanged(false)
{
    
}

RenderNodePrivate::RenderNodePrivate(const std::string& name)
: Gre::Resource(name)
, iIdentifier(Identificator::Create())
, iRenderable(nullptr)
, iTransformation()
, iChilds()
, iParent(nullptr)
, iBoundingBox()
, iModelMatrix(0.0f)
, iTransformationChanged(false)
, iRenderableChanged(false)
{
    
}

RenderNodePrivate::~RenderNodePrivate()
{
    
}

bool RenderNodePrivate::isRenderable() const
{
    return !iRenderable.isInvalid();
}

const Mesh& RenderNodePrivate::getMesh() const
{
    return iRenderable;
}

Mesh& RenderNodePrivate::getMesh()
{
    return iRenderable;
}

void RenderNodePrivate::setMesh(const Gre::Mesh &mesh)
{
    if ( !iRenderable.isInvalid() )
    {
        Resource::removeListener( iRenderable.getName() );
    }
    
    iRenderable = mesh;
    iRenderableChanged = true;
    
    if ( !iRenderable.isInvalid() )
    {
        Resource::addListener( iRenderable );
    }
}

void RenderNodePrivate::setTransformation(const Gre::Transformation &transformation)
{
    iTransformation = transformation;
    iTransformationChanged = true;
}

const Transformation& RenderNodePrivate::getTransformation() const
{
    return iTransformation;
}

Matrix4 RenderNodePrivate::getModelMatrix() const
{
    return iModelMatrix;
}

void RenderNodePrivate::addNode(const RenderNodeHolder &node)
{
    if ( !node.isInvalid() )
    {
        // First, check if this Node is not already one of our children. If this is the case,
        // this is probably a check from 'RenderNodePrivate::onUpdateEvent' when 'iTransformationChanged' is
        // true.
        
        auto it = iChilds.find(node);
        
        if ( it != iChilds.end() )
        {
            // We have to remove the Child from the list, and add it again. Removing the Node from the list
            // lets us adding it as if it was a new Node. Also, we remove the node from the listeners.
            
            iChilds.erase(it);
            Resource::removeListener( node->getName() );
            
            addNode(node);
            return;
        }
        
        if ( iBoundingBox.contains(node->getBoundingBox()) )
        {
            // We will add this Node as a Child. But, we must ensure any Child should have this Node as
            // a Valid Parent. This way we can sort BoundingBox here.
            
            RenderNodeHolderList tmplist;
            
            for ( auto it = iChilds.begin(); it != iChilds.end(); it++ )
            {
                if ( !(*it).isInvalid() )
                {
                    if ( node->getBoundingBox().contains((*it)->getBoundingBox()) )
                    {
                        // Here, the futur Node also contains this Node Child. Add this child to the Node, and remove
                        // this child from the listeners.
                        
                        Resource::removeListener( (*it)->getName() );
                        RenderNodeHolder(node)->addNode((*it));
                    }
                    
                    else
                    {
                        // We add this Child to the 'tmplist', in order to replace this Node's Child. Children in this
                        // list already are listening to this Node.
                        
                        tmplist.add((*it));
                    }
                }
            }
            
            // Replace the list with the correct one. This will destroy every RenderNodeHolder in the list, but
            // we have other ones in this one. Also, add the RenderNode to this one and add it to the Listeners list.
            
            iChilds = tmplist;
            iChilds.add(node);
            Resource::addListener( RenderNode(node) );
            RenderNodeHolder(node)->setParent(RenderNodeHolder(this));
        }
        
        else
        {
            // If this RenderNode do not contains this Node, just add it to the Parent. Keep in mind that,
            // when adding to a Parent, if 'node' contains this Node, it will replace this Node Parent.
            
            if ( !iParent.isInvalid() )
            {
                iParent->addNode(node);
            }
            
            // If we don't have any Parent, this means we are Root, but Root::getBoundingBox() should return
            // an infinite BoundingBox.
        }
    }
}

RenderNodeHolderList& RenderNodePrivate::getChilds()
{
    return iChilds;
}

const RenderNodeHolderList& RenderNodePrivate::getChilds() const
{
    return iChilds;
}

RenderNodeHolder RenderNodePrivate::find(RenderNodeIdentifier identifier)
{
    if ( Identificator::IsValid(identifier) )
    {
        for ( RenderNodeHolder& child : iChilds )
        {
            if ( !child.isInvalid() )
            {
                if ( child->getIdentifier() == identifier )
                {
                    return child;
                }
            }
        }
        
        for ( RenderNodeHolder& child : iChilds )
        {
            if ( !child.isInvalid() )
            {
                RenderNodeHolder ret = child->find(identifier);
                
                if ( !ret.isInvalid() )
                {
                    return ret;
                }
            }
        }
        
#ifdef GreIsDebugMode
        GreDebugPretty() << "'identifier' was not found in Children's list ('" << (int) identifier << "')." << std::endl;
#endif
        return RenderNodeHolder ( nullptr );
    }
    
    else
    {
#ifdef GreIsDebugMode
        GreDebugPretty() << "'identifier' is invalid." << std::endl;
#endif
        return RenderNodeHolder ( nullptr );
    }
}

const RenderNodeHolder RenderNodePrivate::find(RenderNodeIdentifier identifier) const
{
    if ( Identificator::IsValid(identifier) )
    {
        for ( const RenderNodeHolder& child : iChilds )
        {
            if ( !child.isInvalid() )
            {
                if ( child->getIdentifier() == identifier )
                {
                    return child;
                }
            }
        }
        
        for ( const RenderNodeHolder& child : iChilds )
        {
            if ( !child.isInvalid() )
            {
                RenderNodeHolder ret = child->find(identifier);
                
                if ( !ret.isInvalid() )
                {
                    return ret;
                }
            }
        }
        
#ifdef GreIsDebugMode
        GreDebugPretty() << "'identifier' was not found in Children's list ('" << (int) identifier << "')." << std::endl;
#endif
        return RenderNodeHolder ( nullptr );
    }
    
    else
    {
#ifdef GreIsDebugMode
        GreDebugPretty() << "'identifier' is invalid." << std::endl;
#endif
        return RenderNodeHolder ( nullptr );
    }
}

RenderNodeHolderList RenderNodePrivate::getVisibleChildren(const CameraHolder &camera) const
{
    if ( !camera.isInvalid() )
    {
        RenderNodeHolderList ret;
        
        for ( const RenderNodeHolder& child : iChilds )
        {
            if ( !child.isInvalid() )
            {
                if ( camera->isVisible(child->getBoundingBox()) )
                {
                    ret.add( child->getVisibleChildren(camera) );
                    ret.add( child );
                }
            }
        }
        
        return ret;
    }
    
    return RenderNodeHolderList();
}

void RenderNodePrivate::remove(const RenderNodePrivate::Identifier& identifier)
{
    if ( Identificator::IsValid(identifier) )
    {
        for ( auto it = iChilds.begin(); it != iChilds.end(); it++ )
        {
            if ( !(*it).isInvalid() )
            {
                if ( (*it)->getIdentifier() == identifier )
                {
                    Resource::removeListener( (*it)->getName() );
                    iChilds.erase(it);
                    return;
                }
            }
        }
        
#ifdef GreIsDebugMode
        GreDebugPretty() << "RenderNode Resource 'RenderNode#" << std::to_string(identifier) << "' not found." << std::endl;
#endif
    }
    
    else
    {
#ifdef GreIsDebugMode
        GreDebugPretty() << "'identifier' is invalid." << std::endl;
#endif
    }
}

void RenderNodePrivate::removeNotRecursive(const RenderNodePrivate::Identifier& identifier)
{
    if ( RenderNodePrivate::Identificator::IsValid(identifier) )
    {
        for ( auto it = iChilds.begin(); it != iChilds.end(); it++ )
        {
            if ( !(*it).isInvalid() )
            {
                if ( (*it)->getIdentifier() == identifier )
                {
                    // Save Children.
                    
                    RenderNodeHolderList children = (*it)->getChilds();
                    
                    // Erase Child.
                    
                    Resource::removeListener( (*it)->getName() );
                    iChilds.erase(it);
                    
                    // Adds every Children to this Node.
                    
                    for ( RenderNodeHolder& holder : children )
                    {
                        if ( !holder.isInvalid() )
                        {
                            addNode(holder);
                        }
                    }
                    
                    return;
                }
            }
        }
        
#ifdef GreIsDebugMode
        GreDebugPretty() << "RenderNode Resource 'RenderNode#" << std::to_string(identifier) << "' not found." << std::endl;
#endif
    }
    
    else
    {
#ifdef GreIsDebugMode
        GreDebugPretty() << "'identifier' is invalid." << std::endl;
#endif
    }
}

void RenderNodePrivate::clear()
{
    iChilds.clear();
    iRenderable.reset();
    iTransformation = Transformation();
    iParent.reset();
    iBoundingBox.clear();
    iModelMatrix = Matrix4(0);
    iTransformationChanged = true;
    iRenderableChanged = true;
}

const BoundingBox& RenderNodePrivate::getBoundingBox() const
{
    return iBoundingBox;
}

void RenderNodePrivate::translate(const Vector3 &vec)
{
    iTransformation.translate(vec);
    iTransformationChanged = true;
}

void RenderNodePrivate::rotate(float angle, const Vector3 &axis)
{
    iTransformation.rotate(angle, axis);
    iTransformationChanged = true;
}

void RenderNodePrivate::scale(float value)
{
    iTransformation.scale( Vector3(value, value, value) );
    iTransformationChanged = true;
}

RenderNodePrivate::Identifier RenderNodePrivate::getIdentifier() const
{
    return iIdentifier;
}

void RenderNodePrivate::setParent(const RenderNodeHolder &parent)
{
    iParent = parent;
}

void RenderNodePrivate::onUpdateEvent(const Gre::UpdateEvent &e)
{
    if ( iTransformationChanged )
    {
        iModelMatrix = iTransformation.get();
        iTransformationChanged = false;
        
        if ( !iRenderableChanged )
        {
            iBoundingBox.apply(iTransformation);
        }
        
        // If the Transformation has changed, we must check if this Node should behave at the same place
        // or not. So, we must add it again to its parent.
        
        if ( !iParent.isInvalid() )
        {
            iParent->addNode(RenderNodeHolder(this));
        }
    }
    
    if ( iRenderableChanged )
    {
        if ( !iRenderable.isInvalid() )
        {
            iBoundingBox = iRenderable.getBoundingBox();
            iBoundingBox.apply(iTransformation);
        }
        
        iRenderableChanged = false;
    }
}

// ---------------------------------------------------------------------------------------------------

RenderNode::RenderNode(const RenderNodePrivate* pointer)
: Gre::ResourceUser(pointer)
, SpecializedResourceUser<Gre::RenderNodePrivate>(pointer)
{
    
}

RenderNode::RenderNode(const RenderNodeHolder& holder)
: Gre::ResourceUser(holder)
, SpecializedResourceUser<Gre::RenderNodePrivate>(holder)
{
    
}

RenderNode::RenderNode(const RenderNode& user)
: Gre::ResourceUser(user)
, SpecializedResourceUser<Gre::RenderNodePrivate>(user)
{
    
}

RenderNode::~RenderNode()
{
    
}

RenderNode RenderNode::Null = RenderNode(nullptr);

GreEndNamespace
