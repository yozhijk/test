#include "DXInput.h"
#include <Windows.h>
#include <stdexcept>

using namespace core;

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

static vector2 GetMousePos()
{
    POINT cursorPos;
    ::GetCursorPos(&cursorPos);
    return vector2(static_cast<real>(cursorPos.x), static_cast<real>(cursorPos.y));
}

DXInput::DXInput(HWND hWnd) 
    : mousePos_(0,0)
{
    THROW_IF_FAILED(DirectInput8Create(GetModuleHandle(nullptr), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&input_, nullptr), "Cannot create DirectInput object");

    THROW_IF_FAILED(input_->CreateDevice(GUID_SysKeyboard, &keyboard_, nullptr), "Cannot create keyboard device");
    THROW_IF_FAILED(keyboard_->SetDataFormat(&c_dfDIKeyboard), "Cannot set data format for the keyboard");
    THROW_IF_FAILED(keyboard_->SetCooperativeLevel(hWnd, DISCL_BACKGROUND|DISCL_NONEXCLUSIVE), "Failed to set keyboard cooperative level");
    THROW_IF_FAILED(keyboard_->Acquire(), "Failed to acquire keyboard");

    THROW_IF_FAILED(input_->CreateDevice(GUID_SysMouse, &mouse_, nullptr), "Cannot create mouse device");
    THROW_IF_FAILED(mouse_->SetDataFormat(&c_dfDIMouse2), "Cannot set data format for the mouse");
    THROW_IF_FAILED(mouse_->SetCooperativeLevel(hWnd, DISCL_BACKGROUND|DISCL_NONEXCLUSIVE), "Failed to set mouse cooperative level");

    THROW_IF_FAILED(mouse_->Acquire(), "Failed to acquire mouse");
    mousePos_ = GetMousePos();

    ZeroMemory(keys_, sizeof(keys_));

    DIDEVCAPS mouseCaps;
    mouseCaps.dwSize = sizeof(mouseCaps);
    THROW_IF_FAILED(mouse_->GetCapabilities(&mouseCaps), "Couldn't get mouse caps");

    if(!(mouseCaps.dwFlags & DIDC_ATTACHED))
    {
        throw std::runtime_error("Mouse is not attached");
    }
}

bool DXInput::IsKeyPressed(MY_KEY mykey) const
{
    int diKey = DIKeyForMyKey(mykey);

    if (keys_[diKey] & 0x80)
    {
        return true;
    }

    return false;
}

bool DXInput::IsMouseButtonPressed(MY_MOUSE_BUTTON button) const
{
    int diButton = static_cast<int>(button);

    if (mouseState_.rgbButtons[diButton] & 0x80)
    {
        return true;
    }

    return false;
}

vector2 DXInput::GetRelativePosition() const
{
    return vector2(static_cast<real>(mouseState_.lX), static_cast<real>(mouseState_.lY)); 
}

core::vector2 DXInput::GetAbsolutePosition() const
{
    return mousePos_;
}

void DXInput::UpdateState()
{
    THROW_IF_FAILED(keyboard_->GetDeviceState(sizeof(keys_), keys_), "Cannot get keyboard state");

    if (DIERR_INPUTLOST == mouse_->GetDeviceState(sizeof(mouseState_), &mouseState_))
    {
            THROW_IF_FAILED(mouse_->Acquire(), "Failed to acquire mouse");
            mousePos_ = GetMousePos();
            THROW_IF_FAILED(keyboard_->GetDeviceState(sizeof(mouseState_), &mouseState_), "Cannot get keyboard state");
    }

    mousePos_ += GetRelativePosition();
}