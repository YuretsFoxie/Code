#ifndef GDIVIEW_H_INCLUDED
#define GDIVIEW_H_INCLUDED

#include <windows.h>
#include <vector>
#include <string>

using namespace std;

class GDIView
{
public:	
	void onStart(HINSTANCE instance, HWND parent);
	void onStop();
	void resize();
	void paint();
	// void update(const vector<POINT>& shape);
	void print(const string& message);
	
private:
	void registerWindowClass();
	void createWindow();
	
	static DWORD WINAPI staticDraw(void* Param);
	DWORD drawIfNeeded();
	void draw();
	void prepareDrawing();
	void drawShape();
	
	//=====
	
	void drawText();
	
	//=====
	
	HINSTANCE hInstance;
	HWND parentWindow;
	HWND gdiWnd;
	RECT window;
	HDC hDC;
	HDC memDC;
	HBITMAP bitmap;
	HANDLE drawingThreadHandle;
	HPEN greenPen;
	
	//=====
	
	vector<string> consoleBuffer;
	
	//=====
	
	vector<POINT> buffer;
	bool isDrawing;
};

#endif // GDIVIEW_H_INCLUDED
