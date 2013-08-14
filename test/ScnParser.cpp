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
    string tmp, meshTag, meshFile;
    real x,y,z;

    stream >> tmp >> meshTag >> meshFile >> x >> y >> z;

    matrix4x4 m = translation_matrix(vector3(x, y, z));

    if (OnStaticObject)
    {
        OnStaticObject(meshTag, meshFile, m);
    }
}

void ScnParser::ParseCamera(std::istringstream& stream) const
{
    string tmp, cameraTag;
    vector3 pos, at, up;
    real fv, near, far, aspect;
    bool bActive;

    stream >> tmp >> cameraTag >> pos.x() >> pos.y() >> pos.z()
        >> at.x() >> at.y() >> at.z() 
        >> up.x() >> up.y() >> up.z()
        >> fv >> aspect >> near >> far >> bActive;

    if (OnCamera)
    {
        OnCamera(cameraTag, pos, at, up, frustum(fv, aspect, near, far), bActive);
    }
}

void ScnParser::ParsePointLight(std::istringstream& stream) const
{
    string tmp, lightTag;
    vector3 pos;
    color_rgba color;
    bool bCastShadow = false;

    stream >> tmp >> pos.x() >> pos.y() >> pos.z() >> 
        color.x() >> color.y() >> color.z() >> bCastShadow;

    if (OnPointLight)
    {
        OnPointLight(lightTag, pos, color, bCastShadow);
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


    }
}