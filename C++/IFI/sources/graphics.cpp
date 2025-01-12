#include "graphics.h"

// Public Functins

Graphics::Graphics(Settings& settings, Shaders& shaders, Plot& plot1, Plot& plot2, Spectrum& spectrum1, Spectrum& spectrum2, Text& text):
	shaders(shaders),
	plot1(plot1),
	plot2(plot2),
	spectrum1(spectrum1),
	spectrum2(spectrum2),
	text(text)
{
	batchSize = settings.getBatchSize();
	setupPixelFormat();
	enableVerticalSyncronization();
	setupProjections();
	glewInit();
	
	shaders.setup();
	plot1.setup();
	plot2.setup();
	spectrum1.setup();
	spectrum2.setup();
	text.setup();
}

void Graphics::render()
{
	updateCounter++;
	if (updateCounter >= batchSize)
	{
		glClear(GL_COLOR_BUFFER_BIT);
		drawPlots();
		drawSpectrums();
		drawText();
		::SwapBuffers(hdc);
		updateCounter = 0;
	}
}

// Private Functins

void Graphics::setupPixelFormat()
{
	hdc = ::GetDC(::GetActiveWindow());
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

void Graphics::enableVerticalSyncronization()
{
	((BOOL(WINAPI*)(int))wglGetProcAddress("wglSwapIntervalEXT"))(1);
}

void Graphics::setupProjections()
{
	ViewPortParameters textParameters = text.getParameters();
	textOrtho[0] = 2.0f / textParameters.width;
	textOrtho[5] = 2.0f / textParameters.height;
}

void Graphics::drawPlots()
{
	glUseProgram(shaders.getGraphProgram());
	plot1.draw();
	plot2.draw();
}

void Graphics::drawSpectrums()
{
	glUseProgram(shaders.getGraphProgram());
	spectrum1.draw();
	spectrum2.draw();
}

void Graphics::drawText()
{
	glUseProgram(shaders.getTextProgram());
	glUniformMatrix4fv(glGetUniformLocation(shaders.getTextProgram(), "projection"), 1, GL_FALSE, textOrtho);
	text.draw();
}
