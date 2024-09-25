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
	void push(const int value);
	void updateFFT(const std::vector<float>& data);
	
private:
	Graphics();
   ~Graphics();
	
	void enableOpenGL(HWND hwnd, HDC* hdc, HGLRC* hrc);
	void disableOpenGL(HWND hwnd, HDC hdc, HGLRC hrc);
	
	void setupObject(const int index);
	void updateObject(const int index);
	void updateVBO(const int index);
	void drawStaticObjects();
	void updateStaticObjects();
	void switchCurrentPlot();
	
	const int bufferSize = Settings::shared().getData().spectrumN;
	const Range<float> left		= Range<float>(-1, 0);
	const Range<float> top		= Range<float>(0, 1);
	const Range<float> bottom	= Range<float>(-1, 0);
	const Range<float> right	= Range<float>(0, 1);
	
	HWND hWnd;
	HDC hdc;
	HGLRC hrc;
	GLuint shaderProgram;
	GLuint* vboIDs;
	GLuint* vaoIDs;
	
	std::vector<GraphicsItem*> items;
	int currentPlot = 0;
};

#endif // GRAPH_H_INCLUDED
