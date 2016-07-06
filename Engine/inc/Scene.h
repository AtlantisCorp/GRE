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
/// @brief A Generic Base Scene object.
///
/// The default implementation does nothing. You should create
/// or use a plugin that register a correct SceneManagerPrivate object,
/// and load it to your Renderer object.
///
/// Pass Management : The Scene object automaticaly creates a Default
/// PassPurpose::First Pass object. This Pass normally should only draws
/// every objects in the Scene, according to the active Camera, with a
/// pass-through HardwareProgram. But this behaviour is not guaranteed.
///
/// Nodes Management : The Scene should be able to create Nodes. Those
/// Nodes may have different purpose. Normally, each Nodes has a Parent, 
/// but this is not guaranteed and obviously implementation-dependent.
/// 
/// Nodes should be created using `SceneManager::createNode(Type, Name, Other args).`
/// Then they should be added to the Scene or directly to a Node object
/// using SceneManager::addChild() after setting every informations required (like
/// positions and others).
///
/// Nodes Update : Nodes are updated by the SceneManager. The SceneManager
/// only updates Nodes with flag 'iNeedsUpdate' set to true. A Node should
/// be updated when one of its parameters has changed.
///
/// Rendering Notes : Render is done by giving the Renderer the list of Pass
/// the Renderer should render. Those Pass have all required informations 
/// for the Renderer to render the Scene. A Node can customize its Rendering,
/// overriding the function SceneNode::onSettingPass().
///
/// Camera Management : Cameras can be created as freestanding objects. You
/// can register your own Camera using SceneManager::addCamera(). You can attach
/// a Camera to a SceneNode using SceneNode::addCamera().
///
//////////////////////////////////////////////////////////////////////
class DLL_PUBLIC SceneManagerPrivate : public Resource
{
public:
    
    POOLED(Pools::Scene)
    
    SceneManagerPrivate(const std::string& name);
    virtual ~SceneManagerPrivate();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the Root node from the node tree.
    //////////////////////////////////////////////////////////////////////
	virtual SceneNode& getRoot();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the Root node from the node tree.
    //////////////////////////////////////////////////////////////////////
	virtual const SceneNode& getRoot() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Creates a new unnamed SceneNode.
    //////////////////////////////////////////////////////////////////////
    virtual SceneNode createNode();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Creates a new named SceneNode.
    //////////////////////////////////////////////////////////////////////
    virtual SceneNode createNode(const std::string& name);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Destroys a SceneNode using its name.
    //////////////////////////////////////////////////////////////////////
    virtual void destroyNode(const std::string& name);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Destroys a SceneNode.
    //////////////////////////////////////////////////////////////////////
    virtual void destroyNode(SceneNode& node);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Destroys every SceneNode.
    //////////////////////////////////////////////////////////////////////
	virtual void destroyEveryNode();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Empty the Scene and destroy every Node objects.
    //////////////////////////////////////////////////////////////////////
	virtual void clear();
	
