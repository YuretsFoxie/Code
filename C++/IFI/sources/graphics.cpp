#include "graphics.h"
#include "shaderprogram.h"
#include "plot.h"
#include "plotgrid.h"
#include "histogram.h"
#include "histogramgrid.h"
#include "spectrumanalyzer.h"

// Public Functions

void Graphics::setup(HWND hwnd) // OpenGL screen coordinate ranges are -1...1 for both x and y
{
	items.push_back(new Plot(left, top, bufferSize));
	// items.push_back(new Plot(left, bottom, bufferSize));
	items.push_back(new PlotGrid(left, top));
	items.push_back(new Histogram(right, top));
	items.push_back(new HistogramGrid(right, top));
	
	hWnd = hwnd;
	enableOpenGL(hWnd, &hdc, &hrc);
	shaderProgram = ShaderProgram().create();
	
	vaoIDs = new GLuint[items.size()];
	vboIDs = new GLuint[items.size()];
	
	glGenVertexArrays(items.size(), vaoIDs);
	glGenBuffers(items.size(), vboIDs);
	
	for (int i = 0; i < items.size(); i++)
		setupObject(i);
	
	drawStaticObjects();
}

//=====

/*
void Graphics::push(const int value)
{
	// if (currentPlot == 1)
		// SpectrumAnalyzer::shared().push(value);
	
	// if (currentPlot == 0)
	// {
		glClear(GL_COLOR_BUFFER_BIT);
		glUseProgram(shaderProgram);
		updateStaticObjects();
	// }
	
	items[currentPlot]->push(value);
	updateObject(currentPlot);
	// switchCurrentPlot();
	
	// if (currentPlot == 0)
		::SwapBuffers(hdc);
}
*/

void Graphics::push(const int value)
{
	bool shouldUpdate = (updateCounter % 2 == 0);
	
	if (shouldUpdate)
	{
		glClear(GL_COLOR_BUFFER_BIT);
		glUseProgram(shaderProgram);
		updateStaticObjects();
	}
	
	items[currentPlot]->push(value);
	
	if (shouldUpdate)
	{
		updateObject(currentPlot);
		::SwapBuffers(hdc);
	}
	
	updateCounter++;
}

//=====

void Graphics::updateFFT(const std::vector<float>& data)
{
	for (int i = 0; i < items.size(); i++)
		if (items[i]->isFFTUpdatable())
		{
			items[i]->updateFFT(data);
			updateObject(i);
		}
}

// Private Functions

Graphics::Graphics() {}

Graphics::~Graphics()
{
	glDeleteVertexArrays(items.size(), vaoIDs);
	glDeleteBuffers(items.size(), vboIDs);
	glDeleteProgram(shaderProgram);
	disableOpenGL(hWnd, hdc, hrc);
	
	delete[] vaoIDs;
	delete[] vboIDs;
}

void Graphics::enableOpenGL(HWND hwnd, HDC* hdc, HGLRC* hrc)
{
	PIXELFORMATDESCRIPTOR pfd;
	int iFormat;
	*hdc = ::GetDC(hwnd);
	
	::ZeroMemory(&pfd, sizeof(pfd));
	pfd.nSize = sizeof(pfd);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 24;
	pfd.cDepthBits = 16;
	pfd.iLayerType = PFD_MAIN_PLANE;
	
	iFormat = ChoosePixelFormat(*hdc, &pfd);
	::SetPixelFormat(*hdc, iFormat, &pfd);

	*hrc = wglCreateContext(*hdc);
	wglMakeCurrent(*hdc, *hrc);
	
	glewInit();
}

void Graphics::disableOpenGL(HWND hwnd, HDC hdc, HGLRC hrc)
{
	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(hrc);
	::ReleaseDC(hwnd, hdc);
}

void Graphics::setupObject(const int index)
{
	glBindVertexArray(vaoIDs[index]);
	updateVBO(index);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
}

void Graphics::updateObject(const int index)
{
	updateVBO(index);
	glBindVertexArray(vaoIDs[index]);
	glDrawArrays(GL_LINE_STRIP, 0, items[index]->pointsNumber());
}

void Graphics::updateVBO(const int index)
{
	glBindBuffer(GL_ARRAY_BUFFER, vboIDs[index]);
	glBufferData(GL_ARRAY_BUFFER, 2 * items[index]->pointsNumber() * sizeof(float), items[index]->data(), GL_STREAM_DRAW);
}

void Graphics::drawStaticObjects()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glUseProgram(shaderProgram);
	updateStaticObjects();
	::SwapBuffers(hdc);
}

void Graphics::updateStaticObjects()
{
	for (int i = 0; i < items.size(); i++)
		if (!items[i]->isPushable() && !items[i]->isFFTUpdatable())
			updateObject(i);	
}

void Graphics::switchCurrentPlot()
{
	currentPlot = currentPlot == 1 ? 0 : 1;
}
