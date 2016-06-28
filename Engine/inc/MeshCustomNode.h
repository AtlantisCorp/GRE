//
//  MeshCustomNode.h
//  GRE
//
//  Created by Jacques Tronconi on 11/04/2016.
//
//
/*
#ifndef GRE_MeshCustomNode_h
#define GRE_MeshCustomNode_h

#include "CustomNode.h"

GreBeginNamespace

class DLL_PUBLIC MeshCustomNodePrivate : public CustomNodePrivate
{
public:
    
    MeshCustomNodePrivate(const std::string& name, CustomNodeType type, CustomNodePrivate* parent = nullptr);
    virtual ~MeshCustomNodePrivate();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Sets the Mesh in this Node.
    //////////////////////////////////////////////////////////////////////
    void setMesh(const Mesh& v);
    
protected:
    
    /// @brief Holds the Mesh for this Node.
    Mesh _mMesh;
};

class DLL_PUBLIC MeshCustomNode : public CustomNode
{
public:
    
    GreResourceUserMakeConstructorsPooled( MeshCustomNode , Resource );
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Sets the Mesh in this Node.
    //////////////////////////////////////////////////////////////////////
    void setMesh(const Mesh& v);
    
private:
    
    /// @brief Holds shortcut to direct Node.
    std::weak_ptr<MeshCustomNodePrivate> _mMeshNode;
};

GreEndNamespace

#endif
*/