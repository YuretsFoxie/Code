#include "application.h"
#include "settingsModel.h"
#include "dataModel.h"
#include "uart.h"
#include "renderer.h"

#include <iostream>
#include <string>

HWND			Application::hWindow;
HWND 			Application::gdiWnd;
HDC				Application::hDC;
HDC				Application::memDC;
HBITMAP			Application::bitmap;
RECT			Application::window;
HANDLE 			Application::dialogThreadHandle;
OPENFILENAME	Application::file;

// Public Functions

WPARAM Application::run(HINSTANCE instance)
{
	onStart(instance);
	registerWindowClass();
	registerGDIClass();
    createWindows();
	UART::shared().init(hWindow);
    runMainLoop();
	onStop();
	
    return msg.wParam;
}

// Messages Handler

LRESULT CALLBACK Application::wndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    if (message == WM_COMMAND)		onClick(wParam);
	if (message == WM_MOUSEWHEEL)	onWheel(wParam);
	if (message == WM_USER)			onUART(lParam);
	if (message == WM_CLOSE)		onClose();
	
    return ::DefWindowProc(hWnd, message, wParam, lParam);
}

LRESULT CALLBACK Application::gdiProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == WM_SIZE)			onResize();
	if (message == WM_PAINT)		onPaint();
	
	return ::DefWindowProc(hWnd, message, wParam, lParam);
}

void Application::onUART(LPARAM lParam)
{
	cout << string((char*)lParam) << endl;
}

void Application::onClick(WPARAM wParam)
{
	if (wParam == TIME_BUTTON)	SettingsModel::shared().setModeTime();
	if (wParam == SPEED_BUTTON)	SettingsModel::shared().setModeSpeed();
	if (wParam == START_BUTTON)	onButtonStart();
	if (wParam == STOP_BUTTON)	onButtonStop();
	if (wParam == OPEN_BUTTON)	onButtonOpen();
	if (wParam == QUIT_BUTTON)	::PostQuitMessage(0);
}

void Application::onButtonStart()
{	
	cout << "start" << endl;
	DataModel::shared().clear();
	Renderer::shared().clear();
	UART::shared().startReceiving();
	UART::shared().transmit("1");
}

void Application::onButtonStop()
{
	cout << "stop" << endl;
	UART::shared().transmit("0");
	UART::shared().stopReceiving();
	DataModel::shared().save();
}

void Application::onButtonOpen()
{
    char szFileName[MAX_PATH] = "";
    ZeroMemory(&file, sizeof(file));
	
    file.lStructSize = sizeof(file);
    file.hwndOwner = hWindow;
	file.lpstrFilter = "Measurement Data (*.txt)\0*.txt\0";
    file.lpstrFile = szFileName;
    file.nMaxFile = MAX_PATH;
    file.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_NOCHANGEDIR;
    file.lpstrDefExt = "txt";
	
	dialogThreadHandle = ::CreateThread(NULL, 0, staticOpenDialog, (void*) &shared(), 0, NULL);
}

void Application::onWheel(WPARAM wParam)
{
	(GET_WHEEL_DELTA_WPARAM(wParam) > 0) ? SettingsModel::shared().increment() : SettingsModel::shared().decrement();
	updateSettingsWindow();
}

void Application::onResize()
{
	hDC = ::GetDC(gdiWnd);
	memDC = ::CreateCompatibleDC(hDC);
	::GetClientRect(gdiWnd, &window);
	bitmap = ::CreateCompatibleBitmap(hDC, window.right, window.bottom);
	::SelectObject(memDC, bitmap);
}

void Application::onPaint()
{
	PAINTSTRUCT ps;
	hDC = ::BeginPaint(gdiWnd, &ps);
	::BitBlt(hDC, 0, 0, window.right, window.bottom, memDC, 0, 0, SRCCOPY);
	::EndPaint(gdiWnd, &ps);
}

void Application::onClose()
{
	::DeleteObject(bitmap);
	::DeleteDC(memDC);
	::ReleaseDC(gdiWnd, hDC);
	::DestroyWindow(gdiWnd);
	::PostQuitMessage(0);
}

// Life Cycle

void Application::onStart(HINSTANCE instance)
{
	hInstance = instance;
	isDrawing = true;
    drawingThreadHandle = ::CreateThread(NULL, 0, staticDraw, (void*) this, 0, NULL);
	SettingsModel::shared().load();
	Renderer::shared().onStart();
}

void Application::onStop()
{
	isDrawing = false;
	::TerminateThread(drawingThreadHandle, 0);
	::CloseHandle(drawingThreadHandle);
	SettingsModel::shared().save();
	Renderer::shared().onStop();
}

void Application::registerWindowClass()
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

void Application::registerGDIClass()
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

