#ifndef CONTROLLER_H_INCLUDED
#define CONTROLLER_H_INCLUDED

#include <windows.h>
#include "comports.h"
#include "comsettings.h"
#include "view.h"
#include "gdiview.h"
#include "comview.h"

using namespace std;

class Controller: public ObserverInterface
{
public:
	WPARAM run(HINSTANCE instance);
	void onClick(WPARAM wParam, LPARAM lParam);
	void onResize();
	void onPaint();
	
	void update(const string& message) override;
	void update(const int value) override;
	
private:
	void onStart(HINSTANCE instance);
	void runMainLoop();
	void onStop();
	
	void toggleConnection();
	void showCOMSettings();
	void onCOMOK();
	void onCOMCancel();
	
	COMPorts ports;
	COMSettings comSettings;
	
	View view;
	GDIView gdiView;
	COMView comView;
	
	MSG msg;
};

#endif // CONTROLLER_H_INCLUDED
