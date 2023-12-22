///////////////////////////////////////////////////////////////////////////////
// ControllerGL.h
// ==============
// Derived Controller class for OpenGL window
// It is the controller of OpenGL rendering window. It initializes DC and RC,
// when WM_CREATE called, then, start new thread for OpenGL rendering loop.
//
// When this class is constructed, it gets the pointers to model and view
// components.
//
//  AUTHOR: Song Ho Ahn (song.ahn@gamil.com)
// CREATED: 2013-01-14
// UPDATED: 2021-02-16
///////////////////////////////////////////////////////////////////////////////

#ifndef WIN_CONTROLLER_GL_H
#define WIN_CONTROLLER_GL_H

#include <thread>
#include <chrono>
#include <atomic>
#include <string>
#include "Controller.h"
#include "ViewGL.h"
#include "ModelGL.h"


namespace Win
{
    class ControllerGL : public Controller
    {
    public:
        ControllerGL(ModelGL* model, ViewGL* view);
        ~ControllerGL() {};

        LRESULT command(int id, int cmd, LPARAM msg);   // for WM_COMMAND
        LRESULT create();                               // create RC for OpenGL window and start new thread for rendering
        LRESULT destroy();                              // close the RC and destroy OpenGL window
        LRESULT paint();
        LRESULT lButtonDown(WPARAM state, int x, int y);
        LRESULT lButtonUp(WPARAM state, int x, int y);
        LRESULT rButtonDown(WPARAM state, int x, int y);
        LRESULT rButtonUp(WPARAM state, int x, int y);
        LRESULT mouseMove(WPARAM state, int x, int y);
        LRESULT mouseHover(int state, int x, int y);    // for WM_MOUSEHOVER:state, x, y
        LRESULT mouseLeave();                           // for WM_MOUSELEAVE
        LRESULT mouseWheel(int state, int delta, int x, int y); // for WM_MOUSEWHEEL:state, delta, x, y
        LRESULT size(int w, int h, WPARAM wParam);      // for WM_SIZE: width, height, type(SIZE_MAXIMIZED...)

    private:
        void runThread();                           // thread for OpenGL rendering

        ModelGL* model;                             // pointer to model component
        ViewGL* view;                               // pointer to view component
        std::thread glThread;                       // opengl rendering thread object
        volatile bool loopFlag;                     // rendering loop flag
        bool mouseHovered;                          // mouse hover flag
        std::wstring appPath;                       // dir where exe is located
    };
}

#endif
