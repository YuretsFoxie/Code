#ifndef GRAPH_H_INCLUDED
#define GRAPH_H_INCLUDED

#include "plot.h"
#include <windows.h>
#include <GL/glew.h>

class Graphics
{
public:
    static Graphics& shared()
    {
        static Graphics instance;
        return instance;
    }
	
	void setup(HWND hwnd);
	void update(const int value);
	
private:	
	Graphics();
   ~Graphics();
	
	void enableOpenGL(HWND hwnd, HDC* hdc, HGLRC* hrc);
	void disableOpenGL(HWND hwnd, HDC hdc, HGLRC hrc);
	void updateVBO();
	
	HWND hWnd;
	HDC hdc;
	HGLRC hrc;
	GLuint shaderProgram;
	GLuint vboIDs[1], vaoIDs[1];
	
	Plot plot = Plot(4); // 1024
};

#endif // GRAPH_H_INCLUDED
