///////////////////////////////////////////////////////////////////////////////
// main.cpp
// ========
// main driver
//
//  AUTHOR: Song Ho Ahn (song.ahn@gmail.com)
// CREATED: 2013-01-08
// UPDATED: 2021-02-16
///////////////////////////////////////////////////////////////////////////////

#define WIN32_LEAN_AND_MEAN             // exclude rarely-used stuff from Windows headers

#include <windows.h>
#include <commctrl.h>                   // common controls
#include "Window.h"
#include "DialogWindow.h"
#include "ControllerMain.h"
#include "ControllerForm.h"
#include "ControllerGL.h"
#include "ControllerTab1.h"
#include "ControllerTab2.h"
#include "ControllerTab3.h"
#include "ModelGL.h"
#include "ViewGL.h"
#include "ViewForm.h"
#include "ViewTab1.h"
#include "ViewTab2.h"
#include "ViewTab3.h"
#include "resource.h"
#include "Log.h"

// fix CRT link error VS2015: __iob_func
// Also include legacy_stdio_definitions.lib
#if defined(_MSC_VER) && (_MSC_VER >= 1900)
extern "C"
{
    FILE __iob_func[3] = {*stdin, *stdout, *stderr};
}
#endif


// constants ==================================================================
const wchar_t* APP_NAME = L"OBJ Viewer";
const int      GL_WIDTH = 600;
const int      GL_HEIGHT = 600;
//const int      FORM_WIDTH = 350;

// function declarations
int mainMessageLoop(HACCEL hAccelTable=0);




