#include "graphics.h"
#include "shaderprogram.h"
#include <iostream>

// Public Functions

void Graphics::setup(HWND hwnd)
{
	hWnd = hwnd;
	plot.setWindow(hWnd);
	
	enableOpenGL(hWnd, &hdc, &hrc);
	shaderProgram = ShaderProgram().create();
	
	glGenVertexArrays(1, vaoIDs);
	glGenBuffers(1, vboIDs);
	glBindVertexArray(vaoIDs[0]);
	
	updateVBO();
	
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
}

void Graphics::update(const int value)
{
	plot.push(value);
	updateVBO();
	
	glClear(GL_COLOR_BUFFER_BIT);
	glUseProgram(shaderProgram);
	glBindVertexArray(vaoIDs[0]);
	glDrawArrays(GL_LINE_LOOP, 0, plot.currentSize());
	
	::SwapBuffers(hdc);
}

// Private Functions

Graphics::Graphics() {}

Graphics::~Graphics()
{
	glDeleteVertexArrays(1, vaoIDs);
	glDeleteBuffers(1, vboIDs);
	glDeleteProgram(shaderProgram);
	disableOpenGL(hWnd, hdc, hrc);
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

void Graphics::updateVBO()
{
	glBindBuffer(GL_ARRAY_BUFFER, vboIDs[0]);
	glBufferData(GL_ARRAY_BUFFER, 2 * plot.currentSize() * sizeof(float), plot.data(), GL_STREAM_DRAW);
}

// TODO:
// implement drawing two plots and the marking lines (add VAOs and VBOs here)
