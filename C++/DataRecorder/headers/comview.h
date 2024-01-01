#ifndef COMVIEW_H_INCLUDED
#define COMVIEW_H_INCLUDED

#include <windows.h>
#include <vector>
#include <string>
#include "comsettings.h"

using namespace std;

struct COMViewConfiguration
{
	HINSTANCE instance;
	HWND window;
	vector<string> ports;
	vector<int> baudrates;
};



struct RadioButton
{
	HWND handle;
	string name;
};



class COMView
{
public:
	void onStart(const COMViewConfiguration& config);
	HWND show(const COMSettingsData& data);
	COMSettingsData hide();
	string selectedRadioButton(int buttonID);
	
	const int okButtonID = 200;
	const int cancelButtonID = 201;
	
private:
	void registerWindowClass();
	void calculateHeight();
	void createWindow();
	void addControls();
	void addComboBox();
	void addRadioButtons();
	void addButtons();
	int findIndex(const vector<int>& v, const int element);
	
	const int radioButtonID = 300;
	const int width = 245;
	int height;
	
	COMViewConfiguration configuration;
	COMSettingsData settings;
	HWND window;
	HWND combobox;
	vector <RadioButton> radioButtons;
};

#endif // COMVIEW_H_INCLUDED
