#include "SpotLight.h"

using namespace core;

SpotLight::SpotLight()
{
}

SpotLight::~SpotLight()
{

}

void SpotLight::SetPosition( vector3 pos)
{
	pos_ = pos;
}

void SpotLight::SetDirection(vector3 dir)
{
	dir_ = dir;
}

void SpotLight::SetColor( color_rgba color)
{
	color_ = color;
}

void SpotLight::SetAngles( real innerAngle, real outerAngle)
{
	innerAngle_ = innerAngle;
	outerAngle_ = outerAngle;
}

void SpotLight::SetCastShadow(bool bCastShadow)
{
	bCastShadow_ = bCastShadow;
}

 vector3 SpotLight::GetPosition() const
{
	return pos_;
}

 vector3 SpotLight::GetDirection() const
{
	return dir_;
}

 color_rgba SpotLight::GetColor() const
{
	return color_;
}

 core::real SpotLight::GetInnerAngle() const
 {
	 return innerAngle_;
 }
 core::real SpotLight::GetOuterAngle() const
 {
	 return outerAngle_;
 }

bool	SpotLight::GetCastShadow() const
{
	return bCastShadow_;
}