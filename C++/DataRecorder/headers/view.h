#ifndef VIEW_H_INCLUDED
#define VIEW_H_INCLUDED

#include <windows.h>
#include <string>

using namespace std;

class View
{
public:	
	HWND onStart(HINSTANCE instance);
	
	void onConnect();
	void onDisconnect();
	void enableCOMControls();
	void disableCOMControls();
	
	const int quitButtonID = 100;
	const int connectButtonID = 101;
	const int comSettingsButtonID = 102;
	
private:
	void registerWindowClass();
	void createWindow();
	void enterFullScreen();
	void addControls();
	
	HINSTANCE hInstance;
	HWND window;
	HWND quitButton;
	HWND connectButton;
	HWND comSettingsButton;
};

#endif // VIEW_H_INCLUDED
