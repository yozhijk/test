#include "DXInput.h"

#include <stdexcept>

#define THROW_IF_FAILED(x,m) if((x)!=S_OK) throw std::runtime_error(m)


DXInput::DXInput()
{
    THROW_IF_FAILED(DirectInput8Create(GetModuleHandle(nullptr), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&input_, nullptr), "Cannot create DirectInput object");
}

bool DXInput::IsKeyPressed( MY_KEY key )
{
    return false;
}