//////////////////////////////////////////////////////////////////////
//
//  RenderNode.h
//  This source file is part of Gre
//		(Gang's Resource Engine)
//
//  Copyright (c) 2015 - 2017 Luk2010
//  Created on 02/05/2017.
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

#ifndef GRE_RENDERNODE_H
#define GRE_RENDERNODE_H

#include "Renderable.h"
#include "Material.h"
#include "Mesh.h"

GreBeginNamespace

// Uses this pre-definition in order to store the creator of the node.
class RenderScene ;

//////////////////////////////////////////////////////////////////////
/// @brief Holds informations about a node in the scene tree.
///
/// A RenderNode is an object used to describe an element that take part
/// in the scene tree. Material and Mesh are a renderable part, EmissiveMaterial
/// is a 'light' part used to bind lights , and ProjectionMatrix is a
/// Matrix used by the node when it should represents a 'camera'.
///
/// The way a node is added to another node is particular with the scene's
/// tree implementation. For example , the default implementation uses
/// the node's bounding boxes to make children. An Octree scene , for
/// example , may have particular nodes.
///
/// A Node receives 'update' events from its parent. The root node receives
/// it from the Scene object. The model , view and bounding box should be
/// updated during those functions.
///
/// A Node can also send 'PositionChanged' and 'TargetChanged' events to
/// listeners. When those properties change , a boolean flag indicates
/// the node to recalculate the model and view matrix. It has been studied
/// the view matrix is simply the inversed model matrix.
///
/// The node bounding box should be in world space. Some implementation
/// may define it as local space from its parent , but this implementation
/// does not , because this is up to the implementation to decide a node's
/// parent bounding box englobate the child bounding box.
///
/// The parent may be stored as a holder to the node. The node may always
/// check to destroy the holder when changing the parent's node , in order
/// to release the reference counter attached to the resource.
///
//////////////////////////////////////////////////////////////////////
class DLL_PUBLIC RenderNode : public Renderable
{
public:

    // Declaring the scene as a friend of this class lets the scene have every right
    // on the node , which seems totally logical as it should always be the creator of it.
    friend class RenderScene ;

    // Declares here the holders.
    typedef Holder < RenderScene > RenderSceneHolder ;
    typedef Holder < RenderNode > RenderNodeHolder ;
    typedef SpecializedResourceHolderList < RenderNode > RenderNodeHolderList ;

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    RenderNode ( const RenderScene * creator , const std::string & name = std::string () ) ;

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual ~RenderNode () noexcept ( false ) ;

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    const RenderSceneHolder getCreator () const ;

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual const RenderNodeHolder & getParent () const ;

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual const std::list < RenderNodeHolder > & getChildren () const ;

    //////////////////////////////////////////////////////////////////////
    /// @brief May add the given node to the children of this node. The rules
    /// to add the node to the children's list are arbitrary to the node's
    /// implementation. By default , a node is added when its bounding box
    /// is inside this node's bounding box.
    /// @return true if the node has been added to this one or one of its
    /// children.
    //////////////////////////////////////////////////////////////////////
    virtual bool add ( RenderNodeHolder & node ) ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Removes a node from the children.
    /// @return true on success.
    //////////////////////////////////////////////////////////////////////
    virtual bool remove ( RenderNodeHolder & node ) ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Removes every children from this node.
    //////////////////////////////////////////////////////////////////////
    virtual void clearChildren () ;

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    const MeshHolder & getMesh () const ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Changes the current mesh. If the bounding box is managed by
    /// this node , this should update the bounding box to the mesh's default
    /// multiplied by the model matrix.
    //////////////////////////////////////////////////////////////////////
    virtual void setMesh ( const MeshHolder & mesh ) ;

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    const MaterialHolder & getMaterial () const ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Changes the material used to render every submesh from this
    /// node's mesh.
    //////////////////////////////////////////////////////////////////////
    virtual void setMaterial ( const MaterialHolder & material ) ;

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    const MaterialHolder & getEmissiveMaterial () const ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Changes the emissive material used to render emitted light
    /// from this node.
    //////////////////////////////////////////////////////////////////////
    virtual void setEmissiveMaterial ( const MaterialHolder & material ) ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Translates the position by direction.
    //////////////////////////////////////////////////////////////////////
    virtual void translate ( const Vector3 & direction ) ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Translates the node's position and sets the dirty flag.
    //////////////////////////////////////////////////////////////////////
    virtual void translate ( float x , float y , float z ) ;

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual void setPosition ( const Vector3 & position ) ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Changes the node's position and sets the dirty flag.
    //////////////////////////////////////////////////////////////////////
    virtual void setPosition ( float x , float y , float z ) ;

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual const Vector3 & getPosition () const ;

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual void look ( const Vector3 & position ) ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Changes the target position and sets the dirty flag.
    //////////////////////////////////////////////////////////////////////
    virtual void look ( float x , float y , float z ) ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Rotates the target from angle and axis.
    //////////////////////////////////////////////////////////////////////
    virtual void rotate ( float degree , const Vector3 & axis ) ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the target's position.
    //////////////////////////////////////////////////////////////////////
    virtual const Vector3 & getTarget () const ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the direction of the node.
    //////////////////////////////////////////////////////////////////////
    virtual const Vector3 & getDirection () const ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the cross product of upward and forward vectors.
    //////////////////////////////////////////////////////////////////////
    virtual const Vector3 & getRightDirection () const ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the right ( relative +X ) direction.
    //////////////////////////////////////////////////////////////////////
    virtual const Vector3 & getUpwardDirection () const ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the bounding box calculated during update.
    //////////////////////////////////////////////////////////////////////
    virtual const BoundingBox & getBoundingBox () const ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Manually sets the bounding box for this node. Also set the
    /// 'iManualBoundingBox' flag to true.
    //////////////////////////////////////////////////////////////////////
    virtual void setBoundingBox ( const BoundingBox & bbox ) ;

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual bool isManualBoundingBox () const ;

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual void setManualBoundingBox ( bool value ) ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Forces update of the node when dirty.
    //////////////////////////////////////////////////////////////////////
    virtual void update () ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Returns nodes visible from the given projection-view matrix.
    /// Notes transparent nodes should be skipped and the returned list
    /// should not be sorted.
    //////////////////////////////////////////////////////////////////////
    virtual void sort ( const Matrix4 & projectionview , RenderNodeHolderList & result ) const ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the lights from those children.
    //////////////////////////////////////////////////////////////////////
    virtual void lights ( const Matrix4 & projectionview , RenderNodeHolderList & result ) const ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the computed view matrix.
    //////////////////////////////////////////////////////////////////////
    virtual const Matrix4 & getViewMatrix () const ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the model matrix.
    //////////////////////////////////////////////////////////////////////
    virtual const Matrix4 & getModelMatrix () const ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Binds the emissive material to the given technique. Notes
    /// the emissive material may have a shadow texture , which is given
    /// by , for example , TechniqueParam::Texture0 . This texture should be
    /// bound during this function. So the light should not be binded if you
    /// want to draw to any of the textures.
    ///
    /// Notes this function also binds position , direction and view matrix
    /// to compute the light projection-view matrix for the shader.
    ///
    //////////////////////////////////////////////////////////////////////
    virtual void bindEmissiveMaterial ( const TechniqueHolder & technique ) const ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Binds the node's material if the mesh is not invalid.
    //////////////////////////////////////////////////////////////////////
    virtual void use ( const TechniqueHolder & technique ) const ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Activates the view matrix , if this node should be used as
    /// a camera object.
    //////////////////////////////////////////////////////////////////////
    virtual void activeViewMatrix ( bool value ) ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Changes the scale component. Notes this attributes has no
    /// effect on the view matrix.
    //////////////////////////////////////////////////////////////////////
    virtual void scale ( float x , float y , float z ) ;

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual void scale ( const Vector3 & value ) ;

protected:

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual void onUpdateEvent ( const UpdateEvent & e ) ;

protected:

