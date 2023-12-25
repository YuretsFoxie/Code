#include "controller.h"

// Public Functions

WPARAM Controller::run(HINSTANCE hInstance)
{
	onStart(hInstance);	
	runMainLoop();
	onStop();
	
	return msg.wParam;
}

void Controller::onClick(WPARAM wParam)
{
	if (wParam == view.quitButtonID) ::PostQuitMessage(0);
	if (wParam == view.shapeButtonID) model.toggleShape();
}

void Controller::onResize()
{
	gdiView.resize();
}

void Controller::onPaint()
{
	gdiView.paint();
}

void Controller::update(const Shape& shape)
{
	view.update(shape.name);
	gdiView.update(shape.points);
}

// Private Functions

void Controller::onStart(HINSTANCE instance)
{
	HWND window = view.onStart(instance);
	gdiView.onStart(instance, window);
	
	model.addObserver(this);
	model.toggleShape();
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

void Controller::onStop()
{
	gdiView.onStop();
	model.removeObserver(this);
}
