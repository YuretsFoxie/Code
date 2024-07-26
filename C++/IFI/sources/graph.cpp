#include "graph.h"

#include <iostream>

// Public Functions

void Graph::setup(HWND hwnd)
{
	float vertices[] = {
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.0f,  0.5f, 0.0f
	};
	
	hWnd = hwnd;
	enableOpenGL(hWnd, &hdc, &hrc);
	shaderProgram = createShaderProgram();
	
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	
	glBindVertexArray(VAO);
	
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Graph::update()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glUseProgram(shaderProgram);
	glBindVertexArray(VAO);
	glDrawArrays(GL_LINE_STRIP, 0, 3);
	::SwapBuffers(hdc);
}

// Private Functions

Graph::Graph() {}

Graph::~Graph()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
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



//=====

// Swap buffers

// Two VBOs are created and initialized with GL_STREAM_DRAW usage. 
// A VAO is also created and configured to store the vertex attribute setup.
// Each frame, the current VBO is updated with new data using glBufferSubData. 
// The VAO is then bound, and the current VBO is used for rendering. 
// After rendering, the VBO index is swapped to alternate between the two VBOs.

// Initialization
GLuint vboIDs[2];
glGenBuffers(2, vboIDs); // Generate two VBOs
GLuint vaoID;
glGenVertexArrays(1, &vaoID); // Generate a VAO

glBindVertexArray(vaoID);

// Setup VBOs
for (int i = 0; i < 2; ++i)
{
	glBindBuffer(GL_ARRAY_BUFFER, vboIDs[i]);
	glBufferData(GL_ARRAY_BUFFER, bufferSize, nullptr, GL_STREAM_DRAW); // Allocate buffer memory

	// Setup vertex attributes (assuming a simple position attribute)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
	glEnableVertexAttribArray(0);
}

glBindBuffer(GL_ARRAY_BUFFER, 0);
glBindVertexArray(0);



// Rendering Loop
int currentVBO = 0;

while (!shouldCloseWindow)
{
	// Update VBO with new data
	glBindBuffer(GL_ARRAY_BUFFER, vboIDs[currentVBO]);
	glBufferSubData(GL_ARRAY_BUFFER, offset, sizeof(newData), newData);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	// Render using the current VBO
	glBindVertexArray(vaoID);
	glBindBuffer(GL_ARRAY_BUFFER, vboIDs[currentVBO]);
	glDrawArrays(GL_POINTS, 0, numVertices);
	glBindVertexArray(0);
	
	// Swap VBOs
	currentVBO = (currentVBO + 1) % 2;
	
	// Swap buffers, poll events, etc.
	glfwSwapBuffers(window);
	glfwPollEvents();
}



//=====

// Circular buffer

// A VBO and VAO are created and initialized. The VBO is allocated with a fixed buffer size using glBufferData.
//	The currentOffset is calculated to determine where the new data should be written in the buffer.
//	If adding the new data would exceed the buffer size, the offset wraps around to the beginning of the buffer.
//	The glBufferSubData function is used to update the VBO with the new data at the calculated offset.
//	The VAO is bound, and the data is rendered.
//	The offset is updated for the next frame.

// Initialization
// First, initialize your VBO and VAO:

GLuint vboID;
glGenBuffers(1, &vboID);
GLuint vaoID;
glGenVertexArrays(1, &vaoID);

glBindVertexArray(vaoID);
glBindBuffer(GL_ARRAY_BUFFER, vboID);
glBufferData(GL_ARRAY_BUFFER, bufferSize, nullptr, GL_STREAM_DRAW); // Allocate buffer memory

// Setup vertex attributes (assuming a simple position attribute)
glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
glEnableVertexAttribArray(0);

glBindBuffer(GL_ARRAY_BUFFER, 0);
glBindVertexArray(0);



// Rendering Loop
// In the rendering loop, update the VBO with new data using a circular buffer approach:

size_t bufferSize = 1024 * sizeof(float); // Example buffer size
size_t currentOffset = 0;
size_t newDataSize = sizeof(newData); // Size of the new data to be added

while (!shouldCloseWindow)
{
	// Calculate the offset for the new data
	if (currentOffset + newDataSize > bufferSize)
		currentOffset = 0; // Wrap around if we reach the end of the buffer

	// Update VBO with new data
	glBindBuffer(GL_ARRAY_BUFFER, vboID);
	glBufferSubData(GL_ARRAY_BUFFER, currentOffset, newDataSize, newData);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	// Render using the VBO
	glBindVertexArray(vaoID);
	glDrawArrays(GL_POINTS, 0, numVertices);
	glBindVertexArray(0);
	
	// Update the offset for the next frame
	currentOffset += newDataSize;
	
	// Swap buffers, poll events, etc.
	glfwSwapBuffers(window);
	glfwPollEvents();
}
