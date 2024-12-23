#ifndef APPLICATION_H_INCLUDED
#define APPLICATION_H_INCLUDED

#include <windows.h>
#include <vector>
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
	void onFFTCalculated(const std::vector<float>& data);
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
	
	void onPressESC();
	void onPressF1();
	void onPressF2();
	void onPressF3();
	
	HINSTANCE hInstance;
	HWND hWnd;
	MSG msg;
	
	float receivedValue = 0;
	std::vector<float> fftResult = {0};	
	bool shouldUpdateGraphics = false;
	
	int testCount = 0;
};

#endif // APPLICATION_H_INCLUDED
