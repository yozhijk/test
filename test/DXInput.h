#ifndef DXINPUT_H
#define DXINPUT_H

#include <atlbase.h>
#include <dinput.h>
#include "IInput.h"

class DXInput : public IInput
{
public:
    DXInput();

    bool IsKeyPressed( MY_KEY key );


private:
    DXInput(DXInput const&);
    DXInput& operator=(DXInput const&);

    CComPtr<IDirectInput8> input_;
};



#endif // DXINPUT_H