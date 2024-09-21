#ifndef GRAPH_H_INCLUDED
#define GRAPH_H_INCLUDED

#include "graphicsitem.h"
#include <windows.h>
#include <vector>
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
	
	void setupObject(const int index);
	void updateObject(const int index);
	void updateVBO(const int index);
	
	HWND hWnd;
	HDC hdc;
	HGLRC hrc;
	GLuint shaderProgram;
	GLuint vboIDs[3], vaoIDs[3];
	
	std::vector<GraphicsItem*> items;
	int currentPlot = 0;
};

#endif // GRAPH_H_INCLUDED
