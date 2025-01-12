#ifndef GRAPHICS_H_INCLUDED
#define GRAPHICS_H_INCLUDED

#include <windows.h>
#include "settings.h"
#include "shaders.h"
#include "plot.h"
#include "spectrum.h"
#include "text.h"

class Graphics
{
public:
	Graphics(Settings& settings, Shaders& shaders, Plot& plot1, Plot& plot2, Spectrum& spectrum1, Spectrum& spectrum2, Text& text);
	void render();
	
private:
	void setupPixelFormat();
	void enableVerticalSyncronization();
	void setupProjections();
	void drawPlots();
	void drawSpectrums();
	void drawText();
	
	Shaders& shaders;
	Plot& plot1;
	Plot& plot2;
	Spectrum& spectrum1;
	Spectrum& spectrum2;
	Text& text;
	
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
