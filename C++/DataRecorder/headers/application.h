#ifndef APPLICATION_H_INCLUDED
#define APPLICATION_H_INCLUDED

#include <windows.h>

#define TIME_BUTTON		100
#define SPEED_BUTTON	101

#define START_BUTTON	110
#define STOP_BUTTON		111

#define OPEN_BUTTON		120
#define QUIT_BUTTON		121

#define TIME_TEXT		200
#define SPEED_TEXT		201

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
    HINSTANCE hInstance;
    HANDLE drawingThreadHandle;
	MSG msg;
	bool isDrawing;
	
	static HWND hWindow;
	static HWND gdiWnd;
	static HDC hDC;
	static HDC memDC;
	static HBITMAP bitmap;
	static RECT window;
	static HANDLE dialogThreadHandle;
	static OPENFILENAME file;
	
	// Private Functions
	
    Application() {}
	
    // Messages Handler
	
    static LRESULT CALLBACK wndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK gdiProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	
	static void onUART(LPARAM lParam);
	static void onClick(WPARAM wParam);
	static void onButtonStart();
	static void onButtonStop();
	static void onButtonOpen();
	static void onWheel(WPARAM wParam);
	static void onResize();
	static void onPaint();
	static void onClose();
	
	// Life Cycle
	
	void onStart(HINSTANCE instance);
	void onStop();
    void registerWindowClass();
	void registerGDIClass();
    void createWindows();
	void createSettingsWindow();
	void createGraphicWindow();
	void addControls();
	void runMainLoop();
	
	// Actions
	
	static void updateSettingsWindow();
	
	// Dialog
	
	static DWORD WINAPI staticOpenDialog(void* Param);
    DWORD openDialog();
	
	// Drawing
	
	static DWORD WINAPI staticDraw(void* Param);
    DWORD draw();
};

#endif // APPLICATION_H_INCLUDED
