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
// CREATED: 2006-07-09
// UPDATED: 2020-01-27
///////////////////////////////////////////////////////////////////////////////

#ifndef WIN_CONTROLLER_GL_H
#define WIN_CONTROLLER_GL_H

#include <thread>
#include <chrono>
#include <atomic>
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

        LRESULT close();                                // close the RC and OpenGL window
        LRESULT command(int id, int cmd, LPARAM msg);   // for WM_COMMAND
        LRESULT create();                               // create RC for OpenGL window and start new thread for rendering
        LRESULT paint();
        LRESULT size(int w, int h, WPARAM wParam);
        LRESULT lButtonDown(WPARAM state, int x, int y);
        LRESULT lButtonUp(WPARAM state, int x, int y);
        LRESULT rButtonDown(WPARAM state, int x, int y);
        LRESULT rButtonUp(WPARAM state, int x, int y);
        LRESULT mouseMove(WPARAM state, int x, int y);
        LRESULT mouseHover(int state, int x, int y);    // for WM_MOUSEHOVER:state, x, y
        LRESULT mouseLeave();                           // for WM_MOUSELEAVE
        LRESULT mouseWheel(int state, int delta, int x, int y); // for WM_MOUSEWHEEL:state, delta, x, y

    private:
        void runThread();                               // thread for OpenGL rendering
        void swapRedBlue(unsigned char* data, int dataSize, int channelCount);
        void flipImage(unsigned char* data, int width, int height, int channelCount);

        ModelGL* model;                                 //
        ViewGL* view;                                   //
        std::thread glThread;                           // opengl rendering thread object
        volatile bool loopFlag;                         // rendering loop flag
        bool mouseHovered;                              // mouse hover flag
    };
}

#endif
