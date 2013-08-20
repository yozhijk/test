#ifndef SCNFILEPAYLOAD_H
#define SCNFILEPAYLOAD_H
////////////////////////////////////////////
///
/// scn. file payload, consider using 
/// Google protobuffers later
#include <string>
#include "common_types.h"

/*------------------------------
		Static object
------------------------------*/
struct StaticObjectDesc
{
	std::string tag;
	std::string fileName;

	core::matrix4x4 worldMatrix;
};

/*------------------------------
		Camera
------------------------------*/
struct CameraDesc
{
	std::string   tag;
	core::vector3 pos;
	core::vector3 at;
	core::vector3 up; 
	core::frustum frustum;
	bool bActive;
};


/*------------------------------
		Point light
------------------------------*/
struct PointLightDesc
{
	core::vector3 pos;
	core::color_rgba color;
	bool bCastShadow;
};

/*------------------------------
		Spot light
------------------------------*/
struct SpotLightDesc
{
	core::vector3 pos;
	core::vector3 dir;
	core::color_rgba color;
	core::real innerAngle;
	core::real outerAngle;
	bool bCastShadow;
};


template <typename Stream> inline Stream& operator >> (Stream& stream, StaticObjectDesc& staticObjectDesc)
{
	std::string tmp;
	core::real x,y,z;

	stream >> tmp >> staticObjectDesc.tag >> staticObjectDesc.fileName >> x >> y >> z;

	staticObjectDesc.worldMatrix = translation_matrix(vector3(x, y, z));

	return stream;
}

template <typename Stream> inline Stream& operator >> (Stream& stream, CameraDesc& cameraDesc)
{
	std::string tmp;

	stream >> tmp >> cameraDesc.tag >> cameraDesc.pos.x() >> cameraDesc.pos.y() >> cameraDesc.pos.z()
		>> cameraDesc.at.x() >> cameraDesc.at.y() >> cameraDesc.at.z() 
		>> cameraDesc.up.x() >> cameraDesc.up.y() >> cameraDesc.up.z()
		>> cameraDesc.frustum.fovy >> cameraDesc.frustum.aspect >> cameraDesc.frustum.nr >> cameraDesc.frustum.fr >> cameraDesc.bActive;

	return stream;
}

template <typename Stream> inline Stream& operator >> (Stream& stream, PointLightDesc& pointLightDesc)
{
	std::string tmp;

	stream >> tmp >> pointLightDesc.pos.x() >> pointLightDesc.pos.y() >> pointLightDesc.pos.z() >> 
		pointLightDesc.color.x() >> pointLightDesc.color.y() >> pointLightDesc.color.z() >> pointLightDesc.bCastShadow;

	return stream;
}

template <typename Stream> inline Stream& operator >> (Stream& stream, SpotLightDesc& spotLightDesc)
{
	std::string tmp;

	stream >> tmp >>  spotLightDesc.pos.x() >> spotLightDesc.pos.y() >> spotLightDesc.pos.z() 
		>> spotLightDesc.dir.x() >> spotLightDesc.dir.y() >> spotLightDesc.dir.z()
		>> spotLightDesc.color.x() >> spotLightDesc.color.y() >> spotLightDesc.color.z()
		>> spotLightDesc.innerAngle >> spotLightDesc.outerAngle >> spotLightDesc.bCastShadow;

	return stream;
}


#endif