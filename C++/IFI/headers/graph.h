#ifndef GRAPH_H_INCLUDED
#define GRAPH_H_INCLUDED

#include <windows.h>
#include <GL/glew.h>

class Graph
{
public:
    static Graph& shared()
    {
        static Graph instance;
        return instance;
    }
	
	void setup(HWND hwnd);
	void update(const int value);
	
private:
	Graph();
   ~Graph();
	
	void enableOpenGL(HWND hwnd, HDC* hdc, HGLRC* hrc);
	void disableOpenGL(HWND hwnd, HDC hdc, HGLRC hrc);
	GLuint compileShader(GLenum type, const char* source);
	GLuint createShaderProgram();
	
	const char* vertexShaderSource = 
	R"(
		#version 330 core
		layout(location = 0) in vec3 aPos;
		void main()
		{
			gl_Position = vec4(aPos, 1.0);
		}
	)";
	
	const char* fragmentShaderSource = 
	R"(
		#version 330 core
		out vec4 FragColor;
		void main()
		{
			FragColor = vec4(0.0, 1.0, 0.0, 1.0);
		}
	)";
	
	HWND hWnd;	
	HDC hdc;
	HGLRC hrc;
	GLuint shaderProgram;
	GLuint vboIDs[2];
	GLuint vaoID;
	
	size_t bufferSize = 1024 * sizeof(float);
	size_t newDataSize = sizeof(int); // or 3 * sizeof(int) ?
	size_t currentOffset = 0;
	int currentVBO = 0;
	int numVertices = 0;
};

#endif // GRAPH_H_INCLUDED
