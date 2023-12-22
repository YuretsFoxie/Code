#ifndef VIEW_H_INCLUDED
#define VIEW_H_INCLUDED

#include <windows.h>
#include <vector>
#include <string>

using namespace std;

struct RadioButton
{
	HWND handle;
	string name;
};



class View
{
public:
    static View& shared()
    {
        static View view;
        return view;
    }
	
	void setup(const HINSTANCE instance, const vector<string>& comPorts);
	void toggleConnect();
	
	const int quitButtonID = 100;
	const int connectButtonID = 101;
	
private:
	void createWindow();
	void addControls();
	void adjustConsole();
	void onConnect();
	void onDisconnect();
	
	// Variables
	
	vector<RadioButton> radioButtons;
	vector<string> ports;
	
	HINSTANCE hInstance;
	HWND hWindow;
	HWND connectButton;
	HWND quitButton;
	bool isConnected = false;
};

#endif // VIEW_H_INCLUDED
