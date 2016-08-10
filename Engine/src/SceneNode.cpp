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
#include "Scene.h"

GreBeginNamespace

SceneNodePrivate::Identifier SceneNodePrivate::Identificator::identifier = 0;

SceneNodePrivate::Identifier SceneNodePrivate::Identificator::Create()
{
	return (identifier++);
}

SceneNodePrivate::Identifier SceneNodePrivate::Identificator::Last()
{
	return identifier;
}

bool SceneNodePrivate::Identificator::IsValid(SceneNodePrivate::Identifier id)
{
	return id > 0;
}

// ---------------------------------------------------------------------------------------------------

SceneNodePrivate::SceneNodePrivate(const SceneManager& creator)
: NodePrivate(std::string("SceneNodePrivate#") + std::to_string(Identificator::Create())), iRenderable(nullptr)
{
	iCreator = new SceneManager(creator);
	iIdentifier = Identificator::Last();
}

SceneNodePrivate::SceneNodePrivate(const SceneManager& creator, const std::string& name)
: NodePrivate(name), iRenderable(nullptr)
{
	iCreator = new SceneManager(creator);
	iIdentifier = Identificator::Create();
    iRenderable = Mesh::Null;
}

SceneNodePrivate::SceneNodePrivate(const SceneManager& creator, const std::string& name, const Mesh& renderable)
: NodePrivate(name), iRenderable(nullptr)
{
    iCreator = new SceneManager(creator);
    iIdentifier = Identificator::Create();
    iRenderable = renderable;
}

SceneNodePrivate::~SceneNodePrivate()
{
    if(iCreator)
    {
        delete iCreator;
    }
}

bool SceneNodePrivate::isRenderable() const
{
    return !iRenderable.isExpired();
}

const Mesh& SceneNodePrivate::getRenderable() const
{
    return iRenderable;
}

Mesh& SceneNodePrivate::getRenderable()
{
    return iRenderable;
}

void SceneNodePrivate::setRenderable(const Gre::Mesh &mesh)
{
    iRenderable = mesh;
    iRenderableChanged = true;
}

bool SceneNodePrivate::isRenderableChanged() const
{
    return iRenderableChanged;
}

void SceneNodePrivate::setTransformation(const Gre::Transformation &transformation)
{
    iTransformation = transformation;
}

const Transformation& SceneNodePrivate::getTransformation() const
{
    return iTransformation;
}

Matrix4 SceneNodePrivate::getModelMatrix() const
{
    if ( getParent() )
    {
        Matrix4 modelmatrix = (reinterpret_cast<const SceneNodePrivate*>(getParent()))->getModelMatrix();
        return iTransformation.get() * modelmatrix;
    }
    
    else
    {
        return iTransformation.get();
    }
}

// ---------------------------------------------------------------------------------------------------

SceneNode::SceneNode(SceneNodePrivate* node)
: ResourceUser(node)
, Node(node)
, SpecializedResourceUser<Gre::SceneNodePrivate>(node)
{
    
}

SceneNode::SceneNode(const SceneNodeHolder& holder)
: ResourceUser(holder)
, Node(holder.get())
, SpecializedResourceUser<Gre::SceneNodePrivate>(holder)
{
    
}

SceneNode::SceneNode(const SceneNode& user)
: ResourceUser(user)
, Node(user)
, SpecializedResourceUser<Gre::SceneNodePrivate>(user)
{
    
}

SceneNode::~SceneNode()
{
    
}

SpecializedResourceHolder<SceneNodePrivate> SceneNode::lock()
{
    return SpecializedResourceUser<SceneNodePrivate>::lock();
}

const SpecializedResourceHolder<SceneNodePrivate> SceneNode::lock() const
{
    return SpecializedResourceUser<SceneNodePrivate>::lock();
}

bool SceneNode::isRenderable() const
{
    auto ptr = lock();
    if(ptr)
    {
        return ptr->isRenderable();
    }
    
    return false;
}

const Mesh& SceneNode::getRenderable() const
{
    auto ptr = lock();
    if(ptr)
    {
        return ptr->getRenderable();
    }
    
    return Mesh::Null;
}

Mesh& SceneNode::getRenderable()
{
    auto ptr = lock();
    if(ptr)
    {
        return ptr->getRenderable();
    }
    
    return Mesh::Null;
}

void SceneNode::setRenderable(const Gre::Mesh &mesh)
{
    auto ptr = lock();
    if(ptr)
    {
        ptr->setRenderable(mesh);
    }
}

bool SceneNode::isRenderableChanged() const
{
    auto ptr = lock();
    if(ptr)
    {
        return ptr->isRenderableChanged();
    }
    
    return false;
}

void SceneNode::setTransformation(const Gre::Transformation &transformation)
{
    auto ptr = lock();
    if(ptr)
    {
        ptr->setTransformation(transformation);
    }
}

const Transformation& SceneNode::getTransformation() const
{
    auto ptr = lock();
    if(ptr)
    {
        return ptr->getTransformation();
    }
    return Transformation::Default;
}

Matrix4 SceneNode::getModelMatrix() const
{
    auto ptr = lock();
    if ( ptr )
        return ptr->getModelMatrix();
    return Matrix4(0);
}

SceneNode SceneNode::Null = SceneNode(nullptr);

GreEndNamespace
