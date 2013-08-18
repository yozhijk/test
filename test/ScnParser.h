#ifndef SCNPARSER_H
#define SCNPARSER_H

#include <functional>
#include <string>

#include "common_types.h"

class ScnParser
{
public:
	ScnParser(std::string const& fileName);
	~ScnParser();

	void Parse();

	/// Static object creation delegate
	std::function<void(std::string const&, std::string const&, core::matrix4x4 const&)> OnStaticObject;

	/// Dirty hack to workaround variadic templates absence here needed under VS12
	std::function<void(std::string const&, core::vector3 const&, core::vector3 const&, core::vector3 const&, core::frustum const&, bool)> OnCamera;
	std::function<void(std::string const&, core::vector3 const&, core::color_rgba const&, bool)> OnPointLight;

private:
	void ParseStaticObject(std::istringstream& stream) const;
	void ParseCamera(std::istringstream& stream) const;
	void ParsePointLight(std::istringstream& stream) const;

	ScnParser(ScnParser const&);
	ScnParser& operator = (ScnParser const&);

	std::string fileName_;
};

#endif // SCNPARSER_H