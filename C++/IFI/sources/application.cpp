#include <thread>
#include <chrono>
#include "application.h"

// Public Functins

Application::Application(HINSTANCE hInstance, int nCmdShow):
	window(hInstance, nCmdShow),
	settings(Settings("settings.ini")),
	buffer(settings.getMaxPoints()),
	graphics(buffer, settings),
	isRunning(true),
	isReceiving(false),
	hwnd(NULL)
{
	hwnd = window.getHwnd();
	graphics.set(hwnd);
}

void Application::run()
{
	port.setup(settings.getSerialPort(), settings.getBaudRate());
	runCOMPortThread();
	runLoop();
}

// Private Functions

void Application::runCOMPortThread() 
{
	std::thread portThread(&Application::runCOMPort, this);
	portThread.detach();
}

void Application::runCOMPort() 
{
	char array[1];
	DWORD bytesRead;
	
	while (isRunning)
	{
		if (isReceiving)
		{
			::ReadFile(port.getHandle(), array, 1, &bytesRead, NULL);
			if (bytesRead > 0)
				buffer.push(array[0]);
		}
		
		std::this_thread::sleep_for(std::chrono::microseconds(10));
	}
}

void Application::runLoop()
{
	HDC hdc = ::GetDC(hwnd);
	
	while (isRunning)
	{
		window.processMessages(isRunning, isReceiving, port);
		graphics.render();
		std::this_thread::sleep_for(std::chrono::microseconds(10));
	}
	
	::ReleaseDC(hwnd, hdc);
}
