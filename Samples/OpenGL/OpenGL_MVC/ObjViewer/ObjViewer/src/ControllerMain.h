///////////////////////////////////////////////////////////////////////////////
// ControllerMain.h
// ================
// Derived Controller class for main window
//
//  AUTHOR: Song Ho Ahn (song.ahn@gmail.com)
// CREATED: 2013-01-09
// UPDATED: 2020-01-19
///////////////////////////////////////////////////////////////////////////////

#ifndef WIN_CONTROLLER_MAIN_H
#define WIN_CONTROLLER_MAIN_H

#include "Controller.h"
#include "ModelGL.h"

namespace Win
{
    class ControllerMain :public Controller
    {
    public:
        ControllerMain();
        ~ControllerMain() {};

        void setGLHandle(HWND handle)               { glHandle = handle; }
        void setFormHandle(HWND handle)             { formHandle = handle; }
        void setModel(ModelGL* model)               { this->model = model; }

        LRESULT command(int id, int cmd, LPARAM msg);   // for WM_COMMAND
        LRESULT close();                                // for WM_CLOSE
        LRESULT create();                               // for WM_CRERATE
        LRESULT destroy();                              // for WM_DESTROY
        LRESULT keyDown(int key, LPARAM lParam);        // for WM_KEYDOWN
        LRESULT size(int w, int h, WPARAM wParam);      // for WM_SIZE: width, height, type(SIZE_MAXIMIZED...)

    private:
        HWND glHandle;                                  // handle of gl window
        HWND formHandle;                                // handle of form dialog
        ModelGL* model;                                 // pointer to model component

    };
}

#endif
