//////////////////////////////////////////////////////////////////////
//
//  Scene.h
//  This source file is part of Gre 
//		(Gang's Resource Engine)
//
//  Copyright (c) 2015 - 2016 Luk2010
//  Created on 12/12/2015.
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

#ifndef GRE_Scene_h
#define GRE_Scene_h

#include "SpecializedResourceManager.h"
#include "SceneNode.h"
#include "Camera.h"
#include "Pass.h"

#include "RenderTarget.h"
#include "Technique.h"
#include "Renderer.h"

GreBeginNamespace

//////////////////////////////////////////////////////////////////////
/// @brief A Filter to sort RenderSceneNode.
//////////////////////////////////////////////////////////////////////
class DLL_PUBLIC RenderSceneFilter
{
public:
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    RenderSceneFilter() { }
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual ~RenderSceneFilter() { }
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Sorts every RenderNode in given RenderNodeHolderList.
    //////////////////////////////////////////////////////////////////////
    virtual void sort ( RenderNodeHolderList& rnode ) const = 0;
};

//////////////////////////////////////////////////////////////////////
/// @brief Non-sorting RenderSceneFilter.
//////////////////////////////////////////////////////////////////////
class DLL_PUBLIC DefaultRenderSceneFilter : public RenderSceneFilter
{
public:
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    DefaultRenderSceneFilter() { }
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    ~DefaultRenderSceneFilter() { }
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Do nothing.
    //////////////////////////////////////////////////////////////////////
    void sort ( RenderNodeHolderList& rnode ) const { }
};

//////////////////////////////////////////////////////////////////////
/// @brief A Generic Base Scene object.
///
/// The RenderScene object manages a set of RenderNode , a Camera , and
/// creates/modify RenderPass object when queried by the Renderer.
///
/// ### Managing RenderNode
///
/// Creates a RenderNode using '::createNode()' . Configure it and then ,
/// adds it to the Node's tree using '::addNode()' . The RenderNode will be
/// added to the root node.
///
/// ### Managing rendering
///
/// The Rendering is done by multiple RenderPass to multiple Framebuffers.
/// Those Pass can have different HardwarePrograms. They also can affect
/// only particular RenderNode. You can create a specific Pass for a
/// RenderNode using '::createNodePass()' .
///
//////////////////////////////////////////////////////////////////////
class DLL_PUBLIC RenderScene : public Resource
{
public:
    
    POOLED(Pools::Scene)
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    RenderScene(const std::string& name);
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual ~RenderScene();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the Root RenderNode.
    //////////////////////////////////////////////////////////////////////
    virtual const RenderNodeUser getRootNode() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Changes the Root RenderNode and returns the new Root.
    /// Check the return value with the given RenderNode to check if the
    /// Root Node has been successfully changed to your RenderNode.
    //////////////////////////////////////////////////////////////////////
    virtual const RenderNodeHolder & setRootNode ( const RenderNodeHolder & rendernode );
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Creates a new RenderNode with given name.
    /// The returned RenderNode is not added into the Scene tree. You have to
    /// use 'addNode()'. The holder returned by this function own the node.
    /// @remarks If 'name' is empty , "DefaultNode" will be used.
    //////////////////////////////////////////////////////////////////////
    virtual RenderNodeHolder createNode ( const std::string & name = std::string() ) const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Add a RenderNode to this Scene.
    /// If the RenderNode couldn't be added, its 'iParent' property will
    /// still be invalid.
    //////////////////////////////////////////////////////////////////////
    virtual void addNode ( const RenderNodeHolder & rendernode );
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Removes the given RenderNode from this Scene.
    //////////////////////////////////////////////////////////////////////
    virtual void removeNode ( const RenderNodeHolder & rendernode );
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Removes the given RenderNode but not its children.
    //////////////////////////////////////////////////////////////////////
    virtual void removeNodeNoChildren ( const RenderNodeHolder & holder ) ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Removes the RenderNode identified by the given identifier.
    //////////////////////////////////////////////////////////////////////
    virtual void removeNodeFromIdentifier ( const ResourceIdentifier & identifier ) ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Finds a RenderNode from its identifier and returns its
    /// holder.
    //////////////////////////////////////////////////////////////////////
    virtual RenderNodeHolder findHolder ( const ResourceIdentifier & identifier ) ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Finds a RenderNode from its identifier.
    //////////////////////////////////////////////////////////////////////
    virtual RenderNodeUser findNode ( const ResourceIdentifier & identifier ) ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Finds a RenderNode from its identifier.
    //////////////////////////////////////////////////////////////////////
    virtual const RenderNodeUser findNode ( const ResourceIdentifier & identifier ) const ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Draw the RenderScene to the selected RenderTarget.
    //////////////////////////////////////////////////////////////////////
    virtual void draw ( const EventHolder& elapsed ) const ;
    
protected:
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Draw the main technique or sub-techniques ( viewports ) .
    //////////////////////////////////////////////////////////////////////
    virtual void drawTechnique ( const TechniqueHolder& technique , const EventHolder& elapsed ) const ;
    
protected:
    
