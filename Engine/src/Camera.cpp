////////////////////////////////////////////////////
//  File    : Camera.cpp
//  Project : GRE
//
//  Created by Jacques Tronconi on 07/05/2016.
//  
//
////////////////////////////////////////////////////

#include "Camera.h"

GreBeginNamespace

CameraPrivate::CameraPrivate(const std::string& name)
: Resource(name)
{
    iPosition = Vector3(0, 0, 1);
    iHorizontalAngle = 0.0f;
    iVerticalAngle = 0.0f;
    iFieldOfView = 45.0f;
    iNearPlane = 0.01f;
    iFarPlane = 100.0f;
    iViewportRatio = 4.0f / 3.0f;
    iMaxVerticalAngle = 85.0f;
}

CameraPrivate::~CameraPrivate()
{
    
}

const Vector3& CameraPrivate::getPosition() const
{
    return iPosition;
}

void CameraPrivate::setPosition(const Vector3 &position)
{
    iPosition = position;
}

void CameraPrivate::offsetPosition(const Vector3 &offset)
{
    iPosition += offset;
}

float CameraPrivate::getFieldOfView() const
{
    return iFieldOfView;
}

void CameraPrivate::setFieldOfView(float fov)
{
    iFieldOfView = fov;
}

float CameraPrivate::getNearPlane() const
{
    return iNearPlane;
}

float CameraPrivate::getFarPlane() const
{
    return iFarPlane;
}

void CameraPrivate::setNearAndFarPlanes(float near, float far)
{
    iNearPlane = near;
    iFarPlane = far;
}

Matrix4 CameraPrivate::getOrientation() const
{
    Matrix4 orientation;
    orientation = glm::rotate(orientation, glm::radians(iVerticalAngle), Vector3(1,0,0));
    orientation = glm::rotate(orientation, glm::radians(iHorizontalAngle), Vector3(0,1,0));
    return orientation;
}

void CameraPrivate::offsetOrientation(float upAngle, float rightAngle)
{
    iHorizontalAngle = rightAngle;
    iVerticalAngle = upAngle;
}

void CameraPrivate::lookAt(const Vector3 &position)
{
    Vector3 direction = glm::normalize(position - iPosition);
    iVerticalAngle = glm::radians(asinf(-direction.y));
    iHorizontalAngle = -glm::radians(atan2f(-direction.x, -direction.z));
    _normalizeAngles();
}

float CameraPrivate::getViewportAspectRatio() const
{
    return iViewportRatio;
}

void CameraPrivate::setViewportAspectRatio(float ratio)
{
    iViewportRatio = ratio;
}

Vector3 CameraPrivate::forward() const
{
    glm::vec4 forward = glm::inverse(getOrientation()) * glm::vec4(0,0,-1,1);
    return glm::vec3(forward);
}

Vector3 CameraPrivate::up() const
{
    glm::vec4 up = glm::inverse(getOrientation()) * glm::vec4(0,1,0,1);
    return glm::vec3(up);
}

Vector3 CameraPrivate::right() const
{
    glm::vec4 right = glm::inverse(getOrientation()) * glm::vec4(1,0,0,1);
    return glm::vec3(right);
}

Matrix4 CameraPrivate::getMatrix() const
{
    return getProjection() * getView();
}

Matrix4 CameraPrivate::getProjection() const
{
    return glm::perspective(glm::radians(iFieldOfView), iViewportRatio, iNearPlane, iFarPlane);
}

Matrix4 CameraPrivate::getView() const
{
    return getOrientation() * glm::translate(glm::mat4(), -iPosition);
}

void CameraPrivate::_normalizeAngles()
{
    iHorizontalAngle = fmodf(iHorizontalAngle, 360.0f);
    //fmodf can return negative values, but this will make them all positive
    if(iHorizontalAngle < 0.0f)
        iHorizontalAngle += 360.0f;
    
    if(iVerticalAngle > iMaxVerticalAngle)
        iVerticalAngle = iMaxVerticalAngle;
    else if(iVerticalAngle < -iMaxVerticalAngle)
        iVerticalAngle = -iMaxVerticalAngle;
}

// ---------------------------------------------------------------------------------------------------

Camera::Camera(CameraPrivate* pointer)
: SpecializedResourceUser<Gre::CameraPrivate>(pointer)
{
    
}

Camera::Camera(const CameraHolder& holder)
: SpecializedResourceUser<Gre::CameraPrivate>(holder)
{
    
}

Camera::Camera(const Camera& user)
: SpecializedResourceUser<Gre::CameraPrivate>(user)
{
    
}

Camera::~Camera()
{
    
}

GreEndNamespace
