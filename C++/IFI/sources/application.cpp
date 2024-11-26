#include <thread>
#include "application.h"

// Public Functins

Application::Application(HINSTANCE hInstance, int nCmdShow, const Settings& settings):
	isRunning(true),
	isReceiving(false),
	hwnd(NULL), 
	settings(settings),
	window(hInstance, nCmdShow), 
	graphics(),
	buffer(settings.getMaxPoints(),
	settings.getScaleFactor()), 
	renderer(graphics, buffer, settings.getBatchSize())
{
	hwnd = window.getHwnd();
	graphics.initialize(hwnd, settings);
}

void Application::run()
{
	portAdapter.setup(settings.getSerialPort(), settings.getBaudRate());
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
		::ReadFile(portAdapter.getHandle(), array, 1, &bytesRead, NULL);
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
		window.processMessages(isRunning, portAdapter, isReceiving);
		renderer.renderFrame(hdc, updateCounter, isRunning);
	}
	
	::ReleaseDC(hwnd, hdc);
}
