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
    std::function<void (std::string const&, std::string const&, core::matrix4x4 const&)> OnStaticObject;

private:
    ScnParser(ScnParser const&);
    ScnParser& operator = (ScnParser const&);

    std::string fileName_;
};

#endif // SCNPARSER_H