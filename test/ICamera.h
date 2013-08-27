/////////////////////////////////////////////////////
/// @file Camera.h
///	 An interface to a camera
///
/// @author Dmitry Kolzov
///
#ifndef ICAMERA_H
#define ICAMERA_H

#include "common_types.h"

#include "IGameEntity.h"

/////////////////////////////////
///
/// Basic camera interface
///
class ICamera : public IGameEntity
{
public:
	virtual ~ICamera() = 0;

	/// Query methods
	virtual core::matrix4x4 GetViewMatrix() const = 0;
	virtual core::frustum   GetFrustum() const = 0;

	/// Control methods
	/// Rotate camera aroung world up vector
	virtual void Rotate(core::real angle) = 0;
	/// Tilt in [-Pi/2,Pi/2] range
	virtual void Tilt(core::real angle) = 0;
	/// Move camera along current line of sight
	virtual void MoveForward(core::real distance) = 0;
	/// Set aspect ratio for the field of view
	virtual void SetAspectRatio(core::real aspect) = 0;
};

inline ICamera::~ICamera(){}



#endif