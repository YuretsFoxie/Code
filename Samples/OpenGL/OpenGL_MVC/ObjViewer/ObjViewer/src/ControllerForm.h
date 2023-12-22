///////////////////////////////////////////////////////////////////////////////
// ControllerForm.h
// ================
// Derived Controller class for dialog window
//
//  AUTHOR: Song Ho Ahn (song.ahn@gamil.com)
// CREATED: 2013-01-09
// UPDATED: 2021-02-16
///////////////////////////////////////////////////////////////////////////////

#ifndef WIN_CONTROLLER_FORM_H
#define WIN_CONTROLLER_FORM_H

#include "Controller.h"
#include "ViewForm.h"
#include "ViewTab1.h"
#include "ViewTab2.h"
#include "ViewTab3.h"
#include "ModelGL.h"


namespace Win
{
    class ControllerForm : public Controller
    {
    public:
        ControllerForm(ModelGL* model, ViewForm* view);
        ~ControllerForm() {};

        LRESULT command(int id, int cmd, LPARAM msg);   // for WM_COMMAND
        LRESULT create();                               // for WM_CREATE
        LRESULT destroy();                              // for WM_DESTROY
        LRESULT notify(int id, LPARAM lParam);          // for WM_NOTIFY
        LRESULT size(int w, int h, WPARAM wParam);      // for WM_SIZE

        void setTabView1(ViewTab1* view)            { tabView1 = view; }
        void setTabView2(ViewTab2* view)            { tabView2 = view; }
        void setTabView3(ViewTab3* view)            { tabView3 = view; }
        void setGLHandle(HWND handle)               { glHandle = handle; }
        void setStatusHandle(HWND handle)           { statusHandle = handle; }

        // callback function for loading OBJ
        void callbackObjLoaded(bool result, double elapsedMillis);

        // set inital obj file from command-line argument
        // NOTE: call it before creating the window
        void setInitialObjFile(std::wstring& path)  { objFileName = path; }

    private:
        ModelGL* model;                             // pointer to model component
        ViewForm* view;                             // pointer to view component
        ViewTab1* tabView1;                         // pointer to tab view
        ViewTab2* tabView2;                         // pointer to tab view
        ViewTab3* tabView3;                         // pointer to tab view
        HWND glHandle;
        HWND statusHandle;
        std::wstring objFileName;
        HCURSOR defaultCursor;
        HCURSOR waitCursor;

        void openFileDialog();
        void changeCursor(HCURSOR cursor);

        // static function for bridge C-callback
        static void callbackFunction(void* obj, bool result, double elapsedMillis);
    };
}

#endif
