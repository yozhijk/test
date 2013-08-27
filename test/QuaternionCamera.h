#ifndef CAMERA_H
#define CAMERA_H

#include "common_types.h"
#include "utils.h"

#include "ICamera.h"

class QuaternionCamera : public ICamera
{
public:
	QuaternionCamera();
	~QuaternionCamera();

	core::matrix4x4 GetViewMatrix() const;
	core::frustum   GetFrustum() const;

	
	void Rotate(core::real angle);
	void Tilt(core::real angle);
	void MoveForward(core::real distance);
	void SetAspectRatio(core::real aspect);

	void SetFrustum(core::frustum const& frustum);
	void LookAt(core::vector3 const& eye, core::vector3 const& at, core::vector3 const& up);

protected:
	void RotateCamera(core::vector3 const& v, core::real angle);

	core::vector3 GetViewDirection() const;
	core::vector3 GetUpDirection() const;
	core::vector3 GetRightDirection() const;

private:
	core::quat q_;
	core::vector3 p_;
	core::frustum frustum_;

	// Angle to impose tilt constraint
	core::real tilt_;
};

#endif