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

// TODO: Implement the Console and Graph as a GraphicsView subclasses (use the converted coordinate systems), implement scrolling with buttons, limit the messages size. Consider using the Decorator.

// TODO: Fix the COM errors (can not disconnect, the Connect button can be clicked when unneeded, there were also switching problems).
// TODO: Check sending data from the MCU unit, remove the unneeded error messages, peform COM refactoring.
// TODO: Implement displaying the UART data as a graph.

// TODO: Consider unifiing printing with the Log.
// TODO: Consider implementing the Commands, perform general refactoring.

// TODO: Add the FFT unit to the application and test it.
// TODO: Implement displaying the signal spectrum.

/*
View Facade(s)?
View, GDIView, COMView

Controller (Observer, Mediator?)

Model Facade(s)?
COMPorts (Subject), COMSettings
*/
