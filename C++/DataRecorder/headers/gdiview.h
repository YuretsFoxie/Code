#ifndef GDIVIEW_H_INCLUDED
#define GDIVIEW_H_INCLUDED

#include <windows.h>
#include <string>
#include "graph.h"

using namespace std;

class GDIView
{
public:	
	void onStart(HINSTANCE instance, HWND parent);
	void onStop();
	void paint();
	void print(const string& message);
	
private:
	void registerWindowClass();
	void createWindow();
	void setup();
	void addGraphs();
	RECT findQuadrantRect(const int quadrant);
	
	static DWORD WINAPI staticDraw(void* Param);
	DWORD drawIfNeeded();
	void prepareDrawing();
	void draw();
	
	HINSTANCE hInstance;
	HWND parentWindow;
	HWND gdiWnd;
	RECT window;
	HDC hDC;
	HDC memDC;
	HBITMAP bitmap;
	HANDLE drawingThreadHandle;
	HPEN greenPen;
	
	vector<GraphInterface*> graphs;
	bool isDrawing;
};

#endif // GDIVIEW_H_INCLUDED
