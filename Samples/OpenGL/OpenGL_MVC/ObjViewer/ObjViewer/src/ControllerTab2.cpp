///////////////////////////////////////////////////////////////////////////////
// ControllerTab2.cpp
// ==================
// Derived Controller class for tab view
//
//  AUTHOR: Song Ho Ahn (song.ahn@gamil.com)
// CREATED: 2013-01-09
// UPDATED: 2020-01-26
///////////////////////////////////////////////////////////////////////////////

#include <cstdlib>
#include <string>
#include <sstream>
#include "ControllerTab2.h"
#include "wcharUtil.h"
#include "Timer.h"
#include "resource.h"
#include "Log.h"
using namespace Win;



///////////////////////////////////////////////////////////////////////////////
// default contructor
///////////////////////////////////////////////////////////////////////////////
ControllerTab2::ControllerTab2(ModelGL* model, ViewTab2* view) : model(model), view(view),
                                                                 glHandle(0), statusHandle(0)
{
    // create brush for tab control
    bgBrush = ::CreateSolidBrush(RGB(255, 255, 255));
}



///////////////////////////////////////////////////////////////////////////////
// handle WM_DESTROY
///////////////////////////////////////////////////////////////////////////////
LRESULT ControllerTab2::destroy()
{
    Win::log("ControllerTab2 is destroyed.");
    return 0;
}



///////////////////////////////////////////////////////////////////////////////
// handle WM_CREATE
///////////////////////////////////////////////////////////////////////////////
LRESULT ControllerTab2::create()
{
    view->initControls(handle);
    return 0;
}



///////////////////////////////////////////////////////////////////////////////
// handle WM_COMMAND
///////////////////////////////////////////////////////////////////////////////
LRESULT ControllerTab2::command(int id, int command, LPARAM msg)
{
    switch(id)
    {
    case IDC_LIST_DESC:
        if(command == LBN_SELCHANGE)
        {
            view->selectGroup();
        }
        break;
    }

    // return focus to the parent, so it can receive key events
    //::SetFocus(::GetParent(handle));

    return 0;
}



///////////////////////////////////////////////////////////////////////////////
// handle WM_LBUTTONDOWN
///////////////////////////////////////////////////////////////////////////////
LRESULT ControllerTab2::lButtonDown(WPARAM state, int x, int y)
{
    //Win::log("left mouse down");
    return 0;
}



///////////////////////////////////////////////////////////////////////////////
// handle WM_NOTIFY
// The id is not guaranteed to be unique, so use NMHDR.hwndFrom and NMHDR.idFrom.
///////////////////////////////////////////////////////////////////////////////
LRESULT ControllerTab2::notify(int id, LPARAM lParam)
{
    return 0;
}



///////////////////////////////////////////////////////////////////////////////
// handle WM_SIZE
// the input width and height are the display area of the tab control
///////////////////////////////////////////////////////////////////////////////
LRESULT ControllerTab2::size(int w, int h, WPARAM wParam)
{
    // resize this dialog with tab's display area, but do not move (ignore x and y)
    ::SetWindowPos(handle, 0, 0, 0, w, h, SWP_NOMOVE | SWP_NOZORDER);

    // resize list view to fit into this dialog
    view->resizeListBox(w, h);
    return 0;
}



///////////////////////////////////////////////////////////////////////////////
// handle WM_TIMER notification
///////////////////////////////////////////////////////////////////////////////
LRESULT ControllerTab2::timer(WPARAM eventId, LPARAM callback)
{
    return 0;
}



///////////////////////////////////////////////////////////////////////////////
// handle WN_CTLCOLOR*
// return handle to brush
///////////////////////////////////////////////////////////////////////////////
LRESULT ControllerTab2::ctlColorDlg(HDC hdc, HWND handle)
{
    //Win::log(L"WM_CTLCOLORDLG");
    return (INT_PTR)bgBrush;
}

LRESULT ControllerTab2::ctlColorStatic(HDC hdc, HWND handle)
{
    //Win::log(L"WM_CTLCOLORDLG");
    ::SetBkMode(hdc, TRANSPARENT);  // make the text area transparent
    return (INT_PTR)bgBrush;
}

