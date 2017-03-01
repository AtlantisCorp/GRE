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
#include "Light.h"

GreBeginNamespace

/// @brief Options to initialize the RenderScene.
/// A list of options possible are :
///   - "Technique" -> string : Name of the Technique object to load. ( Default = "Default" )
///   - "Loader" -> string : Name of the desired loader. ( Default = "" ) 

typedef std::map < std::string , Variant > RenderSceneOptions ;

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
    RenderScene(const std::string& name , const RenderSceneOptions & options = RenderSceneOptions());
    
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
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Changes 'iTechnique'.
    //////////////////////////////////////////////////////////////////////
    virtual void setTechnique ( const TechniqueUser& technique ) ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the 'iTechnique'.
    //////////////////////////////////////////////////////////////////////
    virtual const TechniqueHolder & getTechnique () const ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the 'iTechnique'.
    //////////////////////////////////////////////////////////////////////
    virtual TechniqueHolder & getTechnique () ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Changes 'iRenderer'.
    //////////////////////////////////////////////////////////////////////
    virtual void setRenderer ( const RendererUser& renderer ) ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Changes 'iRenderTarget'.
    //////////////////////////////////////////////////////////////////////
    virtual void setRenderTarget ( const RenderTargetUser& target ) ;
    
    ////////////////////////////////////////////////////////////////////////
    /// @brief Changes 'iClearColor'.
    ////////////////////////////////////////////////////////////////////////
    virtual void setClearColor ( const Color& color ) ;
    
    ////////////////////////////////////////////////////////////////////////
    /// @brief Returns 'iGlobalLight'.
    ////////////////////////////////////////////////////////////////////////
    virtual const Light & getGlobalLight () const ;
    
    ////////////////////////////////////////////////////////////////////////
    /// @brief Returns 'iGlobalLight'.
    ////////////////////////////////////////////////////////////////////////
    virtual Light & getGlobalLight () ;
    
    ////////////////////////////////////////////////////////////////////////
    /// @brief Changes 'iGlobalLight'.
    ////////////////////////////////////////////////////////////////////////
    virtual void setGlobalLight ( const Light& light ) ;
    
protected:
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Draw the main technique or sub-techniques ( viewports ) .
    //////////////////////////////////////////////////////////////////////
    virtual void drawTechnique ( const TechniqueHolder& technique , const EventHolder& elapsed ) const ;
    
    ////////////////////////////////////////////////////////////////////////
    /// @brief Do some action before rendering.
    ////////////////////////////////////////////////////////////////////////
    virtual void _preRender () const ;
    
    ////////////////////////////////////////////////////////////////////////
    /// @brief Do some action after rendering.
    ////////////////////////////////////////////////////////////////////////
    virtual void _postRender () const ;
    
protected:
    
    /// @brief The Root RenderNode.
    RenderNodeHolder iRootNode ;
    
    /// @brief RenderTarget we want to draw to.
    RenderTargetHolder iRenderTarget ;
    
    /// @brief The Technique we use to draw this RenderScene.
    TechniqueHolder iTechnique ;
    
    /// @brief Renderer we use to draw this RenderScene.
    RendererUser iRenderer ;
    
    /// @brief
    Color iClearColor ;
    
    /// @brief The global directionnal light (simulate the sun).
    Light iGlobalLight ;
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
    /// @brief Loads a scene with given options.
    //////////////////////////////////////////////////////////////////////
    virtual RenderSceneHolder load ( const std::string & name , const RenderSceneOptions & options ) const ;
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
	/// @brief Initialize the default behaviour for this manager.
	/// This installs a default technique with a default program. A valid
	/// HardwareProgramManager and a valid CameraManager are required to
	/// use this function.
	//////////////////////////////////////////////////////////////////////
	void initialize () ;
	
	//////////////////////////////////////////////////////////////////////
	/// @brief Returns 'iInitialized'.
	//////////////////////////////////////////////////////////////////////
	bool isInitialized () const ;
	
    //////////////////////////////////////////////////////////////////////
    /// @brief Loads a RenderSceneHolder directly to the list.
    //////////////////////////////////////////////////////////////////////
    virtual RenderSceneUser load ( const RenderSceneHolder& holder );
    
    //////////////////////////////////////////////////////////////////////
    ///@brief Creates a new RenderScene with given options.
    //////////////////////////////////////////////////////////////////////
    virtual RenderSceneUser load ( const std::string & name , const RenderSceneOptions & options ) ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief This function should draw every RenderScene object.
    //////////////////////////////////////////////////////////////////////
    virtual void drawScenes ( const EventHolder& e ) const ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the technique with given name.
    //////////////////////////////////////////////////////////////////////
    virtual TechniqueUser findTechnique ( const std::string & name ) ;
    
protected:
    
    ////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////
    virtual void onUpdateEvent ( const UpdateEvent& e ) ;
    
protected:
    
    /// @brief Technique registered.
    std::vector < TechniqueHolder > iTechniques ;
	
	/// @brief true if initialized was successfull.
	bool iInitialized ;
};

/// @brief SpecializedCountedObjectHolder for RenderSceneManager.
typedef SpecializedCountedObjectHolder < RenderSceneManager > RenderSceneManagerHolder ;

GreEndNamespace

#endif
