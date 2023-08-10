#ifndef APPLICATION_H_INCLUDED
#define APPLICATION_H_INCLUDED

#define WIN32_LEAN_AND_MEAN 1
#define WIN32_EXTRA_LEAN 1

#define ID_CONNECT_BUTTON	100
#define ID_SEND_BUTTON		101
#define ID_CLEAR_BUTTON		102
#define ID_EDIT		    	200

#include <windows.h>
#include <string>
#include <vector>

using namespace std;

class Application
{
public:
    static Application& shared()
    {
        static Application app;
        return app;
    }
	
    WPARAM run(HINSTANCE instance);
	
private:
	
    // Messages Handler
	
    static LRESULT CALLBACK wndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	
    // Actions
	
	static void onConnectButtonClick();
    static void onSendButtonClick();
    static void onClearButtonClick();
    static void onButtonPressed(WPARAM wParam);
	
    // Private Methods
	
    Application() {}
	
	static void showText();
    static void showText(const LPARAM lParam, const WPARAM wParam);
	static int  getInputIndex();
	
    void registerWindowClass();
    void createWindow();
    void runMainLoop();
    void destroyWindow();
	
    // Variables
	
    HINSTANCE hInstance;
    MSG msg;
	
	static string text;
	static int counter;
	
    static HWND hWnd;
    static HWND edit;
	static HWND connectButton;
    static HWND sendButton;
    static HWND clearButton;
};

#endif // APPLICATION_H_INCLUDED
