#ifndef GRAPHICSVIEW_H_INCLUDED
#define GRAPHICSVIEW_H_INCLUDED

#include <windows.h>
#include <vector>
#include "graphinterface.h"

using namespace std;

class GraphicsView
{
public:	
	void onStart(HINSTANCE instance, HWND parent);
	void onStop();
	void paint();
	void addValue(const int value);
	void print(const int value);
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

#endif // GRAPHICSVIEW_H_INCLUDED
