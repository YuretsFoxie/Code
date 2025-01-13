#ifndef APPLICATION_H_INCLUDED
#define APPLICATION_H_INCLUDED

#include <windows.h>
#include <atomic>
#include "settings.h"
#include "comport.h"
#include "graphics.h"
#include "sounds.h"

class Application 
{
public:
	Application(HINSTANCE hInstance, int nCmdShow, Settings& settings, COMPort* port, Graphics& graphics, Sounds& sounds);
	void run();
	
private:
	void registerWindowClass(HINSTANCE hInstance);
	void createWindowInstance(HINSTANCE hInstance, int nCmdShow);
	void setFullScreenMode();
	
	static LRESULT CALLBACK wndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	void processMessages();
	void onPressESC();
	void onPressF1();
	
	COMPort* port;
	Settings& settings;
	Graphics& graphics;
	Sounds& sounds;
	
	std::atomic<bool> isRunning;
};

#endif // APPLICATION_H_INCLUDED
