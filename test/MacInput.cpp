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

static void OnKey(unsigned char, int, int)
{
    
}

static void OnMouseMove(int, int)
{
    
}

static void OnMouseButton(int, int, int, int)
{
    
}

MacInput::MacInput()
{
}

MacInput::~MacInput()
{
    
}

bool MacInput::IsKeyPressed( MY_KEY key ) const
{
    return false;
}

bool MacInput::IsMouseButtonPressed( MY_MOUSE_BUTTON button ) const
{
    return false;
}

core::vector2 MacInput::GetRelativePosition() const
{
    return vector2(0,0);
}

core::vector2 MacInput::GetAbsolutePosition() const
{
    return vector2(0,0);
}

void MacInput::UpdateState()
{
    
}


