#ifndef GRAPH_H_INCLUDED
#define GRAPH_H_INCLUDED

#include "graphicsitem.h"
#include "settings.h"
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
	void updateWithFFT(const std::vector<float>& data);
	
private:	
	Graphics();
   ~Graphics();
	
	void enableOpenGL(HWND hwnd, HDC* hdc, HGLRC* hrc);
	void disableOpenGL(HWND hwnd, HDC hdc, HGLRC hrc);
	
	void setupObject(const int index);
	void updateObject(const int index);
	void updateVBO(const int index);
	
	const int bufferSize = Settings::shared().getData().spectrumN;
	
	HWND hWnd;
	HDC hdc;
	HGLRC hrc;
	GLuint shaderProgram;
	GLuint vboIDs[4], vaoIDs[4];
	
	std::vector<GraphicsItem*> items;
	int currentPlot = 0;
};

#endif // GRAPH_H_INCLUDED
