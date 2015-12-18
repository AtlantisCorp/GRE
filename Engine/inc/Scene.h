//
//  Scene.h
//  GRE
//
//  Created by Jacques Tronconi on 12/12/2015.
//
//

#ifndef GRE_Scene_h
#define GRE_Scene_h

#include "Mesh.h"
#include "Node.h"
#include "Camera.h"

GRE_BEGIN_NAMESPACE

//////////////////////////////////////////////////////////////////////
/// @brief A Generic Base Scene object.
///
/// The default implementation does nothing. You should create
/// or use a plugin that register a correct ScenePrivate object,
/// and load it to your Renderer object.
///
//////////////////////////////////////////////////////////////////////
class DLL_PUBLIC ScenePrivate : public Resource
{
    
public:
    
    POOLED(Pools::Resource)
    
    ScenePrivate(const std::string& name);
    virtual ~ScenePrivate();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the Root node from the node tree.
    /// Dependent of the implementation. The default implementation
    /// returns Node::Null.
    //////////////////////////////////////////////////////////////////////
    virtual Node& getRoot();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the Root node from the node tree.
    /// Dependent of the implementation. The default implementation
    /// returns Node::Null.
    //////////////////////////////////////////////////////////////////////
    virtual const Node& getRoot() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Creates a new node.
    /// Dependent of the implementation. The default implementation
    /// returns Node::Null.
    /// @param mesh     A Mesh to initialize the Node with.
    //////////////////////////////////////////////////////////////////////
    virtual Node createNode(const Mesh& mesh);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Empty the Scene and destroy every Node objects.
    //////////////////////////////////////////////////////////////////////
    virtual void clear();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the currently active camera.
    //////////////////////////////////////////////////////////////////////
    virtual Camera& getCamera();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the currently active camera.
    //////////////////////////////////////////////////////////////////////
    virtual const Camera& getCamera() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns a list of the Nodes, sorted by given filter.
    /// @param filter   A filter defined by the Node to sort them.
    /// If the filter given is not implemented, Node::Filter::Default is
    /// used as a fallback generally, but this behaviour can be undefined.
    /// @note By default, not implemented function return an empty vector.
    //////////////////////////////////////////////////////////////////////
    std::vector<const Node> getNodesByFilter(Node::Filter filter) const;
};

//////////////////////////////////////////////////////////////////////
/// @brief A Generic Scene object user.
///
/// Always use this object to access the ScenePrivate Resource. You can
/// obtain an object of this type using the ResourceManager Resource
/// loading system, or using Renderer::loadSceneByName(), which use also
/// the ResourceManager Resource loading system, but also call directly
/// Renderer::loadSceneByResource().
///
//////////////////////////////////////////////////////////////////////
class DLL_PUBLIC Scene : public ResourceUser
{
    
public:
    
    Scene();
    Scene(const Scene& rhs);
    Scene(Scene&& rhs);
    explicit Scene(const ResourceUser& rhs);
    
    ~Scene();
    
    Scene& operator = (const Scene& rhs);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the Root node from the node tree, or Node::Null if
    /// ScenePrivate is not valid.
    //////////////////////////////////////////////////////////////////////
    Node& getRoot();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the Root node from the node tree, or Node::Null if
    /// ScenePrivate is not valid.
    //////////////////////////////////////////////////////////////////////
    const Node& getRoot() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Creates a new Node, or Node::Null if ScenePrivate is not
    /// valid.
    /// @param mesh     A Mesh to initialize the Node with.
    //////////////////////////////////////////////////////////////////////
    Node createNode(const Mesh& mesh);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Empty the Scene and destroy every Node objects.
    //////////////////////////////////////////////////////////////////////
    void clear();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the currently active camera.
    //////////////////////////////////////////////////////////////////////
    Camera& getCamera();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the currently active camera.
    //////////////////////////////////////////////////////////////////////
    const Camera& getCamera() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns a list of the Nodes.
    /// It is a convenient function to call for ::getNodesByFilter(Node::Filter::Default) .
    //////////////////////////////////////////////////////////////////////
    std::vector<const Node> getNodes() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns a list of the Nodes, sorted by given filter.
    /// @param filter   A filter defined by the Node to sort them.
    /// If the filter given is not implemented, Node::Filter::Default is
    /// used as a fallback generally, but this behaviour can be undefined.
    //////////////////////////////////////////////////////////////////////
    std::vector<const Node> getNodesByFilter(Node::Filter filter) const;
    
    /// @brief A Null Scene User.
    static Scene Null;
    
private:
    
    std::weak_ptr<ScenePrivate> _mScene;
};

//////////////////////////////////////////////////////////////////////
/// @brief A basic loader to load Scene Objects.
//////////////////////////////////////////////////////////////////////
class DLL_PUBLIC SceneLoader : public ResourceLoader
{
public:
    
    POOLED(Pools::Loader)
    
    SceneLoader();
    virtual ~SceneLoader();
    
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

typedef ResourceLoaderFactory<SceneLoader> SceneLoaderFactory;

GRE_END_NAMESPACE

#endif
