#ifndef DXINPUT_H
#define DXINPUT_H

#include <atlbase.h>
#include <dinput.h>
#include "IInput.h"

class DXInput : public IInput
{
public:
    static const int KEYBUFFER_SIZE = 256;

    DXInput(HWND hWnd);

    bool IsKeyPressed(MY_KEY key);
    void UpdateState();

private:
    DXInput(DXInput const&);
    DXInput& operator=(DXInput const&);

    CComPtr<IDirectInput8> input_;
    CComPtr<IDirectInputDevice8> keyboard_;

    char keys_[KEYBUFFER_SIZE];
};



#endif // DXINPUT_H