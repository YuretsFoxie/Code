///////////////////////////////////////////////////////////////////////////////
// ControllerMain.cpp
// ==================
// Derived Controller class for main window
//
//  AUTHOR: Song Ho Ahn (song.ahn@gmail.com)
// CREATED: 2013-01-09
// UPDATED: 2020-01-19
///////////////////////////////////////////////////////////////////////////////

#include <windows.h>
#include <commctrl.h>                   // common controls
#include "ControllerMain.h"
#include "resource.h"
#include "Log.h"

using namespace Win;



ControllerMain::ControllerMain() : glHandle(0), formHandle(0)
{
}



LRESULT ControllerMain::command(int id, int cmd, LPARAM msg)
{
    /*
    switch(id)
    {
    case ID_FILE_EXIT:
        ::PostMessage(handle, WM_CLOSE, 0, 0);
        break;

    }
    */
    return 0;
}



LRESULT ControllerMain::close()
{
    Win::log("Closing the application...");
    ::DestroyWindow(handle);    // destroy this window and children
    return 0;
}



LRESULT ControllerMain::destroy()
{
    ::PostQuitMessage(0);       // exit the message loop
    Win::log("Main windows is destroyed.");
    return 0;
}



LRESULT ControllerMain::create()
{
    return 0;
}



LRESULT ControllerMain::size(int w, int h, WPARAM wParam)
{
    RECT rect;

    // get client dim of mainWin
    ::GetClientRect(handle, &rect);
    int mainClientWidth = rect.right - rect.left;
    int mainClientHeight = rect.bottom - rect.top;

    // get dim of glWin
    ::GetWindowRect(glHandle, &rect);
    int glWidth = rect.right - rect.left;
    int glHeight = rect.bottom - rect.top;

    // get dim of glDialog
    ::GetWindowRect(formHandle, &rect);
    int formWidth = rect.right - rect.left;

    // resize glWin and reposition glDialog
    glWidth = mainClientWidth - formWidth;
    glHeight = mainClientHeight;
    ::SetWindowPos(glHandle, 0, 0, 0, glWidth, glHeight, SWP_NOZORDER);
    ::SetWindowPos(formHandle, 0, glWidth, 0, formWidth, glHeight, SWP_NOZORDER);
    ::InvalidateRect(formHandle, 0, TRUE);      // force to repaint

    return 0;
}



LRESULT ControllerMain::keyDown(int key, LPARAM lParam)
{
    //Win::log("CTRL MAIN");
    if(key == VK_LEFT)
    {
        model->shiftCameraDelta(-1, 0);
    }
    else if(key == VK_RIGHT)
    {
        model->shiftCameraDelta(1, 0);
    }
    else if(key == VK_UP)
    {
        model->shiftCameraDelta(0, 1);
    }
    else if(key == VK_DOWN)
    {
        model->shiftCameraDelta(0, -1);
    }

    return 0;
}



