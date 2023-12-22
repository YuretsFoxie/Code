///////////////////////////////////////////////////////////////////////////////
// ControllerForm.cpp
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
#include "ControllerForm.h"
#include "wcharUtil.h"
#include "Timer.h"
#include "resource.h"
#include "Log.h"
using namespace Win;

// constants
const char* DEFAULT_DIR = "data/";
const char* DEFAULT_OBJ = "cube_tris.obj";

INT_PTR CALLBACK aboutDialogProcedure(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);



///////////////////////////////////////////////////////////////////////////////
// default contructor
///////////////////////////////////////////////////////////////////////////////
ControllerForm::ControllerForm(ModelGL* model, ViewForm* view) : model(model), view(view),
                                                                 glHandle(0), statusHandle(0)
{
    // remember cursor handles
    defaultCursor = ::LoadCursor(0, IDC_ARROW);
    waitCursor = ::LoadCursor(0, IDC_WAIT);
    //waitCursor = (HCURSOR)::LoadImage(NULL,IDC_WAIT,IMAGE_CURSOR,0,0,LR_SHARED);
}



///////////////////////////////////////////////////////////////////////////////
// handle WM_DESTROY
///////////////////////////////////////////////////////////////////////////////
LRESULT ControllerForm::destroy()
{
    Win::log("Form dialog is destroyed.");
    return 0;
}



///////////////////////////////////////////////////////////////////////////////
// handle WM_CREATE
///////////////////////////////////////////////////////////////////////////////
LRESULT ControllerForm::create()
{
    // initialize all controls
    view->initControls(handle);

    // callback for loading OBJ
    //CallbackResult callback = std::bind(&ControllerForm::callbackObjLoaded, this,
    //                                    std::placeholders::_1, std::placeholders::_2);
    //CallbackResult callback = std::bind(&ControllerForm::callbackObjLoaded,
    //                                    std::placeholders::_1, std::placeholders::_2);

    // check initial obj to load
    if(objFileName.size() > 0)
    {
        // load default obj model
        Win::log(L"Loading OBJ from command-line: %s ...", objFileName.c_str());
        model->startLoadObj(toChar(objFileName.c_str()), callbackFunction, this);
    }
    else
    {
        // load default obj model
        Win::log("Loading a default OBJ: %s ...", DEFAULT_OBJ);
        objFileName = toWchar(DEFAULT_OBJ);
        std::stringstream ss;
        ss << DEFAULT_DIR << DEFAULT_OBJ;
        model->startLoadObj(ss.str().c_str(), callbackFunction, this);
    }

    return 0;
}



///////////////////////////////////////////////////////////////////////////////
// handle WM_COMMAND
///////////////////////////////////////////////////////////////////////////////
LRESULT ControllerForm::command(int id, int command, LPARAM msg)
{
    switch(id)
    {
    case IDC_BUTTON_LOAD:
        if(command == BN_CLICKED)
        {
            openFileDialog();
        }
        break;
    case IDC_BUTTON_ABOUT:
        if(command == BN_CLICKED)
        {
            ::DialogBox((HINSTANCE)::GetWindowLongPtr(handle, GWLP_HINSTANCE), MAKEINTRESOURCE(IDD_ABOUT), handle, aboutDialogProcedure);
        }
        break;
    }

    // return focus to the parent, so it can receive key events
    //::SetFocus(::GetParent(handle));

    return 0;
}



///////////////////////////////////////////////////////////////////////////////
// handle WM_NOTIFY
// The id is not guaranteed to be unique, so use NMHDR.hwndFrom and NMHDR.idFrom.
///////////////////////////////////////////////////////////////////////////////
LRESULT ControllerForm::notify(int id, LPARAM lParam)
{
    // first cast lParam to NMHDR* to know what the control is
    NMHDR* nmhdr = (NMHDR*)lParam;

    switch(nmhdr->code)
    {
    // Tab Control Notification =======
    case TCN_FOCUSCHANGE:   // when focus changed
        //view.tcFocusChange();
        break;

    case TCN_GETOBJECT:     // when an object is dragged over a tab
        // (NMOBJECTNOTIFY*)lParam contains the info of object
        //view.tcGetObject((NMOBJECTNOTIFY*)lParam);
        break;

    case TCN_KEYDOWN:       // when key event occured
        //view.tcKeyDown((NMTCKEYDOWN*)lParam);
        break;

    case TCN_SELCHANGING:   // when about to change
        // return 1 (TRUE) to prevent from changing, return 0 (FALSE) to allow
        break;

    case TCN_SELCHANGE:     // when a tab is selected
        view->changeTabView();
        break;

    default:
        break;
    }

    // handle TreeView notifications
    return 0;
}



///////////////////////////////////////////////////////////////////////////////
// handle WM_SIZE
///////////////////////////////////////////////////////////////////////////////
LRESULT ControllerForm::size(int w, int h, WPARAM wParam)
{
    view->resizeTab();
    return 0;
}



