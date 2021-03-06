#include "QuaternionCamera.h"

using namespace core;

QuaternionCamera::QuaternionCamera()
	: tilt_()
{
}

QuaternionCamera::~QuaternionCamera()
{
}

vector3 QuaternionCamera::GetViewDirection() const
{
	matrix4x4 cameraMatrix = q_.to_matrix();
	return normalize(vector3(cameraMatrix(2,0), cameraMatrix(2,1), cameraMatrix(2,2)));
}

vector3 QuaternionCamera::GetUpDirection() const
{
	matrix4x4 cameraMatrix = q_.to_matrix();
	return normalize(vector3(cameraMatrix(1,0), cameraMatrix(1,1), cameraMatrix(1,2)));
}

vector3 QuaternionCamera::GetRightDirection() const
{
	matrix4x4 cameraMatrix = q_.to_matrix();
	return normalize(vector3(cameraMatrix(0,0), cameraMatrix(0,1), cameraMatrix(0,2)));
}

matrix4x4 QuaternionCamera::GetViewMatrix() const
{
	matrix4x4 cameraMatrix = q_.to_matrix();
	vector3 u = vector3(cameraMatrix(1,0), cameraMatrix(1,1), cameraMatrix(1,2));
	vector3 v = vector3(cameraMatrix(2,0), cameraMatrix(2,1), cameraMatrix(2,2));

	return lookat_matrix_lh_dx(p_, p_ + v, u);
}

frustum QuaternionCamera::GetFrustum () const
{
	return frustum_;
}

void QuaternionCamera::LookAt(vector3 const& eye, vector3 const& at, vector3 const& up)
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

void QuaternionCamera::RotateCamera(vector3 const& v, real angle)
{
	q_ *= rotation_quat(v, angle);
}

void QuaternionCamera::SetFrustum(frustum const& frustum)
{
	frustum_ = frustum;
}

void QuaternionCamera::Rotate(real angle)
{
	RotateCamera(vector3(0.0, 1.0, 0.0), angle);
}

void QuaternionCamera::Tilt(real angle)
{
	if (abs(tilt_ + angle) < static_cast<real>(M_PI_2))
	{
		RotateCamera(GetRightDirection(), angle);
	}

	tilt_ = std::max(std::min(tilt_ + angle, static_cast<real>(M_PI_2)), -static_cast<real>(M_PI_2));
}

void QuaternionCamera::MoveForward(real distance)
{
	p_ += distance * GetViewDirection();
}

void QuaternionCamera::SetAspectRatio(core::real aspect)
{
	frustum_.aspect = aspect;
}