//////////////////////////////////////////////////////////////////////
//
//  RenderNode.cpp
//  This source file is part of Gre
//		(Gang's Resource Engine)
//
//  Copyright (c) 2015 - 2017 Luk2010
//  Created on 05/05/2017.
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

#include "RenderNode.h"
#include "RenderScene.h"

GreBeginNamespace

RenderNode::RenderNode ( const RenderScene * creator , const std::string & name )
: Gre::Renderable ( name )
, iCreator ( creator )
, iParent ( nullptr ) , iMesh ( nullptr )
, iMaterial ( nullptr ) , iEmissiveMaterial ( nullptr )
, iPosition ( 0.0f , 0.0f , 0.0f )
, iTarget ( 0.0f , 0.0f , 1.0f )
, iScale ( 1.0f , 1.0f , 1.0f )
, iModelMatrix ( 1.0f ) , iViewMatrix ( 1.0f )
, iUpward ( 0.0f , 0.0f , 0.0f )
, iForwardDirection ( 0.0f , 0.0f , 0.0f )
, iRightDirection ( 0.0f , 0.0f , 0.0f )
, iMatrixDirty ( false )
, iBoundingboxDirty ( false )
, iManualBoundingBox ( false )
, iActiveViewMatrix ( false )
{

}

RenderNode::~RenderNode () noexcept ( false )
{

}

const RenderNode::RenderSceneHolder RenderNode::getCreator () const
{
    GreAutolock ; return RenderSceneHolder ( iCreator ) ;
}

const RenderNode::RenderNodeHolder & RenderNode::getParent () const
{
    GreAutolock ; return iParent ;
}

const std::list < RenderNodeHolder > & RenderNode::getChildren () const
{
    GreAutolock ; return iChildren ;
}

bool RenderNode::add ( RenderNodeHolder & node )
{
    GreAutolock ;

    if ( node.isInvalid() )
    return false ;

    //////////////////////////////////////////////////////////////////////
    // If the node has not been updated , we force update here. This is because
    // the node may have changed its bounding box , or else.

    update () ;

    //////////////////////////////////////////////////////////////////////
    // If the node's bounding box is not set yet , we just add the node to
    // the children list. When the bounding box will be set , it will add/remove
    // it from the scene tree.

    if ( node->getBoundingBox().isInvalid() )
    {
        node -> iParent = RenderNodeHolder ( this ) ;
        iChildren.push_back( node ) ;
        addFilteredListener( node , { EventType::Update } ) ;
        return true ;
    }


    //////////////////////////////////////////////////////////////////////
    // To add a node to its children , we simply check the bounding box. If
    // it is fully inside , we add it. Before , we always check the node has
    // no parent , and the node won't fit in any of the children.

    IntersectionResult result = node->getBoundingBox().intersect(iBoundingBox) ;

    if ( result != IntersectionResult::Inside )
    return false ;

    for ( auto & child : iChildren )
    {
        bool added = child -> add ( node ) ;

        if ( added )
        return added ;
    }

    node -> iParent = RenderNodeHolder ( this ) ;
    iChildren.push_back ( node ) ;
    addFilteredListener ( node , { EventType::Update } ) ;
    return true ;
}

bool RenderNode::remove ( RenderNodeHolder & node )
{
    GreAutolock ;

    if ( node.isInvalid() )
    return false ;

    //////////////////////////////////////////////////////////////////////
    // Check if the node is one of our children. Notes by default in this
    // function , all children in the node will also be destroyed.

    auto it = std::find ( iChildren.begin() , iChildren.end() , node ) ;

    if ( it != iChildren.end() )
    {
        (*it) -> iParent = nullptr ;
        removeListener ( *it ) ;
        iChildren.erase ( it ) ;

        return true ;
    }

    for ( auto & child : iChildren )
    {
        bool removed = child -> remove ( node ) ;

        if ( removed )
        return true ;
    }

    //////////////////////////////////////////////////////////////////////
    // If we go here this means the node wasn't found.

    return false ;
}

void RenderNode::clearChildren ()
{
    GreAutolock ;

    //////////////////////////////////////////////////////////////////////
    // Removes every children and unregisters them their parent and listening.

    for ( auto & child : iChildren )
    {
        child -> iParent = nullptr ;
        removeListener ( child ) ;
    }

    iChildren.clear () ;
}

const MeshHolder & RenderNode::getMesh () const
{
    GreAutolock ; return iMesh ;
}

void RenderNode::setMesh ( const MeshHolder & mesh )
{
    GreAutolock ;

    //////////////////////////////////////////////////////////////////////
    // Changes the mesh used by this node. Also sets the dirty flag to
    // recalculate the bounding box.

    iMesh = mesh ;

    if ( !iManualBoundingBox )
    iBoundingboxDirty = true ;
}

