#include "controller.h"

Controller controller;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int iCmdShow)
{
	return controller.run(hInstance);
}

LRESULT CALLBACK windowProcedure(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == WM_COMMAND)	controller.onClick(wParam);
	if (message == WM_KEYDOWN)	controller.onPress(wParam);
	if (message == WM_PAINT)	controller.onPaint();
	
	return ::DefWindowProc(hWnd, message, wParam, lParam);
}

// TODO: Complete and test the RawDataGraph.

// TODO: Fix the COM errors (can not disconnect, the Connect button can be clicked when unneeded, there were also switching problems).
// TODO: Check sending data from the MCU unit, remove the unneeded error messages.
// TODO: Implement displaying the UART data as a graph.

// TODO: Perform general refactoring, rename GDIView.

/*
View Facade(s)?
View, GDIView, COMView

Controller (Observer, Mediator?)

Model Facade(s)?
COMPorts (Subject), COMSettings
*/
