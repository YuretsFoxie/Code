#include "graphics.h"

// Public Functins

void Graphics::initialize(HWND hwnd, const Settings& settings)
{
	setupPixelFormat(hwnd);
	glewInit();
	((BOOL(WINAPI*)(int))wglGetProcAddress("wglSwapIntervalEXT"))(1); // enable vsync
	
	shaders.initialize();
	text.initialize(hwnd);
	graph.initialize(settings.getMaxPoints());
}

void Graphics::drawVertices(const std::vector<float>& vertices)
{
	glUseProgram(shaders.getGraphProgram());
	graph.draw(vertices);
}

void Graphics::drawText(const std::string& message, float x, float y)
{
	glUseProgram(shaders.getTextProgram());
	glUniformMatrix4fv(glGetUniformLocation(shaders.getTextProgram(), "projection"), 1, GL_FALSE, ortho);
	text.draw(message, x, y);
}

// Private Functins

void Graphics::setupPixelFormat(HWND hwnd)
{
	HDC hdc = ::GetDC(hwnd);
	PIXELFORMATDESCRIPTOR pfd = 
	{
		sizeof(PIXELFORMATDESCRIPTOR), 1,
		PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER, PFD_TYPE_RGBA,
		32, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 24, 8, 0, PFD_MAIN_PLANE, 0, 0, 0, 0
	};
	
	int pf = ::ChoosePixelFormat(hdc, &pfd);
	SetPixelFormat(hdc, pf, &pfd);
	
	HGLRC hglrc = wglCreateContext(hdc);
	wglMakeCurrent(hdc, hglrc);
}
