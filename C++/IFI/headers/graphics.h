#ifndef GRAPHICS_H_INCLUDED
#define GRAPHICS_H_INCLUDED

#include <windows.h>
#include "settings.h"
#include "shaders.h"
#include "databuffer.h"
#include "plot.h"
#include "text.h"

class Graphics
{
public:
	Graphics(DataBuffer& buffer, Settings& settings): buffer(buffer), settings(settings) {}
	
	void set(const HWND& hwnd);
	void render();
	
private:
	void setupPixelFormat(const HWND& hwnd);
	void enableVerticalSyncronization();
	void setupProjections();
	void drawPlots();
	void drawText();
	
	DataBuffer& buffer;
	Settings& settings;
	Shaders shaders;
	
	ViewPortParameters plotParameters = ViewPortParameters(settings);
	ViewPortParameters textParameters = ViewPortParameters(settings);
	
	Plot plot = Plot(plotParameters);
	Text text = Text(textParameters);
	
	float textOrtho[16] = 
	{
		0, 0, 0, 0,
		0, 0, 0, 0,
		0, 0, -1.0f, 0,
		-1.0f, -1.0f, 0, 1.0f
	};
	
	HDC hdc;
	int batchSize = 0;
	int updateCounter = 0;
};

#endif // GRAPHICS_H_INCLUDED