	//////////////////////////////////////////////////////////////////////
    /// @brief Adds a Camera.
    //////////////////////////////////////////////////////////////////////
    virtual Camera& addCamera(const Camera& camera);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Creates a default Camera and adds it to the SceneManager.
    //////////////////////////////////////////////////////////////////////
    virtual Camera& addCamera(const std::string& name);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the currently active camera.
    //////////////////////////////////////////////////////////////////////
    virtual Camera& getCurrentCamera();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the currently active camera.
    //////////////////////////////////////////////////////////////////////
    virtual const Camera& getCurrentCamera() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Sets the active Camera given its Node.
    //////////////////////////////////////////////////////////////////////
    virtual void setActiveCamera(const Camera& camera);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns a list of the Nodes, sorted by given filter.
    /// @param filter   A filter defined by the Node to sort them.
    /// If the filter given is not implemented, Node::Filter::Default is
    /// used as a fallback generally, but this behaviour can be undefined.
    /// @note By default, not implemented function return an empty vector.
    //////////////////////////////////////////////////////////////////////
    virtual std::vector<const Node> getNodesByFilter(NodePrivate::Filter filter) const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns a list of SceneNode, that can be directly passed to
    /// a Pass object for rendering. The Nodes can be sorted if necessary,
    /// and the PassPurpose is to know the Pass we should point.
    ///
    /// @note
    /// If the given filter is not available, Node::Filter::None is always 
    /// used.
    //////////////////////////////////////////////////////////////////////
    virtual SceneNodeHolderList getNodesForPass(Gre::PassPurpose pass, NodePrivate::Filter filter = NodePrivate::Filter::None) const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the Nodes from given point of view.
    //////////////////////////////////////////////////////////////////////
    virtual SceneNodeList getNodesFrom(const Camera& camera) const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Create a Default Pass given its name and number.
    /// If passNumber is already taken, return Pass::Null.
    //////////////////////////////////////////////////////////////////////
    virtual Pass createAndAddPass(const std::string& name, const PassNumber& passNumber);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Adds given custom created pass.
    /// It may return Pass::Null if an error occured.
    //////////////////////////////////////////////////////////////////////
    virtual Pass addPass(PassHolder& customPass);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the first Pass with given name.
    //////////////////////////////////////////////////////////////////////
    virtual Pass getPassByName(const std::string& name);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the pass at given number.
    //////////////////////////////////////////////////////////////////////
    virtual Pass getPassByNumber(const PassNumber& passNumber);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Removes the first Pass with given name.
    //////////////////////////////////////////////////////////////////////
    virtual void removePassByName(const std::string& name);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Removes the Pass with given number.
    //////////////////////////////////////////////////////////////////////
    virtual void removePassByNumber(const PassNumber& passNumber);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns every active Pass, sorted by PassNumber from first
    /// to last.
    //////////////////////////////////////////////////////////////////////
    virtual PassHolderList getActivePasses() const;
    
protected:
    
    /// @brief Holds every Pass in the Scene.
    PassHolderList iPasses;
    
    typedef std::map<Gre::PassNumber, Gre::Pass> PassesByPurpose;
    
    /// @brief Catalog to Pass objects by name.
    PassesByPurpose iPassesByNumber;
    
    /// @brief The Root SceneNode.
    SceneNode iRootNode;
    
    /// @brief SceneNode's List.
    SceneNodeHolderList iSceneNodes;
    
    /// @brief Holds a List of Camera.
    CameraHolderList iCameras;
    
    /// @brief Hold the Current Camera.
    Camera iCurrentCamera;
};

/// @brief Common typedef to Specialize ResourceHolder.
typedef SpecializedResourceHolder<SceneManagerPrivate> SceneManagerHolder;

/// @brief Common typedef to make SceneManagerHolder List.
typedef SpecializedResourceHolderList<SceneManagerPrivate> SceneManagerHolderList;

//////////////////////////////////////////////////////////////////////
/// @brief ResourceUser specialized for SceneManager.
//////////////////////////////////////////////////////////////////////
class DLL_PUBLIC SceneManager : public SpecializedResourceUser<SceneManagerPrivate>
{
public:
    
    POOLED(Pools::Scene)
    
    //////////////////////////////////////////////////////////////////////
	/// @brief Constructs a SceneManager from raw pointer.
	//////////////////////////////////////////////////////////////////////
    SceneManager(SceneManagerPrivate* scene);
    
    //////////////////////////////////////////////////////////////////////
	/// @brief Constructs a SceneManager from Holder.
	//////////////////////////////////////////////////////////////////////
	SceneManager(const SceneManagerHolder& holder);
	
	//////////////////////////////////////////////////////////////////////
	/// @brief Constructs a SceneManager from User.
	//////////////////////////////////////////////////////////////////////
	SceneManager(const SceneManager& user);
	
	//////////////////////////////////////////////////////////////////////
	/// @brief Destroys the SceneManager.
	//////////////////////////////////////////////////////////////////////
	~SceneManager();
	
	//////////////////////////////////////////////////////////////////////
    /// @brief Returns the Root node from the node tree.
    //////////////////////////////////////////////////////////////////////
	SceneNode& getRoot();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the Root node from the node tree.
    //////////////////////////////////////////////////////////////////////
	const SceneNode& getRoot() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Creates a new unnamed SceneNode.
    //////////////////////////////////////////////////////////////////////
	SceneNode createNode();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Creates a new named SceneNode.
    //////////////////////////////////////////////////////////////////////
	SceneNode createNode(const std::string& name);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Destroys a SceneNode using its name.
    //////////////////////////////////////////////////////////////////////
	void destroyNode(const std::string& name);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Destroys a SceneNode.
    //////////////////////////////////////////////////////////////////////
	void destroyNode(SceneNode& node);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Destroys every SceneNode.
    //////////////////////////////////////////////////////////////////////
	void destroyEveryNode();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Empty the Scene and destroy every Node objects.
    //////////////////////////////////////////////////////////////////////
	void clear();
	
