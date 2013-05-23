#include "DXInput.h"

#include <stdexcept>

#define THROW_IF_FAILED(x,m) if((x)!=S_OK) throw std::runtime_error(m)

int DIKeyForMyKey(MY_KEY key)
{
    switch (key)
    {
    case MK_RIGHT:
        return DIK_RIGHT;
        break;
    case MK_LEFT:
        return DIK_LEFT;
        break;
    case MK_DOWN:
        return DIK_DOWN;
        break;
    case MK_UP:
        return DIK_UP;
        break;
    default:
        // no such key
        assert(false);
    }

    return -1;
}

DXInput::DXInput(HWND hWnd)
{
    THROW_IF_FAILED(DirectInput8Create(GetModuleHandle(nullptr), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&input_, nullptr), "Cannot create DirectInput object");
    THROW_IF_FAILED(input_->CreateDevice(GUID_SysKeyboard, &keyboard_, nullptr), "Cannot create keyboard device");
    THROW_IF_FAILED(keyboard_->SetDataFormat(&c_dfDIKeyboard), "Cannot set data format for the keyboard");
    THROW_IF_FAILED(keyboard_->SetCooperativeLevel(hWnd, DISCL_BACKGROUND|DISCL_NONEXCLUSIVE), "Failed to set keyboard cooperative level");
    THROW_IF_FAILED(keyboard_->Acquire(), "Failed to acquire keyboard");

    ZeroMemory(keys_, sizeof(keys_));
}

bool DXInput::IsKeyPressed(MY_KEY mykey)
{
    int key = DIKeyForMyKey(mykey);

    if (keys_[key] & 0x80)
    {
        return true;
    }

    return false;
}

void DXInput::UpdateState()
{
    THROW_IF_FAILED(keyboard_->GetDeviceState(sizeof(keys_), keys_), "Cannot get keyboard state");
}