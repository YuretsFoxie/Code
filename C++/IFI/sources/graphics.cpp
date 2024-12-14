#include "graphics.h"

// Public Functins

void Graphics::initialize(const HWND& hwnd, const Settings& settings)
{
	setupPixelFormat(hwnd);
	glewInit();
	((BOOL(WINAPI*)(int))wglGetProcAddress("wglSwapIntervalEXT"))(1); // enable vsync
	
	shaders.initialize();
	text.initialize(hwnd);
	graph.initialize(settings.getMaxPoints());
	
	batchSize = settings.getBatchSize();
}

void Graphics::render(HDC hdc, int& updateCounter, std::atomic<bool>& isRunning)
{
	updateCounter++;
	if (updateCounter >= batchSize)
	{
		glClear(GL_COLOR_BUFFER_BIT);
		drawVertices();
		drawText();
		::SwapBuffers(hdc);
		updateCounter = 0;
	}
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

void Graphics::drawVertices()
{
	std::vector<float> vertices;
	buffer.prepare(vertices);
	
	if (!vertices.empty())
	{
		glUseProgram(shaders.getGraphProgram());
		graph.draw(vertices);
	}
}

void Graphics::drawText()
{
	glUseProgram(shaders.getTextProgram());
	glUniformMatrix4fv(glGetUniformLocation(shaders.getTextProgram(), "projection"), 1, GL_FALSE, ortho);
	text.draw("UART Data Visualization", 25.0f, 25.0f);
}
