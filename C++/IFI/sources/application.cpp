#include <thread>
#include <chrono>
#include "application.h"

// Public Functins

Application::Application(HINSTANCE hInstance, int nCmdShow, const Settings& settings):
	isRunning(true),
	isReceiving(false),
	hwnd(NULL), 
	settings(settings),
	window(hInstance, nCmdShow), 
	buffer(settings.getMaxPoints(),
	settings.getScaleFactor()),
	graphics(buffer)
{
	hwnd = window.getHwnd();
	graphics.initialize(hwnd, settings);
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
	if (isReceiving)
	{
		::ReadFile(port.getHandle(), array, 1, &bytesRead, NULL);
		if (bytesRead > 0)
			buffer.push(static_cast<float>(array[0]));
	}
}

void Application::runLoop()
{
	HDC hdc = ::GetDC(hwnd);
	int updateCounter = 0;
	
	while (isRunning)
	{
		window.processMessages(isRunning, port, isReceiving);
		graphics.render(hdc, updateCounter, isRunning);
		std::this_thread::sleep_for(std::chrono::microseconds(10));
	}
	
	::ReleaseDC(hwnd, hdc);
}
