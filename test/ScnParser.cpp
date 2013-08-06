#include "ScnParser.h"

#include "utils.h"
#include <fstream>
#include <sstream>

using namespace core;

ScnParser::ScnParser(std::string const& fileName)
    : OnStaticObject(nullptr)
    , fileName_(fileName)
{

}

ScnParser::~ScnParser()
{

}

void ScnParser::Parse()
{
    using namespace std;
    ifstream objStream(fileName_);

    string line;
    while(getline(objStream,line))
    {
        if (line.find("static_object") != string::npos)
        {
            istringstream stream(line);

            string tmp, meshTag, meshFile;
            core::real x,y,z;
            stream >> tmp >> meshTag >> meshFile >> x >> y >> z;

            matrix4x4 m = translation_matrix(vector3(x, y, z));

            if (OnStaticObject)
            {
               OnStaticObject(meshTag, meshFile, m);
            }
        }
    }
}