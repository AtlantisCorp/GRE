//
//  Camera.cpp
//  GRE
//
//  Created by Jacques Tronconi on 11/12/2015.
//
//

#define _USE_MATH_DEFINES
#include <cmath>
#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>

GRE_BEGIN_NAMESPACE

static const float MaxVerticalAngle = 85.0f; //must be less than 90 to avoid gimbal lock
static const Vector3 Zero = Vector3(0,0,0);

CameraPrivate::CameraPrivate(const std::string& name)
: ListenerPrivate(name), _position(0.0f, 0.0f, 1.0f), _horizontalAngle(0.0f), _verticalAngle(0.0f), _fieldOfView(50.0f), _nearPlane(0.01f), _farPlane(100.0f), _viewportAspectRatio(4.0f/3.0f), _moveSpeed(1.0f)
{
    
}

CameraPrivate::~CameraPrivate()
{
    
}

const Vector3& CameraPrivate::getPosition() const
{
    return _position;
}

void CameraPrivate::setPosition(const Vector3 &position)
{
    _position = position;
}

void CameraPrivate::offsetPosition(const Vector3 &offset)
{
    _position += offset;
}

float CameraPrivate::getFieldOfView() const
{
    return _fieldOfView;
}

void CameraPrivate::setFieldOfView(float fov)
{
    _fieldOfView = fov;
}

float CameraPrivate::getNearPlane() const
{
    return _nearPlane;
}

float CameraPrivate::getFarPlane() const
{
    return _farPlane;
}

void CameraPrivate::setNearAndFarPlanes(float near, float far)
{
    _nearPlane = near;
    _farPlane  = far;
}

Matrix4 CameraPrivate::getOrientation() const
{
    Matrix4 orientation;
    orientation = glm::rotate(orientation, glm::radians(_verticalAngle), Vector3(1,0,0));
    orientation = glm::rotate(orientation, glm::radians(_horizontalAngle), Vector3(0,1,0));
    return orientation;
}

void CameraPrivate::offsetOrientation(float upAngle, float rightAngle)
{
    _horizontalAngle += rightAngle;
    _verticalAngle += upAngle;
}

void CameraPrivate::lookAt(const Vector3 &position)
{
    Vector3 direction = glm::normalize(position - _position);
    _verticalAngle = glm::radians(asinf(-direction.y));
    _horizontalAngle = -glm::radians(atan2f(-direction.x, -direction.z));
    normalizeAngles();
}

float CameraPrivate::getViewportAspectRatio() const
{
    return _viewportAspectRatio;
}

void CameraPrivate::setViewportAspectRatio(float ratio)
{
    _viewportAspectRatio = ratio;
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
    return glm::perspective(glm::radians(_fieldOfView), _viewportAspectRatio, _nearPlane, _farPlane);
}

Matrix4 CameraPrivate::getView() const
{
    return getOrientation() * glm::translate(glm::mat4(), -_position);
}

void CameraPrivate::normalizeAngles()
{
    _horizontalAngle = fmodf(_horizontalAngle, 360.0f);
    //fmodf can return negative values, but this will make them all positive
    if(_horizontalAngle < 0.0f)
        _horizontalAngle += 360.0f;
    
    if(_verticalAngle > MaxVerticalAngle)
        _verticalAngle = MaxVerticalAngle;
    else if(_verticalAngle < -MaxVerticalAngle)
        _verticalAngle = -MaxVerticalAngle;
}

void CameraPrivate::onEvent(const Event &e)
{
    if(e.getType() == EventType::Update)
    {
        UpdateEvent ue = e.to<UpdateEvent>();
        float etime = (float) ue.elapsedTime.count() / 100000000.0f;
        if(_keyboard.isKeyDown(Key::W)) {
            offsetPosition(forward() * etime * _moveSpeed);
        }
        else if(_keyboard.isKeyDown(Key::S)) {
            offsetPosition(-forward() * etime * _moveSpeed);
        }
        if(_keyboard.isKeyDown(Key::A)) {
            offsetPosition(-right() * etime * _moveSpeed);
        } else if(_keyboard.isKeyDown(Key::D)) {
            offsetPosition(right() * etime * _moveSpeed);
        }
    }
}

void CameraPrivate::setKeyboardListened(Gre::Keyboard &keyboard)
{
    _keyboard = keyboard;
}

float CameraPrivate::getMoveSpeed() const
{
    return _moveSpeed;
}

