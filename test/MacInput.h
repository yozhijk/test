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
    MacInput(MacInput const&);
    void operator = (MacInput const&);
};



#endif // MACINPUT_H
