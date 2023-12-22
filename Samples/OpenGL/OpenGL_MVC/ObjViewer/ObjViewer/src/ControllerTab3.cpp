///////////////////////////////////////////////////////////////////////////////
// ControllerTab3.cpp
// ==================
// Derived Controller class for tab view
//
//  AUTHOR: Song Ho Ahn (song.ahn@gamil.com)
// CREATED: 2013-01-09
// UPDATED: 2021-10-02
///////////////////////////////////////////////////////////////////////////////

#include <cstdlib>
#include <string>
#include <sstream>
#include "ControllerTab3.h"
#include "wcharUtil.h"
#include "Timer.h"
#include "resource.h"
#include "Log.h"
using namespace Win;



///////////////////////////////////////////////////////////////////////////////
// default contructor
///////////////////////////////////////////////////////////////////////////////
ControllerTab3::ControllerTab3(ModelGL* model, ViewTab3* view) : model(model), view(view),
                                                                 glHandle(0), statusHandle(0)
{
    // remember cursor handles
    defaultCursor = ::LoadCursor(0, IDC_ARROW);
    waitCursor = ::LoadCursor(0, IDC_WAIT);

    // create brush for tab control
    bgBrush = ::CreateSolidBrush(RGB(255, 255, 255));
}



///////////////////////////////////////////////////////////////////////////////
// handle WM_DESTROY
///////////////////////////////////////////////////////////////////////////////
LRESULT ControllerTab3::destroy()
{
    Win::log("ControllerTab3 is destroyed.");
    return 0;
}



///////////////////////////////////////////////////////////////////////////////
// handle WM_CREATE
///////////////////////////////////////////////////////////////////////////////
LRESULT ControllerTab3::create()
{
    view->initControls(handle);
    return 0;
}



