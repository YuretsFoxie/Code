#include <iostream>
#include "window.h"
#include "console.h"

// Public Functins

Window::Window(HINSTANCE hInstance, int nCmdShow, Settings& settings): settings(settings)
{
	registerWindowClass(hInstance);
	createWindowInstance(hInstance, nCmdShow);
	setFullScreenMode();
	::SetCursor(::LoadCursor(NULL, IDC_ARROW));
}

void Window::processMessages(std::atomic<bool>& isRunning, std::atomic<bool>& isReceiving, COMPort& port)
{
	MSG msg;
	while (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		if (msg.message == WM_QUIT)
			isRunning = false;
			
		if (msg.message == WM_KEYDOWN)
		{
			if (msg.wParam == VK_ESCAPE)	onPressESC(isRunning);
			if (msg.wParam == VK_F1)		onPressF1(isReceiving, port);
			if (msg.wParam == VK_F2)		onPressF2();
		}
		
		::TranslateMessage(&msg);
		::DispatchMessage(&msg);
	}
}

HWND Window::getHwnd() const
{
	return hwnd;
}

// Private Functions

void Window::registerWindowClass(HINSTANCE hInstance)
{
	WNDCLASSEX wc = {sizeof(WNDCLASSEX), CS_OWNDC, wndProc, 0, 0, ::GetModuleHandle(NULL), NULL, NULL, NULL, NULL, "Main", NULL};
	::RegisterClassEx(&wc);
}

void Window::createWindowInstance(HINSTANCE hInstance, int nCmdShow)
{
	hwnd = ::CreateWindowEx(
		0, 
		"Main", 
		"", 
		WS_POPUP | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, 
		0, 
		0, 
		settings.getWindowWidth(), 
		settings.getWindowHeight(), 
		NULL, 
		NULL, 
		hInstance, 
		NULL
	);
	
	::ShowWindow(hwnd, nCmdShow);
}

void Window::setFullScreenMode()
{
	DEVMODE dmSettings;
	memset(&dmSettings, 0, sizeof(dmSettings));
	::EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &dmSettings);
	
	dmSettings.dmPelsWidth = settings.getWindowWidth();
	dmSettings.dmPelsHeight	= settings.getWindowHeight();
	dmSettings.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT;
	
	::ChangeDisplaySettings(&dmSettings, CDS_FULLSCREEN);	
}
	
LRESULT CALLBACK Window::wndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (msg == WM_CLOSE)
		::PostQuitMessage(0);
		
	return DefWindowProc(hwnd, msg, wParam, lParam);
}

void Window::onPressESC(std::atomic<bool>& isRunning)
{
	isRunning = false;
}

void Window::onPressF1(std::atomic<bool>& isReceiving, COMPort& port)
{
	// TODO: Perform the correct fix (this print works as a temporary fix). Try playing the sound instead.
	std::cout << "F1 is pressed" << std::endl;
	
	isReceiving = !isReceiving;
	port.toggleDataTransmission(isReceiving);
}

void Window::onPressF2()
{
	Console::shared().print("test");
}
