//
//  main.cpp
//  test
//
//  Created by Dmitry Kozlov on 28.12.12.
//  Copyright (c) 2012 Dmitry Kozlov. All rights reserved.
//
#include <GLUT/GLUT.h>

#include "MacOS.h"

#include <iostream>
#include <functional>

std::unique_ptr<MacOS> g_OS;

#define WINDOW_WIDTH  800
#define WINDOW_HEIGHT 600

void display()
{
    
}

void reshape(GLint w, GLint h)
{
    g_OS->ResizeWindow(core::ui_size(static_cast<core::uint>(w),static_cast<core::uint>(h)));
}

int main(int argc, char** argv)
{
    g_OS.reset(new MacOS());
    // GLUT Window Initialization:
    glutInit (&argc, argv);
    glutInitWindowSize (WINDOW_WIDTH, WINDOW_HEIGHT);
    glutInitDisplayMode ( GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutCreateWindow ("test");
    // Initialize OpenGL graphics state
    
    // InitGraphics();
    // Register callbacks:
    glutDisplayFunc (display);
    glutReshapeFunc (reshape);
    //glutKeyboardFunc (Keyboard);
    //glutMouseFunc (MouseButton);
    //glutMotionFunc (MouseMotion);
    glutIdleFunc (display);
    // Create our popup menu
    // BuildPopupMenu ();
    // glutAttachMenu (GLUT_RIGHT_BUTTON);
    // Get the initial time, for use by animation

    // Turn the flow of control over to GLUT
    glutMainLoop ();
    return 0;
}
