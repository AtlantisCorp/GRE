//////////////////////////////////////////////////////////////////////
//
//  Scene.h
//  This source file is part of Gre
//		(Gang's Resource Engine)
//
//  Copyright (c) 2015 - 2017 Luk2010
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
#include "Renderable.h"

#include "SceneNode.h"
#include "Camera.h"
#include "Light.h"

GreBeginNamespace

//////////////////////////////////////////////////////////////////////
/// @brief Specialized RenderNode for lights objects.
//////////////////////////////////////////////////////////////////////
class DLL_PUBLIC LightRenderNode : public RenderNode
{
public:

    POOLED ( Pools::Scene )

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    LightRenderNode ( const std::string & name = std::string() ) ;

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    LightRenderNode ( const LightHolder & light , const std::string & name = std::string() ) ;

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual ~LightRenderNode () noexcept ( false ) ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Returns 'iLight'.
    //////////////////////////////////////////////////////////////////////
    virtual const LightHolder & getLight () const ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Returns 'iLight'.
    //////////////////////////////////////////////////////////////////////
    virtual LightHolder & getLight () ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Changes 'iLight'.
    //////////////////////////////////////////////////////////////////////
    virtual void setLight ( const LightHolder& rhs ) ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the Camera used to 'see' from the light point-of-view.
    //////////////////////////////////////////////////////////////////////
    virtual const CameraHolder & getLightCamera () const ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the Camera used to 'see' from the light point-of-view.
    //////////////////////////////////////////////////////////////////////
    virtual CameraHolder & getLightCamera () ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Binds the actual light , with its camera projection-view
    /// matrix , and use the RenderNode::use() function.
    //////////////////////////////////////////////////////////////////////
    virtual void use ( const TechniqueHolder & technique , bool prepareforrender = true ) const ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Loads a Camera bound to the active light position and direction's
    /// changes.
    //////////////////////////////////////////////////////////////////////
    virtual void loadLightCamera () ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Loads a texture that may (or may not) be used as a shadow
    /// map for a technique shader.
    //////////////////////////////////////////////////////////////////////
    virtual void loadShadowTexture ( uint32_t width , uint32_t height ) ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Returns true if the light is visible from the camera.
    //////////////////////////////////////////////////////////////////////
    virtual bool isLightVisibleFrom ( const CameraHolder & camera ) const ;

protected:

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual void onPositionChangedEvent ( const PositionChangedEvent& e ) ;

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual void onDirectionChangedEvent ( const DirectionChangedEvent& e ) ;

protected:

    /// @brief Holds a light object.
    LightHolder iLight ;

    /// @brief Holds a Camera updated with the light position and direction
    /// values. Notes that for the frustrum to be effective , default values
    /// for z-near and z-far are used ( 0.1f and 100.0f ) but one can change those
    /// values without any problems.
    CameraHolder iLightCamera ;

    /// @brief Holds a Texture that can be used (or not) by a ShadowMap technique
    /// shader. This texture is null by default , and must be initialized using
    /// 'loadShadowTexture()' .
    TextureHolder iShadowTexture ;
};

/// @brief
typedef SpecializedCountedObjectUser<LightRenderNode> LightRenderNodeUser ;

