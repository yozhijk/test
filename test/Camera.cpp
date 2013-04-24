#include "Camera.h"

using namespace core;

Camera::Camera()
{

}

matrix4x4 Camera::GetViewMatrix() const
{
    return view_;
}
frustum Camera::GetFrustum () const
{
    return frustum_;
}

void Camera::LookAt(core::vector3 const& eye, core::vector3 const& at, core::vector3 const& up)
{
    view_ = lookat_matrix_lh_dx(eye, at, up);
}

void Camera::SetFrustum(core::frustum const& frustum)
{
    frustum_ = frustum;
}