///////////////////////////////////////////////////////////////////////////////
// main function of a windows application
///////////////////////////////////////////////////////////////////////////////
int WINAPI wWinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPWSTR cmdArgs, int cmdShow)
{
    // init comctl32.dll before creating windows
    INITCOMMONCONTROLSEX commonCtrls;
    commonCtrls.dwSize = sizeof(commonCtrls);
    commonCtrls.dwICC = ICC_STANDARD_CLASSES | ICC_TAB_CLASSES | ICC_BAR_CLASSES | ICC_LISTVIEW_CLASSES | ICC_UPDOWN_CLASS;
    ::InitCommonControlsEx(&commonCtrls);

    Win::ControllerMain ctrlMain;
    Win::Window winMain(hInst, APP_NAME, 0, &ctrlMain);

    // create a window and show
    if(winMain.create())
        Win::log("Main window is created.");
    else
        Win::log("[ERROR] Failed to create main window.");

    // create model and view components for controller
    ModelGL modelGL;
    Win::ViewGL viewGL;

    // create OpenGL rendering window as a child
    Win::ControllerGL ctrlGL(&modelGL, &viewGL);
    Win::Window winGL(hInst, L"WindowGL", winMain.getHandle(), &ctrlGL);
    winGL.setWindowStyle(WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);
    winGL.setWindowStyleEx(WS_EX_WINDOWEDGE);
    winGL.setClassStyle(CS_OWNDC);
    //@@ MS ArticleID: 272222
    // There is a clipping bug when the window is resized larger.
    // Create a window with the max size initially to avoid this clipping bug.
    // Subsequent SetWindowPos() calls to resize the window dimension do not
    // generate the clipping issue.
    int fullWidth = ::GetSystemMetrics(SM_CXSCREEN);    // primary display only
    int fullHeight = ::GetSystemMetrics(SM_CXSCREEN);
    winGL.setWidth(fullWidth);
    winGL.setHeight(fullHeight);
    //Win::log("%d, %d", fullWidth, fullHeight);
    if(winGL.create())
        Win::log("OpenGL rendering window is created.");
    else
        Win::log("[ERROR] Failed to create OpenGL window.");

    // create a child dialog box contains controls
    Win::ViewForm viewForm(&modelGL);
    Win::ControllerForm ctrlForm(&modelGL, &viewForm);
    ctrlForm.setGLHandle(winGL.getHandle()); // to change cursor shape
    if(wcslen(cmdArgs) > 2) // cmdArgs wrapped with ""
    {
        // tell to load obj file from command-line arg
        std::wstring path = cmdArgs;
        path = path.substr(1, path.size()-2); // remove quotes
        ctrlForm.setInitialObjFile(path);
    }
    Win::DialogWindow winForm(hInst, IDD_FORM_MAIN, winMain.getHandle(), &ctrlForm);
    if(winForm.create())
        Win::log("Form dialog is created.");
    else
        Win::log("[ERROR] Failed to create Form dialog.");

    // create tab views for the form
    Win::ViewTab1 viewTab1(&modelGL);
    Win::ViewTab2 viewTab2(&modelGL);
    Win::ViewTab3 viewTab3(&modelGL);
    Win::ControllerTab1 ctrlTab1(&modelGL, &viewTab1);
    Win::ControllerTab2 ctrlTab2(&modelGL, &viewTab2);
    Win::ControllerTab3 ctrlTab3(&modelGL, &viewTab3);
    Win::DialogWindow winTab1(hInst, IDD_FORM_TAB1, winForm.getHandle(), &ctrlTab1);
    Win::DialogWindow winTab2(hInst, IDD_FORM_TAB2, winForm.getHandle(), &ctrlTab2);
    Win::DialogWindow winTab3(hInst, IDD_FORM_TAB3, winForm.getHandle(), &ctrlTab3);
    winTab1.create();
    winTab2.create();
    winTab3.create();

    ctrlForm.setTabView1(&viewTab1);
    ctrlForm.setTabView2(&viewTab2);
    ctrlForm.setTabView3(&viewTab3);
    viewForm.setTabViewHandle(1, winTab1.getHandle());
    viewForm.setTabViewHandle(2, winTab2.getHandle());
    viewForm.setTabViewHandle(3, winTab3.getHandle());

    // send window handles to main ctrl, they are used for resizing window
    ctrlMain.setGLHandle(winGL.getHandle());
    ctrlMain.setFormHandle(winForm.getHandle());
    ctrlMain.setModel(&modelGL);

    // place windows in the right position ================
    RECT rect;

    // compute dim of dialog window (for higher DPI display)
    ::GetWindowRect(winForm.getHandle(), &rect);
    int dialogWidth = rect.right - rect.left;
    int dialogHeight = rect.bottom - rect.top;

    // set dim of opengl window
    int glWidth = dialogHeight;
    int glHeight = dialogHeight;
    ::SetWindowPos(winGL.getHandle(), 0, 0, 0, glWidth, glHeight, SWP_NOZORDER);

    // place the form dialog at the right of the opengl rendering window
    ::SetWindowPos(winForm.getHandle(), 0, glWidth, 0, dialogWidth, dialogHeight, SWP_NOZORDER);

    // set dim of main window
    rect.left = 0;
    rect.right = glWidth + dialogWidth;
    rect.top = 0;
    rect.bottom = glHeight;
    DWORD style = (DWORD)::GetWindowLongPtr(winMain.getHandle(), GWL_STYLE);
    DWORD styleEx = (DWORD)::GetWindowLongPtr(winMain.getHandle(), GWL_EXSTYLE);
    ::AdjustWindowRectEx(&rect, style, FALSE, styleEx);
    ::SetWindowPos(winMain.getHandle(), 0, 0, 0, (rect.right-rect.left), (rect.bottom-rect.top), SWP_NOZORDER);

    // show all windows
    winGL.show();
    winForm.show();
    winMain.show();
    ::SetFocus(ctrlMain.getHandle());   // to receive WM_KEYDOWN event

    // main message loop //////////////////////////////////////////////////////
    int exitCode;
    HACCEL hAccelTable = 0;
    //hAccelTable = ::LoadAccelerators(hInst, MAKEINTRESOURCE(ID_ACCEL));
    exitCode = mainMessageLoop(hAccelTable);
    Win::log("Application is terminated.");

    return exitCode;
}



///////////////////////////////////////////////////////////////////////////////
// main message loop
///////////////////////////////////////////////////////////////////////////////
int mainMessageLoop(HACCEL hAccelTable)
{
    HWND activeHandle;
    MSG msg;

    while(::GetMessage(&msg, 0, 0, 0) > 0)  // loop until WM_QUIT(0) received
    {
        // determine the activated window is dialog box
        // skip if messages are for the dialog windows
        activeHandle = ::GetActiveWindow();
        if(::GetWindowLongPtr(activeHandle, GWL_EXSTYLE) & WS_EX_CONTROLPARENT) // WS_EX_CONTROLPARENT is automatically added by CreateDialogBox()
        {
            if(::IsDialogMessage(activeHandle, &msg))
                continue;   // message handled, back to while-loop
        }

        // now, handle window messages
        if(!::TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
        }
    }

    return (int)msg.wParam;                 // return nExitCode of PostQuitMessage()
}
