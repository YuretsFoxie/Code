#ifndef APPLICATION_H_INCLUDED
#define APPLICATION_H_INCLUDED

#include "settings.h"
#include "comport.h"
#include "graphics.h"
#include "databuffer.h"
#include "window.h"

class Application 
{
public:
	Application(HINSTANCE hInstance, int nCmdShow, const Settings& settings);
	void run();
	
private:
	void runCOMPortThread();
	void runCOMPort();
	void runLoop();
	
	const Settings& settings;
	std::atomic<bool> isRunning;
	std::atomic<bool> isReceiving;
	HWND hwnd;
	COMPort port;
	Graphics graphics;
	DataBuffer buffer;
	Window window;
};

#endif // APPLICATION_H_INCLUDED
