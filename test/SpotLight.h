#ifndef SPOTLIGHT_H
#define SPOTLIGHT_H

#include "IGameEntity.h"
#include "common_types.h"

class SpotLight : public IGameEntity
{
public:
	SpotLight();
	~SpotLight();

	void SetPosition(core::vector3 pos);
	void SetDirection(core::vector3 pos);
	void SetColor(core::color_rgba color);
	void SetAngles(core::real innerAngle, core::real outerAngle);
	void SetCastShadow(bool bCastShadow);

	core::vector3 GetPosition() const;
	core::color_rgba GetColor() const;
	core::vector3 GetDirection() const;
	core::real GetInnerAngle() const;
	core::real GetOuterAngle() const;
	bool	GetCastShadow() const;
private:
	SpotLight(SpotLight const&);
	SpotLight& operator = (SpotLight const&);

	core::vector3 pos_;
	core::vector3 dir_;
	core::color_rgba color_;
	core::real innerAngle_;
	core::real outerAngle_;
	bool bCastShadow_;
};


#endif //POINTLIGHT_H