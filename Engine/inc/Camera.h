//
//  Camera.h
//  GRE
//
//  Created by Jacques Tronconi on 11/12/2015.
//
//

#ifndef GRE_Camera_h
#define GRE_Camera_h

#include "Listener.h"
#include "Keyboard.h"

GRE_BEGIN_NAMESPACE

/// @brief Represents a Camera object.
/// Based on an idea from :
/// https://github.com/tomdalling/opengl-series/blob/master/source/04_camera/source/tdogl/Camera.h
class DLL_PUBLIC CameraPrivate : public ListenerPrivate
{
public:
    
    CameraPrivate(const std::string& name);
    ~CameraPrivate();
    
    /// @brief Return the position of the camera.
    const Vector3& getPosition() const;
    /// @brief Set a new position to the camera.
    void setPosition(const Vector3& position);
    void offsetPosition(const Vector3& offset);
    
    /// @brief Returns the vertical viewing angle of the camera, in degrees.
    float getFieldOfView() const;
    void setFieldOfView(float fov);
    
    /// @brief Returns the closest visible distance from the camera.
    float getNearPlane() const;
    /// @brief Returns the farthest visible distance from the camera.
    float getFarPlane() const;
    
    /// @brief Set the near and far distance planes.
    void setNearAndFarPlanes(float near, float far);
    
    /// @brief Returns a rotation matrix that determines the direction the camera is looking.
    Matrix4 getOrientation() const;
    
    /// @brief Offsets the cameras orientation.
    /// The verticle angle is constrained between 85deg and -85deg to avoid gimbal lock.
    /// @param upAngle     the angle (in degrees) to offset upwards. Negative values are downwards.
    /// @param rightAngle  the angle (in degrees) to offset rightwards. Negative values are leftwards.
    void offsetOrientation(float upAngle, float rightAngle);
    
    /// @brief Orients the camera so it is directly facing 'position'.
    void lookAt(const Vector3& position);
    
    /// @brief The width divided by the height of the screen/window/viewport
    float getViewportAspectRatio() const;
    void setViewportAspectRatio(float ratio);
    
    /// @brief A unit vector representing the direction the camera is facing.
    Vector3 forward() const;
    
    /// @brief A unit vector representing the direction to the right of the camera.
    Vector3 right() const;

    /// @brief A unit vector representing the direction out of the top of the camera.
    Vector3 up() const;
    
    /// @brief A combined matrix including perspective projection.
    Matrix4 getMatrix() const;
    
    /// @brief The perspective projection transformation matrix.
    Matrix4 getProjection() const;
    /// @brief The translation and rotation matrix of the camera.
    Matrix4 getView() const;
    
    /// @brief Updates the Camera depending on the event.
    void onEvent (const Event& e);
    
    /// @brief Set the keyboard listened actually.
    void setKeyboardListened(Keyboard& keyboard);
    
    /// @brief Returns the speed of the camera.
    float getMoveSpeed() const;
    void setMoveSpeed(float speed);
    
private:
    
    Vector3  _position;
    float    _horizontalAngle;
    float    _verticalAngle;
    float    _fieldOfView;
    float    _nearPlane;
    float    _farPlane;
    float    _viewportAspectRatio;
    Keyboard _keyboard;
    float    _moveSpeed;
    
    void normalizeAngles();
};

/// @brief Listener proxy to Camera object.
class DLL_PUBLIC Camera : public Listener
{
public:
    
    Camera(const std::string& name = std::string());
    Camera(const Camera& rhs);
    explicit Camera(ListenerPrivate* rhs);
    
    ~Camera();
    
    /// @brief Custom listening function.
    void listen(Keyboard& keyboard);
    
    /// @brief Return the position of the camera.
    const Vector3& getPosition() const;
    /// @brief Set a new position to the camera.
    void setPosition(const Vector3& position);
    void offsetPosition(const Vector3& offset);
    
    /// @brief Returns the vertical viewing angle of the camera, in degrees.
    float getFieldOfView() const;
    void setFieldOfView(float fov);
    
    /// @brief Returns the closest visible distance from the camera.
    float getNearPlane() const;
    /// @brief Returns the farthest visible distance from the camera.
    float getFarPlane() const;
    
    /// @brief Set the near and far distance planes.
    void setNearAndFarPlanes(float near, float far);
    
    /// @brief Returns a rotation matrix that determines the direction the camera is looking.
    Matrix4 getOrientation() const;
    
    /// @brief Offsets the cameras orientation.
    /// The verticle angle is constrained between 85deg and -85deg to avoid gimbal lock.
    /// @param upAngle     the angle (in degrees) to offset upwards. Negative values are downwards.
    /// @param rightAngle  the angle (in degrees) to offset rightwards. Negative values are leftwards.
    void offsetOrientation(float upAngle, float rightAngle);
    
    /// @brief Orients the camera so it is directly facing 'position'.
    void lookAt(const Vector3& position);
    
    /// @brief The width divided by the height of the screen/window/viewport
    float getViewportAspectRatio() const;
    void setViewportAspectRatio(float ratio);
    
    /// @brief A unit vector representing the direction the camera is facing.
    Vector3 forward() const;
    
    /// @brief A unit vector representing the direction to the right of the camera.
    Vector3 right() const;
    
    /// @brief A unit vector representing the direction out of the top of the camera.
    Vector3 up() const;
    
    /// @brief A combined matrix including perspective projection.
    Matrix4 getMatrix() const;
    
    /// @brief The perspective projection transformation matrix.
    Matrix4 getProjection() const;
    /// @brief The translation and rotation matrix of the camera.
    Matrix4 getView() const;
    
    /// @brief Returns the speed of the camera.
    float getMoveSpeed() const;
    void setMoveSpeed(float speed);
    
    /// @brief A Null Camera object.
    static Camera Null;
    
private:
    
    std::weak_ptr<CameraPrivate> _camera;
};

GRE_END_NAMESPACE

#endif
