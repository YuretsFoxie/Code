#ifndef VIEW_H_INCLUDED
#define VIEW_H_INCLUDED

#include <windows.h>
#include <string>

using namespace std;

class View
{
public:	
	HWND onStart(HINSTANCE instance);
	void update(const string& shape);
	
	const int quitButtonID = 100;
	const int shapeButtonID = 101;
	
private:
	void registerWindowClass();
	void createWindow();
	void addControls();
	
	HINSTANCE hInstance;
	HWND window;
	HWND quitButton;
	HWND shapeButton;
};

#endif // VIEW_H_INCLUDED
