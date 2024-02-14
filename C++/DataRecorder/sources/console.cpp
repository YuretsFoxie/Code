#include <windows.h>
#include "console.h"

Console& Console::shared()
{
	static Console c;
	return c;
}

void Console::setFrame()
{
	int width = ::GetSystemMetrics(SM_CXVIRTUALSCREEN);
	int height = ::GetSystemMetrics(SM_CYVIRTUALSCREEN);
	
	HWND hwnd = ::GetConsoleWindow();
	::MoveWindow(hwnd, 0, height * 0.8 - 31, width, height * 0.2, TRUE);
}
