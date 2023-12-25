#include "view.h"
#include "windowprocedure.h"

HWND View::onStart(HINSTANCE instance)
{
	hInstance = instance;
	
	registerWindowClass();
	createWindow();
	addControls();
	
	::SetFocus(window);
	return window;
}

void View::update(const string& shape)
{
	::SendMessage(shapeButton, WM_SETTEXT, 0, (LPARAM) shape.c_str());
}

void View::registerWindowClass()
{
	WNDCLASS wc = {0};
	
	wc.lpfnWndProc = windowProcedure;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.hInstance = hInstance;
	wc.lpszClassName = "MainWindowClass";
	wc.hCursor = ::LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_APPWORKSPACE);
	
	::RegisterClass(&wc);
}

void View::createWindow()
{
	window = ::CreateWindowEx(WS_EX_APPWINDOW | WS_EX_WINDOWEDGE, 
							   "MainWindowClass",
							   "MVC Sample",
							   WS_MINIMIZEBOX | WS_VISIBLE,
							   100, 100, 500, 400,
							   NULL, NULL, hInstance, NULL);
}

void View::addControls()
{
	shapeButton = ::CreateWindow("button", "Print", WS_CHILD | WS_VISIBLE, 20, 10, 70, 30, window, (HMENU)shapeButtonID, hInstance, NULL);
	quitButton = ::CreateWindow("button", "Quit", WS_CHILD | WS_VISIBLE, 20, 50, 70, 30, window, (HMENU)quitButtonID, hInstance, NULL);
}
