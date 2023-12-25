#ifndef GDIVIEW_H_INCLUDED
#define GDIVIEW_H_INCLUDED

#include <windows.h>
#include <vector>

using namespace std;

class GDIView
{
public:
	void onStart(HINSTANCE instance, HWND parent);
	void onStop();
	void resize();
	void paint();
	void update(const vector<POINT>& shape);
	
private:
	void registerWindowClass();
	void createWindow();
	
	static DWORD WINAPI staticDraw(void* Param);
	DWORD draw();
	void draw(HDC& hdc, RECT& window);
	void prepareDrawing(HDC& hdc, RECT& window);
	void drawShape(HDC& hdc);
	
	HINSTANCE hInstance;
	HWND parentWindow;
	HWND gdiWnd;
	RECT window;
	HDC hDC;
	HDC memDC;
	HBITMAP bitmap;
	HANDLE drawingThreadHandle;
	HPEN greenPen;
	
	vector<POINT> buffer;
	bool isDrawing;
};

#endif // GDIVIEW_H_INCLUDED
