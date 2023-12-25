#include <windows.h>
#include "controller.h"

Controller controller;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int iCmdShow)
{
	return controller.run(hInstance);
}

LRESULT CALLBACK windowProcedure(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == WM_COMMAND)	controller.onClick(wParam);
	if (message == WM_SIZE)		controller.onResize();
	if (message == WM_PAINT)	controller.onPaint();
	
	return ::DefWindowProc(hWnd, message, wParam, lParam);
}



// TODO:
// Add logging, check if everything is constructed and destroyed correctly.
// Implement more advanced example with two GDI windows, control panel (within main window) and opening settings panels.
// Perform refactoring.
