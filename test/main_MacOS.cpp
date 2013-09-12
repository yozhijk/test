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
#include <chrono>

std::unique_ptr<MacOS> g_OS;
std::unique_ptr<App>   g_App;

#define WINDOW_WIDTH  800
#define WINDOW_HEIGHT 600

void display()
{
    static auto prevTime = std::chrono::high_resolution_clock::now();
    
    auto currentTime = std::chrono::high_resolution_clock::now();
    auto timeDelta = std::chrono::duration_cast<std::chrono::duration<double> >(currentTime - prevTime);
    
    g_OS->Loop(timeDelta.count());
    
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
    try
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
    }
    catch (std::runtime_error& e)
    {
        std::cout << e.what();
    }
    return 0;
}
