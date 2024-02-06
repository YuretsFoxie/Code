#include "controller.h"
#include "log.h"

// Public Functions

WPARAM Controller::run(HINSTANCE instance)
{
	onStart(instance);
	runMainLoop();
	onStop();
	
	return msg.wParam;
}

void Controller::onClick(WPARAM wParam)
{
	if (wParam == view.quitButtonID)		::PostQuitMessage(0);
	if (wParam == view.connectButtonID)		toggleConnection();
	if (wParam == view.comSettingsButtonID)	showCOMSettings();
	if (wParam == comView.okButtonID)		onCOMOK();
	if (wParam == comView.cancelButtonID) 	onCOMCancel();
}

void Controller::onPress(WPARAM wParam)
{
	if (wParam == VK_UP)	gdiView.scrollUp();
	if (wParam == VK_DOWN)	gdiView.scrollDown();
}

void Controller::onPaint()
{
	gdiView.paint();
}

void Controller::update(const string& message)
{
	// Log::shared().save(message);
	gdiView.print(message);
}

void Controller::update(const int value)
{
	// Log::shared().save(value);
}

// Private Functions

void Controller::onStart(HINSTANCE instance)
{
	HWND window = view.onStart(instance);
	gdiView.onStart(instance, window);
	
	comView.onStart({
		instance,
		window,
		comSettings.findPorts(),
		comSettings.getBaudrates()
	});
	
	ports.addObserver(this);
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
	ports.removeObserver(this);
}

void Controller::toggleConnection()
{
	if (ports.isConnected())
	{	
		ports.transmit("0");
		ports.disconnect();
		view.onDisconnect();
	}
	else
	{
		ports.selectPort(comSettings.getData());
		ports.transmit("1");
		view.onConnect();
	}
}

void Controller::showCOMSettings()
{
	view.disableCOMControls();
	comView.show(comSettings.getData());
}

void Controller::onCOMOK()
{
	comSettings.setData(comView.hide());
	view.enableCOMControls();
}

void Controller::onCOMCancel()
{
	view.enableCOMControls();
	comView.hide();
}
