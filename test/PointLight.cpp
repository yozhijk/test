#include "PointLight.h"

using namespace core;

PointLight::PointLight()
{
}

PointLight::~PointLight()
{

}

void PointLight::SetPosition( vector3 pos)
{
	pos_ = pos;
}

void PointLight::SetColor( color_rgba color)
{
	color_ = color;
}

void PointLight::SetCastShadow(bool bCastShadow)
{
	bCastShadow_ = bCastShadow;
}

 vector3 PointLight::GetPosition() const
{
	return pos_;
}

 color_rgba PointLight::GetColor() const
{
	return color_;
}

bool	PointLight::GetCastShadow() const
{
	return bCastShadow_;
}