	//////////////////////////////////////////////////////////////////////
    /// @brief Adds a Camera.
    //////////////////////////////////////////////////////////////////////
	Camera& addCamera(const Camera& camera);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Creates a default Camera and adds it to the SceneManager.
    //////////////////////////////////////////////////////////////////////
	Camera& addCamera(const std::string& name);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the currently active camera.
    //////////////////////////////////////////////////////////////////////
	Camera& getCurrentCamera();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the currently active camera.
    //////////////////////////////////////////////////////////////////////
	const Camera& getCurrentCamera() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Sets the active Camera given its Node.
    //////////////////////////////////////////////////////////////////////
	void setActiveCamera(const Camera& camera);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns a list of the Nodes, sorted by given filter.
    /// @param filter   A filter defined by the Node to sort them.
    /// If the filter given is not implemented, Node::Filter::Default is
    /// used as a fallback generally, but this behaviour can be undefined.
    /// @note By default, not implemented function return an empty vector.
    //////////////////////////////////////////////////////////////////////
	std::vector<const Node> getNodesByFilter(NodePrivate::Filter filter) const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns a list of SceneNode, that can be directly passed to
    /// a Pass object for rendering. The Nodes can be sorted if necessary,
    /// and the PassPurpose is to know the Pass we should point.
    ///
    /// @note
    /// If the given filter is not available, Node::Filter::None is always 
    /// used.
    //////////////////////////////////////////////////////////////////////
	std::vector<const SceneNode> getNodesForPass(Gre::PassPurpose pass, NodePrivate::Filter filter = NodePrivate::Filter::None) const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Create a Default Pass given its name and number.
    /// If passNumber is already taken, return Pass::Null.
    //////////////////////////////////////////////////////////////////////
	Pass createAndAddPass(const std::string& name, const PassNumber& passNumber);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Adds given custom created pass.
    /// It may return Pass::Null if an error occured.
    //////////////////////////////////////////////////////////////////////
	Pass addPass(PassHolder& customPass);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the first Pass with given name.
    //////////////////////////////////////////////////////////////////////
	Pass getPassByName(const std::string& name);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the pass at given number.
    //////////////////////////////////////////////////////////////////////
	Pass getPassByNumber(const PassNumber& passNumber);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Removes the first Pass with given name.
    //////////////////////////////////////////////////////////////////////
	void removePassByName(const std::string& name);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Removes the Pass with given number.
    //////////////////////////////////////////////////////////////////////
	void removePassByNumber(const PassNumber& passNumber);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns every active Pass, sorted by PassNumber from first
    /// to last.
    //////////////////////////////////////////////////////////////////////
	PassHolderList getActivePasses() const;
    
    /// @brief Null Object.
    static SceneManager Null;
};

//////////////////////////////////////////////////////////////////////
/// @brief A basic loader to load SceneManager Objects.
//////////////////////////////////////////////////////////////////////
class DLL_PUBLIC SceneManagerLoader : public ResourceLoader
{
public:
    
    POOLED(Pools::Loader)
    
    SceneManagerLoader();
    virtual ~SceneManagerLoader();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns true if Resource::Type is ::Scene.
    //////////////////////////////////////////////////////////////////////
    virtual bool isTypeSupported(Resource::Type type) const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Loads a scene.
    /// Default implementation returns a null pointer.
    //////////////////////////////////////////////////////////////////////
    virtual Resource* load(Resource::Type type, const std::string& name) const;
    
};

typedef ResourceLoaderFactory<SceneManagerLoader> SceneManagerLoaderFactory;

/// @brief SpecializedResourceManager for SceneManagerPrivate.
typedef SpecializedResourceManager<SceneManagerPrivate> SceneManagerManager;

GreEndNamespace

#endif
