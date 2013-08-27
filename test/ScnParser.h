#ifndef SCNPARSER_H
#define SCNPARSER_H

#include <functional>
#include <string>

#include "common_types.h"
#include "ScnFilePayload.h"

/////////////////////////////////////////////
///
/// The class for parsing custom .scn format.
/// You need to set corresponding delegates 
/// and call Parse() method to start parsing.
/// Entities that may be encountered are
/// described in ScnFilePayload.h by corresponding
/// XXXDesc structures.
///
class ScnParser
{
public:
	ScnParser(std::string const& fileName);
	~ScnParser();

	/// Start parsing process
	/// Delegates are called whenever 
	/// corresponding object gets encountered
	void Parse();

	/// Object delegates
	std::function<void(StaticObjectDesc const&)> OnStaticObject;
	std::function<void(CameraDesc const&)> OnCamera;
	std::function<void(PointLightDesc const& pointLightDesc)> OnPointLight;
	std::function<void(SpotLightDesc const& spotLightDesc)> OnSpotLight;

private:
	void ParseStaticObject(std::istringstream& stream) const;
	void ParseCamera(std::istringstream& stream) const;
	void ParsePointLight(std::istringstream& stream) const;
	void ParseSpotLight(std::istringstream& stream) const;

	ScnParser(ScnParser const&);
	ScnParser& operator = (ScnParser const&);

	std::string fileName_;
};

#endif // SCNPARSER_H