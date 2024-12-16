#include "application.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	Application(hInstance, nCmdShow).run();
	return EXIT_SUCCESS;
}
