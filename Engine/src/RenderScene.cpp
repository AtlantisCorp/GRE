//////////////////////////////////////////////////////////////////////
//
//  RenderScene.cpp
//  This source file is part of Gre
//		(Gang's Resource Engine)
//
//  Copyright (c) 2015 - 2017 Luk2010
//  Created on 06/05/2017.
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

#include "RenderScene.h"

GreBeginNamespace

RenderScene::RenderScene ( const std::string & name ) : Gre::Renderable ( name )
{
    iRoot = create ( name + ".root" ) ;
    addFilteredListener ( iRoot , { EventType::Update } ) ;
}

RenderScene::~RenderScene () noexcept ( false )
{

}

RenderNodeHolder RenderScene::create ( const std::string & name ) const
{
    return RenderNodeHolder ( new RenderNode ( this , name ) ) ;
}

bool RenderScene::add ( RenderNodeHolder & node )
{
    GreAutolock ;

    if ( iRoot.isInvalid() || node.isInvalid() )
    return false ;

    node -> update () ;
    return iRoot -> add ( node ) ;
}

bool RenderScene::remove ( RenderNodeHolder & node )
{
    GreAutolock ;

    if ( iRoot.isInvalid() )
    return false ;

    return iRoot -> remove ( node ) ;
}

const RenderNodeHolderList RenderScene::sort ( const Matrix4 & projectionview ) const
{
    GreAutolock ;

    RenderNodeHolderList result ;
    iRoot -> sort ( projectionview , result ) ;

    return result ;
}

const RenderNodeHolderList RenderScene::lights ( const Matrix4 & projectionview ) const
{
    GreAutolock ;

    RenderNodeHolderList result ;
    iRoot -> lights ( projectionview , result ) ;

    return result ;
}

const RenderNodeHolder & RenderScene::getRoot () const
{
    GreAutolock ; return iRoot ;
}

RenderNodeHolder & RenderScene::getRoot ()
{
    GreAutolock ; return iRoot ;
}

// -----------------------------------------------------------------------------
// RenderSceneLoader

RenderSceneLoader::RenderSceneLoader ()
{

}

// -----------------------------------------------------------------------------
// Default loader.

class DefaultRenderSceneLoader : public RenderSceneLoader
{
public:

    POOLED ( Pools::Loader )

    DefaultRenderSceneLoader ()
    { }

    RenderSceneHolder load ( const std::string & name , const ResourceLoaderOptions & ops ) const
    { return RenderSceneHolder(new RenderScene(name)) ; }

    ResourceLoader* clone () const
    { return new DefaultRenderSceneLoader() ; }

    bool isLoadable ( const std::string & path ) const
    { return false ; }
};

// -----------------------------------------------------------------------------
// RenderSceneManager

RenderSceneManager::RenderSceneManager ( const std::string & name )
: Gre::SpecializedResourceManager < RenderScene , RenderSceneLoader > ( name )
{
    iLoaders.registers ( "scenes.loaders.default" , new DefaultRenderSceneLoader() ) ;
}

RenderSceneManager::~RenderSceneManager () noexcept ( false )
{

}

RenderSceneHolder RenderSceneManager::load ( const std::string & name , const ResourceLoaderOptions & ops )
{
    GreAutolock ;

    //////////////////////////////////////////////////////////////////////
    // Checks argument and options.

    if ( name.empty() )
    return RenderSceneHolder ( nullptr ) ;

    auto scenecheck = findFirstHolder ( name ) ;

    if ( !scenecheck.isInvalid() )
    {
        GreDebug ( "[WARN] Scene '" ) << name << "' has already been loaded." << gendl ;
        return RenderSceneHolder ( nullptr ) ;
    }

    auto loadernameit = ops.find ( "scene.loader" ) ;
    std::string loadername = "scenes.loaders.default" ;

    if ( loadernameit != ops.end() )
    loadername = loadernameit->second.to < std::string > () ;

    //////////////////////////////////////////////////////////////////////
    // Tries to find loader.

    auto loader = findLoader ( loadername ) ;

    if ( !loader && loadername != "scenes.loaders.default" )
    loader = findLoader ( "scenes.loaders.default" ) ;

    if ( !loader )
    {
        GreDebug ( "[WARN] No loader '" ) << loadername << "' found for scene '" << name << "'." << gendl ;
        return RenderSceneHolder ( nullptr ) ;
    }

    //////////////////////////////////////////////////////////////////////
    // Tries to load the scene using the loader.

    auto scene = loader -> load ( name , ops ) ;

    if ( scene.isInvalid() )
    return RenderSceneHolder ( nullptr ) ;

    //////////////////////////////////////////////////////////////////////
    // Registers the newly created scene.

    iHolders.add ( scene ) ;
    addFilteredListener ( scene , { EventType::Update } ) ;

    //////////////////////////////////////////////////////////////////////
    // Treats options here.

    auto rootsizeit = ops.find ( "scene.root.size" ) ;

    if ( rootsizeit != ops.end() )
    {
        Vector3 rootsize = rootsizeit->second.to < Vector3 > () ;
        scene -> getRoot() -> setBoundingBox ( BoundingBox(rootsize) ) ;
    }

    GreDebug ( "[INFO] Successfully created scene '" ) << name << "'." << gendl ;
    return scene ;
}

GreEndNamespace
