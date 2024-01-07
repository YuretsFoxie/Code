#include "controller.h"

Controller controller;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int iCmdShow)
{
	return controller.run(hInstance);
}

LRESULT CALLBACK windowProcedure(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == WM_COMMAND)	controller.onClick(wParam, lParam);
	if (message == WM_SIZE)		controller.onResize();
	if (message == WM_PAINT)	controller.onPaint();
	
	return ::DefWindowProc(hWnd, message, wParam, lParam);
}

// TODO: Fix the warnings.
// TODO: Complete printing to the GDI window: limit the message and buffer sizes, implement sxrolling with buttons.
// TODO: Select the subwindows shapes, consider unifiing it with the Log.
// TODO: Consider implementing the Commands, perform general refactoring.

// TODO: Fix the COM errors (the Connect button can be clicked when unneeded, there were also switching problems).
// TODO: Check sending data from the MCU unit, remove the unneeded error messages, peform COM refactoring.
// TODO: Implement displaying the UART data at the half of the GDI window as a graph.

// TODO: Add the FFT unit to the application and test it.
// TODO: Implement displaying the signal spectrum.

/*
View Facade(s)?
View, GDIView, COMView

Controller (Observer, Mediator?)

Model Facade(s)?
COMPorts (Subject), COMSettings
*/

//=====

/*
compiling comports.cpp...
compiling comsettings.cpp...
compiling comview.cpp...
sources\comview.cpp: In member function 'void COMView::addRadioButtons()':
sources\comview.cpp:100:91: warning: cast to pointer from integer of different size [-Wint-to-pointer-cast]
  100 |   HWND button = ::CreateWindowEx(0, "button", name.c_str(), style, 20, y, 85, 25, window, (HMENU)(radioButtonID + i), configuration.instance, 0);
      |                                                                                           ^~~~~~~~~~~~~~~~~~~~~~~~~~
sources\comview.cpp: In member function 'void COMView::addButtons()':
sources\comview.cpp:111:125: warning: cast to pointer from integer of different size [-Wint-to-pointer-cast]
  111 |  HWND okButton = ::CreateWindowEx(0, "button", ("OK"), WS_VISIBLE | WS_CHILD | WS_TABSTOP, 20, height - 80, 90, 40, window, (HMENU)okButtonID, configuration.instance, 0);
      |                                                                                                                             ^~~~~~~~~~~~~~~~~
sources\comview.cpp:112:134: warning: cast to pointer from integer of different size [-Wint-to-pointer-cast]
  112 |  HWND cancelButton = ::CreateWindowEx(0, "button", ("Cancel"), WS_VISIBLE | WS_CHILD | WS_TABSTOP, 130, height - 80, 90, 40, window, (HMENU)cancelButtonID, configuration.instance, 0);
      |                                                                                                                                      ^~~~~~~~~~~~~~~~~~~~~
compiling controller.cpp...
compiling gdiview.cpp...
sources\gdiview.cpp: In member function 'DWORD GDIView::drawIfNeeded()':
sources\gdiview.cpp:99:1: warning: no return statement in function returning non-void [-Wreturn-type]
   99 | }
      | ^
compiling log.cpp...
compiling main.cpp...
compiling observablemodel.cpp...
compiling view.cpp...
In file included from C:/TDM-GCC-64/x86_64-w64-mingw32/include/windows.h:72,
                 from headers/view.h:4,
                 from sources\view.cpp:1:
sources\view.cpp: In member function 'void View::addControls()':
sources\view.cpp:77:100: warning: cast to pointer from integer of different size [-Wint-to-pointer-cast]
   77 |  connectButton = ::CreateWindow("button", "Connect", WS_CHILD | WS_VISIBLE, 0, 0, 110, 50, window, (HMENU)connectButtonID, hInstance, NULL);
      |                                                                                                    ^~~~~~~~~~~~~~~~~~~~~~
sources\view.cpp:78:106: warning: cast to pointer from integer of different size [-Wint-to-pointer-cast]
   78 |  comSettingsButton = ::CreateWindow("button", "Settings", WS_CHILD | WS_VISIBLE, 0, 50, 110, 50, window, (HMENU)comSettingsButtonID, hInstance, NULL);
      |                                                                                                          ^~~~~~~~~~~~~~~~~~~~~~~~~~
sources\view.cpp:79:96: warning: cast to pointer from integer of different size [-Wint-to-pointer-cast]
   79 |  quitButton = ::CreateWindow("button", "Quit", WS_CHILD | WS_VISIBLE, 0, 100, 110, 50, window, (HMENU)quitButtonID, hInstance, NULL);
      |                                                                                                ^~~~~~~~~~~~~~~~~~~
compiling windowprocedure.cpp...
linking...
*/