const MaterialHolder & RenderNode::getMaterial () const
{
    GreAutolock ; return iMaterial ;
}

void RenderNode::setMaterial ( const MaterialHolder & material )
{
    GreAutolock ; iMaterial = material ;
}

const MaterialHolder & RenderNode::getEmissiveMaterial () const
{
    GreAutolock ; return iEmissiveMaterial ;
}

void RenderNode::setEmissiveMaterial ( const MaterialHolder & material )
{
    GreAutolock ; iEmissiveMaterial = material ;
}

void RenderNode::translate ( const Vector3 & direction )
{
    GreAutolock ;

    iPosition = iPosition + direction ;
    iTarget = iTarget + direction ;

    iMatrixDirty = true ;
}

void RenderNode::translate ( float x , float y , float z )
{
    translate ( Vector3(x, y, z) ) ;
}

void RenderNode::setPosition ( const Vector3 & position )
{
    GreAutolock ;

    //////////////////////////////////////////////////////////////////////
    // Computes position vector.

    iPosition = position ;
    iMatrixDirty = true ;
}

void RenderNode::setPosition ( float x , float y , float z )
{
    setPosition ( Vector3(x,y,z) ) ;
}

const Vector3 & RenderNode::getPosition () const
{
    GreAutolock ; return iPosition ;
}

void RenderNode::look ( const Vector3 & position )
{
    GreAutolock ;

    //////////////////////////////////////////////////////////////////////
    // Computes the quaternion from the Transformation::Forward vector.

    iQuaternion = RotationBetweenVectors ( VectorForward , position ) ;

    //////////////////////////////////////////////////////////////////////
    // Also computes the target.

    iTarget = position ;

    //////////////////////////////////////////////////////////////////////
    // Flags the matrix as dirty. The next time updated it will be recalculated.

    iMatrixDirty = true ;
}

void RenderNode::look ( float x , float y , float z )
{
    look ( Vector3(x, y, z) ) ;
}

void RenderNode::rotate ( float degree , const Vector3 & axis )
{
    GreAutolock ;

    //////////////////////////////////////////////////////////////////////
    // Computes the quaternion from the rotation. Notes the old quaternion
    // will be rotated from given axis and degree.

    iQuaternion = glm::angleAxis(degree, axis) * iQuaternion ;

    //////////////////////////////////////////////////////////////////////
    // Computes also the target vector. The rotation is made from the position
    // origin , so we must take it in account.

    iTarget = iPosition + ( iQuaternion * (iTarget - iPosition) ) ;

    //////////////////////////////////////////////////////////////////////
    // Flags the matrix as dirty to be recalculated.

    iMatrixDirty = true ;
}

const Vector3 & RenderNode::getTarget () const
{
    GreAutolock ; return iTarget ;
}

const Vector3 & RenderNode::getDirection () const
{
    GreAutolock ; return iForwardDirection ;
}

const Vector3 & RenderNode::getRightDirection () const
{
    GreAutolock ; return iRightDirection ;
}

const Vector3 & RenderNode::getUpwardDirection () const
{
    GreAutolock ; return iUpward ;
}

const BoundingBox & RenderNode::getBoundingBox () const
{
    GreAutolock ; return iBoundingBox ;
}

void RenderNode::setBoundingBox ( const BoundingBox & bbox )
{
    GreAutolock ;

    iBoundingBox = bbox ;
    iManualBoundingBox = true ;
    iBoundingboxDirty = true ;
}

bool RenderNode::isManualBoundingBox () const
{
    GreAutolock ; return iManualBoundingBox ;
}

void RenderNode::setManualBoundingBox ( bool value )
{
    GreAutolock ;

    if ( value != iManualBoundingBox )
    {
        iManualBoundingBox = value ;
        iBoundingboxDirty = true ;
    }
}

