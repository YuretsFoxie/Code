#include "graph.h"
#include "shaderprogram.h"
#include <iostream>

// Public Functions

void Graph::setup(HWND hwnd)
{
	hWnd = hwnd;
	enableOpenGL(hWnd, &hdc, &hrc);
	shaderProgram = ShaderProgram().create();
	
	glGenVertexArrays(2, vaoIDs);
	glGenBuffers(2, vboIDs);
	
	glBindVertexArray(vaoIDs[0]);
	glBindBuffer(GL_ARRAY_BUFFER, vboIDs[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(firstTriangle), firstTriangle, GL_STREAM_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	
	/*
	glBindVertexArray(vaoIDs[1]);
	glBindBuffer(GL_ARRAY_BUFFER, vboIDs[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(secondTriangle), secondTriangle, GL_STREAM_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	*/
}

void Graph::update(const int value)
{
	//=====
	
	firstTriangle[2] += (float)value / 50.0;
	firstTriangle[3] -= (float)value / 50.0;
	
	glBindBuffer(GL_ARRAY_BUFFER, vboIDs[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(firstTriangle), firstTriangle, GL_STREAM_DRAW);
	
	//=====
	
	glClear(GL_COLOR_BUFFER_BIT);
	glUseProgram(shaderProgram);
	
	glBindVertexArray(vaoIDs[0]);
	glDrawArrays(GL_LINE_LOOP, 0, 3);
	
	/*
	glBindVertexArray(vaoIDs[1]);
	glDrawArrays(GL_LINE_LOOP, 0, 3);
	*/
	
	::SwapBuffers(hdc);
}

// Private Functions

Graph::Graph() {}

Graph::~Graph()
{
	glDeleteVertexArrays(2, vaoIDs);
	glDeleteBuffers(2, vboIDs);
	glDeleteProgram(shaderProgram);
	disableOpenGL(hWnd, hdc, hrc);
}

void Graph::enableOpenGL(HWND hwnd, HDC* hdc, HGLRC* hrc)
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

void Graph::disableOpenGL(HWND hwnd, HDC hdc, HGLRC hrc)
{
	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(hrc);
	::ReleaseDC(hwnd, hdc);
}
