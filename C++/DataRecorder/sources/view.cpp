#include "view.h"
#include "windowprocedure.h"

HWND View::onStart(HINSTANCE instance)
{
	hInstance = instance;
	
	registerWindowClass();
	createWindow();
	enterFullScreen();
	addControls();
	
	::SetFocus(window);
	return window;
}

void View::onConnect()
{
	::SendMessage(connectButton, WM_SETTEXT, 0, (LPARAM) "Disconnect");
}

void View::onDisconnect()
{
	::SendMessage(connectButton, WM_SETTEXT, 0, (LPARAM) "Connect");
}

void View::enableCOMControls()
{
	::EnableWindow(connectButton, true);
	::EnableWindow(comSettingsButton, true);
}

void View::disableCOMControls()
{
	::EnableWindow(connectButton, false);
	::EnableWindow(comSettingsButton, false);
}

void View::registerWindowClass()
{
	WNDCLASS wc = {0};
	
	wc.lpfnWndProc = windowProcedure;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.hInstance = hInstance;
	wc.lpszClassName = "MainWindowClass";
	wc.hCursor = ::LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW);
	
	::RegisterClass(&wc);
}

void View::createWindow()
{
	window = ::CreateWindowEx(WS_EX_APPWINDOW | WS_EX_WINDOWEDGE, 
							   "MainWindowClass",
							   "Data Recorder",
							   WS_MINIMIZEBOX | WS_VISIBLE,
							   0, 0, ::GetSystemMetrics(SM_CXVIRTUALSCREEN), ::GetSystemMetrics(SM_CYVIRTUALSCREEN),
							   NULL, NULL, hInstance, NULL);
}

void View::enterFullScreen()
{
	DWORD dwStyle = ::GetWindowLong(window, GWL_STYLE);
	DWORD dwRemove = WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX;
	DWORD dwNewStyle = dwStyle & ~dwRemove;
	::SetWindowLong(window, GWL_STYLE, dwNewStyle);
	::SetWindowPos(window, NULL, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE | SWP_FRAMECHANGED);
	
	HDC hDC = ::GetWindowDC(NULL); 
	::SetWindowPos(window, NULL, 0, 0, ::GetDeviceCaps(hDC, HORZRES), ::GetDeviceCaps(hDC, VERTRES), SWP_FRAMECHANGED);
}

void View::addControls()
{
	connectButton = ::CreateWindow("button", "Connect", WS_CHILD | WS_VISIBLE, 0, 0, 110, 50, window, (HMENU)connectButtonID, hInstance, NULL);
	comSettingsButton = ::CreateWindow("button", "Settings", WS_CHILD | WS_VISIBLE, 0, 50, 110, 50, window, (HMENU)comSettingsButtonID, hInstance, NULL);
	quitButton = ::CreateWindow("button", "Quit", WS_CHILD | WS_VISIBLE, 0, 100, 110, 50, window, (HMENU)quitButtonID, hInstance, NULL);
	
	::SendMessage(connectButton, WM_CHANGEUISTATE, (WPARAM)(0x10001), 0);
	::SendMessage(comSettingsButton, WM_CHANGEUISTATE, (WPARAM)(0x10001), 0);
	::SendMessage(quitButton, WM_CHANGEUISTATE, (WPARAM)(0x10001), 0);
}
