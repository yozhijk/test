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

 vector3 SpotLight::Position() const
{
	return pos_;
}

 vector3 SpotLight::Direction() const
{
	return dir_;
}

 color_rgba SpotLight::Color() const
{
	return color_;
}

 core::real SpotLight::InnerAngle() const
 {
	 return innerAngle_;
 }
 core::real SpotLight::OuterAngle() const
 {
	 return outerAngle_;
 }

bool	SpotLight::CastShadow() const
{
	return bCastShadow_;
}