///////////////////////////////////////////////////////////////////////////////
// handle WM_COMMAND
///////////////////////////////////////////////////////////////////////////////
LRESULT ControllerTab3::command(int id, int command, LPARAM msg)
{
    switch(id)
    {
    case IDC_BUTTON_RESET_T:
        if(command == BN_CLICKED)
        {
            view->resetPosition();
        }
        break;
    case IDC_BUTTON_RESET_R:
        if(command == BN_CLICKED)
        {
            view->resetRotation();
        }
        break;
    case IDC_BUTTON_RESET_S:
        if(command == BN_CLICKED)
        {
            view->resetScale();
        }
        break;
    case IDC_BUTTON_SAVE_OBJ:
        if(command == BN_CLICKED)
        {
            saveFileDialog();
        }
        break;
    case IDC_EDIT_TX:
        if(command == EN_CHANGE)
        {
            view->changePositionX();
        }
        break;
    case IDC_EDIT_TY:
        if(command == EN_CHANGE)
        {
            view->changePositionY();
        }
        break;
    case IDC_EDIT_TZ:
        if(command == EN_CHANGE)
        {
            view->changePositionZ();
        }
        break;
    case IDC_EDIT_RX:
        if(command == EN_CHANGE)
        {
            view->changeAngleX();
        }
        break;
    case IDC_EDIT_RY:
        if(command == EN_CHANGE)
        {
            view->changeAngleY();
        }
        break;
    case IDC_EDIT_RZ:
        if(command == EN_CHANGE)
        {
            view->changeAngleZ();
        }
        break;
    case IDC_EDIT_SX:
        if(command == EN_CHANGE)
        {
            view->changeScaleX();
        }
        break;
    case IDC_EDIT_SY:
        if(command == EN_CHANGE)
        {
            view->changeScaleY();
        }
        break;
    case IDC_EDIT_SZ:
        if(command == EN_CHANGE)
        {
            view->changeScaleZ();
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
LRESULT ControllerTab3::lButtonDown(WPARAM state, int x, int y)
{
    //Win::log("left mouse down");
    return 0;
}



///////////////////////////////////////////////////////////////////////////////
// handle WM_NOTIFY
// The id is not guaranteed to be unique, so use NMHDR.hwndFrom and NMHDR.idFrom.
///////////////////////////////////////////////////////////////////////////////
LRESULT ControllerTab3::notify(int id, LPARAM lParam)
{
    // first cast lParam to NMHDR* to know what the control is
    NMHDR* nmhdr = (NMHDR*)lParam;
    HWND from = nmhdr->hwndFrom;

    switch(nmhdr->code)
    {
    // UpDownBox notifications =========
    case UDN_DELTAPOS:         // the change of position has begun
        {
        // cast again lParam to NMUPDOWN*
        NMUPDOWN* nmUpDown = (NMUPDOWN*)lParam;
        view->changeUpDownPosition(from, nmUpDown->iPos + nmUpDown->iDelta);
        break;
        }
    }

    return 0;
}



///////////////////////////////////////////////////////////////////////////////
// handle WM_SIZE
// the input width and height are the display area of the tab control
///////////////////////////////////////////////////////////////////////////////
LRESULT ControllerTab3::size(int w, int h, WPARAM wParam)
{
    // resize this dialog with tab's display area, but do not move (ignore x and y)
    ::SetWindowPos(handle, 0, 0, 0, w, h, SWP_NOMOVE | SWP_NOZORDER);

    return 0;
}



///////////////////////////////////////////////////////////////////////////////
// handle WM_TIMER notification
///////////////////////////////////////////////////////////////////////////////
LRESULT ControllerTab3::timer(WPARAM eventId, LPARAM callback)
{
    return 0;
}



///////////////////////////////////////////////////////////////////////////////
// handle WN_CTLCOLOR*
// return handle to brush
///////////////////////////////////////////////////////////////////////////////
LRESULT ControllerTab3::ctlColorDlg(HDC hdc, HWND handle)
{
    //Win::log(L"WM_CTLCOLORDLG");
    return (INT_PTR)bgBrush;
}

LRESULT ControllerTab3::ctlColorStatic(HDC hdc, HWND handle)
{
    //Win::log(L"WM_CTLCOLORDLG");
    ::SetBkMode(hdc, TRANSPARENT);  // make the text area transparent
    return (INT_PTR)bgBrush;
}



///////////////////////////////////////////////////////////////////////////////
// change cursor on this dialog and gl window
// NOTE: SetCursor() won't work if the cursor is moved
///////////////////////////////////////////////////////////////////////////////
void ControllerTab3::changeCursor(HCURSOR cursor)
{
    ::SetClassLongPtr(handle, GCLP_HCURSOR, (LONG_PTR)cursor);
    ::SetClassLongPtr(glHandle, GCLP_HCURSOR, (LONG_PTR)cursor);
    ::SendMessage(handle, WM_SETCURSOR, 0, 0);
    ::SendMessage(glHandle, WM_SETCURSOR, 0, 0);
}



///////////////////////////////////////////////////////////////////////////////
// open a dialog to save a obj file
///////////////////////////////////////////////////////////////////////////////
void ControllerTab3::saveFileDialog()
{
    const int MAX_CHARS = 512;

    OPENFILENAME ofn;
    memset(&ofn, 0, sizeof(ofn));

    wchar_t fileName[MAX_CHARS] = L"";  // file path to save

    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = 0;
    ofn.lpstrFilter = L"OBJ Files (*.obj)\0*.obj\0All Files (*.*)\0*.*\0\0";
    ofn.lpstrFile = fileName;
    ofn.nMaxFile = MAX_CHARS;
    ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
    ofn.lpstrDefExt = L"obj";

    if(::GetSaveFileName(&ofn))
    {
        Win::log(L"Saving OBJ: %ls...", fileName);
        objFileName = fileName;
        view->setObjFile(fileName);

        // change cursor
        changeCursor(waitCursor);

        // bind class member function and args
        //CallbackResult callback = std::bind(&ControllerTab3::callbackObjSaved, this,
        //                                    std::placeholders::_1, std::placeholders::_2);
        model->startSaveObj(toChar(fileName), callbackFunction, this);

        view->disableControls();
    }
}



///////////////////////////////////////////////////////////////////////////////
// static function to bridge C-callback
///////////////////////////////////////////////////////////////////////////////
void ControllerTab3::callbackFunction(void* obj, bool result, double elapsedMillis)
{
    if(obj)
    {
        ((ControllerTab3*)obj)->callbackObjSaved(result, elapsedMillis);
    }
}



///////////////////////////////////////////////////////////////////////////////
// callback functons
///////////////////////////////////////////////////////////////////////////////
void ControllerTab3::callbackObjSaved(bool result, double elapsedMillis)
{
    std::wstringstream wss;
    if(result)
    {
        wss << L"Success to save " << objFileName;
    }
    else
    {
        wss << L"[ERROR] Failed to save " << objFileName;
    }

    Win::log(wss.str());
    Win::log("Elapsed Time: %f ms.\n", elapsedMillis);

    // restore cursor & controls
    view->enableControls();
    changeCursor(defaultCursor);
}
