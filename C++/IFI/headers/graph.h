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
	
	HWND hWnd;
	HDC hdc;
	HGLRC hrc;
	GLuint shaderProgram;
	
	float firstTriangle[6] = {
		-0.9f, -0.5f,
		 0.0f, -0.5f,
		-0.45f, 0.5f
	};
	
	float secondTriangle[6] = {
		0.2f, -0.5f,
		0.9f, -0.5f,
		0.45f, 0.5f
	};
	
	GLuint vboIDs[2], vaoIDs[2];
	
	static const size_t bufferSize = 8; // 1024;
	size_t currentOffset = 0;
	int numVertices = 0;
};

#endif // GRAPH_H_INCLUDED
