//
//  Node.h
//  GRE
//
//  Created by Jacques Tronconi on 14/12/2015.
//
//

#ifndef GRE_Node_h
#define GRE_Node_h

#include "Pools.h"
#include "Resource.h"
#include "Listener.h"
#include "Emitter.h"
#include "Mesh.h"

GRE_BEGIN_NAMESPACE

//////////////////////////////////////////////////////////////////////
/// @brief A Generic Base Node object.
///
/// Why make a Resource object for a Node ?
///
/// A Node object can be created by any plugin (from the Scene object),
/// and so needs to have an interface with which the user will be able to
/// interact normally.
///
/// The Node implementator should not take care about adding those
/// objects to the ResourceManager. He can just hold an std::vector filled
/// with std::shared_ptr, pointing to NodePrivate object.
/// The Goal here is to make Node an interface fully customizable for the
/// maintainer, but also easilly usable by the user.
///
//////////////////////////////////////////////////////////////////////
class DLL_PUBLIC NodePrivate : public Resource
{
public:
    
    typedef std::weak_ptr<NodePrivate> NodePtr;
    
    POOLED(Pools::Resource)
    
    NodePrivate(const std::string& name = std::string());
    virtual ~NodePrivate();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Adds a new child to this Node.
    /// In the case the Node could not be added, Node::Null is returned by
    /// by the Node user object. This function should return nullptr.
    /// The Node may or may not store the child, depending on the
    /// implementation. Keep in mind that a std::shared_ptr has to manage
    /// the lifetime of the NodePrivate object, because Node is a ResourceUser
    /// so it only has a std::weak_ptr.
    /// @param child    The Node to add.
    //////////////////////////////////////////////////////////////////////
    virtual NodePtr addChild(NodePtr child);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Translate the Node by given vector.
    //////////////////////////////////////////////////////////////////////
    virtual void translate(const Vector3& vector);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Set the visible property of the Node.
    /// Normally, if the visible property is set to false, the Node will
    /// not be drew.
    //////////////////////////////////////////////////////////////////////
    virtual void setVisible(bool visible);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the value of the visible property.
    //////////////////////////////////////////////////////////////////////
    virtual bool isVisible() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns a Matrix 4 holding the local scaling, translation and
    /// rotation of this node.
    //////////////////////////////////////////////////////////////////////
    virtual const Matrix4& getTransformation() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the Mesh contained in a Node.
    //////////////////////////////////////////////////////////////////////
    virtual Mesh& getMesh();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the Mesh contained in a Node.
    //////////////////////////////////////////////////////////////////////
    virtual const Mesh& getMesh() const;
    
protected:
    
    /// @brief Visible property (will be drew or not.)
    bool _mIsVisible;
    
};

//////////////////////////////////////////////////////////////////////
/// @brief Proxy object to use a Node.
/// The Node User object can also be a Listener. It permits the Node to
/// be updated by the Scene object, and by the Renderer.
//////////////////////////////////////////////////////////////////////
class DLL_PUBLIC Node : public ResourceUser, public Transmitter
{
public:
    
    /// @brief Describes some filters used when accessing Nodes.
    enum class Filter
    {
        FarthestToNearest,  ///< @brief Filter Nodes from the farthest to the nearest.
        NearestToFarthest,  ///< @brief Filter Nodes from the nearest to the farthest.
        Default             ///< @brief No filters.
    };
    
public:
    
    POOLED(Pools::Resource)
    
    Node();
    Node(Node&& rhs);
    Node(const Node& rhs);
    explicit Node(const ResourceUser& rhs);
    
    ~Node();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Adds a new child to this Node.
    /// @param child    The Node to add.
    //////////////////////////////////////////////////////////////////////
    Node addChild(Node child);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Translate the Node by given vector.
    //////////////////////////////////////////////////////////////////////
    void translate(const Vector3& vector);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Set the visible property of the Node.
    //////////////////////////////////////////////////////////////////////
    void setVisible(bool visible);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the value of the visible property.
    //////////////////////////////////////////////////////////////////////
    bool isVisible() const;
    
    void rotate(float angle, const Vector3& axe) { }
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns a Matrix 4 holding the local scaling, translation and
    /// rotation of this node.
    //////////////////////////////////////////////////////////////////////
    const Matrix4& getTransformation() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the Mesh contained in a Node.
    //////////////////////////////////////////////////////////////////////
    Mesh& getMesh();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the Mesh contained in a Node.
    //////////////////////////////////////////////////////////////////////
    const Mesh& getMesh() const;
    
    /// @brief Represent a Node null.
    static Node Null;
    
private:
    
    /// @brief Hold a pointer to real class object.
    std::weak_ptr<NodePrivate> _mNode;
};

GRE_END_NAMESPACE

#endif
