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

class ICamera : public IGameEntity
{
public:
	virtual ~ICamera() = 0;

	virtual core::matrix4x4 GetViewMatrix() const = 0;
	virtual core::frustum   GetFrustum() const = 0;

	virtual void Rotate(core::real angle) = 0;
	virtual void Tilt(core::real angle) = 0;
	virtual void MoveForward(core::real distance) = 0;
	virtual void SetAspectRatio(core::real aspect) = 0;
};

inline ICamera::~ICamera(){}



#endif