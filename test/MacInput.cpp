//
//  MacInput.cpp
//  test
//
//  Created by Dmitry Kozlov on 29.05.13.
//  Copyright (c) 2013 Dmitry Kozlov. All rights reserved.
//

#include "MacInput.h"
#include <GLUT/GLUT.h>

using namespace core;

char MacInput::keys_[256]={0};
bool MacInput::mouseButtons_[3]={0};
vector2 MacInput::mouseDelta_;

static MY_MOUSE_BUTTON GLUTButtonToMyButton(int button)
{
    switch (button) {
        case GLUT_LEFT_BUTTON:
            return MMB_LEFT;
            break;
        case GLUT_RIGHT_BUTTON:
            return MMB_RIGHT;
            break;
        case GLUT_MIDDLE_BUTTON:
            return MMB_MIDLE;
            break;
        default:
            break;
    }
    assert(false);
    return (MY_MOUSE_BUTTON)-1;
}

static MY_KEY GLUTKeyToMyKey(int key)
{
    
    switch (key) {
        case GLUT_KEY_UP:
            return MK_UP;
            break;
        case GLUT_KEY_DOWN:
            return MK_DOWN;
            break;
        case GLUT_KEY_LEFT:
            return MK_LEFT;
            break;
        case GLUT_KEY_RIGHT:
            return MK_RIGHT;
        default:
            break;
    }
    assert(false);
    return (MY_KEY)-1;
}

void MacInput::OnSpecialKeyUp(int key, int x, int y)
{
    keys_[GLUTKeyToMyKey(key)] = 0;
}


void MacInput::OnSpecialKey(int key, int x, int y)
{
    keys_[GLUTKeyToMyKey(key)] = 1;
}

void MacInput::OnMouseMove(int x, int y)
{
    static vector2 mousePos = vector2(x,y);
    mouseDelta_ = vector2(x,y) - mousePos;
    mousePos = vector2(x,y);
}

void MacInput::OnMouseButton(int button, int state, int x, int y)
{
    mouseButtons_[GLUTButtonToMyButton(button)] = (state == GLUT_DOWN ? true : false);
}

MacInput::MacInput()
{
    glutSpecialFunc(MacInput::OnSpecialKey);
    glutSpecialUpFunc(MacInput::OnSpecialKeyUp);
    glutMotionFunc(MacInput::OnMouseMove);
    glutPassiveMotionFunc(MacInput::OnMouseMove);
    glutMouseFunc(MacInput::OnMouseButton);
}

MacInput::~MacInput()
{
}

bool MacInput::IsKeyPressed( MY_KEY key ) const
{
    return keys_[key];
}

bool MacInput::IsMouseButtonPressed( MY_MOUSE_BUTTON button ) const
{
    return mouseButtons_[button];
}

core::vector2 MacInput::GetRelativePosition() const
{
    return mouseDelta_;
}

core::vector2 MacInput::GetAbsolutePosition() const
{
    assert(false);
    return vector2(0,0);
}

void MacInput::UpdateState()
{
    
}


