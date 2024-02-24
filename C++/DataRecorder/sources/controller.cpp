#include "controller.h"

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
	if (wParam == view.testButtonID)		runTest();
	if (wParam == comView.okButtonID)		onCOMOK();
	if (wParam == comView.cancelButtonID) 	onCOMCancel();
}

void Controller::onPress(WPARAM wParam) {}

void Controller::onPaint()
{
	graphicsView.paint();
}

void Controller::update(const string& message)
{
	graphicsView.print(message);
}

void Controller::update(const int value)
{
	graphicsView.print(value);
}

// Private Functions

void Controller::onStart(HINSTANCE instance)
{
	graphicsView.print("started...");
	
	HWND window = view.onStart(instance);
	graphicsView.onStart(instance, window);
	
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
	graphicsView.onStop();
	ports.removeObserver(this);
}

void Controller::toggleConnection()
{
	ports.isConnected() ? disconnect() : connect();
}

void Controller::connect()
{
	ports.selectPort(comSettings.getData());
	ports.transmit("1");
	
	if (ports.isConnected())
		view.onConnect();
}

void Controller::disconnect()
{
	ports.transmit("0");
	ports.disconnect();
	
	if (!ports.isConnected())
		view.onDisconnect();
}

void Controller::showCOMSettings()
{
	view.disableCOMControls();
	comView.show(comSettings.getData());
}

void Controller::runTest() {}

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