    /// @brief The Root RenderNode.
    RenderNodeHolder iRootNode ;
    
    /// @brief RenderTarget we want to draw to.
    RenderTargetHolder iRenderTarget ;
    
    /// @brief The Technique we use to draw this RenderScene.
    TechniqueHolder iTechnique ;
    
    /// @brief Renderer we use to draw this RenderScene.
    RendererUser iRenderer ;
};

/// @brief Common typedef to Specialize ResourceHolder.
typedef SpecializedCountedObjectHolder<RenderScene> RenderSceneHolder ;

/// @brief Common typedef to make RenderSceneHolder List.
typedef SpecializedResourceHolderList<RenderScene> RenderSceneHolderList ;

/// @brief SpecializedCountedObjectUser for RenderScenePrivate.
typedef SpecializedCountedObjectUser<RenderScene> RenderSceneUser ;

//////////////////////////////////////////////////////////////////////
/// @brief A basic loader to load RenderScene Objects.
/// The Default RenderSceneLoader loads a default RenderScene object. This
/// RenderScene aims to be a sort of binary tree. It is not very optimized,
/// although it should be sufficient for little Scenes.
//////////////////////////////////////////////////////////////////////
class DLL_PUBLIC RenderSceneLoader : public ResourceLoader
{
public:
    
    POOLED(Pools::Loader)
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    RenderSceneLoader();
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual ~RenderSceneLoader();
    
    ////////////////////////////////////////////////////////////////////////
    /// @brief Returns a clone of this object.
    /// Typically, this function is implemented as 'return new MyLoaderClass();',
    /// but you are free to do whatever you want.
    ////////////////////////////////////////////////////////////////////////
    virtual ResourceLoader* clone() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns true if the file given is loadable by this loader.
    //////////////////////////////////////////////////////////////////////
    virtual bool isLoadable( const std::string& filepath ) const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Loads a scene from a given filepath , if possible.
    /// Notes that default implementation is not able to load a RenderScene
    /// from a file.
    //////////////////////////////////////////////////////////////////////
    virtual RenderSceneHolder load ( const std::string& name , const std::string& filepath = std::string() ) const;
    
};

/// @brief ResourceLoaderFactory for RenderSceneLoader.
typedef ResourceLoaderFactory<RenderSceneLoader> RenderSceneLoaderFactory;

//////////////////////////////////////////////////////////////////////
/// @brief A RenderScene Resource Manager.
//////////////////////////////////////////////////////////////////////
class DLL_PUBLIC RenderSceneManager : public SpecializedResourceManager<RenderScene, RenderSceneLoader>
{
public:
    
    POOLED( Pools::Manager )
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    RenderSceneManager ( const std::string& name = "DefaultRenderSceneManager" );
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual ~RenderSceneManager () noexcept ( false );
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Loads a RenderScene into the RenderScene list.
    /// If 'filename' is empty, the function will use the Default RenderSceneLoader.
    //////////////////////////////////////////////////////////////////////
    virtual RenderSceneUser load ( const std::string& name , const std::string& filename = std::string() );
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Loads a RenderSceneHolder directly to the list.
    //////////////////////////////////////////////////////////////////////
    virtual RenderSceneUser load ( const RenderSceneHolder& holder );
    
    //////////////////////////////////////////////////////////////////////
    /// @brief This function should draw every RenderScene object.
    //////////////////////////////////////////////////////////////////////
    virtual void drawScenes ( const EventHolder& e ) const ;
};

/// @brief SpecializedCountedObjectHolder for RenderSceneManager.
typedef SpecializedCountedObjectHolder < RenderSceneManager > RenderSceneManagerHolder ;

GreEndNamespace

#endif
