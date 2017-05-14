//////////////////////////////////////////////////////////////////////
//
//  RenderScene.h
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

#ifndef GRE_RENDERSCENE_H
#define GRE_RENDERSCENE_H

#include "RenderNode.h"

GreBeginNamespace

enum class Intersection : int
{
    Frontside , Backside , Confounded
};

//////////////////////////////////////////////////////////////////////
/// @brief Manages nodes.
//////////////////////////////////////////////////////////////////////
class DLL_PUBLIC RenderScene : public Renderable
{
public:

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    RenderScene ( const std::string & name ) ;

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual ~RenderScene () noexcept ( false ) ;

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual RenderNodeHolder create ( const std::string & name = std::string () ) const ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Adds a node to the scene tree.
    //////////////////////////////////////////////////////////////////////
    virtual bool add ( RenderNodeHolder & node ) ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Removes a node from the scene tree.
    //////////////////////////////////////////////////////////////////////
    virtual bool remove ( RenderNodeHolder & node ) ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Computes a sorted list of nodes , visible from the
    /// ProjecitonViewMatrix. As those nodes are sensibly not transparent ,
    /// no comparation is needed to draw them. To get a list of sorted
    /// transparent objects , see '::sortTransparent()'.
    //////////////////////////////////////////////////////////////////////
    virtual const RenderNodeHolderList sort ( const Matrix4 & projectionview ) const ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the lights.
    //////////////////////////////////////////////////////////////////////
    virtual const RenderNodeHolderList lights ( const Matrix4 & projectionview ) const ;

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual const RenderNodeHolder & getRoot () const ;

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual RenderNodeHolder & getRoot () ;

protected:

    /// @brief Root RenderNode.
    RenderNodeHolder iRoot ;
};

/// @brief
typedef Holder < RenderScene > RenderSceneHolder ;

/// @brief
typedef SpecializedResourceHolderList < RenderScene > RenderSceneHolderList ;

//////////////////////////////////////////////////////////////////////
/// @brief Loads a RenderScene with given options.
//////////////////////////////////////////////////////////////////////
class DLL_PUBLIC RenderSceneLoader : public ResourceLoader
{
public:

    POOLED ( Pools::Loader )

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    RenderSceneLoader () ;

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual RenderSceneHolder load ( const std::string & name , const ResourceLoaderOptions & ops ) const = 0 ;
};

//////////////////////////////////////////////////////////////////////
/// @brief Manages every RenderScene.
///
/// Loading options currently supported by the manager are :
///
///   - 'scene.loader' where 'scenes.loaders.default' is the default
///     loader installed. If not present , the default loader will
///     always be used. If the default loader is also not present ,
///     nothing will be loaded.
///
///   - 'scene.root.size' : Size of the root's bounding box. Generally
///     this means the total size of the scene.
///
//////////////////////////////////////////////////////////////////////
class DLL_PUBLIC RenderSceneManager : public SpecializedResourceManager < RenderScene , RenderSceneLoader >
{
public:

    POOLED ( Pools::Manager )

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    RenderSceneManager ( const std::string & name = std::string () ) ;

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual ~RenderSceneManager () noexcept ( false ) ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Loads a scene with given options.
    //////////////////////////////////////////////////////////////////////
    virtual RenderSceneHolder load ( const std::string & name , const ResourceLoaderOptions & ops ) ;
};

/// @brief
typedef Holder < RenderSceneManager > RenderSceneManagerHolder ;

GreEndNamespace

#endif // GRE_RENDERSCENE_H
