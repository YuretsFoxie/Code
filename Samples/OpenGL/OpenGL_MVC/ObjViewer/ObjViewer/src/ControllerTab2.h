///////////////////////////////////////////////////////////////////////////////
// ControllerTab2.h
// ================
// Derived Controller class for tab view
//
//  AUTHOR: Song Ho Ahn (song.ahn@gamil.com)
// CREATED: 2013-01-09
// UPDATED: 2020-01-26
///////////////////////////////////////////////////////////////////////////////

#ifndef WIN_CONTROLLER_TAB2_H
#define WIN_CONTROLLER_TAB2_H

#include "Controller.h"
#include "ViewTab2.h"
#include "ModelGL.h"


namespace Win
{
    class ControllerTab2 : public Controller
    {
    public:
        ControllerTab2(ModelGL* model, ViewTab2* view);
        ~ControllerTab2() {};

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

    private:
        ModelGL* model;                             // pointer to model component
        ViewTab2* view;                             // pointer to view component
        HWND glHandle;
        HWND statusHandle;
        HBRUSH bgBrush;                             // bush for tab background color

    };
}

#endif
