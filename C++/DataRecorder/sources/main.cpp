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

// TODO: Implement printing to the GDI window.
// TODO: Consider implementing the Commands, perform general refactoring.

// TODO: Fix sending data from the MCU unit, peform the COM testing and refactoring.
// TODO: Implement displaying the UART data at the half of the GDI window.

// TODO: Add the FFT unit to the application and test it.
// TODO: Implement displaying the signal spectrum.

/*
View Facade(s)?
View, GDIView, COMView

Controller (Observer)

Model Facade(s)?
COMPorts (Subject), COMSettings
*/
