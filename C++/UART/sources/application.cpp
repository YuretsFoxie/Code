#include <algorithm>
#include <bitset>
#include "application.h"
#include "uart.h"

string Application::text;
int Application::counter;

HWND Application::hWnd;
HWND Application::connectButton;
HWND Application::edit;
HWND Application::sendButton;
HWND Application::clearButton;

// Public Methods

WPARAM Application::run(HINSTANCE instance)
{
    hInstance = instance;
	
	registerWindowClass();
    createWindow();
	showText();
    runMainLoop();
    destroyWindow();

    return msg.wParam;
}

// Messages Handler

LRESULT CALLBACK Application::wndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch(message)
    {
    case WM_USER:
        showText(lParam, wParam);
        break;

    case WM_KEYDOWN:
        onButtonPressed(wParam);
        break;

    case WM_COMMAND:
		if(wParam == ID_CONNECT_BUTTON)
			onConnectButtonClick();
			
        if(wParam == ID_SEND_BUTTON)
            onSendButtonClick();

        if(wParam == ID_CLEAR_BUTTON)
            onClearButtonClick();

        ::SetFocus(hWnd);

        break;

    case WM_CLOSE:
        ::PostQuitMessage(0);
        return 0;
    }

    return ::DefWindowProc(hWnd, message, wParam, lParam);
}

// Actions

void Application::onConnectButtonClick()
{
	if ( UART::shared().getIsReceiving() )
    {
        UART::shared().stopReceiving();
        ::SendMessage(connectButton, WM_SETTEXT, 0, (LPARAM) "Start");
    }
    else
    {
        UART::shared().startReceiving();

        if ( UART::shared().getIsReceiving() )
            ::SendMessage(connectButton, WM_SETTEXT, 0, (LPARAM) "Stop");
    }
}

void Application::onSendButtonClick()
{	
	if (text.length() > 0)
		UART::shared().transmit(text);	
}

void Application::onClearButtonClick()
{
	::SetWindowText(edit, 0);
	
	text = "";
	showText();
}

void Application::onButtonPressed(WPARAM wParam)
{	
	switch (wParam)
	{
	case VK_RETURN:
		onSendButtonClick();
		break;
		
	case VK_BACK:
		if (text.length() > 0)
			text.pop_back();
		
		showText();
		break;
		
	case VK_SHIFT:
		onClearButtonClick();
		break;
		
	case VK_CONTROL:
		onConnectButtonClick();
		break;
		
	case VK_ESCAPE:
		::PostQuitMessage(0);
		break;
		
	case VK_OEM_PLUS:
		text = text + string(1, '+');
		showText();
		break;
		
	default:
		text = text + string(1, char(wParam));
		showText();
	}
}

// Private Methods

void Application::showText()
{
    ::SendMessage(edit, EM_SETSEL, getInputIndex(), ::GetWindowTextLength(edit));
    ::SendMessage(edit, EM_REPLACESEL, 0, (LPARAM) ("> " + text).c_str());	
}

void Application::showText(const LPARAM lParam, const WPARAM wParam)
{
	int len = ::GetWindowTextLength(edit);
	int index = getInputIndex();
	int i = (len == index + 2) ? index - 3 : len;
	
	string prefix = (i > 0) ? "\r\n< " : "< ";
	
	string s = string((char*)lParam);
	replace(s.begin(), s.end(), '\n', ' ');
	replace(s.begin(), s.end(), '\r', ' ');
	
	string str;
	
	if(wParam) {
		str = prefix + s + "\r\n\r\n> ";
		
	} else {
		prefix += "\r\n";
		
		for(int i = 0; i < s.size(); i++)
		{
			counter++;
			bitset<8> byte(s[i]);
			prefix += byte.to_string().insert(4, " ") + " (" + s[i] + ") " + to_string(counter) + "\r\n";
		}
		
		str = prefix + "\r\n> ";
	}
	
	::SendMessage(edit, EM_SETSEL, (i > 0) ? i : 0, len);
	::SendMessage(edit, EM_REPLACESEL, 0, (LPARAM) str.c_str());
	
	text = "";
}

int Application::getInputIndex()
{
	int len = ::GetWindowTextLength(edit) + 1;
	char* buf = new char[len];
	
	::GetWindowText(edit, (LPSTR)&buf[0], len);
	string str = string(buf);
	
	delete [] buf;	
	return str.find_last_of(">");
}

void Application::registerWindowClass()
{
    WNDCLASS wc = {0};
	wc.lpfnWndProc = wndProc;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.hInstance = hInstance;
	wc.lpszClassName = "ClassName";
	wc.hCursor = ::LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)COLOR_APPWORKSPACE;

	::RegisterClass(&wc);
}

void Application::createWindow()
{
    hWnd = ::CreateWindowEx(WS_EX_APPWINDOW | WS_EX_WINDOWEDGE,
                            "ClassName",
                            "UART",
                            (WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME ^ WS_MAXIMIZEBOX) | WS_VISIBLE,
                            (::GetSystemMetrics(SM_CXSCREEN) - 500) / 2,
                            (::GetSystemMetrics(SM_CYSCREEN) - 580) / 2,
                            500, 580,
                            NULL, NULL, hInstance, NULL);

    edit = ::CreateWindow("edit", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_MULTILINE | ES_AUTOVSCROLL | ES_READONLY, 10, 10, 475, 490, hWnd, (HMENU)ID_EDIT, hInstance, NULL);
    
	connectButton	= ::CreateWindow("button", "Start", WS_CHILD | WS_VISIBLE, 10, 510, 70, 30, hWnd, (HMENU)ID_CONNECT_BUTTON, hInstance, NULL);
	sendButton		= ::CreateWindow("button", "Send", WS_CHILD | WS_VISIBLE, 90, 510, 70, 30, hWnd, (HMENU)ID_SEND_BUTTON, hInstance, NULL);
    clearButton		= ::CreateWindow("button", "Clear", WS_CHILD | WS_VISIBLE, 170, 510, 70, 30, hWnd, (HMENU)ID_CLEAR_BUTTON, hInstance, NULL);
	
    UART::shared().init(hWnd);
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

void Application::destroyWindow()
{
    ::DestroyWindow(hWnd);
}
