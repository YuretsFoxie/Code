#include "graph.h"

#include <iostream>

// TODO:
// Implement shifting the new data (adding an X coordinate).

// Check if the numVertices is set correctly
// Check if newDataSize is calculated correctly
// Consider reducind the data dimension from 3 to 2

// Public Functions

void Graph::setup(HWND hwnd)
{
	hWnd = hwnd;
	enableOpenGL(hWnd, &hdc, &hrc);
	shaderProgram = createShaderProgram();
	
	glGenBuffers(2, vboIDs);
	glGenVertexArrays(1, &vaoID);
	glBindVertexArray(vaoID);
	
	for (int i = 0; i < 2; ++i)
	{
		glBindBuffer(GL_ARRAY_BUFFER, vboIDs[i]);
		glBufferData(GL_ARRAY_BUFFER, bufferSize, nullptr, GL_STREAM_DRAW);
		
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
		glEnableVertexAttribArray(0);
	}
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Graph::update(const int value)
{
	numVertices++;
	
	if (currentOffset + newDataSize > bufferSize)
	{
		numVertices = 1024;
		currentOffset = 0;
	}
	
	glClear(GL_COLOR_BUFFER_BIT);
	glUseProgram(shaderProgram);
	
	glBindBuffer(GL_ARRAY_BUFFER, vboIDs[currentVBO]);
	glBufferSubData(GL_ARRAY_BUFFER, currentOffset, newDataSize, (void*)value);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	glBindVertexArray(vaoID);
	glBindBuffer(GL_ARRAY_BUFFER, vboIDs[currentVBO]);
	glDrawArrays(GL_POINTS, 0, numVertices);
	glBindVertexArray(0);
	
	currentOffset += newDataSize;
	currentVBO = (currentVBO + 1) % 2;
	
	::SwapBuffers(hdc);
}

// Private Functions

Graph::Graph() {}

Graph::~Graph()
{
	glDeleteVertexArrays(1, &vaoID);
	glDeleteBuffers(1, &vboIDs[0]);
	glDeleteBuffers(1, &vboIDs[1]);
	glDeleteProgram(shaderProgram);
	disableOpenGL(hWnd, hdc, hrc);
}

void Graph::enableOpenGL(HWND hwnd, HDC* hdc, HGLRC* hrc)
{
	PIXELFORMATDESCRIPTOR pfd;
	int iFormat;
	
	*hdc = GetDC(hwnd);
	
	ZeroMemory(&pfd, sizeof(pfd));
	pfd.nSize = sizeof(pfd);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 24;
	pfd.cDepthBits = 16;
	pfd.iLayerType = PFD_MAIN_PLANE;
	
	iFormat = ChoosePixelFormat(*hdc, &pfd);
	SetPixelFormat(*hdc, iFormat, &pfd);

	*hrc = wglCreateContext(*hdc);
	wglMakeCurrent(*hdc, *hrc);
	
	glewInit();
}

void Graph::disableOpenGL(HWND hwnd, HDC hdc, HGLRC hrc)
{
	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(hrc);
	ReleaseDC(hwnd, hdc);
}

GLuint Graph::compileShader(GLenum type, const char* source)
{
	GLuint shader = glCreateShader(type);
	glShaderSource(shader, 1, &source, NULL);
	glCompileShader(shader);
	
	GLint success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	
	if (!success)
	{
		char infoLog[512];
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		std::cout << "error: shader compilation failed\n" << infoLog << std::endl;
	}
	
	return shader;
}

GLuint Graph::createShaderProgram()
{
	GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexShaderSource);
	GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);
	
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	
	GLint success;
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);

	if (!success)
	{
		char infoLog[512];
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "error: program linking failed\n" << infoLog << std::endl;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	
	return shaderProgram;
}
