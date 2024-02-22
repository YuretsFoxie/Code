#include <iostream>
#include "gdiview.h"
#include "windowprocedure.h"
#include "frame.h"

// Public Functions

void GDIView::onStart(HINSTANCE instance, HWND parent)
{	
	hInstance = instance;
	parentWindow = parent;
	
	registerWindowClass();
	createWindow();
	setup();
	addGraphs();
}

void GDIView::onStop()
{
	isDrawing = false;
	::TerminateThread(drawingThreadHandle, 0);
	::CloseHandle(drawingThreadHandle);
	::DeleteObject(greenPen);
	::DeleteObject(bitmap);
	::DeleteDC(memDC);
	::ReleaseDC(gdiWnd, hDC);
}

void GDIView::paint()
{
	PAINTSTRUCT ps;
	hDC = ::BeginPaint(gdiWnd, &ps);
	::BitBlt(hDC, 0, 0, window.right, window.bottom, memDC, 0, 0, SRCCOPY);
	::EndPaint(gdiWnd, &ps);
}

void GDIView::addValue(const int value)
{
	for (auto graph: graphs)
		graph->addValue(value);
}

// Private Functions

void GDIView::registerWindowClass()
{
	WNDCLASS wc = {0};
	wc.lpfnWndProc = windowProcedure;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.hInstance = hInstance;
	wc.lpszClassName = "GDIWindowClass";
	wc.hCursor = ::LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_APPWORKSPACE);
	::RegisterClass(&wc);
}

void GDIView::createWindow()
{
	RECT client;
	::GetClientRect(parentWindow, &client);
	
	gdiWnd = ::CreateWindowEx(WS_EX_APPWINDOW | WS_EX_WINDOWEDGE,
							  "GDIWindowClass",
							  "",
							  WS_MINIMIZEBOX | WS_VISIBLE | WS_CHILD,
							  110, 0, client.right - 110, client.bottom,
							  parentWindow, NULL, hInstance, NULL);
}

void GDIView::setup()
{
	isDrawing = true;
	greenPen = ::CreatePen(PS_SOLID, 1, RGB(0, 255, 0));
    drawingThreadHandle = ::CreateThread(NULL, 0, staticDraw, (void*) this, 0, NULL);
	hDC = ::GetDC(gdiWnd);
	memDC = ::CreateCompatibleDC(hDC);
	
	::GetClientRect(gdiWnd, &window);
	bitmap = ::CreateCompatibleBitmap(hDC, window.right, window.bottom);
	::SelectObject(memDC, bitmap);
}

void GDIView::addGraphs()
{
	/*
	graphs.push_back(new RawDataGraph(memDC, FirstQuadrant(window)));
	graphs.push_back(new RawDataGraph(memDC, SecondQuadrant(window)));
	graphs.push_back(new RawDataGraph(memDC, ThirdQuadrant(window)));
	graphs.push_back(new RawDataGraph(memDC, FourthQuadrant(window)));
	*/
	
	graphs.push_back(new RawDataGraph(memDC, AllQuadrants(window)));
}

DWORD WINAPI GDIView::staticDraw(void* Param)
{
	return ((GDIView*) Param)->drawIfNeeded();
}

DWORD GDIView::drawIfNeeded()
{
	while(isDrawing)
	{
		prepareDrawing();
		draw();
		::RedrawWindow(gdiWnd, NULL, NULL, RDW_INVALIDATE);		
		::Sleep(100);
	}
	
	return 0;
}

void GDIView::prepareDrawing()
{
	::FillRect(memDC, &window, reinterpret_cast<HBRUSH>(::GetStockObject(BLACK_BRUSH)));
	::SelectObject(memDC, greenPen);
	::SetBkColor(memDC, RGB(0, 0, 0));
	::SetTextColor(memDC, RGB(0, 255, 0));
}

void GDIView::draw()
{
	for (auto graph: graphs)
		graph->render();
}
