#include "controller.h"
#include "view.h"
#include "comports.h"

#include <iostream>

// Public Functions

WPARAM Controller::run(HINSTANCE instance)
{
	onStart(instance);
	registerWindowClass();
	
	vector<string> port = COMPorts::shared().findPorts();
	View::shared().setup(instance, port);
	
    runMainLoop();
	onStop();
	
    return msg.wParam;
}

void Controller::showMessage(const string& message)
{
	cout << message << '\n';
}

void Controller::handleReceivedData(const int data)
{
	cout << data << ' ';
}

void Controller::selectPort(const string& port)
{
	COMPorts::shared().selectPort(port);
}

void Controller::disconnect()
{
	COMPorts::shared().disconnect();
}

// Message Handlers

LRESULT CALLBACK Controller::wndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{	
	if (message == WM_COMMAND)	onClick(wParam);
	
	if (message == WM_CTLCOLORSTATIC) // makes the control headers transparent
	{
		::SetBkMode((HDC)wParam, TRANSPARENT);
		return (INT_PTR)(HBRUSH)::GetStockObject(NULL_BRUSH);
	}
	
    return ::DefWindowProc(hWnd, message, wParam, lParam);
}

void Controller::onClick(WPARAM wParam)
{	
	if (wParam == View::shared().quitButtonID) onClose();
	if (wParam == View::shared().connectButtonID) onConnect();
}

void Controller::onClose()
{
	::PostQuitMessage(0);
}

void Controller::onConnect()
{
	View::shared().toggleConnect();
}

// Life Cycle

void Controller::onStart(HINSTANCE instance)
{
	hInstance = instance;
}

void Controller::onStop() {}

void Controller::registerWindowClass()
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

void Controller::runMainLoop()
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

// TODO: Consider implementing the separated COM Settings dialog.
// TODO: check the error flags
// TODO: Complete the COMPort functionality and test it, implement the correct error handling, but remove the unneeded error handling.
// TODO: Perform the refactoring, consider using the object composition instead of the singletones.
// TODO: implement the error logging (write them to the file).