void RenderNode::update ()
{
    GreAutolock ;

    bool recalculateparent = false ;
    bool translatebbox = true ;

    //////////////////////////////////////////////////////////////////////
    // Checks the bounding box dirty flag. When updating it , we translate
    // its points with the node's position.

    if ( iBoundingboxDirty && !iManualBoundingBox )
    {
        if ( !iMesh.isInvalid() )
        {
            iBoundingBox = iMesh -> getBoundingBox () ;
            iBoundingBox.translateTo ( iPosition ) ;
            translatebbox = false ;
        }

        else
        iBoundingBox = BoundingBox () ;

        recalculateparent = true ;
    }

    iBoundingboxDirty = false ;

    //////////////////////////////////////////////////////////////////////
    // Checks the matrix dirty flag.

    if ( iMatrixDirty )
    {
        Vector3 up = VectorUpward ;

        //////////////////////////////////////////////////////////////////////
        // Calculates the forward, rightward and upward direction of the node.

        iForwardDirection = iTarget - iPosition ;

        if ( glm::length(iForwardDirection) <= 0.01f )
        iForwardDirection = VectorForward ;
        else
        iForwardDirection = glm::normalize ( iForwardDirection ) ;

        iRightDirection = glm::normalize ( glm::cross ( iForwardDirection , up ) ) ;
        iUpward = glm::normalize ( glm::cross ( iRightDirection , iForwardDirection ) ) ;

        //////////////////////////////////////////////////////////////////////
        // Calculates the model matrix. Notes the model matrix is only made
        // from directions and position vectors. The view matrix is currently
        // made from right-handed calculation.

        iModelMatrix [0] = glm::vec4 ( iRightDirection , 0.0f ) ;
        iModelMatrix [1] = glm::vec4 ( iUpward , 0.0f ) ;
        iModelMatrix [2] = glm::vec4 ( iForwardDirection , 0.0f ) ;
        iModelMatrix [3] = glm::vec4 ( iPosition , 1.0f ) ;
        iModelMatrix = glm::scale ( iModelMatrix , iScale ) ;

        if ( iActiveViewMatrix )
        iViewMatrix = glm::lookAtRH(iPosition, iTarget, iUpward) ;

        //////////////////////////////////////////////////////////////////////
        // Updates the bounding box if not yet done.

        if ( translatebbox )
        iBoundingBox.translateTo ( iPosition ) ;

        //////////////////////////////////////////////////////////////////////
        // Reset flags.

        iMatrixDirty = false ;
        recalculateparent = true ;
    }

    if ( recalculateparent && iCreator && !iParent.isInvalid() )
    {
        RenderNodeHolder thisnode (this) ;
        const_cast<RenderScene*>(iCreator) -> remove ( thisnode ) ;
        const_cast<RenderScene*>(iCreator) -> add ( thisnode ) ;
    }
}

void RenderNode::sort ( const Matrix4 & projectionview , RenderNodeHolderList & result ) const
{
    GreAutolock ;

    for ( auto & child : iChildren )
    child -> sort ( projectionview , result ) ;

    if ( iBoundingBox.isInvalid() )
    return ;

    float diameter = iBoundingBox.diameterlen () ;
    Vector4 center = Vector4 ( iBoundingBox.center () , 1.0f ) ;

    Vector4 coords = glm::normalize ( projectionview * iModelMatrix * center ) ;

    if ( coords.z < -diameter ) return ;
    if ( fabsf(coords.x) > 1 + diameter || fabsf(coords.y) > 1 + diameter ) return ;
    result.push_back ( RenderNodeHolder(this) ) ;
}

void RenderNode::lights ( const Matrix4 & projectionview , RenderNodeHolderList & result ) const
{
    GreAutolock ;

    for ( auto & child : iChildren )
    child -> lights ( projectionview , result ) ;

    if ( !iEmissiveMaterial.isInvalid() )
    result.push_back ( RenderNodeHolder(this) ) ;
}

const Matrix4 & RenderNode::getViewMatrix () const
{
    GreAutolock ; return iViewMatrix ;
}

const Matrix4 & RenderNode::getModelMatrix () const
{
    GreAutolock ; return iModelMatrix ;
}

void RenderNode::bindEmissiveMaterial ( const TechniqueHolder & technique ) const
{
    GreAutolock ;

    if ( iEmissiveMaterial.isInvalid() )
    return ;

    iEmissiveMaterial -> use ( technique ) ;

    //////////////////////////////////////////////////////////////////////
    // Now binds position , direction and projection-view.

    TechniqueParam alias = technique -> getCurrentLightAlias() ;
    technique -> setAliasedParameterStructValue(alias, TechniqueParam::LightPosition, HdwProgVarType::Float3, iPosition) ;
    technique -> setAliasedParameterStructValue(alias, TechniqueParam::LightDirection, HdwProgVarType::Float3, iForwardDirection) ;

    const Matrix4 projection = technique -> getProjectionMatrix() ;
    technique -> setAliasedParameterStructValue(alias, TechniqueParam::LightShadowMatrix, HdwProgVarType::Matrix4, projection * iViewMatrix) ;
}

void RenderNode::use ( const TechniqueHolder & technique ) const
{
    GreAutolock ;

    if ( !iMaterial.isInvalid() )
    iMaterial -> use ( technique ) ;

    TechniqueParamBinder::use(technique) ;
}

void RenderNode::activeViewMatrix ( bool value )
{
    GreAutolock ; iActiveViewMatrix = value ;
}

void RenderNode::scale ( float x , float y , float z )
{
    scale ( Vector3(x,y,z) ) ;
}

void RenderNode::scale ( const Vector3 & value )
{
    GreAutolock ;
    iScale = value ;
    iMatrixDirty = true ;
}

void RenderNode::onUpdateEvent ( const UpdateEvent & e )
{
    update () ;
}

GreEndNamespace
