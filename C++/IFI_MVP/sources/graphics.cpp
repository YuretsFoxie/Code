#include "graphics.h"
#include "shaderprogram.h"
#include "plot.h"

// Public Functions

void Graphics::setup(HWND hwnd) // OpenGL screen coordinate ranges are -1...1 for both x and y
{
	items.push_back(new Plot(left, top, bufferSize));
	
	hWnd = hwnd;
	enableOpenGL(hWnd, &hdc, &hrc);
	shaderProgram = ShaderProgram().create();
	
	vaoIDs = new GLuint[items.size()];
	vboIDs = new GLuint[items.size()];
	
	glGenVertexArrays(items.size(), vaoIDs);
	glGenBuffers(items.size(), vboIDs);
	
	for (int i = 0; i < items.size(); i++)
		setupObject(i);
}

void Graphics::push(const int value)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glUseProgram(shaderProgram);
	
	items[0]->push(value);
	
	updateObject(0);
	::SwapBuffers(hdc);
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
