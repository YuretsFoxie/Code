#ifndef APPLICATION_H_INCLUDED
#define APPLICATION_H_INCLUDED

#include "settings.h"
#include "window.h"
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
	
	Window window;
	DataBuffer buffer;
	COMPort port;
	Graphics graphics;
	
	std::atomic<bool> isRunning;
	std::atomic<bool> isReceiving;
	HWND hwnd;
};

#endif // APPLICATION_H_INCLUDED