/// @brief
typedef SpecializedCountedObjectHolder<LightRenderNode> LightRenderNodeHolder ;

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
/// The RenderScene object is responsible for a Root node defining a Scene,
/// a set of Cameras that should track the visible nodes.
///
/// ### Managing RenderNode
///
/// Creates a RenderNode using '::createNode()' . Configure it and then ,
/// adds it to the Node's tree using '::addNode()' . The RenderNode will be
/// added to the root node.
///
/// ### Managing Camera's
///
/// Cameras can be added to filters Visible and Unvisibles nodes. This
/// operation takes place in the update function. Notes that cameras registered
/// to the scene manager only serves to update the visible nodes list. Cameras
/// updates is made by the Camera Manager , or by a Camera Controller.
///
/// ### Managing the render
///
/// One can differ the render directly from the Scene object in modifying
/// the binding made in the 'use' function. By default, this function
/// binds only custom parameters.
///
//////////////////////////////////////////////////////////////////////
class DLL_PUBLIC RenderScene : public Renderable
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
    /// @brief Creates a special Light nodes, with given name.
    //////////////////////////////////////////////////////////////////////
    virtual LightRenderNodeHolder createLightNode ( const std::string & name = std::string() ) const ;

    //////////////////////////////////////////////////////////////////////
    /// @brief
    //////////////////////////////////////////////////////////////////////
    virtual LightRenderNodeHolder createLightNode ( const LightHolder& light , const std::string& name = std::string() ) const ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Adds a LightRenderNode and adds the light to the list of
    /// rendered lights.
    //////////////////////////////////////////////////////////////////////
    virtual void addLightNode ( const LightRenderNodeHolder & lightnode ) ;

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

    ////////////////////////////////////////////////////////////////////////
    /// @brief Returns the nodes updated for the given Camera.
    ////////////////////////////////////////////////////////////////////////
    virtual const RenderNodeHolderList & getNodesForCamera ( const CameraHolder & camera ) const ;

    ////////////////////////////////////////////////////////////////////////
    /// @brief Returns the lights activated with a range visible by the given
    /// camera.
    ////////////////////////////////////////////////////////////////////////
    virtual const std::list<LightRenderNodeHolder> & getActivatedLightsForCamera ( const CameraHolder & camera ) const ;

    ////////////////////////////////////////////////////////////////////////
    /// @brief Registers a new camera. When this object process an update event ,
    /// it will look for nodes that are visible by this camera.
    ///
    /// @note
    /// It should assume your nodes are correctly grouped in supernodes and every
    /// children nodes has a bounding box completly contained in the parent's
    /// bounding box.
    ///
    /// @note
    /// This function does not update in any way the content of the camera ,
    /// or at least it should not ( an overwritten function could do it , but
    /// this is not recommended unless the scene object wants to correct the
    /// camera position depending on nodes ) .
    ///
    ////////////////////////////////////////////////////////////////////////
    virtual void addCamera ( const CameraHolder & camera ) ;

    ////////////////////////////////////////////////////////////////////////
    /// @brief Returns an holder to a Camera if this one is registered in the
    /// scene manager object.
    ////////////////////////////////////////////////////////////////////////
    virtual const CameraHolder getCamera ( const std::string & name ) const ;

protected:

    ////////////////////////////////////////////////////////////////////////
    /// @brief Updates the scene managed nodes. First , it updates the currently
    /// activated light nodes list. Then , it will look for every nodes to
    /// see if they are visible are not. Please @see notes from 'addCamera'.
    ///
    /// @note
    /// The Scene object is , by default , updated by the Scene Manager. You can
    /// deactivate this behaviour by setting the loading option 'SendUpdate' to
    /// false.
    ///
    ////////////////////////////////////////////////////////////////////////
    virtual void onUpdateEvent ( const UpdateEvent & e ) ;

    ////////////////////////////////////////////////////////////////////////
    /// @brief Computes light nodes visible by the camera.
    //////////////////////////////////////////////////////////////////////
    virtual std::list < LightRenderNodeHolder > iComputeLightNodes (const CameraHolder & camera ,
                                                                    float elapsed) const ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Computes the nodes visible by the camera.
    //////////////////////////////////////////////////////////////////////
    virtual RenderNodeHolderList iComputeNodes (const CameraHolder & camera ,
                                                float elapsed) const ;

protected:

    /// @brief The Root RenderNode.
    RenderNodeHolder iRootNode ;

    /// @brief Holds nodes sorted by camera. The nodes in this map are updated
    /// only if the camera or any nodes in the scene has moved.
    std::map < CameraHolder , RenderNodeHolderList > iNodesByCamera ;

    /// @brief Holds every light nodes. At update , if the light is visible by the
    /// camera , they will be added to the activated lights list.
    std::list < LightRenderNodeHolder > iLightNodes ;

    /// @brief Holds every light nodes activated and with a range visible by the
    /// camera holded.
    std::map < CameraHolder , std::list<LightRenderNodeHolder> > iLightNodesByCamera ;
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

protected:

	/// @brief true if initialized was successfull.
	bool iInitialized ;
};

/// @brief SpecializedCountedObjectHolder for RenderSceneManager.
typedef SpecializedCountedObjectHolder < RenderSceneManager > RenderSceneManagerHolder ;

GreEndNamespace

#endif
