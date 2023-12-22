///////////////////////////////////////////////////////////////////////////////
// ViewForm.h
// ==========
// View component of tab control dialog window
//
//  AUTHORL Song Ho Ahn (song.ahn@gmail.com)
// CREATED: 2013-01-09
// UPDATED: 2020-02-05
///////////////////////////////////////////////////////////////////////////////

#ifndef VIEW_FORM_H
#define VIEW_FORM_H

#include <windows.h>
#include "Controls.h"
#include "ModelGL.h"

namespace Win
{
    class ViewForm
    {
    public:
        ViewForm(ModelGL* model);
        ~ViewForm();

        void initControls(HWND handle);         // init all controls
        void enableControls();
        void disableControls();
        void resizeTab();                       // called when window resized
        void changeTabView();                   // called when tab selected
        void setTabViewHandle(int index, HWND handle);

    protected:

    private:
        ModelGL* model;
        HWND parentHandle;
        HWND tabViewHandle1;
        HWND tabViewHandle2;
        HWND tabViewHandle3;
        HWND currTabViewHandle;

        // controls
        Win::Button buttonLoad;
        Win::Button buttonAbout;
        Win::Tab tab;
    };
}

#endif
