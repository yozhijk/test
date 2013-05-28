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
    MK_RIGHT,
    MK_LEFT,
    MK_UP,
    MK_DOWN
};

enum MY_MOUSE_BUTTON
{
    MMB_LEFT,
    MMB_MIDLE,
    MMB_RIGHT
};

/////////////////////////////////////////////////////
///  The abstraction for platform input capabilities
///
class IInput
{ 
public:
    virtual ~IInput() = 0;

    virtual bool IsKeyPressed( MY_KEY key ) const = 0;
    virtual bool IsMouseButtonPressed( MY_MOUSE_BUTTON button ) const = 0;
    virtual core::vector2 GetRelativePosition() const = 0;
    virtual core::vector2 GetAbsolutePosition() const = 0;
    virtual void UpdateState() = 0;\

protected:
    void operator = (IInput const&);
};

inline IInput::~IInput() {}

#endif // IINPUT_H