#ifndef APPLICATION_H_INCLUDED
#define APPLICATION_H_INCLUDED

#include "settings.h"
#include "comportadapter.h"
#include "graphics.h"
#include "databuffer.h"
#include "window.h"
#include "renderer.h"

// #include "textsubwindow.h"
// #include "graphsubwindow.h"

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
	COMPortAdapter portAdapter;
	Graphics graphics;
	DataBuffer buffer;
	Window window;
	Renderer renderer;
};

#endif // APPLICATION_H_INCLUDED
