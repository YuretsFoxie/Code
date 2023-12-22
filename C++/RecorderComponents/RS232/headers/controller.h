#ifndef CONTROLLER_H_INCLUDED
#define CONTROLLER_H_INCLUDED

#include <windows.h>
#include <string>

using namespace std;

class Controller
{
public:
    static Controller& shared()
    {
        static Controller app;
        return app;
    }
	
    WPARAM run(HINSTANCE instance);
	void showMessage(const string& message);
	void handleReceivedData(const int data);
	void selectPort(const string& port);
	void disconnect();
	
private:
    Controller() {}
	
    // Message Handlers
	
    static LRESULT CALLBACK wndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	
	static void onClick(WPARAM wParam);
	static void onClose();
	static void onConnect();
	
	// Life Cycle
	
	void onStart(HINSTANCE instance);
	void onStop();
    void registerWindowClass();
	void runMainLoop();
	
	// Variables
	
	HINSTANCE hInstance;
	MSG msg;
};

#endif // CONTROLLER_H_INCLUDED
