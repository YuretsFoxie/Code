///////////////////////////////////////////////////////////////////////////////
// ControllerTab1.cpp
// ==================
// Derived Controller class for dialog window
//
//  AUTHOR: Song Ho Ahn (song.ahn@gamil.com)
// CREATED: 2013-01-09
// UPDATED: 2021-10-02
///////////////////////////////////////////////////////////////////////////////

#include <cstdlib>
#include <string>
#include <sstream>
#include "ControllerTab1.h"
#include "wcharUtil.h"
#include "Timer.h"
#include "resource.h"
#include "Log.h"
using namespace Win;

INT_PTR CALLBACK aboutDialogProcedure(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

// low-level mouse event hook
HWND parentHandle = 0;  // handle for this formview dialog
ModelGL* modelPtr;      // need to send camera shift values
HHOOK mouseHook = 0;
LRESULT CALLBACK lowLevelMouseProcedure(int code, WPARAM wParam, LPARAM lParam);



///////////////////////////////////////////////////////////////////////////////
// default contructor
///////////////////////////////////////////////////////////////////////////////
ControllerTab1::ControllerTab1(ModelGL* model, ViewTab1* view) : model(model), view(view),
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
LRESULT ControllerTab1::destroy()
{
    // uninstall mouse hook
    ::UnhookWindowsHookEx(mouseHook);
    Win::log("Uninstalled low-level mouse hook.");

    Win::log("Form dialog is destroyed.");
    return 0;
}



///////////////////////////////////////////////////////////////////////////////
// handle WM_CREATE
///////////////////////////////////////////////////////////////////////////////
LRESULT ControllerTab1::create()
{
    // initialize all controls
    view->initControls(handle);

    // insall low-level mouse hook to detect mouse down events on buttons
    parentHandle = handle;
    modelPtr = model;
    HINSTANCE instance = ::GetModuleHandle(0);
    mouseHook = ::SetWindowsHookEx(WH_MOUSE_LL, lowLevelMouseProcedure, instance, 0);
    if(mouseHook)
        Win::log("Installed low-level mouse hook in ControllerTab1 class.");
    else
        Win::log("[ERROR] Failed to install mouse hook.");

    return 0;
}



///////////////////////////////////////////////////////////////////////////////
// handle WM_COMMAND
///////////////////////////////////////////////////////////////////////////////
LRESULT ControllerTab1::command(int id, int command, LPARAM msg)
{
    switch(id)
    {
    case IDC_BUTTON_SCREENSHOT:
        if(command == BN_CLICKED)
        {
            saveScreenshotDialog();
        }
        break;
    case IDC_CHECK_GRID:
        if(command == BN_CLICKED)
        {
            view->toggleGrid();
        }
        break;
    case IDC_CHECK_TEXTURE:
        if(command == BN_CLICKED)
        {
            view->toggleTexture();
        }
        break;
    case IDC_CHECK_WIREFRAME:
        if(command == BN_CLICKED)
        {
            view->toggleWireframe();
        }
        break;
    case IDC_CHECK_DEFAULT_TEXTURE:
        if(command == BN_CLICKED)
        {
            view->toggleDefaultTexture();
        }
        break;
    case IDC_CHECK_BOUNDINGBOX:
        if(command == BN_CLICKED)
        {
            view->toggleBoundingBox();
        }
        break;
    case IDC_CHECK_BLACK:
        if(command == BN_CLICKED)
        {
            view->toggleBlackBackground();
        }
        break;
    case IDC_BUTTON_RESET_CAMERA:
        if(command == BN_CLICKED)
        {
            model->resetMatrices();
        }
        break;
    case IDC_BUTTON_RESET_MATERIAL:
        if(command == BN_CLICKED)
        {
            model->resetMaterial();
            view->resetMaterialTrackBars();
        }
        break;
    case IDC_BUTTON_RESET_LIGHT:
        if(command == BN_CLICKED)
        {
            model->resetLightPosition();
            view->resetLightTrackBars();
        }
        break;
    // combobox event
    case IDC_COMBO_SCREENSHOT:
        if(command == CBN_SELCHANGE)
        {
            view->changeScreenshotSize();
        }
        else if(command == CBN_DROPDOWN)
        {
            //
        }
        else if(command == CBN_CLOSEUP)
        {
            //;
        }
        else if(command == CBN_SELENDOK)
        {
            //;
        }
        else if(command == CBN_SELENDCANCEL)
        {
            //;
        }
        break;
    }

    // return focus to the parent, so it can receive key events
    //::SetFocus(::GetParent(handle));

    return 0;
}



///////////////////////////////////////////////////////////////////////////////
// handle horizontal scroll notification
///////////////////////////////////////////////////////////////////////////////
LRESULT ControllerTab1::hScroll(WPARAM wParam, LPARAM lParam)
{
    // check if the message comming from trackbar
    HWND trackbarHandle = (HWND)lParam;

    int position = HIWORD(wParam);              // current tick mark position
    if(trackbarHandle)
    {
        // get control ID
        int trackbarId = ::GetDlgCtrlID(trackbarHandle);

        switch(LOWORD(wParam))
        {
        case TB_THUMBTRACK:     // user dragged the slider
            //Win::log("trackbar: %d", position);
            view->updateTrackbars(trackbarHandle, position);
            break;

        case TB_THUMBPOSITION:  // by WM_LBUTTONUP
            break;

        case TB_LINEUP:         // by VK_RIGHT, VK_DOWN
            break;

        case TB_LINEDOWN:       // by VK_LEFT, VK_UP
            break;

        case TB_TOP:            // by VK_HOME
            break;

        case TB_BOTTOM:         // by VK_END
            break;

        case TB_PAGEUP:         // by VK_PRIOR (User click the channel to prior.)
            break;

        case TB_PAGEDOWN:       // by VK_NEXT (User click the channel to next.)
            break;

        case TB_ENDTRACK:       // by WM_KEYUP (User release a key.)
            position = (int)::SendMessage(trackbarHandle, TBM_GETPOS, 0, 0);
            view->updateTrackbars(trackbarHandle, position);
            break;
        }
    }

    return 0;
}



///////////////////////////////////////////////////////////////////////////////
// handle WM_LBUTTONDOWN
///////////////////////////////////////////////////////////////////////////////
LRESULT ControllerTab1::lButtonDown(WPARAM state, int x, int y)
{
    //Win::log("left mouse down");
    return 0;
}



///////////////////////////////////////////////////////////////////////////////
// handle WM_NOTIFY
// The id is not guaranteed to be unique, so use NMHDR.hwndFrom and NMHDR.idFrom.
///////////////////////////////////////////////////////////////////////////////
LRESULT ControllerTab1::notify(int id, LPARAM lParam)
{
    return 0;
}



///////////////////////////////////////////////////////////////////////////////
// handle WM_SIZE
///////////////////////////////////////////////////////////////////////////////
LRESULT ControllerTab1::size(int w, int h, WPARAM wParam)
{
    return 0;
}



///////////////////////////////////////////////////////////////////////////////
// handle WM_TIMER notification
///////////////////////////////////////////////////////////////////////////////
LRESULT ControllerTab1::timer(WPARAM eventId, LPARAM callback)
{
    return 0;
}



///////////////////////////////////////////////////////////////////////////////
// handle WN_CTLCOLOR*
// return handle to brush
///////////////////////////////////////////////////////////////////////////////
LRESULT ControllerTab1::ctlColorDlg(HDC hdc, HWND handle)
{
    return (INT_PTR)bgBrush;
}

LRESULT ControllerTab1::ctlColorStatic(HDC hdc, HWND handle)
{
    ::SetBkMode(hdc, TRANSPARENT);  // make the text area transparent
    return (INT_PTR)bgBrush;
}



///////////////////////////////////////////////////////////////////////////////
// open a dialog to save the screenshot
///////////////////////////////////////////////////////////////////////////////
void ControllerTab1::saveScreenshotDialog()
{
    const int MAX_CHARS = 512;

    OPENFILENAME ofn;
    memset(&ofn, 0, sizeof(ofn));

    wchar_t fileName[MAX_CHARS] = L"";  // file path to open

    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = 0;
    ofn.lpstrFilter = L"PNG Files (*.png)\0*.png\0All Files (*.*)\0*.*\0\0";
    ofn.lpstrFile = fileName;
    ofn.nMaxFile = MAX_CHARS;
    ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
    ofn.lpstrDefExt = L"png";

    if(::GetSaveFileName(&ofn))
    {
        Win::log(L"Saving PNG: %ls...", fileName);
        screenshotFileName = fileName;
        view->setScreenshotFile(fileName);

        // change cursor
        changeCursor(waitCursor);

        //CallbackResult callback = std::bind(&ControllerTab1::callbackScreenCaptured, this,
        //                                    std::placeholders::_1, std::placeholders::_2);
        model->startSaveImage(toChar(fileName), callbackFunction, this);
        view->disableControls();
    }
}



///////////////////////////////////////////////////////////////////////////////
// change cursor on this dialog and gl window
// NOTE: SetCursor() won't work if the cursor is moved
///////////////////////////////////////////////////////////////////////////////
void ControllerTab1::changeCursor(HCURSOR cursor)
{
    ::SetClassLongPtr(handle, GCLP_HCURSOR, (LONG_PTR)cursor);
    ::SetClassLongPtr(glHandle, GCLP_HCURSOR, (LONG_PTR)cursor);
    ::SendMessage(handle, WM_SETCURSOR, 0, 0);
    ::SendMessage(glHandle, WM_SETCURSOR, 0, 0);
}



///////////////////////////////////////////////////////////////////////////////
// callback functons
///////////////////////////////////////////////////////////////////////////////
void ControllerTab1::callbackScreenCaptured(bool result, double elapsedMillis)
{
    if(result)
    {
        Win::log(L"Saved PNG: %ls (%.1f ms)", screenshotFileName.c_str(), elapsedMillis);
    }
    else
    {
        Win::log(L"[ERROR] Failed to save %ls", screenshotFileName.c_str());
    }
    view->enableControls();

    // restore cursor
    changeCursor(defaultCursor);
}



///////////////////////////////////////////////////////////////////////////////
// static function to bridge C-callback
///////////////////////////////////////////////////////////////////////////////
void ControllerTab1::callbackFunction(void* obj, bool result, double elapsedMillis)
{
    if(obj)
    {
        ((ControllerTab1*)obj)->callbackScreenCaptured(result, elapsedMillis);
    }
}



///////////////////////////////////////////////////////////////////////////////
// hook procedure for low-level mouse events
// It is used to receive mouse events on buttons in this formview
///////////////////////////////////////////////////////////////////////////////
LRESULT CALLBACK lowLevelMouseProcedure(int code, WPARAM wParam, LPARAM lParam)
{
    POINT pos;      // mouse position
    HWND handle;    // window handle under mouse position

    if(code >= 0)
    {
        switch(wParam)
        {
        case WM_LBUTTONDOWN:
            ::GetCursorPos(&pos); // get cursor position, @@lParam won't work
            handle = ::WindowFromPoint(pos);
            //Win::log("Pos: %d, %d", pos.x, pos.y);
            if(handle == ::GetDlgItem(parentHandle, IDC_BUTTON_LEFT))
                modelPtr->startShiftCamera(-1, 0);
            else if(handle == ::GetDlgItem(parentHandle, IDC_BUTTON_RIGHT))
                modelPtr->startShiftCamera(1, 0);
            else if(handle == ::GetDlgItem(parentHandle, IDC_BUTTON_UP))
                modelPtr->startShiftCamera(0, 1);
            else if(handle == ::GetDlgItem(parentHandle, IDC_BUTTON_DOWN))
                modelPtr->startShiftCamera(0, -1);
            break;

        case WM_LBUTTONUP:
            modelPtr->stopShiftCamera();
            break;
        }
    }

    // chain to the next hook procedures
    return ::CallNextHookEx(mouseHook, code, wParam, lParam);
}
