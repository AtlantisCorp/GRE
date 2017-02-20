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

RenderNode::RenderNode()
: Gre::Resource()
, iRenderable(nullptr)
, iTransformation()
, iChilds()
, iParent(nullptr)
, iBoundingBox()
, iTransformationChanged(false)
, iRenderableChanged(false)
{
    setName ( std::string("RenderNode#") + std::to_string(getIdentifier()) ) ;
    iModelMatrix = glm::mat4 (1.0f) ;
}

RenderNode::RenderNode(const std::string& name)
: Gre::Resource(name)
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

RenderNode::~RenderNode() noexcept ( false )
{
    
}

bool RenderNode::isRenderable() const
{
    GreAutolock ;
    return !iRenderable.isInvalid();
}

const MeshUser& RenderNode::getMesh() const
{
    return iRenderable;
}

MeshUser& RenderNode::getMesh()
{
    return iRenderable;
}

void RenderNode::setMesh(const Gre::MeshUser &mesh)
{
    GreAutolock ;
    
    if ( !iRenderable.isInvalid() )
    {
        Resource::removeListener( iRenderable );
    }
    
    iRenderable = mesh;
    iRenderableChanged = true;
    
    if ( !iRenderable.isInvalid() )
    {
        Resource::addListener( iRenderable );
    }
}

void RenderNode::setTransformation(const Gre::Transformation &transformation)
{
    GreAutolock ;
    iTransformation = transformation;
    iTransformationChanged = true;
}

const Transformation& RenderNode::getTransformation() const
{
    return iTransformation;
}

Matrix4 RenderNode::getModelMatrix() const
{
    return iModelMatrix;
}

void RenderNode::addNode(const RenderNodeHolder &node)
{
    if ( !node.isInvalid() )
    {
        GreAutolock ;
        
        // First, check if this Node is not already one of our children. If this is the case,
        // this is probably a check from 'RenderNode::onUpdateEvent' when 'iTransformationChanged' is
        // true.
        
        auto it = iChilds.find( node->getIdentifier() );
        
        if ( it != iChilds.end() )
        {
            // We have to remove the Child from the list, and add it again. Removing the Node from the list
            // lets us adding it as if it was a new Node. Also, we remove the node from the listeners.
            
            iChilds.erase(it);
            Resource::removeListener( RenderNodeUser(node) );
            
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
                        
                        Resource::removeListener( RenderNodeUser(*it) );
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
            Resource::addListener( RenderNodeUser (node) );
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

RenderNodeHolderList& RenderNode::getChildren()
{
    return iChilds;
}

const RenderNodeHolderList& RenderNode::getChildren() const
{
    return iChilds;
}

bool RenderNode::hasChildren() const
{
    return iChilds.size() > 0 ;
}

const RenderNodeHolder& RenderNode::getParent() const
{
    return iParent ;
}

RenderNodeHolder RenderNode::find(ResourceIdentifier identifier)
{
    GreAutolock;
    
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
    GreDebugPretty() << "'identifier' was not found in Children's list ('" << (int) identifier << "')." << Gre::gendl;
#endif
    return RenderNodeHolder ( nullptr );
}

const RenderNodeHolder RenderNode::find(ResourceIdentifier identifier) const
{
    GreAutolock ;
    
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
    GreDebugPretty() << "'identifier' was not found in Children's list ('" << (int) identifier << "')." << Gre::gendl;
#endif
    return RenderNodeHolder ( nullptr );
}

void RenderNode::remove(const ResourceIdentifier& identifier)
{
    GreAutolock ;
    
    for ( auto it = iChilds.begin(); it != iChilds.end(); it++ )
    {
        if ( !(*it).isInvalid() )
        {
            if ( (*it)->getIdentifier() == identifier )
            {
                Resource::removeListener( RenderNodeUser(*it) );
                iChilds.erase(it);
                return;
            }
        }
    }
    
#ifdef GreIsDebugMode
    GreDebugPretty() << "RenderNode Resource 'RenderNode#" << std::to_string(identifier) << "' not found." << Gre::gendl;
#endif
}

void RenderNode::removeNotRecursive(const ResourceIdentifier& identifier)
{
    GreAutolock ;
    
    for ( auto it = iChilds.begin(); it != iChilds.end(); it++ )
    {
        if ( !(*it).isInvalid() )
        {
            if ( (*it)->getIdentifier() == identifier )
            {
                // Save Children.
                
                RenderNodeHolderList children = (*it)->getChildren();
                
                // Erase Child.
                
                Resource::removeListener( RenderNodeUser(*it) );
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
    GreDebugPretty() << "RenderNode Resource 'RenderNode#" << std::to_string(identifier) << "' not found." << Gre::gendl;
#endif
}

void RenderNode::clear()
{
    GreAutolock ;
    
    iChilds.clear();
    iRenderable.clear();
    iTransformation = Transformation();
    iParent.clear();
    iBoundingBox.clear();
    iModelMatrix = Matrix4(0);
    iTransformationChanged = true;
    iRenderableChanged = true;
    Resource::clear();
}

const BoundingBox& RenderNode::getBoundingBox() const
{
    return iBoundingBox;
}

void RenderNode::translate(const Vector3 &vec)
{
    GreAutolock ;
    iTransformation.translate(vec);
    iTransformationChanged = true;
}

void RenderNode::rotate(float angle, const Vector3 &axis)
{
    GreAutolock ;
    iTransformation.rotate(angle, axis);
    iTransformationChanged = true;
}

void RenderNode::scale(float value)
{
    GreAutolock ;
    iTransformation.scale( Vector3(value, value, value) );
    iTransformationChanged = true;
}

void RenderNode::setParent(const RenderNodeHolder &parent)
{
    GreAutolock ;
    iParent = parent;
}

const MaterialHolder& RenderNode::getMaterial() const
{
    GreAutolock ; return iMaterial ;
}

bool RenderNode::isVisible(const CameraUser &camera) const
{
    if ( camera.isInvalid() )
        return false ;
    
    return camera.lock()->isVisible( iBoundingBox ) ;
}

void RenderNode::onUpdateEvent(const Gre::UpdateEvent &e)
{
    Resource::onUpdateEvent(e);
    
    GreAutolock ;
    
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
            iBoundingBox = iRenderable.lock()->getBoundingBox();
            iBoundingBox.apply(iTransformation);
        }
        
        iRenderableChanged = false;
    }
}

GreEndNamespace
