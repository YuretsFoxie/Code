#ifndef APPLICATION_H_INCLUDED
#define APPLICATION_H_INCLUDED

#include <windows.h>
#include <string>

class Application
{
public:
    static Application& shared()
    {
        static Application instance;
        return instance;
    }
	
    WPARAM run(HINSTANCE instance);
	void onReceived(const int value);
	void showText(const std::string& str);
	
private:
    Application();
   ~Application();
	
    static LRESULT CALLBACK wndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	
	void registerWindowClass();
	void createWindow();
	void setupConsole();
	void runMainLoop();
	void printHint();
	
	HINSTANCE hInstance;
	HWND hWnd;
	MSG msg;
	
	int receivedValue = 0;
	bool isNewValueReceived = false;
};

#endif // APPLICATION_H_INCLUDED
