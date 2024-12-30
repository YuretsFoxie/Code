#ifndef APPLICATION_H_INCLUDED
#define APPLICATION_H_INCLUDED

#include "window.h"
#include "settings.h"
#include "databuffer.h"
#include "comport.h"
#include "graphics.h"

class Application 
{
public:
	Application(HINSTANCE hInstance, int nCmdShow, Settings settings);
	void run();
	
private:
	void runCOMPortThread();
	void runCOMPort();
	void runLoop();
	
	Settings settings;
	Window window;
	DataBuffer buffer;
	COMPort port;
	Graphics graphics;
	
	std::atomic<bool> isRunning;
	std::atomic<bool> isReceiving;
	HWND hwnd;
};

#endif // APPLICATION_H_INCLUDED