void Application::createWindows()
{
	createSettingsWindow();
	createGraphicWindow();
	addControls();
	updateSettingsWindow();
	onResize();
	::SetFocus(hWindow);
}

void Application::createSettingsWindow()
{
	hWindow = ::CreateWindowEx(WS_EX_APPWINDOW | WS_EX_WINDOWEDGE,
							   "ClassName",
							   "Data Recorder",
							   WS_MINIMIZEBOX | WS_VISIBLE,
							   0, 0, 240, 375,
							   NULL, NULL, hInstance, NULL);
}

void Application::createGraphicWindow()
{	
	gdiWnd = ::CreateWindowEx(WS_EX_APPWINDOW | WS_EX_WINDOWEDGE,
							  "ClassName2",
							  "",
							  WS_MINIMIZEBOX | WS_VISIBLE,
							  240, 0, ::GetSystemMetrics(SM_CXSCREEN) - 240, ::GetSystemMetrics(SM_CYSCREEN) - 38,
							  hWindow, NULL, NULL, NULL);
}

void Application::addControls()
{
	::CreateWindowEx(WS_EX_TRANSPARENT, "button", "Settings",WS_CHILD|WS_VISIBLE|BS_GROUPBOX , 20, 10, 195, 110,hWindow, NULL, hInstance, NULL);
	::CreateWindowEx(WS_EX_TRANSPARENT, "button", "Connection",WS_CHILD|WS_VISIBLE|BS_GROUPBOX , 20, 130, 195, 110,hWindow, NULL, hInstance, NULL);
	
	::CreateWindow("button", "Speed",	WS_CHILD | WS_VISIBLE, 30, 40, 70, 30, hWindow, (HMENU)SPEED_BUTTON, hInstance, NULL);
	::CreateWindow("button", "Time",	WS_CHILD | WS_VISIBLE, 30, 80, 70, 30, hWindow, (HMENU)TIME_BUTTON, hInstance, NULL);
	::CreateWindow("button", "Start",	WS_CHILD | WS_VISIBLE, 30, 160, 70, 30, hWindow, (HMENU)START_BUTTON, hInstance, NULL);
	::CreateWindow("button", "Stop",	WS_CHILD | WS_VISIBLE, 30, 200, 70, 30, hWindow, (HMENU)STOP_BUTTON, hInstance, NULL);
	::CreateWindow("button", "Open",	WS_CHILD | WS_VISIBLE, 30, 260, 70, 30, hWindow, (HMENU)OPEN_BUTTON, hInstance, NULL);
	::CreateWindow("button", "Quit",	WS_CHILD | WS_VISIBLE, 30, 300, 70, 30, hWindow, (HMENU)QUIT_BUTTON, hInstance, NULL);
	
	::CreateWindow("static", "", WS_CHILD | WS_VISIBLE | SS_LEFT | SS_CENTERIMAGE, 120, 40, 70, 30, hWindow, (HMENU)SPEED_TEXT, hInstance, NULL);
	::CreateWindow("static", "", WS_CHILD | WS_VISIBLE | SS_LEFT | SS_CENTERIMAGE, 120, 80, 70, 30, hWindow, (HMENU)TIME_TEXT, hInstance, NULL);
}

void Application::runMainLoop()
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

// Actions

void Application::updateSettingsWindow()
{
	SettingsModel m = SettingsModel::shared();
	
	string time = "1/" + to_string(m.getTime()) + " s";
	string speed = to_string(m.getSpeed());
	
	::SendMessage(::GetDlgItem(hWindow, TIME_TEXT), WM_SETTEXT, 0, (LPARAM) time.c_str());
	::SendMessage(::GetDlgItem(hWindow, SPEED_TEXT), WM_SETTEXT, 0, (LPARAM) speed.c_str());
}

// Dialog

DWORD WINAPI  Application::staticOpenDialog(void* Param)
{
	return ((Application*) Param)->openDialog();
}

DWORD Application::openDialog()
{
	if (::GetOpenFileName(&file))
	{
		DataModel::shared().clear();
		Renderer::shared().clear();
		DataModel::shared().load(file.lpstrFile);
	}
	
	::TerminateThread(dialogThreadHandle, 0);
	::CloseHandle(dialogThreadHandle);
}

// Drawing

DWORD WINAPI Application::staticDraw(void* Param)
{
	return ((Application*) Param)->draw();
}

DWORD Application::draw()
{
	while(isDrawing)
	{
		Renderer::shared().draw(memDC, window);
		::RedrawWindow(gdiWnd, NULL, NULL, RDW_INVALIDATE);		
		::Sleep(1000 / 64);
	}
}

// If the time scale is 1/128 s (the one second's data is displayed at the full screen), the realtime speed is 32.