void CameraPrivate::setMoveSpeed(float speed)
{
    _moveSpeed = speed;
}

Camera::Camera(const std::string& name)
: Listener(new CameraPrivate(name)), _camera()
{
    _camera = std::dynamic_pointer_cast<CameraPrivate>(_mListener);
}

Camera::Camera(const Camera& rhs)
: Listener(rhs)
{
    _camera = std::dynamic_pointer_cast<CameraPrivate>(_mListener);
}

Camera::Camera(ListenerPrivate* rhs)
: Listener(rhs)
{
    _camera = std::dynamic_pointer_cast<CameraPrivate>(_mListener);
}

Camera::~Camera()
{
    
}

void Camera::listen(Keyboard& keyboard)
{
    Listener::listen(keyboard);
    auto ptr = _camera.lock();
    if(ptr)
        ptr->setKeyboardListened(keyboard);
}

const Vector3& Camera::getPosition() const
{
    auto ptr = _camera.lock();
    if(ptr)
        return ptr->getPosition();
    return Zero;
}

void Camera::setPosition(const Vector3 &position)
{
    auto ptr = _camera.lock();
    if(ptr)
        ptr->setPosition(position);
}

void Camera::offsetPosition(const Vector3 &offset)
{
    auto ptr = _camera.lock();
    if(ptr)
        ptr->offsetPosition(offset);
}

float Camera::getFieldOfView() const
{
    auto ptr = _camera.lock();
    if(ptr)
        return ptr->getFieldOfView();
    return 0.0f;
}

void Camera::setFieldOfView(float fov)
{
    auto ptr = _camera.lock();
    if(ptr)
        ptr->setFieldOfView(fov);
}

float Camera::getNearPlane() const
{
    auto ptr = _camera.lock();
    if(ptr)
        return ptr->getNearPlane();
    return 0.0f;
}

float Camera::getFarPlane() const
{
    auto ptr = _camera.lock();
    if(ptr)
        return ptr->getFarPlane();
    return 0.0f;
}

void Camera::setNearAndFarPlanes(float near, float far)
{
    auto ptr = _camera.lock();
    if(ptr)
        ptr->setNearAndFarPlanes(near, far);
}

Matrix4 Camera::getOrientation() const
{
    auto ptr = _camera.lock();
    if(ptr)
        return ptr->getOrientation();
    return Matrix4();
}

void Camera::offsetOrientation(float upAngle, float rightAngle)
{
    auto ptr = _camera.lock();
    if(ptr)
        ptr->offsetOrientation(upAngle, rightAngle);
}

void Camera::lookAt(const Vector3 &position)
{
    auto ptr = _camera.lock();
    if(ptr)
        ptr->lookAt(position);
}

float Camera::getViewportAspectRatio() const
{
    auto ptr = _camera.lock();
    if(ptr)
        return ptr->getViewportAspectRatio();
    return 0.0f;
}

void Camera::setViewportAspectRatio(float ratio)
{
    auto ptr = _camera.lock();
    if(ptr)
        ptr->setViewportAspectRatio(ratio);
}

Vector3 Camera::forward() const
{
    auto ptr = _camera.lock();
    if(ptr)
        return ptr->forward();
    return Vector3();
}

Vector3 Camera::right() const
{
    auto ptr = _camera.lock();
    if(ptr)
        return ptr->right();
    return Vector3();
}

Vector3 Camera::up() const
{
    auto ptr = _camera.lock();
    if(ptr)
        return ptr->up();
    return Vector3();
}

Matrix4 Camera::getMatrix() const
{
    auto ptr = _camera.lock();
    if(ptr)
        return ptr->getMatrix();
    return Matrix4();
}

Matrix4 Camera::getProjection() const
{
    auto ptr = _camera.lock();
    if(ptr)
        return ptr->getProjection();
    return Matrix4();
}

Matrix4 Camera::getView() const
{
    auto ptr = _camera.lock();
    if(ptr)
        return ptr->getView();
    return Matrix4();
}

float Camera::getMoveSpeed() const
{
    auto ptr = _camera.lock();
    if(ptr)
        return ptr->getMoveSpeed();
    return 0.0f;
}

void Camera::setMoveSpeed(float speed)
{
    auto ptr = _camera.lock();
    if(ptr)
        ptr->setMoveSpeed(speed);
}

GRE_END_NAMESPACE