///////////////////////////////////////////////////////////////////////////////
// open a dialog to open obj file
///////////////////////////////////////////////////////////////////////////////
void ControllerForm::openFileDialog()
{
    const int MAX_CHARS = 512;

    OPENFILENAME ofn;
    memset(&ofn, 0, sizeof(ofn));

    wchar_t fileName[MAX_CHARS] = L"";  // file path to open

    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = handle;
    ofn.lpstrFilter = L"OBJ Files (*.obj)\0*.obj\0All Files (*.*)\0*.*\0\0";
    ofn.lpstrFile = fileName;
    ofn.nMaxFile = MAX_CHARS;
    ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
    ofn.lpstrDefExt = L"obj";

    if(::GetOpenFileName(&ofn))
    {
        // default window title
        ::SetWindowText(::GetParent(handle), L"OBJ Viewer");

        Win::log(L"Loading OBJ: %ls...", fileName);
        objFileName = fileName;

        // change cursor
        changeCursor(waitCursor);

        // bind class member function and args
        //CallbackResult callback = std::bind(&ControllerForm::callbackObjLoaded, this,
        //                                    std::placeholders::_1, std::placeholders::_2);
        model->startLoadObj(toChar(fileName), callbackFunction, this);

        view->disableControls();
        tabView1->disableControls();
        tabView2->disableControls();
        tabView3->disableControls();
    }
}



///////////////////////////////////////////////////////////////////////////////
// change cursor on this dialog and gl window
// NOTE: SetCursor() won't work if the cursor is moved
///////////////////////////////////////////////////////////////////////////////
void ControllerForm::changeCursor(HCURSOR cursor)
{
    ::SetClassLongPtr(handle, GCLP_HCURSOR, (LONG_PTR)cursor);
    ::SetClassLongPtr(glHandle, GCLP_HCURSOR, (LONG_PTR)cursor);
    ::SendMessage(handle, WM_SETCURSOR, 0, 0);
    ::SendMessage(glHandle, WM_SETCURSOR, 0, 0);
}



///////////////////////////////////////////////////////////////////////////////
// callback functions
///////////////////////////////////////////////////////////////////////////////
void ControllerForm::callbackObjLoaded(bool result, double elapsedMillis)
{
    std::wstringstream wss;
    if(result)
    {
        // update window title
        std::wstring title = L"OBJ Viewer: ";
        //title += objFileName.substr(objFileName.find_last_of(L"/\\")+1);
        title += objFileName;
        ::SetWindowText(::GetParent(handle), title.c_str());

        tabView2->setDescription();
        tabView3->resetTransforms();
        wss << L"Success to open " << objFileName;
    }
    else
    {
        tabView2->clearDescription();
        tabView3->resetTransforms();
        wss << L"[ERROR] Failed to open " << objFileName;
    }

    view->enableControls();
    tabView1->enableControls();
    tabView2->enableControls();
    tabView3->enableControls();

    Win::log(wss.str());
    Win::log("Elapsed Time: %f ms.\n", elapsedMillis);

    // restore cursor
    changeCursor(defaultCursor);
}



//*****************************************************************************
///////////////////////////////////////////////////////////////////////////////
// static function for C-callback
///////////////////////////////////////////////////////////////////////////////
void ControllerForm::callbackFunction(void* obj, bool result, double elapsedMillis)
{
    // pass it to member function
    if(obj)
    {
        ((ControllerForm*)obj)->callbackObjLoaded(result, elapsedMillis);
    }
}



//*****************************************************************************
///////////////////////////////////////////////////////////////////////////////
// dialog procedure for About window
///////////////////////////////////////////////////////////////////////////////
INT_PTR CALLBACK aboutDialogProcedure(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch(msg)
    {
    case WM_INITDIALOG:
        {
        // get dim of parent window in screen space
        RECT rect;
        HWND parentHandle = ::GetParent(hwnd);
        ::GetWindowRect(parentHandle, &rect);

        // get center position of parent window
        int cx = (rect.right + rect.left) / 2;
        int cy = (rect.bottom + rect.top) / 2;

        // get dim of dialog
        ::GetWindowRect(hwnd, &rect);
        int hw = (rect.right - rect.left) / 2;  // half width
        int hh = (rect.bottom - rect.top) / 2;  // half height

        // position this dialog to the center of its parent without resizing
        ::SetWindowPos(hwnd, HWND_TOP, cx - hw, cy - hh, 0, 0, SWP_NOSIZE);
        }
        break;

    case WM_CLOSE:
        ::EndDialog(hwnd, 0);
        break;

    case WM_COMMAND:
        if(LOWORD(wParam) == IDC_OK && HIWORD(wParam) == BN_CLICKED)
        {
            ::EndDialog(hwnd, 0);
        }
        break;
    }

    return false;
}



