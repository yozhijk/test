#ifndef POINTLIGHT_H
#define POINTLIGHT_H

#include "IGameEntity.h"
#include "common_types.h"

class PointLight : public IGameEntity
{
public:
	PointLight();
	~PointLight();

	void SetPosition(core::vector3 pos);
	void SetColor(core::color_rgba color);
	void SetCastShadow(bool bCastShadow);

	core::vector3 GetPosition() const;
	core::color_rgba GetColor() const;
	bool GetCastShadow() const;

private:
	PointLight(PointLight const&);
	PointLight& operator = (PointLight const&);

	core::vector3 pos_;
	core::color_rgba color_;
	bool bCastShadow_;
};


#endif //POINTLIGHT_H
