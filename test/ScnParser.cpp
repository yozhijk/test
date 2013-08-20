#include "ScnParser.h"

#include "utils.h"
#include <fstream>
#include <sstream>

using namespace core;
using namespace std;

ScnParser::ScnParser(std::string const& fileName)
	: OnStaticObject(nullptr)
	, fileName_(fileName)
{

}

ScnParser::~ScnParser()
{

}

void ScnParser::ParseStaticObject(std::istringstream& stream) const
{
	StaticObjectDesc staticObjectDesc;
	
	stream >> staticObjectDesc;

	if (OnStaticObject)
	{
		OnStaticObject(staticObjectDesc);
	}
}

void ScnParser::ParseCamera(std::istringstream& stream) const
{
	CameraDesc cameraDesc;

	stream >> cameraDesc;

	if (OnCamera)
	{
		OnCamera(cameraDesc);
	}
}

void ScnParser::ParsePointLight(std::istringstream& stream) const
{
	PointLightDesc pointLightDesc;

	stream >> pointLightDesc;

	if (OnPointLight)
	{
		OnPointLight(pointLightDesc);
	}
}

void ScnParser::ParseSpotLight(std::istringstream& stream) const
{
	SpotLightDesc spotLightDesc;

	stream >> spotLightDesc;

	if (OnSpotLight)
	{
		OnSpotLight(spotLightDesc);
	}
}

void ScnParser::Parse()
{
	ifstream objStream(fileName_);

	string line;
	while(getline(objStream,line))
	{
		if (line.find("static_object") != string::npos)
		{
			istringstream stream(line);
			ParseStaticObject(stream);
		}

		if (line.find("camera") != string::npos)
		{
			istringstream stream(line);
			ParseCamera(stream);
		}

		if (line.find("point_light") != string::npos)
		{
			istringstream stream(line);
			ParsePointLight(stream);
		}

		if (line.find("spot_light") != string::npos)
		{
			istringstream stream(line);
			ParseSpotLight(stream);
		}
	}
}