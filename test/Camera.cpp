#include "Camera.h"

using namespace core;

Camera::Camera()
{
}

matrix4x4 Camera::GetViewMatrix() const
{

    matrix4x4 cameraMatrix = q_.to_matrix();
    vector3 u = vector3(cameraMatrix(0,1), cameraMatrix(1,1), cameraMatrix(2,1));
    vector3 v = vector3(cameraMatrix(0,2), cameraMatrix(1,2), cameraMatrix(2,2));

    return lookat_matrix_lh_dx(p_, p_ + v, u);
}

frustum Camera::GetFrustum () const
{
    return frustum_;
}

void Camera::LookAt(vector3 const& eye, vector3 const& at, vector3 const& up)
{
    vector3 v = normalize(at - eye);
    vector3 r = cross(normalize(up), v);
    vector3 u = cross(v, r);

    q_ = quat(matrix4x4(r.x(), r.y(), r.z(), 0,
                        u.x(), u.y(), u.z(), 0,
                        v.x(), v.y(), v.z(), 0,
                        0, 0, 0, 1));
    p_ = eye;
}

void Camera::RotateCamera(vector3 const& v, real angle)
{
    q_ = rotate_quat(q_, rotation_quat(v, angle));
}

void Camera::SetFrustum(frustum const& frustum)
{
    frustum_ = frustum;
}