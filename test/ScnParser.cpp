#include "ScnParser.h"

#include "utils.h"

using namespace core;

ScnParser::ScnParser(std::string const& fileName)
    : OnStaticObject(nullptr)
{

}

ScnParser::~ScnParser()
{

}

void ScnParser::Parse()
{
#ifdef _TEST
    for (int i=0;i<10;++i)
        for (int j=0;j<10;++j)
            {
                matrix4x4 m = translation_matrix(vector3(-20 + i*3, -20 + j*3, 10));
                if (OnStaticObject)
                {
                    OnStaticObject("Monkey mesh", "monkey", m);
                }
            }
#endif
}