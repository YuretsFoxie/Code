///////////////////////////////////////////////////////////////////////////////
// ControllerMain.h
// ================
// Derived Controller class for main window
//
//  AUTHOR: Song Ho Ahn (song.ahn@gmail.com)
// CREATED: 2006-07-09
// UPDATED: 2020-01-27
///////////////////////////////////////////////////////////////////////////////

#ifndef WIN_CONTROLLER_MAIN_H
#define WIN_CONTROLLER_MAIN_H

#include "Controller.h"

namespace Win
{
    class ControllerMain :public Controller
    {
    public:
        ControllerMain();
        ~ControllerMain() {};

        void setGLHandle(HWND handle)               { glHandle = handle; }
        void setFormHandle(HWND handle)             { formHandle = handle; }

        LRESULT command(int id, int cmd, LPARAM msg);   // for WM_COMMAND
        LRESULT close();
        LRESULT create();                               // for WM_CRERATE
        LRESULT destroy();
        LRESULT size(int w, int h, WPARAM wParam);      // for WM_SIZE: width, height, type(SIZE_MAXIMIZED...)

    private:
        HWND glHandle;                                  // handle of gl window
        HWND formHandle;                                // handle of form dialog

    };
}

#endif
