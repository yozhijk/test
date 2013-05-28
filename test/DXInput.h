#ifndef DXINPUT_H
#define DXINPUT_H

#include <atlbase.h>
#include <dinput.h>

#include "common_types.h"
#include "IInput.h"

class DXInput : public IInput
{
public:
    static const int KEYBUFFER_SIZE = 256;

    DXInput(HWND hWnd);

    bool IsKeyPressed(MY_KEY key) const;
    bool IsMouseButtonPressed( MY_MOUSE_BUTTON button ) const;
    core::vector2 GetRelativePosition() const;
    core::vector2 GetAbsolutePosition() const;
    void UpdateState();

private:
    DXInput(DXInput const&);
    DXInput& operator=(DXInput const&);

    CComPtr<IDirectInput8> input_;
    CComPtr<IDirectInputDevice8> keyboard_;
    CComPtr<IDirectInputDevice8> mouse_;

    DIMOUSESTATE2 mouseState_;
    char keys_[KEYBUFFER_SIZE];

    core::vector2 mousePos_;
};



#endif // DXINPUT_H