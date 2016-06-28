//
//  CameraCustomNode.h
//  GRE
//
//  Created by Jacques Tronconi on 11/04/2016.
//
//
/*
#ifndef GRE_CameraCustomNode_h
#define GRE_CameraCustomNode_h

#include "CustomNode.h"

GreBeginNamespace

class DLL_PUBLIC CameraCustomNodePrivate : public CustomNodePrivate
{
public:
    
    CameraCustomNodePrivate(const std::string& name, CustomNodeType type, CustomNodePrivate* parent = nullptr);
    virtual ~CameraCustomNodePrivate();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Sets the Camera in this Node.
    //////////////////////////////////////////////////////////////////////
    void setCamera(const Camera& v);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Shortcut for Camera::lookAt().
    //////////////////////////////////////////////////////////////////////
    void setLookAt(const Vector3& lookAt);
    
protected:
    
    /// @brief Holds the Node's Camera.
    Camera _mCamera;
};

class DLL_PUBLIC CameraCustomNode : public CustomNode
{
public:
    
    GreResourceUserMakeConstructorsPooled( CameraCustomNode , Resource );
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Sets the Camera in this Node.
    //////////////////////////////////////////////////////////////////////
    void setCamera(const Camera& camera);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Shortcut for Camera::lookAt().
    //////////////////////////////////////////////////////////////////////
    void setLookAt(const Vector3& lookAt);
    
private:
    
    /// @brief Holds shortcut to direct Node.
    std::weak_ptr<CameraCustomNodePrivate> _mCameraNode;
};

GreEndNamespace

#endif
*/