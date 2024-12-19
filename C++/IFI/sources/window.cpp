#include <iostream>
#include "window.h"

// Public Functins

Window::Window(HINSTANCE hInstance, int nCmdShow)
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
			
			//=====
			
			std::cout << "test" << std::endl;
			
			//=====
			
			if (msg.wParam == VK_ESCAPE)
				isRunning = false;
				
			if (msg.wParam == VK_F1)
			{	
				isReceiving = !isReceiving;
				port.toggleDataTransmission(isReceiving);
			}
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
		1024,
		768, 
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
	
	dmSettings.dmPelsWidth = 1024;
	dmSettings.dmPelsHeight	= 768;
	dmSettings.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT;
	
	::ChangeDisplaySettings(&dmSettings, CDS_FULLSCREEN);	
}
	
LRESULT CALLBACK Window::wndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (msg == WM_CLOSE)
		::PostQuitMessage(0);
		
	return DefWindowProc(hwnd, msg, wParam, lParam);
}
