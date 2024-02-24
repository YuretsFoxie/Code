#include "graphicsview.h"
#include "windowprocedure.h"
#include "rawdatagraph.h"
#include "console.h"

// Public Functions

void GraphicsView::onStart(HINSTANCE instance, HWND parent)
{	
	hInstance = instance;
	parentWindow = parent;
	
	registerWindowClass();
	createWindow();
	setup();
	addGraphs();
}

void GraphicsView::onStop()
{
	isDrawing = false;
	::TerminateThread(drawingThreadHandle, 0);
	::CloseHandle(drawingThreadHandle);
	::DeleteObject(greenPen);
	::DeleteObject(bitmap);
	::DeleteDC(memDC);
	::ReleaseDC(gdiWnd, hDC);
}

void GraphicsView::paint()
{
	PAINTSTRUCT ps;
	hDC = ::BeginPaint(gdiWnd, &ps);
	::BitBlt(hDC, 0, 0, window.right, window.bottom, memDC, 0, 0, SRCCOPY);
	::EndPaint(gdiWnd, &ps);
}

void GraphicsView::addValue(const int value)
{
	for (auto graph: graphs)
		graph->addValue(value);
}

void GraphicsView::print(const int value)
{
	for (auto graph: graphs)
		graph->print(value);
}

void GraphicsView::print(const string& message)
{
	for (auto graph: graphs)
		graph->print(message);
}

// Private Functions

void GraphicsView::registerWindowClass()
{
	WNDCLASS wc = {0};
	wc.lpfnWndProc = windowProcedure;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.hInstance = hInstance;
	wc.lpszClassName = "GDIWindowClass";
	wc.hCursor = ::LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = NULL; // reinterpret_cast<HBRUSH>(COLOR_APPWORKSPACE);
	::RegisterClass(&wc);
}

void GraphicsView::createWindow()
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

void GraphicsView::setup()
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

void GraphicsView::addGraphs()
{
	/*
	graphs.push_back(new RawDataGraph(memDC, FirstQuadrant(window)));
	graphs.push_back(new RawDataGraph(memDC, SecondQuadrant(window)));
	graphs.push_back(new RawDataGraph(memDC, ThirdQuadrant(window)));
	graphs.push_back(new RawDataGraph(memDC, FourthQuadrant(window)));
	*/
	
	graphs.push_back(new Console(memDC, FirstQuadrant(window)));
	graphs.push_back(new RawDataGraph(memDC, SecondQuadrant(window)));
	
	// graphs.push_back(new RawDataGraph(memDC, AllQuadrants(window)));
}

DWORD WINAPI GraphicsView::staticDraw(void* Param)
{
	return ((GraphicsView*) Param)->drawIfNeeded();
}

DWORD GraphicsView::drawIfNeeded()
{
	while(isDrawing)
	{
		prepareDrawing();
		draw();
		::RedrawWindow(gdiWnd, NULL, NULL, RDW_INVALIDATE);
		::Sleep(10);
	}
	
	return 0;
}

void GraphicsView::prepareDrawing()
{
	::FillRect(memDC, &window, reinterpret_cast<HBRUSH>(::GetStockObject(BLACK_BRUSH)));
	::SelectObject(memDC, greenPen);
	::SetBkColor(memDC, RGB(0, 0, 0));
	::SetTextColor(memDC, RGB(0, 255, 0));
}

void GraphicsView::draw()
{
	for (auto graph: graphs)
		graph->render();
}
