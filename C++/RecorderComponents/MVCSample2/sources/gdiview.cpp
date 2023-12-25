#include "gdiview.h"
#include "windowprocedure.h"

// Public Functions

void GDIView::onStart(HINSTANCE instance, HWND parent)
{	
	hInstance = instance;
	parentWindow = parent;
	
	registerWindowClass();
	createWindow();
	resize();
	
	isDrawing = true;
	greenPen = ::CreatePen(PS_SOLID, 1, RGB(0, 255, 0));
    drawingThreadHandle = ::CreateThread(NULL, 0, staticDraw, (void*) this, 0, NULL);
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
	return ((GDIView*) Param)->draw();
}

DWORD GDIView::draw()
{
	while(isDrawing)
	{
		draw(memDC, window);
		::RedrawWindow(gdiWnd, NULL, NULL, RDW_INVALIDATE);		
		::Sleep(100);
	}
}

void GDIView::draw(HDC& hdc, RECT& window)
{
	prepareDrawing(hdc, window);
	drawShape(hdc);
}

void GDIView::prepareDrawing(HDC& hdc, RECT& window)
{
	::FillRect(hdc, &window, reinterpret_cast<HBRUSH>(::GetStockObject(BLACK_BRUSH)));
	::SelectObject(hdc, greenPen);
	::SetBkColor(hdc, RGB(0, 0, 0));
	::SetTextColor(hdc, RGB(0, 255, 0));
}

void GDIView::drawShape(HDC& hdc)
{
	::Polyline(hdc, &buffer[0], buffer.size());
}
