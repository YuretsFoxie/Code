///////////////////////////////////////////////////////////////////////////////
// ControllerTab3.h
// ================
// Derived Controller class for tab view
//
//  AUTHOR: Song Ho Ahn (song.ahn@gamil.com)
// CREATED: 2013-01-09
// UPDATED: 2021-10-02
///////////////////////////////////////////////////////////////////////////////

#ifndef WIN_CONTROLLER_TAB3_H
#define WIN_CONTROLLER_TAB3_H

#include "Controller.h"
#include "ViewTab3.h"
#include "ModelGL.h"


namespace Win
{
    class ControllerTab3 : public Controller
    {
    public:
        ControllerTab3(ModelGL* model, ViewTab3* view);
        ~ControllerTab3() {};

        LRESULT command(int id, int cmd, LPARAM msg);   // for WM_COMMAND
        LRESULT create();                               // for WM_CREATE
        LRESULT destroy();                              // for WM_DESTROY
        LRESULT lButtonDown(WPARAM state, int x, int y);// for WM_LBUTTONDOWN
        LRESULT notify(int id, LPARAM lParam);          // for WM_NOTIFY
        LRESULT size(int w, int h, WPARAM wParam);      // for WM_SIZE
        LRESULT timer(WPARAM eventId, LPARAM callback); // for WM_TIMER
        LRESULT ctlColorDlg(HDC hdc, HWND handle);      // for WM_CTLCOLORDLG
        LRESULT ctlColorStatic(HDC hdc, HWND handle);   // for WM_CTLCOLORSTATIC

        void setGLHandle(HWND handle)               { glHandle = handle; }
        void setStatusHandle(HWND handle)           { statusHandle = handle; }

        // callback function for saving OBJ
        void callbackObjSaved(bool result, double elapsedMillis);

    private:
        ModelGL* model;                             // pointer to model component
        ViewTab3* view;                             // pointer to view component
        HWND glHandle;
        HWND statusHandle;
        HBRUSH bgBrush;                             // bush for tab background color
        std::wstring objFileName;
        HCURSOR defaultCursor;
        HCURSOR waitCursor;

        void saveFileDialog();
        void changeCursor(HCURSOR cursor);

        // static function for C-callback
        static void callbackFunction(void* obj, bool result, double elapsedMillis);
    };
}

#endif
