///////////////////////////////////////////////////////////////////////////////
// ViewForm.cpp
// ============
// View component of tab control dialog window
//
//  AUTHORL Song Ho Ahn (song.ahn@gmail.com)
// CREATED: 2013-01-09
// UPDATED: 2020-02-05
///////////////////////////////////////////////////////////////////////////////

#include <sstream>
#include <iomanip>
#include <cmath>
#include "ViewForm.h"
#include "resource.h"
#include "Log.h"
#include "wcharUtil.h"
using namespace Win;



///////////////////////////////////////////////////////////////////////////////
// default ctor
///////////////////////////////////////////////////////////////////////////////
ViewForm::ViewForm(ModelGL* model) : model(model), parentHandle(0)
{
}


///////////////////////////////////////////////////////////////////////////////
// default dtor
///////////////////////////////////////////////////////////////////////////////
ViewForm::~ViewForm()
{
}



///////////////////////////////////////////////////////////////////////////////
// initialize all controls
///////////////////////////////////////////////////////////////////////////////
void ViewForm::initControls(HWND handle)
{
    // remember the handle to parent window
    parentHandle = handle;

    buttonLoad.set(handle, IDC_BUTTON_LOAD);
    tab.set(handle, IDC_TAB);

    buttonAbout.set(handle, IDC_BUTTON_ABOUT);
    buttonAbout.setImage(::LoadIcon(0, IDI_INFORMATION));

    // add items to tab control: tabIndex, title, imageIndex
    tab.insertItem(0, L"View", -1);
    tab.insertItem(1, L"Info", -1);
    tab.insertItem(2, L"Transform", -1);

}



///////////////////////////////////////////////////////////////////////////////
// enable/disable controls
///////////////////////////////////////////////////////////////////////////////
void ViewForm::enableControls()
{
    buttonLoad.enable();
    buttonAbout.enable();
}
void ViewForm::disableControls()
{
    buttonLoad.disable();
    buttonAbout.disable();
}



///////////////////////////////////////////////////////////////////////////////
// resize tab control so it fit to the bottom of parent window
///////////////////////////////////////////////////////////////////////////////
void ViewForm::resizeTab()
{
    RECT rect;

    // get parent's position (x,y) in window coords in screen space
    ::GetWindowRect(parentHandle, &rect);
    int parentX = rect.left;
    int parentY = rect.top;
    int parentRight = rect.right;
    int parentBottom = rect.bottom;

    // compute new tab's position, width and height
    ::GetWindowRect(tab.getHandle(), &rect);
    int x = rect.left - parentX;
    int y = rect.top - parentY;
    int spacing = parentRight - rect.right;
    int width = rect.right - rect.left;
    int height = parentBottom - rect.top - spacing;
    if(height < 0)
        height = 0;

    ::SetWindowPos(tab.getHandle(), 0, x, y, width, height, SWP_NOZORDER);

    // compute display area of tab control
    ::GetClientRect(tab.getHandle(), &rect);
    tab.adjustRect(false, &rect);
    width = rect.right - rect.left;
    height = rect.bottom - rect.top;

    // resize child tab views
    ::SendMessage(tabViewHandle1, WM_SIZE, 0, MAKELPARAM(width, height));
    ::SendMessage(tabViewHandle2, WM_SIZE, 0, MAKELPARAM(width, height));
}



///////////////////////////////////////////////////////////////////////////////
// remember the window handle of tab view
///////////////////////////////////////////////////////////////////////////////
void ViewForm::setTabViewHandle(int index, HWND handle)
{
    if(index == 1)
    {
        tabViewHandle1 = handle;
        currTabViewHandle = handle;
        ::ShowWindow(handle, SW_SHOW);  // make visible by default
    }
    else if(index == 2)
    {
        tabViewHandle2 = handle;
    }
    else if(index == 3)
    {
        tabViewHandle3 = handle;
    }

    // resize tab view rect to fit into tab control
    RECT rect;
    ::GetClientRect(tab.getHandle(), &rect);
    tab.adjustRect(false, &rect);               // compute display area inside tab control
    ::SetWindowPos(handle, 0, rect.left, rect.top, rect.right-rect.left, rect.bottom-rect.top, SWP_NOZORDER);

    // re-set parent handle of this tab view
    ::SetParent(handle, tab.getHandle());
}



///////////////////////////////////////////////////////////////////////////////
// handle when tab is selected
///////////////////////////////////////////////////////////////////////////////
void ViewForm::changeTabView()
{
    // get index of selected tab
    int selectedIndex = tab.getCurSel();    // 0-based

    // determine selected HWND
    HWND selectedHandle;
    if(selectedIndex == 0)
        selectedHandle = tabViewHandle1;
    else if(selectedIndex == 1)
        selectedHandle = tabViewHandle2;
    else if(selectedIndex == 2)
        selectedHandle = tabViewHandle3;

    // toggle selected dialog
    ::ShowWindow(currTabViewHandle, SW_HIDE);
    ::ShowWindow(selectedHandle, SW_SHOW);
    currTabViewHandle = selectedHandle;
}
