#include "graphics.h"
#include "console.h"

// Public Functins

void Graphics::set(const HWND& hwnd)
{
	setupPixelFormat(hwnd);
	setupProjections();
	glewInit();
	
	shaders.initialize();
	text.initialize();
	plot.initialize(settings.getMaxPoints(), settings.getMaxADCValue());
	
	batchSize = settings.getBatchSize();
	((BOOL(WINAPI*)(int))wglGetProcAddress("wglSwapIntervalEXT"))(1); // enable vsync
	
	Console::shared().addWindow(text);
}

void Graphics::render()
{
	updateCounter++;
	if (updateCounter >= batchSize)
	{
		glClear(GL_COLOR_BUFFER_BIT);
		drawPlots();
		drawText();
		::SwapBuffers(hdc);
		updateCounter = 0;
	}
}

// Private Functins

void Graphics::setupPixelFormat(const HWND& hwnd)
{
	hdc = ::GetDC(hwnd);
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

void Graphics::setupProjections()
{
	textOrtho[0] = 2.0f / textParameters.width;
	textOrtho[5] = 2.0f / textParameters.height;
}

void Graphics::drawPlots()
{
	std::vector<float> data = buffer.getData();
	
	if (!data.empty())
	{
		glViewport(plotParameters.x, plotParameters.y, plotParameters.width, plotParameters.height);
		glUseProgram(shaders.getGraphProgram());
		plot.draw(data);
	}
}

void Graphics::drawText()
{
	glViewport(textParameters.x, textParameters.y, textParameters.width, textParameters.height);
	glUseProgram(shaders.getTextProgram());
	glUniformMatrix4fv(glGetUniformLocation(shaders.getTextProgram(), "projection"), 1, GL_FALSE, textOrtho);
	text.draw();
}
