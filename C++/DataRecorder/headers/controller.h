#ifndef CONTROLLER_H_INCLUDED
#define CONTROLLER_H_INCLUDED

#include <windows.h>
#include "model.h"
#include "view.h"
#include "gdiview.h"

using namespace std;

class Controller: public ObserverInterface
{
public:
	WPARAM run(HINSTANCE hInstance);
	void onClick(WPARAM wParam);
	void onResize();
	void onPaint();
	void update(const Shape& shape) override;
	
private:
	void onStart(HINSTANCE instance);
	void runMainLoop();
	void onStop();
		
	Model model;
	View view;
	GDIView gdiView;
	
	MSG msg;
};

#endif // CONTROLLER_H_INCLUDED
