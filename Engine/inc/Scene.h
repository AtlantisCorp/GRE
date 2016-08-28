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

#include "SceneNode.h"
#include "Camera.h"
#include "Pass.h"

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
/// Pass Management : The RenderScene object automaticaly creates a Default
/// PassPurpose::First Pass object. This Pass normally should only draws
/// every objects in the Scene, according to the active Camera, with a
/// pass-through HardwareProgram. But this behaviour is not guaranteed.
///
/// Rendering Notes : Render is done by giving the Renderer the list of Pass
/// the Renderer should render. Those Pass have all required informations 
/// for the Renderer to render the RenderScene.
///
/// Camera Management : Cameras can be created as freestanding objects. You can
/// connect a Camera to a RenderNode using 'Camera::connectRenderNode()',
/// this RenderNode will be available as a RenderNodeHolder in your Camera
/// class.
///
/// You should create a RenderNode using 'RenderScene::createNode()'. This
/// RenderNode will not be connected to any other RenderNode. Set a Mesh,
/// set its position ( and other Transformation properties ), and add it
/// to the RenderScene using 'RenderScene::addNode()'. This will add the
/// RenderNode to the Root RenderNode. If the RenderNode given is not
/// acceptable to the Root Node ( 'iParent' property is still invalid ),
/// you can change the Root Node by using 'RenderScene::setRoot()'.
///
/// RenderNodes updating is managed by the RenderScene object. The
/// RenderSceneManager updates each RenderScene, those will update only
/// the Root Node which will update its children, ... Add some custom
/// behaviours when updating a RenderNode using 'Resource::addAction()'
/// functions, or 'Resource::onNextEvent()'.
///
/// Rendering particular RenderNode for a particular Pass : Overwrite the
/// 'Pass::isRenderNodeDrawable()' function to filter RenderNodes returned
/// by the 'RenderScene::findNodesForPass()' function.
///
//////////////////////////////////////////////////////////////////////
class DLL_PUBLIC RenderScenePrivate : public Resource
{
public:
    
    POOLED(Pools::Scene)
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    RenderScenePrivate(const std::string& name);
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual ~RenderScenePrivate();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the Root RenderNode.
    //////////////////////////////////////////////////////////////////////
    virtual const RenderNodeHolder& getRootNode() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Changes the Root RenderNode and returns the new Root.
    /// Check the return value with the given RenderNode to check if the
    /// Root Node has been successfully changed to your RenderNode.
    //////////////////////////////////////////////////////////////////////
    virtual const RenderNodeHolder& setRootNode ( const RenderNodeHolder& rendernode );
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Creates a new RenderNode.
    //////////////////////////////////////////////////////////////////////
    virtual RenderNodeHolder createNode() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Creates a new RenderNode with given name.
    //////////////////////////////////////////////////////////////////////
    virtual RenderNodeHolder createNodeWithName ( const std::string& name ) const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Add a RenderNode to this Scene.
    /// If the RenderNode couldn't be added, its 'iParent' property will
    /// still be invalid.
    //////////////////////////////////////////////////////////////////////
    virtual void addNode ( const RenderNodeHolder& rendernode );
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Removes the given RenderNode from this Scene.
    //////////////////////////////////////////////////////////////////////
    virtual void removeNode ( RenderNodeIdentifier identifier );
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Finds a RenderNode with its identifier.
    //////////////////////////////////////////////////////////////////////
    virtual RenderNodeHolder findNode ( RenderNodeIdentifier identifier );
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Finds a RenderNode with its identifier.
    //////////////////////////////////////////////////////////////////////
    virtual const RenderNodeHolder findNode ( RenderNodeIdentifier identifier ) const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns a list of RenderNode, visible by the given Camera
    /// object, and sorted with the given filter.
    //////////////////////////////////////////////////////////////////////
    virtual RenderNodeHolderList findNodes ( const CameraHolder& camera , const RenderSceneFilter& filter ) const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns a list of RenderNode, visible by the given Camera
    /// object, sorted with the given filter and by the given Pass.
    ///
    /// This is a slow function , because it has to get RenderNodes visible
    /// by the CameraHolder, throw objects not for given pass , and then
    /// sort those objects.
    //////////////////////////////////////////////////////////////////////
    virtual RenderNodeHolderList findNodesForPass ( PassNumber passnumber , const CameraHolder& camera , const RenderSceneFilter& filter ) const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Create a Default Pass given its name and number.
    /// If passNumber is already taken, return Pass::Null.
    //////////////////////////////////////////////////////////////////////
    virtual Pass createAndAddPass ( const std::string& name , const PassNumber& passNumber );
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Adds given custom created pass.
    /// It may return Pass::Null if an error occured.
    //////////////////////////////////////////////////////////////////////
    virtual Pass addPass ( const PassHolder& customPass );
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the first Pass with given name.
    //////////////////////////////////////////////////////////////////////
    virtual Pass getPassByName ( const std::string& name );
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the pass at given number.
    //////////////////////////////////////////////////////////////////////
    virtual Pass getPassByNumber ( const PassNumber& passNumber );
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns a PassHolder from given PassNumber.
    //////////////////////////////////////////////////////////////////////
    virtual PassHolder getPassHolder ( const PassNumber& passnumber );
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns a PassHolder from given PassNumber.
    //////////////////////////////////////////////////////////////////////
    virtual const PassHolder getPassHolder ( const PassNumber& passnumber ) const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Removes the first Pass with given name.
    //////////////////////////////////////////////////////////////////////
    virtual void removePassByName ( const std::string& name );
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Removes the Pass with given number.
    //////////////////////////////////////////////////////////////////////
    virtual void removePassByNumber ( const PassNumber& passNumber );
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns every active Pass, sorted by PassNumber from first
    /// to last.
    //////////////////////////////////////////////////////////////////////
    virtual PassHolderList getActivePasses() const;
    
protected:
    
    /// @brief Holds every Pass in the Scene.
    PassHolderList iPasses;
    
    /// @brief The Root RenderNode.
    RenderNodeHolder iRootNode;
};

/// @brief Common typedef to Specialize ResourceHolder.
typedef SpecializedResourceHolder<RenderScenePrivate> RenderSceneHolder;

/// @brief Common typedef to make RenderSceneHolder List.
typedef SpecializedResourceHolderList<RenderScenePrivate> RenderSceneHolderList;

//////////////////////////////////////////////////////////////////////
/// @brief SpecializedResourceUser for RenderScenePrivate.
//////////////////////////////////////////////////////////////////////
class DLL_PUBLIC RenderScene : public SpecializedResourceUser<RenderScenePrivate>
{
public:
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    RenderScene ( const RenderScenePrivate* pointer );
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    RenderScene ( const RenderSceneHolder& holder );
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    RenderScene ( const RenderScene& user );
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual ~RenderScene();
    
    /// @brief Null RenderScene.
    static RenderScene Null;
};

//////////////////////////////////////////////////////////////////////
/// @brief A basic loader to load RenderScene Objects.
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
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Loads a scene.
    //////////////////////////////////////////////////////////////////////
    virtual RenderSceneHolder load ( const std::string& name ) const = 0;
    
};

/// @brief ResourceLoaderFactory for RenderSceneLoader.
typedef ResourceLoaderFactory<RenderSceneLoader> RenderSceneLoaderFactory;

/// @brief SpecializedResourceManager for RenderScenePrivate.
typedef SpecializedResourceManager<RenderScenePrivate> RenderSceneManager;

GreEndNamespace

#endif
