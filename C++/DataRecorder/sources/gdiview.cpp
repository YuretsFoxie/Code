#include "gdiview.h"
#include "windowprocedure.h"

// Public Functions

void GDIView::onStart(HINSTANCE instance, HWND parent)
{	
	hInstance = instance;
	parentWindow = parent;
	isDrawing = true;
	greenPen = ::CreatePen(PS_SOLID, 1, RGB(0, 255, 0));
    drawingThreadHandle = ::CreateThread(NULL, 0, staticDraw, (void*) this, 0, NULL);
	
	registerWindowClass();
	createWindow();
	resize();
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

void GDIView::resize()
{
	hDC = ::GetDC(gdiWnd);
	memDC = ::CreateCompatibleDC(hDC);
	::GetClientRect(gdiWnd, &window);
	bitmap = ::CreateCompatibleBitmap(hDC, window.right, window.bottom);
	::SelectObject(memDC, bitmap);
}

void GDIView::paint()
{
	PAINTSTRUCT ps;
	hDC = ::BeginPaint(gdiWnd, &ps);
	::BitBlt(hDC, 0, 0, window.right, window.bottom, memDC, 0, 0, SRCCOPY);
	::EndPaint(gdiWnd, &ps);
}

void GDIView::update(const vector<POINT>& shape)
{
	buffer = shape;
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

DWORD WINAPI GDIView::staticDraw(void* Param)
{
	return ((GDIView*) Param)->drawIfNeeded();
}

DWORD GDIView::drawIfNeeded()
{
	while(isDrawing)
	{
		draw();
		::RedrawWindow(gdiWnd, NULL, NULL, RDW_INVALIDATE);		
		::Sleep(100);
	}
}

void GDIView::draw()
{
	prepareDrawing();
	drawShape();
}

void GDIView::prepareDrawing()
{
	::FillRect(memDC, &window, reinterpret_cast<HBRUSH>(::GetStockObject(BLACK_BRUSH)));
	::SelectObject(memDC, greenPen);
	::SetBkColor(memDC, RGB(0, 0, 0));
	::SetTextColor(memDC, RGB(0, 255, 0));
}

void GDIView::drawShape()
{
	::Polyline(memDC, &buffer[0], buffer.size());
}
