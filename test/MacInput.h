//
//  MacInput.h
//  Temporary GLUT shitty input (non thread-safe)
//
//  Created by Dmitry Kozlov on 29.05.13.
//  Copyright (c) 2013 Dmitry Kozlov. All rights reserved.
//

#ifndef MACINPUT_H
#define MACINPUT_H

#include "IInput.h"

class MacInput : public IInput
{
public:
    MacInput();
    ~MacInput();
    
    bool IsKeyPressed( MY_KEY key ) const;
    bool IsMouseButtonPressed( MY_MOUSE_BUTTON button ) const;
    core::vector2 GetRelativePosition() const;
    core::vector2 GetAbsolutePosition() const;
    void UpdateState();
    
private:
    static void OnSpecialKeyUp(int, int, int);
    static void OnSpecialKey(int, int, int);
    static void OnMouseMove(int, int);
    static void OnMouseButton(int, int, int, int);
    
    MacInput(MacInput const&);
    void operator = (MacInput const&);
    
    static char keys_[256];
    static bool mouseButtons_[3];
    static core::vector2 mouseDelta_;
};



#endif // MACINPUT_H
