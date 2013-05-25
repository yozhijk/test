#include "Camera.h"

using namespace core;

Camera::Camera()
{
}

core::vector3 Camera::GetViewDirection() const
{
    matrix4x4 cameraMatrix = q_.to_matrix();
    return normalize(vector3(cameraMatrix(2,0), cameraMatrix(2,1), cameraMatrix(2,2)));
}

core::vector3 Camera::GetUpDirection() const
{
    matrix4x4 cameraMatrix = q_.to_matrix();
    return normalize(vector3(cameraMatrix(1,0), cameraMatrix(1,1), cameraMatrix(1,2)));
}

core::vector3 Camera::GetRightDirection() const
{
    matrix4x4 cameraMatrix = q_.to_matrix();
    return normalize(vector3(cameraMatrix(0,0), cameraMatrix(0,1), cameraMatrix(0,2)));
}

matrix4x4 Camera::GetViewMatrix() const
{
    matrix4x4 cameraMatrix = q_.to_matrix();
    vector3 u = vector3(cameraMatrix(1,0), cameraMatrix(1,1), cameraMatrix(1,2));
    vector3 v = vector3(cameraMatrix(2,0), cameraMatrix(2,1), cameraMatrix(2,2));

    return lookat_matrix_lh_dx(p_, p_ + v, u);
}

frustum Camera::GetFrustum () const
{
    return frustum_;
}

void Camera::LookAt(vector3 const& eye, vector3 const& at, vector3 const& up)
{
    vector3 v = normalize(at - eye);
    vector3 r = normalize(cross(normalize(up), v));
    vector3 u = normalize(cross(v, r));

    /// matrix should have basis vectors in rows
    /// to be used for quaternion construction
    /// would be good to add several options
    /// to quaternion class
    matrix4x4 cameraMatrix = matrix4x4(r.x(), u.x() , v.x() , 0,
                        r.y(), u.y(), v.y(), 0,
                        r.z(), u.z(), v.z(), 0,
                        0, 0, 0, 1);

    q_ = normalize(quat(cameraMatrix));

    p_ = eye;
}

void Camera::RotateCamera(vector3 const& v, real angle)
{
    q_ *= rotation_quat(v,angle);
}

void Camera::SetFrustum(frustum const& frustum)
{
    frustum_ = frustum;
}

void Camera::Rotate(core::real angle)
{
    RotateCamera(vector3(0,1,0), angle);
}

void Camera::Tilt(core::real angle)
{
    RotateCamera(GetRightDirection(), angle);
}