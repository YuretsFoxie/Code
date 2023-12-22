#include <windows.h>
#include <iostream>
#include <vector>
#include <string>

using namespace std;

HINSTANCE hInstance;
HANDLE drawingThreadHandle;
MSG msg;
HPEN greenPen;
bool isDrawing;
	
HWND hWindow;
HWND gdiWnd;
HDC hDC;
HDC memDC;
HBITMAP bitmap;
RECT window;



WPARAM run(HINSTANCE instance);
LRESULT CALLBACK wndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK gdiProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
void onClick(WPARAM wParam);
void onResize();
void onPaint();
void onClose();
void onStart(HINSTANCE instance);
void onStop();
void registerWindowClass();
void registerGDIClass();
void createWindows();
void createSettingsWindow();
void createGraphicWindow();
void addControls();
void runMainLoop();
DWORD WINAPI staticDraw(void* Param);
DWORD draw();
void draw(HDC& hdc, RECT& window);
void prepareDrawing(HDC& hdc, RECT& window);
void drawShape(HDC& hdc);



WPARAM run(HINSTANCE instance)
{
	onStart(instance);
	registerWindowClass();
	registerGDIClass();
    createWindows();
    runMainLoop();
	onStop();
	
    return msg.wParam;
}

// Messages Handler

LRESULT CALLBACK wndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    if (message == WM_COMMAND)	onClick(wParam);
	if (message == WM_CLOSE)	onClose();
	
    return ::DefWindowProc(hWnd, message, wParam, lParam);
}

LRESULT CALLBACK gdiProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == WM_SIZE)		onResize();
	if (message == WM_PAINT)	onPaint();
	
	return ::DefWindowProc(hWnd, message, wParam, lParam);
}

void onClick(WPARAM wParam)
{
	if (wParam == 100)	::PostQuitMessage(0);
}

void onResize()
{
	hDC = ::GetDC(gdiWnd);
	memDC = ::CreateCompatibleDC(hDC);
	::GetClientRect(gdiWnd, &window);
	bitmap = ::CreateCompatibleBitmap(hDC, window.right, window.bottom);
	::SelectObject(memDC, bitmap);
}

void onPaint()
{
	PAINTSTRUCT ps;
	hDC = ::BeginPaint(gdiWnd, &ps);
	::BitBlt(hDC, 0, 0, window.right, window.bottom, memDC, 0, 0, SRCCOPY);
	::EndPaint(gdiWnd, &ps);
}

void onClose()
{
	::DeleteObject(bitmap);
	::DeleteDC(memDC);
	::ReleaseDC(gdiWnd, hDC);
	::DestroyWindow(gdiWnd);
	::PostQuitMessage(0);
}

// Life Cycle

void onStart(HINSTANCE instance)
{
	hInstance = instance;
	isDrawing = true;
    // drawingThreadHandle = ::CreateThread(NULL, 0, staticDraw, (void*) this, 0, NULL);
	drawingThreadHandle = ::CreateThread(NULL, 0, staticDraw, NULL, 0, NULL);
	greenPen = ::CreatePen(PS_SOLID, 1, RGB(0, 255, 0));
}

void onStop()
{
	isDrawing = false;
	::TerminateThread(drawingThreadHandle, 0);
	::CloseHandle(drawingThreadHandle);
	::DeleteObject(greenPen);
}

void registerWindowClass()
{
    WNDCLASS wc = {0};
	wc.lpfnWndProc = wndProc;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.hInstance = hInstance;
	wc.lpszClassName = "ClassName";
	wc.hCursor = ::LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_APPWORKSPACE);
	::RegisterClass(&wc);
}

void registerGDIClass()
{
	WNDCLASS wc = {0};
	wc.lpfnWndProc = gdiProc;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.hInstance = hInstance;
	wc.lpszClassName = "ClassName2";
	wc.hCursor = ::LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_APPWORKSPACE);
	::RegisterClass(&wc);
}

void createWindows()
{
	createSettingsWindow();
	createGraphicWindow();
	addControls();
	onResize();
	::SetFocus(hWindow);
}

void createSettingsWindow()
{
	hWindow = ::CreateWindowEx(WS_EX_APPWINDOW | WS_EX_WINDOWEDGE,
							   "ClassName",
							   "Data Recorder",
							   WS_MINIMIZEBOX | WS_VISIBLE,
							   0, 0, 240, 375,
							   NULL, NULL, hInstance, NULL);
}

void createGraphicWindow()
{	
	gdiWnd = ::CreateWindowEx(WS_EX_APPWINDOW | WS_EX_WINDOWEDGE,
							  "ClassName2",
							  "",
							  WS_MINIMIZEBOX | WS_VISIBLE,
							  240, 0, ::GetSystemMetrics(SM_CXSCREEN) - 240, ::GetSystemMetrics(SM_CYSCREEN) - 38,
							  hWindow, NULL, NULL, NULL);
}

void addControls()
{
	::CreateWindow("button", "Quit", WS_CHILD | WS_VISIBLE, 30, 300, 70, 30, hWindow, (HMENU)100, hInstance, NULL);
}

void runMainLoop()
{
    while(true)
        if (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
                break;
				
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
        }
}

// Drawing

DWORD WINAPI staticDraw(void* Param)
{
	// return ((Application*) Param)->draw();
	return draw();
}

DWORD draw()
{
	while(isDrawing)
	{
		draw(memDC, window);
		::RedrawWindow(gdiWnd, NULL, NULL, RDW_INVALIDATE);		
		::Sleep(1000);
	}
}

void draw(HDC& hdc, RECT& window)
{
	prepareDrawing(hdc, window);
	drawShape(hdc);
}

void prepareDrawing(HDC& hdc, RECT& window)
{
	::FillRect(hdc, &window, reinterpret_cast<HBRUSH>(::GetStockObject(BLACK_BRUSH)));
	::SelectObject(hdc, greenPen);
	::SetBkColor(hdc, RGB(0, 0, 0));
	::SetTextColor(hdc, RGB(0, 255, 0));
}

void drawShape(HDC& hdc)
{
	vector<POINT> buffer = {{100, 100}, {200, 200}, {200, 100}, {100, 100}};
	::Polyline(hdc, &buffer[0], buffer.size());
}



int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int iCmdShow)
{
    return run(hInstance);
}
