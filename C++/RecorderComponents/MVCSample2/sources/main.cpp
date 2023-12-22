#include "controller.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int iCmdShow)
{
	return Controller::shared().run(hInstance);
}

// http://www.songho.ca/opengl/gl_mvc.html
