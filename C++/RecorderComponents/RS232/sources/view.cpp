#include "view.h"
#include "controller.h"

// Public Functions

void View::setup(const HINSTANCE instance, const vector<string>& comPorts)
{
	hInstance = instance;
	ports = comPorts;
	
	createWindow();
	addControls();
	adjustConsole();
	::SetFocus(hWindow);
}

void View::toggleConnect()
{
	isConnected ? onDisconnect() : onConnect();
}

// Private Functions

void View::createWindow()
{
	hWindow = ::CreateWindowEx(WS_EX_APPWINDOW | WS_EX_WINDOWEDGE,
							   "ClassName",
							   "Data Recorder",
							   WS_MINIMIZEBOX | WS_VISIBLE,
							   0, 0, 100, 400,
							   NULL, NULL, hInstance, NULL);
}

void View::addControls()
{
	const int size = ports.size();
	const int buttonYShift = 20;
	const int buttonYStep = 40;
	const int groupHeight = 20 + buttonYStep * (size + 1);
	const int connectY = buttonYShift + buttonYStep * size;
	
	::CreateWindowEx(WS_EX_TRANSPARENT, "button", "", WS_CHILD | WS_VISIBLE | BS_GROUPBOX, 10, 0, 110, groupHeight, hWindow, NULL, hInstance, NULL);
	
	for (int i = 0; i < size; i++)
	{
		DWORD style = i == 0 ? WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON | WS_GROUP : WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON;
		HWND button = ::CreateWindowEx(WS_EX_WINDOWEDGE, "button", ports[i].c_str(), style, 20, buttonYShift + buttonYStep * i, 70, 30, hWindow, (HMENU)(200 + i), hInstance, NULL);
		
		if (i == 0)
			::SendMessage(button, BM_SETCHECK, BST_CHECKED, 0);
		
		radioButtons.push_back({button, ports[i]});
	}
	
	connectButton = ::CreateWindow("button", "Connect", WS_CHILD | WS_VISIBLE, 20, connectY, 90, 30, hWindow, (HMENU)connectButtonID, hInstance, NULL);
	quitButton = ::CreateWindow("button", "Quit", WS_CHILD | WS_VISIBLE, 20, 330, 90, 30, hWindow, (HMENU)quitButtonID, hInstance, NULL);
}

void View::adjustConsole()
{
	RECT actualDesktop;
	::GetWindowRect(::GetDesktopWindow(), &actualDesktop);
	
	HWND console = ::GetConsoleWindow();
	RECT rect = {128, 0, actualDesktop.right + 10, actualDesktop.bottom - 30};
	
	::MoveWindow(console, rect.left, rect.top, rect.right-rect.left, rect.bottom-rect.top, TRUE);
}

void View::onConnect()
{
	isConnected = true;
	
	for (auto button: radioButtons)
	{
		::EnableWindow(button.handle, false);
		
		if (::SendMessage(button.handle, BM_GETCHECK, 0, 0) == BST_CHECKED)
			Controller::shared().selectPort(button.name);
	}
	
	::SendMessage(connectButton, WM_SETTEXT, 0, (LPARAM) "Disconnect");
	::SetFocus(hWindow);
	::Beep(1000, 100);
}

void View::onDisconnect()
{
	isConnected = false;
	
	for (auto button: radioButtons)
		::EnableWindow(button.handle, true);
	
	Controller::shared().disconnect();
	
	::SendMessage(connectButton, WM_SETTEXT, 0, (LPARAM) "Connect");
	::SetFocus(hWindow);
	::Beep(1000, 100);
}
