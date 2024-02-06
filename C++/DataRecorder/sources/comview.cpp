#include "comview.h"
#include "windowprocedure.h"

void COMView::onStart(const COMViewConfiguration& config)
{
	configuration = config;
}

HWND COMView::show(const COMSettingsData& data)
{
	settings = data;
	registerWindowClass();
	calculateHeight();
	createWindow();
	addControls();
	
	::SetFocus(window);
	return window;
}

COMSettingsData COMView::hide()
{
	for (auto button: radioButtons)
		if (::SendMessage(button.handle, BM_GETCHECK, 0, 0) == BST_CHECKED)
			settings.port = button.name;
	
	int index = ::SendMessage(combobox, CB_GETCURSEL, 0, 0);
	settings.baudrate = configuration.baudrates[index];
	
	::DestroyWindow(window);
	return settings;
}

string COMView::selectedRadioButton(int buttonID)
{
	int i = buttonID - radioButtonID;
	bool isButton = (0 <= i && i < configuration.ports.size());
	return isButton ? configuration.ports[i] : "";
}

void COMView::registerWindowClass()
{
	WNDCLASS wc = {0};
	
	wc.lpfnWndProc = windowProcedure;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.hInstance = configuration.instance;
	wc.lpszClassName = "COMWindowClass";
	wc.hCursor = ::LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW);
	
	::RegisterClass(&wc);
}

void COMView::calculateHeight()
{
	height = 160 + 25 * configuration.ports.size();
}

void COMView::createWindow()
{
	int x = ::GetSystemMetrics(SM_CXVIRTUALSCREEN) * 0.5 - width * 0.5;
	int y = ::GetSystemMetrics(SM_CYVIRTUALSCREEN) * 0.5 - height * 0.5;
	
	window = ::CreateWindowEx(WS_EX_APPWINDOW | WS_EX_WINDOWEDGE, 
							   "COMWindowClass",
							   "Connection Settings",
							   WS_MINIMIZEBOX | WS_VISIBLE,
							   x, y, width, height,
							   configuration.window, NULL, configuration.instance, NULL);			   
}

void COMView::addControls()
{
	addComboBox();
	addRadioButtons();
	addButtons();
}

void COMView::addComboBox()
{
	::CreateWindowEx(0, "static", ("Buadrate:"), WS_VISIBLE | WS_CHILD | WS_GROUP | SS_LEFT, 20, 15, 135, 15, window, (HMENU)0, configuration.instance, 0);
	combobox = ::CreateWindowEx(0, "combobox", (""), WS_VISIBLE | WS_CHILD | CBS_DROPDOWN | CBS_HASSTRINGS, 20, 35, 200, 500, window, (HMENU)0, configuration.instance, 0);
	
	for(auto baudrate: configuration.baudrates)
		::SendMessage(combobox,(UINT) CB_ADDSTRING, 0, (LPARAM) to_string(baudrate).c_str());
	
	int index = findIndex(configuration.baudrates, settings.baudrate);
	::SendMessage(combobox, CB_SETCURSEL, index, 0);
}

void COMView::addRadioButtons()
{
	for (int i = 0; i < configuration.ports.size(); i++)
	{
		string name = configuration.ports[i];
		DWORD style = i == 0 ? WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON | WS_GROUP : WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON;
		int y = 65 + 25 * i;
		
		HWND button = ::CreateWindowEx(0, "button", name.c_str(), style, 20, y, 85, 25, window, reinterpret_cast<HMENU>(radioButtonID + i), configuration.instance, 0);
		
		if (name == settings.port)
			::SendMessage(button, BM_CLICK, 0, 0);
		
		radioButtons.push_back( {button, name} );
	}
}

void COMView::addButtons()
{
	HWND okButton = ::CreateWindowEx(0, "button", ("OK"), WS_VISIBLE | WS_CHILD | WS_TABSTOP, 20, height - 80, 90, 40, window, reinterpret_cast<HMENU>(okButtonID), configuration.instance, 0);
	HWND cancelButton = ::CreateWindowEx(0, "button", ("Cancel"), WS_VISIBLE | WS_CHILD | WS_TABSTOP, 130, height - 80, 90, 40, window, reinterpret_cast<HMENU>(cancelButtonID), configuration.instance, 0);
	
	::SendMessage(okButton, WM_CHANGEUISTATE, (WPARAM)(0x10001), 0);
	::SendMessage(cancelButton, WM_CHANGEUISTATE, (WPARAM)(0x10001), 0);
}

int COMView::findIndex(const vector<int>& v, const int element)
{
	for (int i = 0; i < v.size(); i++)
		if (v[i] == element)
			return i;
	
	return -1;
}