    /// @brief Holds a pointer to the scene that created this node. This
    /// pointer may or may not be invalidated. In every case , invalidating
    /// this pointer should be done when destroying the scene. The user may
    /// destroy the scene only when he is sure that every node's holders
    /// external to the scene object have been released.
    const RenderScene * iCreator ;

    /// @brief Parent's node.
    RenderNodeHolder iParent ;

    /// @brief May contains a list of children for this node. Notes that does not
    /// defines the node some constraints about its children (like a limit or else).
    std::list < RenderNodeHolder > iChildren ;

    /// @brief May hold the mesh used to render this node.
    MeshHolder iMesh ;

    /// @brief May hold the material used to render the node. If null , the renderpass
    /// should check each submesh's default material and applies them to the technique.
    MaterialHolder iMaterial ;

    /// @brief May hold the material used to render a 'light' , and corresponds to the
    /// emissive material projected by this node. If null , the node can't be considered
    /// as a light.
    MaterialHolder iEmissiveMaterial ;

    /// @brief World-space position of this node.
    Vector3 iPosition ;

    /// @brief World-space position of the node's target.
    Vector3 iTarget ;

    /// @brief Scale of this node to be applied to the model
    /// matrix , one component for each axis.
    Vector3 iScale ;

    /// @brief Quaternion holding the rotation component of this node.
    glm::quat iQuaternion ;

    /// @brief May hold the model matrix , calculated when updating properties as position ,
    /// target or rotation.
    mutable Matrix4 iModelMatrix ;

    /// @brief Inversed model matrix.
    mutable Matrix4 iViewMatrix ;

    /// @brief Bounding box used for this node.
    mutable BoundingBox iBoundingBox ;

    /// @brief World-space direction of the node's upward. This upward direction is
    /// the cross product between forward and right.
    mutable Vector3 iUpward ;

    /// @brief World-space forward direction.
    mutable Vector3 iForwardDirection ;

    /// @brief World-space direction of the node's right. Given by cross-product between
    /// global upward and forward.
    mutable Vector3 iRightDirection ;

    /// @brief Boolean flag to indicate whether the model/view matrix should be recalculated.
    /// Notes that recalculating the matrix also recalculates the bounding box , and so the node
    /// should recalculate which node is its parent.
    mutable bool iMatrixDirty ;

    /// @brief Boolean flag to indicate the bounding box should be recalculated.
    mutable bool iBoundingboxDirty ;

    /// @brief Flag to indicate if the bounding box is managed by the user , or by the node. When
    /// managed by the node it will multiply the model matrix and the mesh's bounding box. Default
    /// 'false' value is managed by the node. Notes usually root nodes have manual bounding box as
    /// they may have a large bounding box with no mesh , but a root node may also be a big cube
    /// with a cubemap to render the sky.
    bool iManualBoundingBox ;

    /// @brief Updates the view matrix only if this flag is true. Default is false. It should be
    /// activated only when this node should be used as a camera.
    bool iActiveViewMatrix ;
};

/// @brief
typedef Holder < RenderNode > RenderNodeHolder ;

/// @brief
typedef SpecializedResourceHolderList < RenderNode > RenderNodeHolderList ;

/// @brief Function declaration to compare two node.
typedef bool (*RenderNodeCmp) ( const RenderNodeHolder & n1 , const RenderNodeHolder & n2 ) ;

GreEndNamespace

#endif // GRE_RENDERNODE_H
