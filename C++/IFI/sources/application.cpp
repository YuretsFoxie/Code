#include <thread>
#include <chrono>
#include "application.h"

// Public Functins

Application::Application(HINSTANCE hInstance, int nCmdShow, Settings& settings, COMPort* port, Graphics& graphics):
	settings(settings),
	port(port),
	graphics(graphics),
	isRunning(true)
{
	registerWindowClass(hInstance);
	createWindowInstance(hInstance, nCmdShow);
	setFullScreenMode();
	::SetCursor(::LoadCursor(NULL, IDC_ARROW));	
}

void Application::run()
{
	while (isRunning)
	{
		processMessages();
		graphics.render();
		std::this_thread::sleep_for(std::chrono::microseconds(10));
	}
}

// Private Functions

void Application::registerWindowClass(HINSTANCE hInstance)
{
	WNDCLASSEX wc = {sizeof(WNDCLASSEX), CS_OWNDC, wndProc, 0, 0, ::GetModuleHandle(NULL), NULL, NULL, NULL, NULL, "Main", NULL};
	::RegisterClassEx(&wc);
}

void Application::createWindowInstance(HINSTANCE hInstance, int nCmdShow)
{
	HWND hwnd = ::CreateWindowEx(
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

void Application::setFullScreenMode()
{
	DEVMODE dmSettings;
	memset(&dmSettings, 0, sizeof(dmSettings));
	::EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &dmSettings);
	
	dmSettings.dmPelsWidth = settings.getWindowWidth();
	dmSettings.dmPelsHeight	= settings.getWindowHeight();
	dmSettings.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT;
	
	::ChangeDisplaySettings(&dmSettings, CDS_FULLSCREEN);	
}

LRESULT CALLBACK Application::wndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (msg == WM_CLOSE)
		::PostQuitMessage(0);
		
	return DefWindowProc(hwnd, msg, wParam, lParam);
}

void Application::processMessages()
{
	MSG msg;
	while (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		if (msg.message == WM_QUIT)
		{
			isRunning = false;
		}
		
		if (msg.message == WM_KEYDOWN)
		{
			if (msg.wParam == VK_ESCAPE)	onPressESC();
			if (msg.wParam == VK_F1)		onPressF1();
		}
		
		::TranslateMessage(&msg);
		::DispatchMessage(&msg);
	}
}

void Application::onPressESC()
{
	isRunning = false;
}

void Application::onPressF1()
{
	port->toggleTransmission();
}
