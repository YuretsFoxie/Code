#include "application.h"
#include "comport.h"
#include "graphics.h"
#include "generator.h"
#include <iostream>

// Public Functions

WPARAM Application::run(HINSTANCE instance)
{
	hInstance = instance;
	
	registerWindowClass();
	createWindow();
	setupConsole();
	printHint();
	Graphics::shared().setup(hWnd);
	
	runMainLoop();
    return msg.wParam;
}

void Application::onReceived(const int value)
{
	receivedValue = value;
	isValueReceived = true;
}

void Application::onFFTCalculated(const std::vector<float>& data)
{
	fftResult = data;
	isFFTResultReceived = true;
}

void Application::showText(const std::string& str)
{
	std::cout << str << "\n";
}

// Message Handler

LRESULT CALLBACK Application::wndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    if (message == WM_KEYDOWN)
	{
		if (wParam == VK_ESCAPE)	::PostQuitMessage(0);
		if (wParam == VK_F1)		COMPort::shared().toggleReceiving();
		if (wParam == VK_F2)		Generator::shared().toggle();
	}
	
    return ::DefWindowProc(hWnd, message, wParam, lParam);
}

// Private Functions

Application::Application() {}

Application::~Application()
{
	::DestroyWindow(hWnd);
}

void Application::registerWindowClass()
{
    WNDCLASS wc = {0};
	wc.lpfnWndProc = wndProc;
	wc.style = CS_OWNDC; // CS_HREDRAW | CS_VREDRAW;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = ::LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = reinterpret_cast<HBRUSH>(::GetStockObject(BLACK_BRUSH));
	wc.lpszMenuName = NULL;
	wc.lpszClassName = "ClassName";
	::RegisterClass(&wc);
}

void Application::createWindow()
{	
	hWnd = ::CreateWindowEx(WS_EX_APPWINDOW | WS_EX_WINDOWEDGE,
							"ClassName",
							"Foxie Window",
							WS_MINIMIZEBOX | WS_VISIBLE,
							-4, 0, ::GetSystemMetrics(SM_CXSCREEN) + 8, ::GetSystemMetrics(SM_CYSCREEN) * 0.8,
							NULL, NULL, hInstance, NULL);
}

void Application::setupConsole()
{
	::MoveWindow(::GetConsoleWindow(), 
				 -8, 
				 ::GetSystemMetrics(SM_CYSCREEN) * 0.8 - 34, 
				 ::GetSystemMetrics(SM_CXSCREEN) + 16, 
				 ::GetSystemMetrics(SM_CYSCREEN) * 0.2 + 4,
				 TRUE);
}

void Application::runMainLoop()
{
	while(true)
	{
		if (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				break;
			
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}
		
		if (isValueReceived)
		{
			Graphics::shared().update(receivedValue);
			isValueReceived = false;
		}
		
		if (isFFTResultReceived)
		{
			Graphics::shared().updateWithFFT(fftResult);
			isFFTResultReceived = false;
		}
	}
}

void Application::printHint()
{
	showText("F1  - start/stop data receiving\nF2  - start/stop generator\nEsc - quit");
}
