#ifndef WINDOW_H_INCLUDED
#define WINDOW_H_INCLUDED

#include <windows.h>
#include <atomic>
#include "comportadapter.h"

class Window
{
public:
	Window(HINSTANCE hInstance, int nCmdShow);
	
	void processMessages(std::atomic<bool>& isRunning, COMPortAdapter& port, std::atomic<bool>& isReceiving);
	HWND getHwnd() const;
	
private:
	void registerWindowClass(HINSTANCE hInstance);
	HWND createWindowInstance(HINSTANCE hInstance, int nCmdShow);
	void setFullScreenMode();
	static LRESULT CALLBACK wndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	
	HWND hwnd;
};

#endif // WINDOW_H_INCLUDED
