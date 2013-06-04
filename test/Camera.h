#ifndef CAMERA_H
#define CAMERA_H

#include "common_types.h"
#include "utils.h"

class Camera
{
public:
    Camera();

    core::vector3 GetViewDirection() const;
    core::vector3 GetUpDirection() const;
    core::vector3 GetRightDirection() const;

    core::matrix4x4 GetViewMatrix() const;
    core::frustum   GetFrustum() const;

    void LookAt(core::vector3 const& eye, core::vector3 const& at, core::vector3 const& up);
    void SetFrustum(core::frustum const& frustum);

    void Rotate(core::real angle);
    void Tilt(core::real angle);
    void MoveForward(core::real distance);

protected:
    void RotateCamera(core::vector3 const& v, core::real angle);

private:
    core::quat q_;
    core::vector3 p_;
    core::frustum frustum_;
};




#endif