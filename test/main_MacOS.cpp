//
//  main.cpp
//  test
//
//  Created by Dmitry Kozlov on 28.12.12.
//  Copyright (c) 2012 Dmitry Kozlov. All rights reserved.
//
#include <GLUT/GLUT.h>

#include "MacOS.h"
#include "App.h"

#include <time.h>
#include <iostream>
#include <functional>

std::unique_ptr<MacOS> g_OS;
std::unique_ptr<App>   g_App;

#define WINDOW_WIDTH  800
#define WINDOW_HEIGHT 600

void display()
{
    /// TODO: switch to using hight resoluion MAC specific timer later
    static clock_t prevTime = clock();
    
    clock_t currentTime = clock();
    core::real timeDelta = (core::real)(currentTime - prevTime)/CLOCKS_PER_SEC;
    
    g_OS->Loop(timeDelta);
    
    prevTime = currentTime;
}

void update()
{
    /// TODO: split update/render cycle later
    glutPostRedisplay();
}

void reshape(GLint w, GLint h)
{
    g_OS->ResizeWindow(core::ui_size(static_cast<core::uint>(w),static_cast<core::uint>(h)));
}

int main(int argc, char** argv)
{
    g_OS.reset(new MacOS());
    g_App.reset(new App(*g_OS));
    
    // GLUT Window Initialization:
    glutInit (&argc, argv);
    glutInitWindowSize (WINDOW_WIDTH, WINDOW_HEIGHT);
    glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutCreateWindow ("test");
    // Initialize OpenGL graphics state
    
    // InitGraphics();
    // Register callbacks:
    glutDisplayFunc (display);
    glutReshapeFunc (reshape);
    //glutKeyboardFunc (Keyboard);
    //glutMouseFunc (MouseButton);
    //glutMotionFunc (MouseMotion);
    glutIdleFunc (update);
    // Create our popup menu
    // BuildPopupMenu ();
    // glutAttachMenu (GLUT_RIGHT_BUTTON);
    
    // Get the initial time, for use by animation
    g_OS->SetWindowParams(core::ui_rect(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT));
    
    // Turn the flow of control over to GLUT
    glutMainLoop ();
    return 0;
}
