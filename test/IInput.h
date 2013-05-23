/////////////////////////////////////////////////////
/// @file IInput.h
///     An interface to input devices
///
/// @author Dmitry Kolzov
///
#ifndef IINPUT_H
#define IINPUT_H

#include <memory>

#include "common_types.h"

enum MY_KEY
{

};

/////////////////////////////////////////////////////
///  The abstraction for platform input capabilities
///
class IInput
{ 
public:
    virtual ~IInput() = 0;

    virtual bool IsKeyPressed( MY_KEY key ) = 0;
};

inline IInput::~IInput() {}

#endif // IINPUT_H