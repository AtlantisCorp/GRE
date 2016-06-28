//
//  CameraCustomNode.cpp
//  GRE
//
//  Created by Jacques Tronconi on 11/04/2016.
//
//
/*
#include "CameraCustomNode.h"

GreBeginNamespace

void CameraCustomNodePrivate::setCamera(const Camera& camera)
{
    _mCamera = camera;
}

void CameraCustomNodePrivate::setLookAt(const Vector3 &lookAt)
{
    _mCamera.lookAt(lookAt);
}

// ---------------------------------------------------------------------------------------------------

void CameraCustomNode::setCamera(const Camera& camera)
{
    auto ptr = _mCameraNode.lock();
    if(ptr)
        ptr->setCamera(camera);
}

void CameraCustomNode::setLookAt(const Vector3 &lookAt)
{
    auto ptr = _mCameraNode.lock();
    if(ptr)
        ptr->setLookAt(lookAt);
}

GreEndNamespace
*/