#ifndef WINDOW_H_INCLUDED
#define WINDOW_H_INCLUDED

#include <windows.h>
#include <atomic>
#include "settings.h"
#include "comport.h"

class Window
{
public:
	Window(HINSTANCE hInstance, int nCmdShow, Settings& settings);
	
	void processMessages(std::atomic<bool>& isRunning, std::atomic<bool>& isReceiving, COMPort& port);
	HWND getHwnd() const;
	
private:
	void registerWindowClass(HINSTANCE hInstance);
	void createWindowInstance(HINSTANCE hInstance, int nCmdShow);
	void setFullScreenMode();
	static LRESULT CALLBACK wndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	
	void onPressESC(std::atomic<bool>& isRunning);
	void onPressF1(std::atomic<bool>& isReceiving, COMPort& port);
	
	Settings& settings;
	HWND hwnd;
};

#endif // WINDOW_H_INCLUDED
